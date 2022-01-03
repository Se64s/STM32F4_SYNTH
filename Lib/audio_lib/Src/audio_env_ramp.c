/**
 * @file    audio_envramp.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to generate ramp envelopes.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_env_ramp.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function definition -----------------------------------------------*/
/* Public function definition ------------------------------------------------*/

void RAMP_init(env_ramp_ctrl_t *pxRampCtrl, float fSampleRate)
{
    ERR_ASSERT( pxRampCtrl != NULL );
    ERR_ASSERT( fSampleRate != 0.0F );

    pxRampCtrl->fCurrentValue = 0.0F;
    pxRampCtrl->fIncrement = 0.0F;
    pxRampCtrl->fSampleRate = fSampleRate;
    pxRampCtrl->u32Cnt = 0.0F;
}

void RAMP_set_samplerate(env_ramp_ctrl_t *pxRampCtrl, float fSampleRate)
{
    ERR_ASSERT( pxRampCtrl != NULL );
    ERR_ASSERT( fSampleRate != 0.0F );

    pxRampCtrl->fSampleRate = fSampleRate;
}

void RAMP_set_value(env_ramp_ctrl_t *pxRampCtrl, float fNewValue)
{
    ERR_ASSERT( pxRampCtrl != NULL );

    pxRampCtrl->fCurrentValue = fNewValue;
    pxRampCtrl->fIncrement = 0.0F;
    pxRampCtrl->u32Cnt = 0.0F;
}

void RAMP_ramp_to(env_ramp_ctrl_t *pxRampCtrl, float fValue, float fTime)
{
    ERR_ASSERT( pxRampCtrl != NULL );

    pxRampCtrl->fIncrement = (fValue - pxRampCtrl->fCurrentValue) / (pxRampCtrl->fSampleRate * fTime);
    pxRampCtrl->u32Cnt = (uint32_t)(pxRampCtrl->fSampleRate * fTime);
}

float RAMP_process(env_ramp_ctrl_t *pxRampCtrl)
{
    ERR_ASSERT( pxRampCtrl != NULL );

    if ( pxRampCtrl->u32Cnt > 0U )
    {
        pxRampCtrl->u32Cnt--;
        pxRampCtrl->fCurrentValue += pxRampCtrl->fIncrement;
    }

    return pxRampCtrl->fCurrentValue;
}

bool RAMP_finished(env_ramp_ctrl_t *pxRampCtrl)
{
    ERR_ASSERT( pxRampCtrl != NULL );

    return (pxRampCtrl->u32Cnt == 0U);
}

/* EOF */