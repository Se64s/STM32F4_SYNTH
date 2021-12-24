/**
 * @file    cli_task.c
 * @author  Sebastián Del Moral
 * @brief   Tsk for handle CLI inteface.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "cli_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "sys_log.h"
#include "sys_usart.h"

#include "shell.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Logging interface */
#define CLI_IF          ( SYS_LOG_IF_0 )

/* General logging level */
#define CLI_LVL         ( SYS_LOG_LEVEL_DBG )

/* Task internal signals */
#define EVT_TX_DONE     (uint32_t)(1 << 0)
#define EVT_RX_DATA     (uint32_t)(1 << 1)
#define EVT_RX_IDLE     (uint32_t)(1 << 2)
#define EVT_COM_ERR     (uint32_t)(1 << 3)

/* Serial interface use for CLI */
#define CLI_USART       ( SYS_USART_0 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Definitions for CLI tasks */
osThreadId_t CliTaskHandle;
const osThreadAttr_t CliTask_attributes = {
    .name = CLI_TASK_NAME,
    .stack_size = CLI_TASK_STACK,
    .priority = (osPriority_t) CLI_TASK_PRIORITY,
};

/* Event flags */
osEventFlagsId_t evt_id;

/* Private function prototypes -----------------------------------------------*/

static void cli_serial_cb(sys_usart_event_t event);
static int app_cli_send_chr_wrapper(char c);
static void CliTask_main(void *argument);

/* Private function definition -----------------------------------------------*/

/**
 * @brief Handler for serial events
 * 
 * @param event 
 */
void cli_serial_cb(sys_usart_event_t event)
{
    switch (event)
    {
        case SYS_USART_EVENT_RX_IDLE:
            osEventFlagsSet(evt_id, EVT_RX_IDLE);
            break;

        case SYS_USART_EVENT_TX_DONE:
            osEventFlagsSet(evt_id, EVT_TX_DONE);
            break;

        case SYS_USART_EVENT_RX_DONE:
            osEventFlagsSet(evt_id, EVT_RX_DATA);
            break;

        case SYS_USART_EVENT_RX_BUFF_FULL:
            break;

        case SYS_USART_EVENT_ERROR:
            osEventFlagsSet(evt_id, EVT_COM_ERR);
            break;

        default:
        break;
    }
}

/**
 * @brief Wrapper send function used in shell implementation
 * 
 * @param c character to send.
 * @return int number of bytes to sent.
 */
static int app_cli_send_chr_wrapper(char c)
{
    uint8_t u8TxData = (uint8_t)c;

    sys_state_t eRetval = SYS_ERROR;

    // Repeat send if busy or error.
    do {
        eRetval = sys_usart_send(CLI_USART, &u8TxData, 1U);
    } while (eRetval != SYS_SUCCESS);

    return 1U;
}

/**
  * @brief  Function implementing the main thread.
  * @param  argument: Not used
  * @retval None
  */
static void CliTask_main(void *argument)
{
    uint8_t u8RxData = 0;

    /* Shell send mapping */
    sShellImpl shell_impl = {
        .send_char = app_cli_send_chr_wrapper,
    };
    shell_boot(&shell_impl);

    sys_log_print(CLI_IF, CLI_LVL, "Init CLI task");

    shell_put_line("START CLI...");

    /* Init reception */
    (void)sys_usart_read(CLI_USART, &u8RxData, 1U);

    /* Infinite loop */
    for(;;)
    {
        uint32_t u32Flags = osEventFlagsWait(evt_id, EVT_RX_DATA | EVT_COM_ERR | EVT_RX_IDLE, osFlagsWaitAny, osWaitForever);

        if ( (u32Flags & EVT_RX_DATA) == EVT_RX_DATA )
        {
            shell_receive_char(u8RxData);
        }

        (void)sys_usart_read(CLI_USART, &u8RxData, 1U);
    }
}

/* Public function prototypes -----------------------------------------------*/

/**
  * @brief  CLI task init.
  * @param  None
  * @retval None
  */
void CliTask_Init(void)
{
    CliTaskHandle = osThreadNew(CliTask_main, NULL, &CliTask_attributes);
    ERR_ASSERT(CliTaskHandle != NULL);

    evt_id = osEventFlagsNew(NULL);
    ERR_ASSERT(evt_id != NULL);

    ERR_ASSERT(sys_usart_init(CLI_USART, cli_serial_cb) == SYS_SUCCESS);
}

/* EOF */
