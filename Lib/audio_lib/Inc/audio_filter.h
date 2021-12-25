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
typedef struct AudioFilterLP {
    float pfA[AUDIO_FILTER_LP_RES_NUM_A_COEF];  /**< Array of A coefficients: pfA[0] = a1, pfA[1] = a2, ...           */
    float pfYn[AUDIO_FILTER_LP_RES_ORDER];      /**< Y prev values:           pfYn[0] = y[n-1], pfYn[1] = y[n-2], ... */
    float pfB[AUDIO_FILTER_LP_RES_NUM_B_COEF];  /**< Array of B coefficients: pfB[0] = b0, pfB[1] = b1, ...           */
    float pfXn[AUDIO_FILTER_LP_RES_ORDER];      /**< X prev values:           pfXn[0] = x[n-1], pfXn[1] = x[n-2], ... */
    float fSampleRate;                          /**< Filter sample rate */
    float fFrequency;                           /**< Cutoff frequency */
    float fQ;                                   /**< Q parameter */
    bool bReady;                                /**< Ready for render */
} AudioFilterLP_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init filter structure
 * 
 * @param xFilter pointer to filter structure.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_FILTER_LP_init(AudioFilterLP_t *xFilter);

/**
 * @brief Reset samples from filter structure.
 * 
 * @param xFilter pointer to filter structure.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_FILTER_LP_reset(AudioFilterLP_t *xFilter);

/**
 * @brief Set new sample rate to filter structure and recompute coeficients.
 * 
 * @param xFilter pointer to filter structure.
 * @param fNewSamplerate new sample to set into filter structure.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_FILTER_LP_set_samplerate(AudioFilterLP_t *xFilter, float fNewSamplerate);

/**
 * @brief Set new filter cutoff frequency.
 * 
 * @param xFilter pointer to filter structure.
 * @param fNewFrequency new cutoff value for filter.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_FILTER_LP_set_frequency(AudioFilterLP_t *xFilter, float fNewFrequency);

/**
 * @brief Set new Q value to filter.
 * 
 * @param xFilter pointer to filtar structure.
 * @param fNewQ New value for filter.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_FILTER_LP_set_q(AudioFilterLP_t *xFilter, float fNewQ);

/**
 * @brief Render new value passing over filter structure.
 * 
 * @param xFilter pointer to filter structure.
 * @param fSampleIn input sample.
 * @return float output filtered sample.
 */
float AUDIO_FILTER_LP_process(AudioFilterLP_t *xFilter, float fSampleIn);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_FILTER_H__ */

/* EOF */
