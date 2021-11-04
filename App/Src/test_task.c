/**
 * @file    test_task.c
 * @author  Sebastián Del Moral
 * @brief   Playground task.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "test_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "sys_log.h"
#include "sys_i2s.h"
#include "sys_gpio.h"

#include "wavetable.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// #define TRACE_AUDIO_UPDATE

#define TEST_IF                 ( SYS_LOG_IF_0 )
#define TEST_LVL                ( SYS_LOG_LEVEL_DBG )

#define TEST_I2S                ( SYS_I2S_0 )
#define TEST_PIN                ( SYS_GPIO_0 )

/* Task internal signals */
#define EVT_TX_DONE             (uint32_t)(1 << 0)
#define EVT_HALF_TX_DONE        (uint32_t)(1 << 1)
#define EVT_COM_ERR             (uint32_t)(1 << 2)

/* Wave tables dataset */
#define AUDIO_SAMPLE_RATE       ( 192000U )
#define AUDIO_AMPLITUDE         ( 2147483647.0F )      // Max value int32

#define AUDIO_BUFF_SIZE         ( 512U )
#define AUDIO_HALF_BUFF_SIZE    ( AUDIO_BUFF_SIZE / 2U )

#define AUDIO_BUFF_INIT_INDEX   ( 0U )
#define AUDIO_BUFF_HALF_INDEX   ( AUDIO_HALF_BUFF_SIZE )

#define AUDIO_TRANSFER_SIZE     ( sizeof(u16AudioBuffer) / sizeof(uint32_t) )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Definitions for testTask */
osThreadId_t testTaskHandle;
const osThreadAttr_t testTask_attributes = {
    .name = TEST_TASK_NAME,
    .stack_size = TEST_TASK_STACK,
    .priority = (osPriority_t)TEST_TASK_PRIORITY,
};

/* Event flags */
osEventFlagsId_t evt_id;

/* Wave tables dataset */
waveTableVoice_t xWaveTableVoice;

/* Audio buffer */
uint16_t u16AudioBuffer[AUDIO_BUFF_SIZE] = { 0U };

/* Private function prototypes -----------------------------------------------*/

static void update_audio_buffer(uint16_t *pu16Buffer, uint16_t u16StartIndex);
static void testTask_main(void *argument);
static void test_i2s_cb(sys_i2s_event_t event);

/* Private function definition -----------------------------------------------*/

/**
 * @brief Prepare data in audio buffer.
 * 
 * @param pu16Buffer Output audio buffer.
 * @param u16StartIndex Start of the index where put new data.
 */
static void update_audio_buffer(uint16_t *pu16Buffer, uint16_t u16StartIndex)
{
#ifdef TRACE_AUDIO_UPDATE
    sys_gpio_set_level(TEST_PIN, SYS_GPIO_STATE_SET);
#endif // TRACE_AUDIO_UPDATE

    for (uint32_t i = 0; i < AUDIO_HALF_BUFF_SIZE; i += 4U)
    {
        int32_t i32Data = (int32_t)WAVE_get_next_sample(&xWaveTableVoice);

        // Prepare data for Codec
        uint16_t u16Data0 = (uint16_t)((i32Data & 0xFFFF0000) >> 16U);
        uint16_t u16Data1 = (uint16_t)((i32Data & 0x0000FFFF) << 0U);
        // Channel L
        pu16Buffer[u16StartIndex++] = u16Data0;
        pu16Buffer[u16StartIndex++] = u16Data1;
        // Channel R
        pu16Buffer[u16StartIndex++] = u16Data0;
        pu16Buffer[u16StartIndex++] = u16Data1;
    }

#ifdef TRACE_AUDIO_UPDATE
    sys_gpio_set_level(TEST_PIN, SYS_GPIO_STATE_RESET);
#endif // TRACE_AUDIO_UPDATE
}

/**
 * @brief Handler for serial events
 * 
 * @param event 
 */
void test_i2s_cb(sys_i2s_event_t event)
{
    switch (event)
    {
        case SYS_I2S_EVENT_TX_DONE:
            update_audio_buffer(u16AudioBuffer, AUDIO_BUFF_HALF_INDEX);
            osEventFlagsSet(evt_id, EVT_TX_DONE);
            break;

        case SYS_I2S_EVENT_HALF_TX_DONE:
            update_audio_buffer(u16AudioBuffer, AUDIO_BUFF_INIT_INDEX);
            osEventFlagsSet(evt_id, EVT_HALF_TX_DONE);
            break;

        case SYS_I2S_EVENT_ERROR:
            sys_i2s_send(TEST_I2S, u16AudioBuffer, AUDIO_TRANSFER_SIZE);
            osEventFlagsSet(evt_id, EVT_COM_ERR);
            break;

        default:
        break;
    }
}

/**
  * @brief  Function implementing the main thread.
  * @param  argument: Not used
  * @retval None
  */
static void testTask_main(void *argument)
{
    /* Test note array */
    float fTestNoteArray[] = {
        261.6,
        293.67,
        329.63,
        349.23,
        392.0,
        440.0,
        493.88
    };

    sys_log_print(TEST_IF, TEST_LVL, "Init I2S Test task");

    /* Init wavetable */
    WAVE_init_voice(&xWaveTableVoice, AUDIO_SAMPLE_RATE, AUDIO_AMPLITUDE);

    /* Start transfer to codec */
    update_audio_buffer(u16AudioBuffer, AUDIO_BUFF_INIT_INDEX);
    sys_i2s_send(TEST_I2S, u16AudioBuffer, AUDIO_TRANSFER_SIZE);

    /* Infinite loop */
    for(;;)
    {
        /* Select new wavetable */
        for (uint32_t wave = 0U; wave < (uint32_t)WAVE_NUM; wave++)
        {
            WAVE_change_wave(&xWaveTableVoice, (wave_id_t)wave);

            /* Select new note */
            uint32_t u32NumNotes = sizeof(fTestNoteArray) / sizeof(fTestNoteArray[0]);

            for (uint32_t note = 0; note < u32NumNotes; note++)
            {
                WAVE_update_freq(&xWaveTableVoice, fTestNoteArray[note]);
                osDelay(1000U);
            }
        }
    }
}

/* Public function prototypes -----------------------------------------------*/

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void testTask_Init(void)
{
    testTaskHandle = osThreadNew(testTask_main, NULL, &testTask_attributes);
    ERR_ASSERT(testTaskHandle != NULL);

    evt_id = osEventFlagsNew(NULL);
    ERR_ASSERT(evt_id != NULL);

    ERR_ASSERT(sys_i2s_init(TEST_I2S, test_i2s_cb) == SYS_SUCCESS);
    ERR_ASSERT(sys_gpio_init(TEST_PIN, SYS_GPIO_MODE_OUT) == SYS_SUCCESS);
}

/* EOF */
