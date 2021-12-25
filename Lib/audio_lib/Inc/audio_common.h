/**
 * @file    audio_common.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Set of common definition use in audio modules.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_COMMON_H__
#define __AUDIO_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

// Error control module
#include "sys_error.h"

/* Exported types ------------------------------------------------------------*/

/** Audio lib return codes */
typedef enum {
    AUDIO_OK           = 0U,
    AUDIO_ERR          = 1U,
    AUDIO_PARAM_ERROR  = 2U,
} audio_ret_t;

/** Audio wave table ids */
typedef enum {
    AUDIO_WAVE_SAW    = 0U,
    AUDIO_WAVE_RSAW   = 1U,
    AUDIO_WAVE_SQR    = 2U,
    AUDIO_WAVE_SIN    = 3U,
    AUDIO_WAVE_TRI    = 4U,
    AUDIO_WAVE_NUM
} audio_wave_id_t;

/** Identifier for all available voices */
typedef enum audio_voice_id {
    AUDIO_VOICE_0 = 0U,
    AUDIO_VOICE_1 = 1U,
    AUDIO_VOICE_2 = 2U,
    AUDIO_VOICE_3 = 3U,
    AUDIO_VOICE_4 = 4U,
    AUDIO_VOICE_5 = 5U,
    AUDIO_VOICE_6 = 6U,
    AUDIO_VOICE_7 = 7U,
    AUDIO_VOICE_NUM
} audio_voice_id_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_COMMON_H__ */

/* EOF */
