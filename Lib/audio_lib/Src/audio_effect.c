/**
 * @file    audio_effect.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to handle effect engine.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_effect.h"

#include "audio_filter.h"
#include "audio_delay.h"

/* Private typedef -----------------------------------------------------------*/

/* Effect render function format */
typedef float (*effect_render)(float fInputData);

/* Handler structure for effects */
typedef struct effect_handler {
    audio_effect_id_t eId;
    bool bActive;
    effect_render Render;
} effect_handler_t;

/* Private define ------------------------------------------------------------*/

#define AUDIO_DELAY_BUFF_SIZE       ( 9600U ) // Max 0.2s delay

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Filter */
AudioFilterLP_t xFilterLP = { 0U };

/* Delay */
float fDelayBuffer[AUDIO_DELAY_BUFF_SIZE] = { 0.0F };
AudioDelayCtrl_t xDelayCtrl = { 0U };

/* List of effect handlers */
effect_handler_t pxEffectList[AUDIO_EFFECT_SLOT_NUM] = { 0U };

/* Private function prototypes -----------------------------------------------*/

static float render_none(float fInputData)
{
    return 0.0F;
}

static float render_filter(float fInputData)
{
    return AUDIO_FILTER_LP_process(&xFilterLP, fInputData);
}

static float render_delay(float fInputData)
{
    return AUDIO_DELAY_process(&xDelayCtrl, fInputData);
}

/* Private function definition -----------------------------------------------*/
/* Public function definition ------------------------------------------------*/

void AUDIO_EFFECT_init(float fSampleRate)
{
    /* Init effect list */
    for ( uint32_t i = 0U; i < AUDIO_EFFECT_SLOT_NUM; i++ )
    {
        pxEffectList[i].bActive = false;
        pxEffectList[i].eId = AUDIO_EFFECT_NONE;
        pxEffectList[i].Render = render_none;
    }

    /* Initiate delay module */
    AUDIO_DELAY_init(&xDelayCtrl, fSampleRate, fDelayBuffer, AUDIO_DELAY_BUFF_SIZE);

    /* Initiate filter module */
    AUDIO_FILTER_LP_init(&xFilterLP);
    AUDIO_FILTER_LP_set_samplerate(&xFilterLP, fSampleRate);
}

audio_ret_t AUDIO_EFFECT_set_slot(audio_effect_slot_t eSlot, audio_effect_id_t eId)
{
    audio_ret_t eRetVal = AUDIO_PARAM_ERROR;

    if ( (eSlot < AUDIO_EFFECT_SLOT_NUM) && (eId < AUDIO_EFFECT_NUM) )
    {
        pxEffectList[eSlot].eId = eId;

        if ( eId == AUDIO_EFFECT_NONE )
        {
            pxEffectList[eSlot].bActive = false;
            pxEffectList[eSlot].Render = render_none;
        }
        else if ( eId == AUDIO_EFFECT_FILTER_LP )
        {
            pxEffectList[eSlot].Render = render_filter;
        }
        else if ( eId == AUDIO_EFFECT_DELAY )
        {
            pxEffectList[eSlot].Render = render_delay;
        }
        else
        {
            // None
        }

        eRetVal = AUDIO_OK;
    }

    return eRetVal;
}

audio_effect_id_t AUDIO_EFFECT_get_slot(audio_effect_slot_t eSlot)
{
    audio_effect_id_t eEffectId = AUDIO_EFFECT_NONE;

    if ( eSlot < AUDIO_EFFECT_SLOT_NUM )
    {
        eEffectId = pxEffectList[eSlot].eId;
    }

    return eEffectId;
}

bool AUDIO_EFFECT_get_slot_act(audio_effect_slot_t eSlot)
{
    bool bSlotActive = false;

    if ( eSlot < AUDIO_EFFECT_SLOT_NUM )
    {
        bSlotActive = pxEffectList[eSlot].bActive;
    }

    return bSlotActive;
}

audio_ret_t AUDIO_EFFECT_activate_slot(audio_effect_slot_t eSlot, bool bActive)
{
    audio_ret_t eRetVal = AUDIO_PARAM_ERROR;

    if ( eSlot < AUDIO_EFFECT_SLOT_NUM )
    {
        pxEffectList[eSlot].bActive = bActive;

        eRetVal = AUDIO_OK;
    }

    return eRetVal;
}

audio_ret_t AUDIO_EFFECT_parameter_update(audio_effect_parameter_id_t eId, float fNewValue)
{
    audio_ret_t eRetVal = AUDIO_PARAM_ERROR;

    if ( eId < AUDIO_EFFECT_PARAMETER_NUM )
    {
        if ( eId == AUDIO_EFFECT_PARAMETER_FILTER_FREQ )
        {
            eRetVal = AUDIO_FILTER_LP_set_frequency(&xFilterLP, fNewValue);
        }
        else if ( eId == AUDIO_EFFECT_PARAMETER_FILTER_Q )
        {
            eRetVal = AUDIO_FILTER_LP_set_q(&xFilterLP, fNewValue);
        }
        else if ( eId == AUDIO_EFFECT_PARAMETER_DELAY_TIME )
        {
            eRetVal = AUDIO_DELAY_update_delay(&xDelayCtrl, fNewValue);
        }
        else if ( eId == AUDIO_EFFECT_PARAMETER_DELAY_FB )
        {
            eRetVal = AUDIO_DELAY_update_feedback(&xDelayCtrl, fNewValue);
        }
        else
        {
            // None
        }
    }

    return eRetVal;
}

float AUDIO_EFFECT_parameter_get(audio_effect_parameter_id_t eId)
{
    float fValue = 0.0F;

    if ( eId < AUDIO_EFFECT_PARAMETER_NUM )
    {
        if ( eId == AUDIO_EFFECT_PARAMETER_FILTER_FREQ )
        {
            fValue = xFilterLP.fFrequency;
        }
        else if ( eId == AUDIO_EFFECT_PARAMETER_FILTER_Q )
        {
            fValue = xFilterLP.fQ;
        }
        else if ( eId == AUDIO_EFFECT_PARAMETER_DELAY_TIME )
        {
            fValue = xDelayCtrl.fDelayTime;
        }
        else if ( eId == AUDIO_EFFECT_PARAMETER_DELAY_FB )
        {
            fValue = xDelayCtrl.fFeedback;
        }
        else
        {
            // None
        }
    }

    return fValue;
}

float AUDIO_EFFECT_render(float fInputData)
{
    float fRenderData = fInputData;

    for ( uint32_t i = 0U; i < AUDIO_EFFECT_SLOT_NUM; i++ )
    {
        if ( pxEffectList[i].bActive )
        {
            fRenderData = pxEffectList[i].Render(fRenderData);
        }
    }

    return fRenderData;
}

/* EOF */