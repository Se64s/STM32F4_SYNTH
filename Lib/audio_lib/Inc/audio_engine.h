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
#include "audio_common.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initiate audio engine resources.
 * 
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_init(void);

/**
 * @brief Release audio engine resources.
 * 
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_deinit(void);

/**
 * @brief Update frquency of voice.
 * 
 * @param eVoice voice id to update.
 * @param bState new state, true for enable outrput, folse for disable output.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_voice_set_state(audio_voice_id_t eVoice, bool bState);

/**
 * @brief Update voice frequency.
 * 
 * @param eVoice voice id to update.
 * @param fFreq new frequency to apply on voice.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_voice_set_freq(audio_voice_id_t eVoice, float fFreq);

/**
 * @brief Set voice frequency from midi note.
 * 
 * @param eVoice voice to update.
 * @param u8MidiNote midi note.
 * @param u8MidiVel midi velocity.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_voice_set_midi_note(audio_voice_id_t eVoice, uint8_t u8MidiNote, uint8_t u8MidiVel);

/**
 * @brief Change current voice waveform.
 * 
 * @param eVoice voice id to update.
 * @param eWaveId waveform id to apply.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_voice_set_waveform(audio_voice_id_t eVoice, audio_wave_id_t eWaveId);

/**
 * @brief Update all waveform parameters
 * 
 * @param eVoice voice id to update.
 * @param eWaveId waveform id to apply.
 * @param fFreq new frequency to apply on voice.
 * @param bState new state, true for enable outrput, folse for disable output.
 * @return audio_ret_t 
 */
audio_ret_t AUDIO_voice_update(audio_voice_id_t eVoice, audio_wave_id_t eWaveId, float fFreq, bool bState);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_ENGINE_H__ */

/* EOF */
