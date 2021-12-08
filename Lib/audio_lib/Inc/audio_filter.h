/**
 * @file    audio_filter.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to generate filters.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_FILTER_H__
#define __AUDIO_FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_common.h"

/* Exported types ------------------------------------------------------------*/

#define AUDIO_FILTER_LP_RES_NUM_A_COEF      ( 2U )
#define AUDIO_FILTER_LP_RES_NUM_B_COEF      ( 3U )
#define AUDIO_FILTER_LP_RES_ORDER           ( 2U )

/* Exported constants --------------------------------------------------------*/

/**
 * @brief Control structure to handle Sencond order LP filters.
 * 
 */
typedef struct AudioFilter2ndOrder {
    float pfA[AUDIO_FILTER_LP_RES_NUM_A_COEF];  /**< Array of A coefficients: pfA[0] = a1, pfA[1] = a2, ...           */
    float pfYn[AUDIO_FILTER_LP_RES_ORDER];      /**< Y prev values:           pfYn[0] = y[n-1], pfYn[1] = y[n-2], ... */
    float pfB[AUDIO_FILTER_LP_RES_NUM_B_COEF];  /**< Array of B coefficients: pfB[0] = b0, pfB[1] = b1, ...           */
    float pfXn[AUDIO_FILTER_LP_RES_ORDER];      /**< X prev values:           pfXn[0] = x[n-1], pfXn[1] = x[n-2], ... */
} AudioFilter2ndOrder_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Compute new coefficients for LP res filter.
 * 
 * @param xFilter Filter control structure.
 * @param fSampleRate Audio sample rate of adio source.
 * @param fFreq Cutting frequency of the filter.
 * @param fQ Q factor of the filter.
 */
void AUDIO_FILTER_compute_coefficients_LP_RES(AudioFilter2ndOrder_t *xFilter, float fSampleRate, float fFreq, float fQ);

/**
 * @brief Update filter state with an input sample.
 * 
 * @param xFilter pointer to filter structure.
 * @param fSampleIn Sample in.
 * @return float Output sample.
 */
float AUDIO_FILTER_2nd_order_render(AudioFilter2ndOrder_t *xFilter, float fSampleIn);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_FILTER_H__ */

/* EOF */
