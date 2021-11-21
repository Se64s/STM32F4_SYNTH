/**
 * @file    midi_task.c
 * @author  Sebastián Del Moral
 * @brief   Midi task handler.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "midi_task.h"

#include "circular_buffer.h"

#include "audio_engine.h"

#include "midi_lib.h"
#include "midi_voice.h"

#include "sys_log.h"
#include "sys_usart.h"
#include "sys_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Logging interface */
#define MIDI_DBG_IF                 ( SYS_LOG_IF_0 )

/* General logging level */
#define MIDI_DBG_LVL                ( SYS_LOG_LEVEL_DBG )

/* Serial interface use for CLI */
#define MIDI_USART                  ( SYS_USART_1 )

/* Status gpio output */
#define MIDI_STATUS_LED             ( SYS_GPIO_1 )

/* Serial buff size for midi RX channel */
#define MIDI_CBUFF_RX_SIZE          ( 256U )
#define MIDI_DMA_BUFF_RX_SIZE       ( 128U )

/* Nuber of voices to control over midi */
#define MIDI_NUM_VOICE              ( 8U )  // Call to voice lib to get num voices?

/* Init midi channel */
#define MIDI_CH_DEFAULT             ( 0U )

/* Task defined events */
#define MIDI_EVT_DATA_IN            (uint32_t)( 1 << 0 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Definitions for Midi tasks */
osThreadId_t MidiTaskHandle;
const osThreadAttr_t MidiTask_attributes = {
    .name = MIDI_TASK_NAME,
    .stack_size = MIDI_TASK_STACK,
    .priority = (osPriority_t) MIDI_TASK_PRIORITY,
};

/* Task event handler */
osEventFlagsId_t midi_evt_handler;

/* Handler for midi handler engine */
midi_handler_t MidiEngine;

/* Handler for voice engine */
midi_voice_handler_t MidiVoiceEngine;
midi_voice_t MidiVoiceList[MIDI_NUM_VOICE];

/* Midi Rx buffer */
circular_buf_t MidiCBuff = { 0U };
uint8_t MidiRxCBuff[MIDI_CBUFF_RX_SIZE] = { 0U };
uint8_t MidiRxDMABuff[MIDI_DMA_BUFF_RX_SIZE] = { 0U };

/* Private function prototypes -----------------------------------------------*/

static void midi_cmd_1_cb(uint8_t cmd, uint8_t data);
static void midi_cmd_2_cb(uint8_t cmd, uint8_t data0, uint8_t data1);
static void midi_rt_cb(uint8_t rt_data);
static void midi_serial_cb(sys_usart_event_t event);
static void midi_voice_cb(uint8_t u8Voice, uint8_t u8Note, uint8_t u8Velocity, midi_voice_state_t eAction);
static void MidiTask_main(void *argument);

/* Private function definition -----------------------------------------------*/

/* Callbacks for Midi engine -------------------------------------------------*/

/**
 * @brief Handler for midi commands with one data byte.
 * 
 * @param cmd midi command.
 * @param data midi data.
 */
static void midi_cmd_1_cb(uint8_t cmd, uint8_t data)
{
    (void)cmd;
    (void)data;

    static uint32_t u32Cmd1Cnt = 0U;
    u32Cmd1Cnt++;
}

/**
 * @brief Handler for midi commands with two data bytes.
 * 
 * @param cmd midi command.
 * @param data0 midi data0
 * @param data1 midi data1
 */
static void midi_cmd_2_cb(uint8_t cmd, uint8_t data0, uint8_t data1)
{
    static uint32_t u32Cmd2Cnt = 0U;
    uint8_t u8Status = MIDI_CMD_GET_STATUS(cmd);

    if ( (u8Status == MIDI_STATUS_NOTE_OFF) || (u8Status == MIDI_STATUS_NOTE_ON) )
    {
        (void)VOICE_update_note(&MidiVoiceEngine, cmd, data0, data1);
    }

    u32Cmd2Cnt++;
}

/**
 * @brief Handler for midi RT commands
 * 
 * @param rt_data midi RT command
 */
static void midi_rt_cb(uint8_t rt_data)
{
    (void)rt_data;
}

/* Callbacks for serial interface engine -------------------------------------*/

/**
 * @brief Handler for serial events
 * 
 * @param event 
 */
void midi_serial_cb(sys_usart_event_t event)
{
    switch (event)
    {
        case SYS_USART_EVENT_RX_IDLE:
            {
                /* Get all data from DMA buffer */
                uint16_t u16RxCount = sys_usart_get_read_count(MIDI_USART);
                for (uint16_t i = 0U; i < u16RxCount; i++)
                {
                    circular_buf_put2(&MidiCBuff, MidiRxDMABuff[i]);
                }
                /* Enable reception */
                sys_usart_read(MIDI_USART, MidiRxDMABuff, MIDI_DMA_BUFF_RX_SIZE);
                /* Set data available flag */
                osEventFlagsSet(midi_evt_handler, MIDI_EVT_DATA_IN);
            }
            break;

        case SYS_USART_EVENT_TX_DONE:
            break;

        case SYS_USART_EVENT_RX_DONE:
        case SYS_USART_EVENT_RX_BUFF_FULL:
            {
                /* Get all data from DMA buffer */
                for (uint16_t i = 0U; i < MIDI_DMA_BUFF_RX_SIZE; i++)
                {
                    circular_buf_put2(&MidiCBuff, MidiRxDMABuff[i]);
                }
                /* Enable reception */
                sys_usart_read(MIDI_USART, MidiRxDMABuff, MIDI_DMA_BUFF_RX_SIZE);
                /* Set data available flag */
                osEventFlagsSet(midi_evt_handler, MIDI_EVT_DATA_IN);
            }
            break;

        case SYS_USART_EVENT_ERROR:
            {
                /* Enable reception */
                sys_usart_read(MIDI_USART, MidiRxDMABuff, MIDI_DMA_BUFF_RX_SIZE);
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Callback to handle midi voice actions.
 * 
 * @param u8Voice voice channel to update.
 * @param u8Note note to apply.
 * @param u8Velocity velocity of new update.
 * @param eAction action to apply on new voice.
 */
void midi_voice_cb(uint8_t u8Voice, uint8_t u8Note, uint8_t u8Velocity, midi_voice_state_t eAction)
{
    if ( eAction == VOICE_STATE_ON )
    {
        AUDIO_voice_set_midi_note((audio_voice_id_t)u8Voice, u8Note, u8Velocity);
        AUDIO_voice_set_state((audio_voice_id_t)u8Voice, true);
    }
    else if ( eAction == VOICE_STATE_OFF )
    {
        AUDIO_voice_set_state((audio_voice_id_t)u8Voice, false);
    }
}

/* Main task function --------------------------------------------------------*/

/**
  * @brief  Function implementing the main thread.
  * @param  argument: Not used
  * @retval None
  */
static void MidiTask_main(void *argument)
{
    sys_log_print(MIDI_DBG_IF, MIDI_DBG_LVL, "Init Midi task");

    sys_gpio_set_level(MIDI_STATUS_LED, SYS_GPIO_STATE_SET);

    /* Enable reception */
    sys_usart_read(MIDI_USART, MidiRxDMABuff, MIDI_DMA_BUFF_RX_SIZE);

    /* Infinite loop */
    for(;;)
    {
        uint32_t u32Flags = osEventFlagsWait(midi_evt_handler, MIDI_EVT_DATA_IN, osFlagsWaitAny, osWaitForever);

        sys_gpio_set_level(MIDI_STATUS_LED, SYS_GPIO_STATE_RESET);

        if ( SYS_CHECK_EVT(u32Flags, MIDI_EVT_DATA_IN) )
        {
            /* Clear rx buffer */
            uint8_t u8RxData = 0U;

            while ( circular_buf_get(&MidiCBuff, &u8RxData) == 0U )
            {
                if ( midi_update_fsm(&MidiEngine, u8RxData) != midiOk )
                {
                    sys_log_print(MIDI_DBG_IF, MIDI_DBG_LVL, "ERROR processing midi FSM");
                }
            }
        }

        sys_gpio_set_level(MIDI_STATUS_LED, SYS_GPIO_STATE_SET);
    }
}

/* Public function prototypes -----------------------------------------------*/

/**
  * @brief  CLI task init.
  * @param  None
  * @retval None
  */
void MidiTask_Init(void)
{
    MidiTaskHandle = osThreadNew(MidiTask_main, NULL, &MidiTask_attributes);
    ERR_ASSERT(MidiTaskHandle != NULL);

    midi_evt_handler = osEventFlagsNew(NULL);
    ERR_ASSERT(midi_evt_handler != NULL);

    ERR_ASSERT(sys_usart_init(MIDI_USART, midi_serial_cb) == SYS_SUCCESS);

    ERR_ASSERT(sys_gpio_init(MIDI_STATUS_LED, SYS_GPIO_MODE_OUT) == SYS_SUCCESS);

    ERR_ASSERT(midi_init(&MidiEngine, NULL, 0U, NULL, midi_cmd_1_cb, midi_cmd_2_cb, midi_rt_cb) == midiOk);

    ERR_ASSERT(VOICE_init(&MidiVoiceEngine, MidiVoiceList, MIDI_NUM_VOICE, MIDI_CH_DEFAULT, midi_voice_cb) == midiOk);

    /* Init rx buffer */
    circular_buf_init(&MidiCBuff, MidiRxCBuff, MIDI_CBUFF_RX_SIZE);
}

/* EOF */
