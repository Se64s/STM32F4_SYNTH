/**
 * @file    main.c
 * @author  Sebastián Del Moral (sebmorgal@gmail.com)
 * @brief   Main application start code
 * @version 0.1
 * @date    2021-10-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

#include "sys_mcu.h"
#include "sys_log.h"

#include "audio_task.h"
#include "cli_task.h"
#include "midi_task.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* SYS Configuration--------------------------------------------------------*/
    (void)sys_mcu_init();
    (void)sys_log_init();

    /* RTOS init */
    osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */

    /* Init system tasks */
    AudioTask_Init();
    CliTask_Init();
    MidiTask_Init();

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    while (1)
    {
    }
}

/* EOF */
