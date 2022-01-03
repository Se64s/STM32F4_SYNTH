/**
 * @file    audio_env_adsr.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to generate ADSR envelopes.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_ENV_ADSR_H__
#define __AUDIO_ENV_ADSR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_common.h"
#include "audio_env_ramp.h"

/* Exported types ------------------------------------------------------------*/

typedef enum env_adsr_state {
    ENV_ADSR_STATE_OFF,
    ENV_ADSR_STATE_ATTACK,
    ENV_ADSR_STATE_DECAY,
    ENV_ADSR_STATE_SUSTAIN,
    ENV_ADSR_STATE_RELEASE,
    ENV_ADSR_STATE_NUM
} env_adsr_state_t;

typedef struct env_adsr_ctrl {
    env_adsr_state_t eState;
    float fAttackTime;
    float fDecayTime;
    float fReleaseTime;
    float fAttackLvl;
    float fSustainLvl;
    env_ramp_ctrl_t xRamp;
} env_adsr_ctrl_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

void ADSR_init(env_adsr_ctrl_t *pxAdsrCtrl, float fSampleRate);

void ADSR_trigger(env_adsr_ctrl_t *pxAdsrCtrl);
void ADSR_release(env_adsr_ctrl_t *pxAdsrCtrl);

float ADSR_process(env_adsr_ctrl_t *pxAdsrCtrl);

bool ADSR_is_active(env_adsr_ctrl_t *pxAdsrCtrl);

float ADSR_get_attack_time(env_adsr_ctrl_t *pxAdsrCtrl);
float ADSR_get_decay_time(env_adsr_ctrl_t *pxAdsrCtrl);
float ADSR_get_release_time(env_adsr_ctrl_t *pxAdsrCtrl);
float ADSR_get_attack_lvl(env_adsr_ctrl_t *pxAdsrCtrl);
float ADSR_get_sustain_lvl(env_adsr_ctrl_t *pxAdsrCtrl);

void ADSR_set_attack_time(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue);
void ADSR_set_decay_time(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue);
void ADSR_set_release_time(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue);
void ADSR_set_attack_lvl(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue);
void ADSR_set_sustain_lvl(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_ENV_ADSR_H__ */

/* EOF */
