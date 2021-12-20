/**
 * @file    audio_delay.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to generate delay effect.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_delay.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function definition -----------------------------------------------*/
/* Public function definition ------------------------------------------------*/

audio_ret_t AUDIO_DELAY_init(AudioDelayCtrl_t *pxDelayCtrl, uint32_t u32SampleRate, float *pfData, uint32_t u32BufferSize)
{
    ERR_ASSERT( pxDelayCtrl != NULL );
    ERR_ASSERT( pfData != NULL );

    audio_ret_t eRetVal = AUDIO_WAVE_PARAM_ERROR;

    if ( (u32SampleRate != 0U) && (u32BufferSize > 0U) && (pxDelayCtrl != NULL) && (pfData != NULL) )
    {
        pxDelayCtrl->fBuffer = pfData;
        pxDelayCtrl->u32BufferSize = u32BufferSize;
        pxDelayCtrl->u32SampleRate = u32SampleRate;

        pxDelayCtrl->u32WritePos = 0U;
        pxDelayCtrl->u32ReadPos = 0U;
        pxDelayCtrl->fFeedback = 0.0F;

        eRetVal = AUDIO_WAVE_OK;
    }

    return eRetVal;
}

audio_ret_t AUDIO_DELAY_update_delay(AudioDelayCtrl_t *pxDelayCtrl, float fDelayTime)
{
    ERR_ASSERT( pxDelayCtrl != NULL );

    audio_ret_t eRetVal = AUDIO_WAVE_PARAM_ERROR;

    float fMaxDelay = (float)pxDelayCtrl->u32BufferSize / (float)pxDelayCtrl->u32SampleRate;

    if ( fDelayTime < fMaxDelay )
    {
        float fDelayNumSamples = fDelayTime * (float)pxDelayCtrl->u32SampleRate;

        pxDelayCtrl->u32ReadPos = ((pxDelayCtrl->u32WritePos + pxDelayCtrl->u32BufferSize) - (uint32_t)fDelayNumSamples) % pxDelayCtrl->u32BufferSize;

        eRetVal = AUDIO_WAVE_OK;
    }

    return eRetVal;
}

audio_ret_t AUDIO_DELAY_update_feedback(AudioDelayCtrl_t *pxDelayCtrl, float fFeedback)
{
    ERR_ASSERT( pxDelayCtrl != NULL );

    audio_ret_t eRetVal = AUDIO_WAVE_PARAM_ERROR;

    if ( (fFeedback >= 0.0F) && (fFeedback < 1.0F) )
    {
        pxDelayCtrl->fFeedback = fFeedback;
    }

    return eRetVal;
}

float AUDIO_DELAY_get_max_delay(AudioDelayCtrl_t *pxDelayCtrl)
{
    ERR_ASSERT( pxDelayCtrl != NULL );

    return pxDelayCtrl->u32SampleRate * (float)pxDelayCtrl->u32BufferSize;
}

float AUDIO_DELAY_process(AudioDelayCtrl_t *pxDelayCtrl, float fInputSample)
{
    ERR_ASSERT( pxDelayCtrl != NULL );

    // Compute delayed sample
    float fOutData = pxDelayCtrl->fBuffer[pxDelayCtrl->u32ReadPos];
    pxDelayCtrl->u32ReadPos++;
    if (pxDelayCtrl->u32ReadPos > pxDelayCtrl->u32BufferSize)
    {
        pxDelayCtrl->u32ReadPos = 0U;
    }

    // Add current sample to delay structure
    pxDelayCtrl->fBuffer[pxDelayCtrl->u32WritePos] = fInputSample + pxDelayCtrl->fFeedback * fOutData;
    pxDelayCtrl->u32WritePos++;
    if (pxDelayCtrl->u32WritePos > pxDelayCtrl->u32BufferSize)
    {
        pxDelayCtrl->u32WritePos = 0U;
    }

    return fOutData;
}

/* EOF */