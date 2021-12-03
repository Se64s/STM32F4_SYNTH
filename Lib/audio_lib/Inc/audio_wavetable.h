/**
 * @file    audio_wavetable.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Wavetable library.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_WAVE_TABLE_H__
#define __AUDIO_WAVE_TABLE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Voice control structure.
 * 
 */
typedef struct AudiowaveTableVoice {
    bool bActive;                   /**< State of current voice */
    float fFreq;                    /**< Current frequency in Hz */
    float fCurrentSample;           /**< Index of current sample */
    float fMaxAmplitude;            /**< Max signal amplitude */
    float fAmplitude;               /**< Current amplitude */
    uint32_t u32SampleRate;         /**< Signal sample rate */
    const float *pu32WaveTable;     /**< Pointer to wave table in flash */
    uint32_t u32WaveTableSize;      /**< Size of wave table */
    audio_wave_id_t eWaveId;        /**< Selector of wave table */
} AudioWaveTableVoice_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initiate wavetable voice.
 * 
 * @param pVoice pointer to control structure to initialise.
 * @param u32SampleRate Sample rate in Hz.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_WAVE_init_voice(AudioWaveTableVoice_t *pVoice, uint32_t u32SampleRate, float fMaxAmplitude );

/**
 * @brief Select waveform for voice.
 * 
 * @param pVoice pointer to control structure to apply new wave table.
 * @param eWave wave id to use.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_WAVE_change_wave(AudioWaveTableVoice_t *pVoice, audio_wave_id_t eWave);

/**
 * @brief Update output frequency of wavetable voice.
 * 
 * @param pVoice pointer of control structure to update.
 * @param fFreq new frequency
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_WAVE_update_freq(AudioWaveTableVoice_t *pVoice, float fFreq);

/**
 * @brief Update voice amplitude
 * 
 * @param pVoice pointer of control structure to update.
 * @param fAmp new amplitude mod (0.0 to 1.0).
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_WAVE_update_amp(AudioWaveTableVoice_t *pVoice, float fAmp);

/**
 * @brief Set active state in provided voice.
 * 
 * @param pVoice pointer to voice to activate.
 * @param bState new state to set, tru voice on, false voice off.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_WAVE_set_active(AudioWaveTableVoice_t *pVoice, bool bState);

/**
 * @brief Get next sample of wave voice.
 * 
 * @param pVoice pointer of wavetable voice.
 * @return float next sample value.
 */
float AUDIO_WAVE_get_next_sample(AudioWaveTableVoice_t *pVoice);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_WAVE_TABLE_H__ */

/* EOF */
