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
    {
        audio_cmd_t xAudioCmd = { 0 };
        xAudioCmd.eCmdId = AUDIO_CMD_SET_WAVEFORM;
        xAudioCmd.xCmdPayload.xSetWave.eVoiceId = AUDIO_VOICE_NUM;
        xAudioCmd.xCmdPayload.xSetWave.eWaveId = AUDIO_WAVE_TRI;

        (void)AUDIO_handle_cmd(xAudioCmd);
    }

    /* Infinite loop */
    for(;;)
    {
        sys_log_print(AUDIO_IF, AUDIO_LVL, "Audio task tick");

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

    ERR_ASSERT(AUDIO_init() == AUDIO_OK);
}

/* EOF */
