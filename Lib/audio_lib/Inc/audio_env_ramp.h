/**
 * @file    audio_env_ramp.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to generate ramp envelopes.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_ENV_RAMP_H__
#define __AUDIO_ENV_RAMP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_common.h"

/* Exported types ------------------------------------------------------------*/

typedef struct env_ramp_ctrl {
    float fSampleRate;
    float fCurrentValue;
    float fIncrement;
    uint32_t u32Cnt;
} env_ramp_ctrl_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

void RAMP_init(env_ramp_ctrl_t *pxRampCtrl, float fSampleRate);

void RAMP_set_samplerate(env_ramp_ctrl_t *pxRampCtrl, float fSampleRate);
void RAMP_set_value(env_ramp_ctrl_t *pxRampCtrl, float fNewValue);

void RAMP_ramp_to(env_ramp_ctrl_t *pxRampCtrl, float fValue, float fTime);

float RAMP_process(env_ramp_ctrl_t *pxRampCtrl);

bool RAMP_finished(env_ramp_ctrl_t *pxRampCtrl);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_ENV_RAMP_H__ */

/* EOF */
