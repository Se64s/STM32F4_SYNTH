/**
 * @file    audio_env_adsr.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to generate ADSR envelopes.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_env_adsr.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DEFAULT_ATTACK_TIME     ( 0.05F )
#define DEFAULT_DECAY_TIME      ( 0.1F )
#define DEFAULT_RELEASE_TIME    ( 0.2F )

#define DEFAULT_ATTACK_LVL      ( 1.0F )
#define DEFAULT_SUSTAIN_LVL     ( 0.7F )

#define MAX_LVL                 ( 1.0F )
#define MIN_LVL                 ( 0.0F )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function definition -----------------------------------------------*/
/* Public function definition ------------------------------------------------*/

void ADSR_init(env_adsr_ctrl_t *pxAdsrCtrl, float fSampleRate)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );
    ERR_ASSERT( fSampleRate > 0.0F );

    pxAdsrCtrl->fAttackTime = DEFAULT_ATTACK_TIME;
    pxAdsrCtrl->fDecayTime = DEFAULT_DECAY_TIME;
    pxAdsrCtrl->fReleaseTime = DEFAULT_RELEASE_TIME;

    pxAdsrCtrl->fAttackLvl = DEFAULT_ATTACK_LVL;
    pxAdsrCtrl->fSustainLvl = DEFAULT_SUSTAIN_LVL;

    pxAdsrCtrl->eState = ENV_ADSR_STATE_OFF;

    RAMP_init(&pxAdsrCtrl->xRamp, fSampleRate);
}

void ADSR_trigger(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    pxAdsrCtrl->eState = ENV_ADSR_STATE_ATTACK;

    RAMP_ramp_to(&pxAdsrCtrl->xRamp, pxAdsrCtrl->fAttackLvl, pxAdsrCtrl->fAttackTime);
}

void ADSR_release(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    if ( (pxAdsrCtrl->eState != ENV_ADSR_STATE_OFF) && (pxAdsrCtrl->eState != ENV_ADSR_STATE_RELEASE) )
    {
        pxAdsrCtrl->eState = ENV_ADSR_STATE_RELEASE;

        RAMP_ramp_to(&pxAdsrCtrl->xRamp, 0.0F, pxAdsrCtrl->fReleaseTime);
    }
}

float ADSR_process(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    float fOutData = RAMP_process(&pxAdsrCtrl->xRamp);

    if ( pxAdsrCtrl->eState == ENV_ADSR_STATE_OFF )
    {
        /* Nothing to do */
    }
    else if ( pxAdsrCtrl->eState == ENV_ADSR_STATE_ATTACK )
    {
        if ( RAMP_finished(&pxAdsrCtrl->xRamp) )
        {
            pxAdsrCtrl->eState = ENV_ADSR_STATE_DECAY;
            RAMP_ramp_to(&pxAdsrCtrl->xRamp, pxAdsrCtrl->fSustainLvl, pxAdsrCtrl->fDecayTime);
        }
    }
    else if ( pxAdsrCtrl->eState == ENV_ADSR_STATE_DECAY )
    {
        if ( RAMP_finished(&pxAdsrCtrl->xRamp) )
        {
            pxAdsrCtrl->eState = ENV_ADSR_STATE_SUSTAIN;
        }
    }
    else if ( pxAdsrCtrl->eState == ENV_ADSR_STATE_SUSTAIN )
    {
        /* Nothing to do */
    }
    else if ( pxAdsrCtrl->eState == ENV_ADSR_STATE_RELEASE )
    {
        if ( RAMP_finished(&pxAdsrCtrl->xRamp) )
        {
            pxAdsrCtrl->eState = ENV_ADSR_STATE_OFF;
        }
    }

    return fOutData;
}

bool ADSR_is_active(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    return (pxAdsrCtrl->eState != ENV_ADSR_STATE_OFF);
}

float ADSR_get_attack_time(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    return pxAdsrCtrl->fAttackTime;
}

float ADSR_get_decay_time(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    return pxAdsrCtrl->fDecayTime;
}

float ADSR_get_release_time(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    return pxAdsrCtrl->fReleaseTime;
}

float ADSR_get_attack_lvl(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    return pxAdsrCtrl->fAttackLvl;
}

float ADSR_get_sustain_lvl(env_adsr_ctrl_t *pxAdsrCtrl)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );

    return pxAdsrCtrl->fSustainLvl;
}

void ADSR_set_attack_time(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );
    ERR_ASSERT( fNewValue >= 0.0F );

    pxAdsrCtrl->fAttackTime = fNewValue;
}

void ADSR_set_decay_time(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );
    ERR_ASSERT( fNewValue >= 0.0F );

    pxAdsrCtrl->fDecayTime = fNewValue;
}

void ADSR_set_release_time(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );
    ERR_ASSERT( fNewValue >= 0.0F );

    pxAdsrCtrl->fReleaseTime = fNewValue;
}

void ADSR_set_attack_lvl(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );
    ERR_ASSERT( fNewValue >= MIN_LVL );
    ERR_ASSERT( fNewValue <= MAX_LVL );

    pxAdsrCtrl->fAttackLvl = fNewValue;
}

void ADSR_set_sustain_lvl(env_adsr_ctrl_t *pxAdsrCtrl, float fNewValue)
{
    ERR_ASSERT( pxAdsrCtrl != NULL );
    ERR_ASSERT( fNewValue >= MIN_LVL );
    ERR_ASSERT( fNewValue <= MAX_LVL );

    if ( fNewValue <= pxAdsrCtrl->fAttackLvl )
    {
        pxAdsrCtrl->fSustainLvl = fNewValue;
    }
}

/* EOF */