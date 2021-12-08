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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function definition -----------------------------------------------*/
/* Public function definition ------------------------------------------------*/

void AUDIO_FILTER_compute_coefficients_LP_RES(AudioFilter2ndOrder_t *xFilter, float fSampleRate, float fFreq, float fQ)
{
    ERR_ASSERT(xFilter != NULL);

    float k = tanf(PI * fFreq / fSampleRate);
    float norm = 1.0F / (1.0F + k / fQ + k * k);

    xFilter->pfB[0U] = k * k * norm;
    xFilter->pfB[1U] = 2.0 * xFilter->pfB[0U];
    xFilter->pfB[2U] = xFilter->pfB[0U];

    xFilter->pfA[0U] = 2.0F * (k * k - 1.0F) * norm;
    xFilter->pfA[1U] = (1.0F - k / fQ + k * k) * norm;
}

float AUDIO_FILTER_2nd_order_render(AudioFilter2ndOrder_t *xFilter, float fSampleIn)
{
    ERR_ASSERT(xFilter != NULL);

    /* y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] + a1*y[n-1] + a2*y[n-2] */
    float fSampleOut = xFilter->pfB[0U] * fSampleIn + 
                        xFilter->pfB[1U] * xFilter->pfXn[0U] + 
                        xFilter->pfB[2U] * xFilter->pfXn[1U] - 
                        xFilter->pfA[0U] * xFilter->pfYn[0U] - 
                        xFilter->pfA[1U] * xFilter->pfYn[1U];

    /* Update filter state */
    xFilter->pfXn[1U] = xFilter->pfXn[0U];
    xFilter->pfXn[0U] = fSampleIn;
    xFilter->pfYn[1U] = xFilter->pfYn[0U];
    xFilter->pfYn[0U] = fSampleOut;

    return fSampleOut;
}

/* EOF */