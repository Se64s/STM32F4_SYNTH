/**
 * @file    audio_task.c
 * @author  Sebastián Del Moral
 * @brief   Audio output handler.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "sys_log.h"

#include "audio_engine.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define AUDIO_IF        ( SYS_LOG_IF_0 )
#define AUDIO_LVL       ( SYS_LOG_LEVEL_DBG )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Definitions for testTask */
osThreadId_t AudioTaskHandle;
const osThreadAttr_t AudioTask_attributes = {
    .name = AUDIO_TASK_NAME,
    .stack_size = AUDIO_TASK_STACK,
    .priority = (osPriority_t)AUDIO_TASK_PRIORITY,
};

/* Private function prototypes -----------------------------------------------*/

static void AudioTask_main(void *argument);

/* Private function definition -----------------------------------------------*/

/**
  * @brief  Function implementing the main thread.
  * @param  argument: Not used
  * @retval None
  */
static void AudioTask_main(void *argument)
{
    sys_log_print(AUDIO_IF, AUDIO_LVL, "Init audio engine task");

    /* Setup all waves */
    (void)AUDIO_voice_set_waveform(AUDIO_VOICE_NUM, AUDIO_WAVE_SIN);

    /* Infinite loop */
    for(;;)
    {
        /* Add all voices */
        float base_note = 440.0;

        for (uint32_t voice = 0U; voice < (uint32_t)AUDIO_VOICE_NUM; voice++)
        {
            // (void)AUDIO_voice_update(voice, AUDIO_WAVE_SIN, base_note * (voice + 1U), true);
            (void)AUDIO_voice_update(AUDIO_VOICE_NUM, AUDIO_WAVE_SIN, base_note * (voice + 1U), true);
            osDelay(1000U);
        }

        /* Clear all voices */
        (void)AUDIO_voice_set_state(AUDIO_VOICE_NUM, false);
        osDelay(1000U);
    }
}

/* Public function prototypes -----------------------------------------------*/

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void AudioTask_Init(void)
{
    AudioTaskHandle = osThreadNew(AudioTask_main, NULL, &AudioTask_attributes);
    ERR_ASSERT(AudioTaskHandle != NULL);

    ERR_ASSERT(AUDIO_init() == SYS_SUCCESS);
}

/* EOF */
