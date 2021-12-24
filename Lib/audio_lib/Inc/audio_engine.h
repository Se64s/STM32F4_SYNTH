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


/* Defined engine commands  */
typedef enum {
    AUDIO_CMD_ACTIVATE_VOICE = 0x00,
    AUDIO_CMD_SET_FREQ,
    AUDIO_CMD_SET_MIDI_NOTE,
    AUDIO_CMD_SET_WAVEFORM,
    AUDIO_CMD_UPDATE_FILTER,
    AUDIO_CMD_UPDATE_DELAY,
    AUDIO_CMD_NUM_CMD
} audio_cmd_id_t;

/* Payload for each command */

typedef struct audio_cmd_act_voice {
    audio_voice_id_t eVoiceId;
    bool bState;
} audio_cmd_payload_act_voice_t;

typedef struct audio_cmd_set_freq {
    audio_voice_id_t eVoiceId;
    float fFreq;
} audio_cmd_payload_set_freq_t;

typedef struct audio_cmd_set_midi_note {
    audio_voice_id_t eVoiceId;
    uint8_t u8Note;
    uint8_t u8Velocity;
    bool bActive;
} audio_cmd_payload_set_midi_note_t;

typedef struct audio_cmd_set_wave {
    audio_voice_id_t eVoiceId;
    audio_wave_id_t eWaveId;
} audio_cmd_payload_set_wave_t;

typedef struct audio_cmd_update_filter {
    float fFreq;
    float fQ;
} audio_cmd_payload_update_filter_t;

typedef struct audio_cmd_update_delay {
    float fTime;
    float fFeedback;
} audio_cmd_payload_update_delay_t;

/* Gather all commands in same structure */

typedef union audio_cmd_payload {
    audio_cmd_payload_act_voice_t      xActVoice;
    audio_cmd_payload_set_freq_t       xSetFreq;
    audio_cmd_payload_set_midi_note_t  xSetMidiNote;
    audio_cmd_payload_set_wave_t       xSetWave;
    audio_cmd_payload_update_filter_t  xUpdateFilter;
    audio_cmd_payload_update_delay_t   xUpdateDelay;
} audio_cmd_payload_t;

/* Command base structure */

typedef struct audio_cmd {
    audio_cmd_id_t eCmdId;
    audio_cmd_payload_t xCmdPayload;
} audio_cmd_t;

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
 * @brief Handle new command in audio engine
 * 
 * @param xAudioCmd Command payload.
 * @return audio_ret_t Operation result.
 */
audio_ret_t AUDIO_handle_cmd(audio_cmd_t xAudioCmd);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_ENGINE_H__ */

/* EOF */
