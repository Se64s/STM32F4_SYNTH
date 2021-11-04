/**
 * @file    wavetable.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Wavetable library.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WAVE_TABLE_H__
#define __WAVE_TABLE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stddef.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Wave table wave id
 * 
 */
typedef enum {
    WAVE_SAW    = 0U,
    WAVE_RSAW   = 1U,
    WAVE_SQR    = 2U,
    WAVE_SIN    = 3U,
    WAVE_TRI    = 4U,
    WAVE_NUM
} wave_id_t;

/**
 * @brief Wave return code.
 * 
 */
typedef enum {
    WAVE_OK     = 0U,
    WAVE_ERR    = 1U,
} wave_ret_t;

/**
 * @brief Voice control structure.
 * 
 */
typedef struct waveTableVoice {
    float fFreq;
    float fCurrentSample;
    float fMaxAmplitude;
    uint32_t u32SampleRate;
    const float *pu32WaveTable;
    uint32_t u32WaveTableSize;
    wave_id_t eWaveId;
} waveTableVoice_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initiate wavetable voice.
 * 
 * @param pVoice pointer to control structure to initialise.
 * @param u32SampleRate Sample rate in Hz.
 * @return wave_ret_t operation result.
 */
wave_ret_t WAVE_init_voice(waveTableVoice_t *pVoice, uint32_t u32SampleRate, float fMaxAmplitude );

/**
 * @brief Select waveform for voice.
 * 
 * @param pVoice pointer to control structure to apply new wave table.
 * @param eWave wave id to use.
 * @return wave_ret_t operation result.
 */
wave_ret_t WAVE_change_wave(waveTableVoice_t *pVoice, wave_id_t eWave);

/**
 * @brief Update output frequency of wavetable voice.
 * 
 * @param pVoice pointer of control structure to update.
 * @param fFreq new frequency
 * @return wave_ret_t operation result.
 */
wave_ret_t WAVE_update_freq(waveTableVoice_t *pVoice, float fFreq);

/**
 * @brief Get next sample of wave voice.
 * 
 * @param pVoice pointer of wavetable voice.
 * @return float next sample value.
 */
float WAVE_get_next_sample(waveTableVoice_t *pVoice);

#ifdef __cplusplus
}
#endif
#endif /*__WAVE_TABLE_H__ */

/* EOF */
