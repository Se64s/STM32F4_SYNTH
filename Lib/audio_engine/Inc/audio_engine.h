/**
 * @file    audio_engine.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Engine to handle audio generation in external dac.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_ENGINE_H__
#define __AUDIO_ENGINE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

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

/** Waveform ids */
typedef enum {
    AUDIO_WAVE_SAW  = 0U,
    AUDIO_WAVE_RSAW = 1U,
    AUDIO_WAVE_SQR  = 2U,
    AUDIO_WAVE_SIN  = 3U,
    AUDIO_WAVE_TRI  = 4U,
    AUDIO_WAVE_NUM
} audio_wave_id_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Uncomment to enable GPIO toggle for transfer time tracking */
#define AUDIO_TRASNFER_TRACE

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initiate audio engine resources.
 * 
 * @return sys_state_t operation result.
 */
sys_state_t AUDIO_init(void);

/**
 * @brief Release audio engine resources.
 * 
 * @return sys_state_t operation result.
 */
sys_state_t AUDIO_deinit(void);

/**
 * @brief Update frquency of voice.
 * 
 * @param eVoice voice id to update.
 * @param bState new state, true for enable outrput, folse for disable output.
 * @return sys_state_t operation result.
 */
sys_state_t AUDIO_voice_set_state(audio_voice_id_t eVoice, bool bState);

/**
 * @brief Update voice frequency.
 * 
 * @param eVoice voice id to update.
 * @param fFreq new frequency to apply on voice.
 * @return sys_state_t operation result.
 */
sys_state_t AUDIO_voice_set_freq(audio_voice_id_t eVoice, float fFreq);

/**
 * @brief Change current voice waveform.
 * 
 * @param eVoice voice id to update.
 * @param eWaveId waveform id to apply.
 * @return sys_state_t operation result.
 */
sys_state_t AUDIO_voice_set_waveform(audio_voice_id_t eVoice, audio_wave_id_t eWaveId);

/**
 * @brief Update all waveform parameters
 * 
 * @param eVoice voice id to update.
 * @param eWaveId waveform id to apply.
 * @param fFreq new frequency to apply on voice.
 * @param bState new state, true for enable outrput, folse for disable output.
 * @return sys_state_t 
 */
sys_state_t AUDIO_voice_update(audio_voice_id_t eVoice, audio_wave_id_t eWaveId, float fFreq, bool bState);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_ENGINE_H__ */

/* EOF */
