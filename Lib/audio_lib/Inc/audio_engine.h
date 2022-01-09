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
#include "audio_effect.h"

/* Exported types ------------------------------------------------------------*/

/* Defined engine commands  */
typedef enum {
    AUDIO_CMD_ACTIVATE_VOICE = 0x00,
    AUDIO_CMD_SET_FREQ,
    AUDIO_CMD_SET_MIDI_NOTE,
    AUDIO_CMD_SET_WAVEFORM,
    AUDIO_CMD_SET_DETUNE,
    AUDIO_CMD_SET_ADSR,
    AUDIO_CMD_EFFECT_UPDATE,
    AUDIO_CMD_EFFECT_SET_SLOT,
    AUDIO_CMD_EFFECT_ACTIVATE,
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

typedef struct audio_cmd_set_detune {
    audio_voice_id_t eVoiceId;
    float fDetuneLvl;
} audio_cmd_set_detune_t;

typedef struct audio_cmd_set_adsr {
    audio_voice_id_t eVoiceId;
    float fAttackTime;
    float fDecayTime;
    float fReleaseTime;
    float fAttackLvl;
    float fSustainLvl;
} audio_cmd_set_adsr_t;

typedef struct audio_cmd_effect_update {
    audio_effect_parameter_id_t eParamId;
    float fNewValue;
} audio_cmd_effect_update_t;

typedef struct audio_cmd_effect_set_slot {
    audio_effect_slot_t eSlot;
    audio_effect_id_t eEffectId;
} audio_cmd_effect_set_slot_t;

typedef struct audio_cmd_effect_activate_slot {
    audio_effect_slot_t eSlot;
    bool bActive;
} audio_cmd_effect_activate_slot_t;

/* Gather all commands in same structure */

typedef union audio_cmd_payload {
    audio_cmd_payload_act_voice_t      xActVoice;
    audio_cmd_payload_set_freq_t       xSetFreq;
    audio_cmd_payload_set_midi_note_t  xSetMidiNote;
    audio_cmd_payload_set_wave_t       xSetWave;
    audio_cmd_set_detune_t             xSetDetune;
    audio_cmd_set_adsr_t               xSetAdsr;
    audio_cmd_effect_update_t          xEffectUpdate;
    audio_cmd_effect_set_slot_t        xSetSlot;
    audio_cmd_effect_activate_slot_t   xActivateSlot;
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
