/**
 * @file    audio_filter.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to handle filters.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_filter.h"
#include "arm_math.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// Default filter values
#define DEFAULT_LP_SAMPLERATE   ( 48000.0F )
#define DEFAULT_LP_FREQ         ( 1000.0F )
#define DEFAULT_LP_Q            ( 0.7F )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void filter_lp_compute_coef(AudioFilterLP_t *xFilter, float fFreq, float fQ);

/* Private function definition -----------------------------------------------*/

void filter_lp_compute_coef(AudioFilterLP_t *xFilter, float fFreq, float fQ)
{
    // Helper variables
    float w = fFreq * 2.0F * PI;
    float t = 1.0F / xFilter->fSampleRate;

    // Calculate coefficients
    float a0 = 4.0F + ( (w / fQ) * 2.0F * t ) + pow(w, 2.0F) * pow(t, 2.0F);

    xFilter->pfB[0U] = pow(w, 2.0) * pow(t, 2.0) / a0;
    xFilter->pfB[2U] = xFilter->pfB[0U];
    xFilter->pfB[1U] = pow(w, 2.0F) * 2.0F * pow(t, 2.0F) / a0;
    xFilter->pfA[0U] = ( ( 2.0F * pow(t, 2.0F) * pow(w, 2.0F) ) -8.0F ) / a0;
    xFilter->pfA[1U] = ( 4.0F - ( w / fQ * 2.0F * t ) + ( pow(w, 2.0F) * pow(t, 2.0F) ) ) / a0;

    xFilter->bReady = true;
}

/* Public function definition ------------------------------------------------*/

audio_ret_t AUDIO_FILTER_LP_init(AudioFilterLP_t *xFilter)
{
    ERR_ASSERT(xFilter != NULL);

    audio_ret_t eRetval = AUDIO_PARAM_ERROR;

    if ( xFilter != NULL )
    {
        (void)AUDIO_FILTER_LP_set_samplerate(xFilter, DEFAULT_LP_SAMPLERATE);
        (void)AUDIO_FILTER_LP_reset(xFilter);

        xFilter->fFrequency = DEFAULT_LP_FREQ;
        xFilter->fQ = DEFAULT_LP_Q;
        xFilter->bReady = false;

        eRetval = AUDIO_OK;
    }

    return eRetval;
}

audio_ret_t AUDIO_FILTER_LP_reset(AudioFilterLP_t *xFilter)
{
    ERR_ASSERT( xFilter != NULL );

    audio_ret_t eRetval = AUDIO_PARAM_ERROR;

    if ( xFilter != NULL )
    {
        for ( uint32_t iOrder = 0U; iOrder < AUDIO_FILTER_LP_RES_ORDER; iOrder++ )
        {
            xFilter->pfXn[iOrder] = 0.0F;
            xFilter->pfYn[iOrder] = 0.0F;
        }

        eRetval = AUDIO_OK;
    }

    return eRetval;
}

audio_ret_t AUDIO_FILTER_LP_set_samplerate(AudioFilterLP_t *xFilter, float fNewSamplerate)
{
    ERR_ASSERT(xFilter != NULL);

    audio_ret_t eRetval = AUDIO_PARAM_ERROR;

    if ( (xFilter != NULL) && (fNewSamplerate > 0.0F) )
    {
        xFilter->fSampleRate = fNewSamplerate;

        if ( xFilter->bReady )
        {
            filter_lp_compute_coef(xFilter, xFilter->fFrequency, xFilter->fQ);
        }

        eRetval = AUDIO_OK;
    }

    return eRetval;
}

audio_ret_t AUDIO_FILTER_LP_set_frequency(AudioFilterLP_t *xFilter, float fNewFrequency)
{
    ERR_ASSERT(xFilter != NULL);

    audio_ret_t eRetval = AUDIO_PARAM_ERROR;

    if ( (xFilter != NULL) && (fNewFrequency >= 0.0F) )
    {
        xFilter->fFrequency = fNewFrequency;

        filter_lp_compute_coef(xFilter, xFilter->fFrequency, xFilter->fQ);

        eRetval = AUDIO_OK;
    }

    return eRetval;
}

audio_ret_t AUDIO_FILTER_LP_set_q(AudioFilterLP_t *xFilter, float fNewQ)
{
    ERR_ASSERT(xFilter != NULL);

    audio_ret_t eRetval = AUDIO_PARAM_ERROR;

    if ( (xFilter != NULL) && (fNewQ >= 0.0F) )
    {
        xFilter->fQ = fNewQ;

        filter_lp_compute_coef(xFilter, xFilter->fFrequency, xFilter->fQ);

        eRetval = AUDIO_OK;
    }

    return eRetval;
}

float AUDIO_FILTER_LP_process(AudioFilterLP_t *xFilter, float fSampleIn)
{
    ERR_ASSERT(xFilter != NULL);

    float fSampleOut = fSampleIn;

    if ( xFilter->bReady )
    {
        /* y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] + a1*y[n-1] + a2*y[n-2] */
        fSampleOut = fSampleIn * xFilter->pfB[0U] + 
                    xFilter->pfXn[0U] * xFilter->pfB[1U] + xFilter->pfXn[1U] * xFilter->pfB[2U] - 
                    xFilter->pfYn[0U] * xFilter->pfA[0U] - xFilter->pfYn[1U] * xFilter->pfA[1U];
        
        /* Update filter state */
        xFilter->pfXn[1U] = xFilter->pfXn[0U];
        xFilter->pfXn[0U] = fSampleIn;
        xFilter->pfYn[1U] = xFilter->pfYn[0U];
        xFilter->pfYn[0U] = fSampleOut;
    }

    return fSampleOut;
}

/* EOF */