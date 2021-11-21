/**
 * @file    midi_voice.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Handler for midi voices.
 * @version 0.1
 * @date    2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MIDI_VOICE_H
#define __MIDI_VOICE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

#include "midi_common.h"

/* Exported types ------------------------------------------------------------*/

/** Voice states */
typedef enum midi_voice_state {
    VOICE_STATE_NONE    = 0U,   /**< No change in voice. */
    VOICE_STATE_ON,             /**< Voice active. */
    VOICE_STATE_OFF,            /**< Voice idle. */
    VOICE_STATE_ERROR,          /**< Error processing state. */
    VOICE_STATE_NUM,            /**< Num states. */
} midi_voice_state_t;

/** CB to handle voice actions */
typedef void (*midi_voice_action_cb_t)(uint8_t u8Voice, uint8_t u8Note, uint8_t u8Velocity, midi_voice_state_t eAction);

/** Voice control data */
typedef struct midi_voice {
    uint8_t u8Ch;
    uint8_t u8Note;
    uint8_t u8Velocity;
    midi_voice_state_t eState;
} midi_voice_t;

/** Voice control data */
typedef struct midi_voice_handler {
    midi_voice_t *pxVoiceList;
    midiMode_t eMode;
    uint8_t u8VoiceNum;
    uint8_t u8MidiCh;
    midi_voice_action_cb_t xVoiceActionCB;
} midi_voice_handler_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initiate handler structure.
 * 
 * @param pxVoiceHandler pointer to voice handler control structure to initiate.
 * @param pxVoiceList pointer to the list of voices to use with handler.
 * @param u8VoiceNum number of voices in list.
 * @param u8MidiCh Assigned midi channel.
 * @param xActionCB Callback to handle actions derived of voice processing.
 * @return midiStatus_t Operation result.
 */
midiStatus_t VOICE_init(midi_voice_handler_t *pxVoiceHandler, midi_voice_t *pxVoiceList, uint8_t u8VoiceNum, uint8_t u8MidiCh, midi_voice_action_cb_t xActionCB);

/**
 * @brief Deinit control structure.
 * 
 * @param pxVoiceHandler pointer to the voice control structure to deinit.
 * @return midiStatus_t operation result.
 */
midiStatus_t VOICE_deinit(midi_voice_handler_t *pxVoiceHandler);

/**
 * @brief Reset all voices of control structure.
 * 
 * @param pxVoiceHandler pointer to handler vairable.
 * @return midiStatus_t operation result.
 */
midiStatus_t VOICE_reset(midi_voice_handler_t *pxVoiceHandler);

/**
 * @brief Update voice handler with new midi voice command.
 * 
 * @param pxVoiceHandler pointer to handler control structure.
 * @param u8Cmd New cmd Midi status.
 * @param u8Note New cmd Midi note.
 * @param u8Velocity New cmd Midi inout velocity.
 * @return midiStatus_t 
 */
midiStatus_t VOICE_update_note(midi_voice_handler_t *pxVoiceHandler, uint8_t u8Cmd, uint8_t u8Note, uint8_t u8Velocity);

/**
 * @brief Apply new midi mode to control voice structure.
 * 
 * @param pxVoiceHandler pointer to voice control structure to apply new midi mode.
 * @param u8NewMode New midi mode to apply.
 * @return midiStatus_t operation result.
 */
midiStatus_t VOICE_update_mode(midi_voice_handler_t *pxVoiceHandler, midiMode_t eNewMode);

/**
 * @brief Update midi channel used by voice handler.
 * 
 * @param pxVoiceHandler pointer to voice handler to update.
 * @param u8NewCh New midi channel to assign.
 * @return midiStatus_t operation result.
 */
midiStatus_t VOICE_update_ch(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewCh);

#ifdef __cplusplus
}
#endif

#endif /* __MIDI_VOICE_H */

/* EOF */
