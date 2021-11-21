/**
 * @file    midi_voice.c
 * @author  Sebastián Del Moral Gallardo
 * @brief   Module to handle midi voice updates.
 * @version 0.1
 * @date    2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "midi_voice.h"
#include "user_assert.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void clear_voice_list(midi_voice_handler_t *pxVoiceHandler);
static bool get_first_free_voice(midi_voice_handler_t *pxVoiceHandler, uint8_t *pu8VoiceCh);
static bool search_voice_with_state(midi_voice_handler_t *pxVoiceHandler, uint8_t u8Note, midi_voice_state_t eState, uint8_t *pu8VoiceCh);

static void handle_note_on_mono(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewNote, uint8_t u8NewVelocity, uint8_t u8Voice);
static void handle_note_on_poly(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewNote, uint8_t u8NewVelocity);

static void handle_note_off_mono(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewNote, uint8_t u8NewVelocity, uint8_t u8Voice);
static void handle_note_off_poly(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewNote, uint8_t u8NewVelocity);

/* Private function definition -----------------------------------------------*/

static void clear_voice_list(midi_voice_handler_t *pxVoiceHandler)
{
    ERR_ASSERT( pxVoiceHandler != NULL );

    for ( uint8_t i = 0U; i < pxVoiceHandler->u8VoiceNum; i++ )
    {
        pxVoiceHandler->pxVoiceList[i].u8Ch = i;
        pxVoiceHandler->pxVoiceList[i].u8Note = 0xFFU;
        pxVoiceHandler->pxVoiceList[i].u8Velocity = 0xFFU;
        pxVoiceHandler->pxVoiceList[i].eState = VOICE_STATE_OFF;
    }
}

static bool get_first_free_voice(midi_voice_handler_t *pxVoiceHandler, uint8_t *pu8VoiceCh)
{
    ERR_ASSERT( pxVoiceHandler != NULL );
    ERR_ASSERT( pu8VoiceCh != NULL );

    bool bFreeVoice = false;

    for ( uint8_t i = 0U; i < pxVoiceHandler->u8VoiceNum; i++ )
    {
        if ( pxVoiceHandler->pxVoiceList[i].eState == VOICE_STATE_OFF )
        {
            *pu8VoiceCh = pxVoiceHandler->pxVoiceList[i].u8Ch;
            bFreeVoice = true;
            break;
        }
    }

    return bFreeVoice;
}

static bool search_voice_with_state(midi_voice_handler_t *pxVoiceHandler, uint8_t u8Note, midi_voice_state_t eState, uint8_t *pu8VoiceCh)
{
    ERR_ASSERT( pxVoiceHandler != NULL );
    ERR_ASSERT( eState < VOICE_STATE_NUM );
    ERR_ASSERT( pu8VoiceCh != NULL );

    bool bFound = false;

    for ( uint8_t i = 0U; i < pxVoiceHandler->u8VoiceNum; i++ )
    {
        if ( (pxVoiceHandler->pxVoiceList[i].u8Note == u8Note) && (pxVoiceHandler->pxVoiceList[i].eState == eState) )
        {
            *pu8VoiceCh = pxVoiceHandler->pxVoiceList[i].u8Ch;
            bFound = true;
            break;
        }
    }

    return bFound;
}

/* ------------------------------------------------------------------------- */

static void handle_note_on_mono(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewNote, uint8_t u8NewVelocity, uint8_t u8Voice)
{
    if ( u8Voice < pxVoiceHandler->u8VoiceNum )
    {
        /* Check if current voice is IDLE or new note is over the current active note (old school monofonic) */
        if ( (pxVoiceHandler->pxVoiceList[u8Voice].eState == VOICE_STATE_OFF) ||
            ((pxVoiceHandler->pxVoiceList[u8Voice].eState == VOICE_STATE_ON) && (pxVoiceHandler->pxVoiceList[u8Voice].u8Note < u8NewNote)) )
        {
            pxVoiceHandler->pxVoiceList[u8Voice].eState = VOICE_STATE_ON;
            pxVoiceHandler->pxVoiceList[u8Voice].u8Note = u8NewNote;
            pxVoiceHandler->pxVoiceList[u8Voice].u8Velocity = u8NewVelocity;

            // Call callback to trigger action.
            if ( pxVoiceHandler->xVoiceActionCB != NULL )
            {
                pxVoiceHandler->xVoiceActionCB(u8Voice, u8NewNote, u8NewVelocity, VOICE_STATE_ON);
            }
        }
    }
}

static void handle_note_on_poly(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewNote, uint8_t u8NewVelocity)
{
    uint8_t u8VoiceId = 0U;
    
    // Check if note already active
    if ( !search_voice_with_state(pxVoiceHandler, u8NewNote, VOICE_STATE_ON, &u8VoiceId) )
    {
        // Search for a free slot
        if ( get_first_free_voice(pxVoiceHandler, &u8VoiceId) )
        {
            // Free voice slot found, assign new note to free slot
            if ( u8VoiceId < pxVoiceHandler->u8VoiceNum )
            {
                pxVoiceHandler->pxVoiceList[u8VoiceId].eState = VOICE_STATE_ON;
                pxVoiceHandler->pxVoiceList[u8VoiceId].u8Note = u8NewNote;
                pxVoiceHandler->pxVoiceList[u8VoiceId].u8Velocity = u8NewVelocity;

                // Call callback to trigger action.
                if ( pxVoiceHandler->xVoiceActionCB != NULL )
                {
                    pxVoiceHandler->xVoiceActionCB(u8VoiceId, u8NewNote, u8NewVelocity, VOICE_STATE_ON);
                }
            }
        }
    }
}

static void handle_note_off_mono(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewNote, uint8_t u8NewVelocity, uint8_t u8Voice)
{
    if ( u8Voice < pxVoiceHandler->u8VoiceNum )
    {
        /* Check if current voice is IDLE or new note is over the current active note (old school monofonic) */
        if ( (pxVoiceHandler->pxVoiceList[u8Voice].eState == VOICE_STATE_ON) && (pxVoiceHandler->pxVoiceList[u8Voice].u8Note == u8NewNote) )
        {
            pxVoiceHandler->pxVoiceList[u8Voice].eState = VOICE_STATE_OFF;
            pxVoiceHandler->pxVoiceList[u8Voice].u8Note = 0U;
            pxVoiceHandler->pxVoiceList[u8Voice].u8Velocity = 0U;

            // Call callback to trigger action.
            if ( pxVoiceHandler->xVoiceActionCB != NULL )
            {
                pxVoiceHandler->xVoiceActionCB(u8Voice, u8NewNote, u8NewVelocity, VOICE_STATE_OFF);
            }
        }
    }
}

static void handle_note_off_poly(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewNote, uint8_t u8NewVelocity)
{
    uint8_t u8VoiceId = 0U;

    // Check if note already active
    if ( search_voice_with_state(pxVoiceHandler, u8NewNote, VOICE_STATE_ON, &u8VoiceId) )
    {
        if ( u8VoiceId < pxVoiceHandler->u8VoiceNum )
        {
            // Free voice slot
            pxVoiceHandler->pxVoiceList[u8VoiceId].eState = VOICE_STATE_OFF;
            pxVoiceHandler->pxVoiceList[u8VoiceId].u8Note = 0U;
            pxVoiceHandler->pxVoiceList[u8VoiceId].u8Velocity = 0U;

            // Call callback to trigger action.
            if ( pxVoiceHandler->xVoiceActionCB != NULL )
            {
                pxVoiceHandler->xVoiceActionCB(u8VoiceId, u8NewNote, u8NewVelocity, VOICE_STATE_OFF);
            }
        }
    }
}

/* Public function prototypes -----------------------------------------------*/

midiStatus_t VOICE_init(midi_voice_handler_t *pxVoiceHandler, midi_voice_t *pxVoiceList, uint8_t u8VoiceNum, uint8_t u8MidiCh, midi_voice_action_cb_t xActionCB)
{
    ERR_ASSERT( pxVoiceHandler != NULL );
    ERR_ASSERT( pxVoiceList != NULL );
    ERR_ASSERT( xActionCB != NULL );
    ERR_ASSERT( u8VoiceNum != 0U );
    ERR_ASSERT( u8MidiCh <= MIDI_CHANNEL_MAX_VALUE );

    midiStatus_t xRetval = midiBadParam;

    if ( (pxVoiceHandler != NULL) && 
        (pxVoiceList != NULL) && 
        (u8VoiceNum != 0U) && 
        (xActionCB != 0U) &&
        (u8MidiCh <= MIDI_CHANNEL_MAX_VALUE) )
    {
        pxVoiceHandler->eMode = MidiMode1;
        pxVoiceHandler->pxVoiceList = pxVoiceList;
        pxVoiceHandler->u8VoiceNum = u8VoiceNum;
        pxVoiceHandler->xVoiceActionCB = xActionCB;
        pxVoiceHandler->u8MidiCh = u8MidiCh;

        clear_voice_list(pxVoiceHandler);

        xRetval = midiOk;
    }

    return xRetval;
}

midiStatus_t VOICE_deinit(midi_voice_handler_t *pxVoiceHandler)
{
    ERR_ASSERT( pxVoiceHandler != NULL );

    midiStatus_t xRetval = midiBadParam;

    if ( (pxVoiceHandler != NULL) )
    {
        pxVoiceHandler->eMode = MidiModeNum;
        pxVoiceHandler->pxVoiceList = NULL;
        pxVoiceHandler->u8VoiceNum = 0U;
        pxVoiceHandler->xVoiceActionCB = NULL;

        xRetval = midiOk;
    }

    return xRetval;
}

midiStatus_t VOICE_reset(midi_voice_handler_t *pxVoiceHandler)
{
    ERR_ASSERT( pxVoiceHandler != NULL );

    midiStatus_t xRetval = midiBadParam;

    if ( (pxVoiceHandler != NULL) && (pxVoiceHandler->u8VoiceNum != 0U) )
    {
        clear_voice_list(pxVoiceHandler);

        xRetval = midiOk;
    }

    return xRetval;
}

midiStatus_t VOICE_update_note(midi_voice_handler_t *pxVoiceHandler, uint8_t u8Cmd, uint8_t u8Note, uint8_t u8Velocity)
{
    ERR_ASSERT( pxVoiceHandler != NULL );

    midiStatus_t xRetval = midiBadParam;

    if ( pxVoiceHandler != NULL )
    {
        uint8_t u8Status = MIDI_CMD_GET_STATUS(u8Cmd);
        uint8_t u8Ch = MIDI_CMD_GET_CH(u8Cmd);

        /*
         * Handle mode1 (poly, all channels) and mode2 (mono, all channels)
         */
        if ( (pxVoiceHandler->eMode == MidiMode1) || (pxVoiceHandler->eMode == MidiMode2) )
        {
            if ( (u8Status == MIDI_STATUS_NOTE_ON) && (u8Velocity != 0U) )
            {
                if ( pxVoiceHandler->eMode == MidiMode1 )
                {
                    handle_note_on_poly(pxVoiceHandler, u8Note, u8Velocity);
                    xRetval = midiOk;
                }
                else if ( pxVoiceHandler->eMode == MidiMode2 )
                {
                    handle_note_on_mono(pxVoiceHandler, u8Note, u8Velocity, 0U);
                    xRetval = midiOk;
                }
            }
            else if ( (u8Status == MIDI_STATUS_NOTE_OFF) || 
                    ((u8Status == MIDI_STATUS_NOTE_ON) && (u8Velocity == 0U)) )
            {
                if ( pxVoiceHandler->eMode == MidiMode1 )
                {
                    handle_note_off_poly(pxVoiceHandler, u8Note, u8Velocity);
                    xRetval = midiOk;
                }
                else if ( pxVoiceHandler->eMode == MidiMode2 )
                {
                    handle_note_off_mono(pxVoiceHandler, u8Note, u8Velocity, 0U);
                    xRetval = midiOk;
                }
            }
        }
        /*
         * Handle mode3 (poly, channel filtering) and mode4 (mono, channel filtering from n to n + num_voices)
         */
        else if ( (pxVoiceHandler->eMode == MidiMode3) || (pxVoiceHandler->eMode == MidiMode4) )
        {
            if ( ((pxVoiceHandler->eMode == MidiMode3) && (u8Ch == pxVoiceHandler->u8MidiCh)) ||
                 ((pxVoiceHandler->eMode == MidiMode4) && ((u8Ch >= pxVoiceHandler->u8MidiCh) && (u8Ch < (pxVoiceHandler->u8MidiCh + pxVoiceHandler->u8VoiceNum)))))
            {
                if ( (u8Status == MIDI_STATUS_NOTE_ON) && (u8Velocity != 0U) )
                {
                    if ( pxVoiceHandler->eMode == MidiMode3 )
                    {
                        handle_note_on_poly(pxVoiceHandler, u8Note, u8Velocity);
                        xRetval = midiOk;
                    }
                    else if ( pxVoiceHandler->eMode == MidiMode4 )
                    {
                        uint8_t u8Voice = u8Ch - pxVoiceHandler->u8MidiCh;
                        handle_note_on_mono(pxVoiceHandler, u8Note, u8Velocity, u8Voice);
                        xRetval = midiOk;
                    }
                }
                else if ( (u8Status == MIDI_STATUS_NOTE_OFF) || 
                        ((u8Status == MIDI_STATUS_NOTE_ON) && (u8Velocity == 0U)) )
                {
                    if ( pxVoiceHandler->eMode == MidiMode3 )
                    {
                        handle_note_off_poly(pxVoiceHandler, u8Note, u8Velocity);
                        xRetval = midiOk;
                    }
                    else if ( pxVoiceHandler->eMode == MidiMode4 )
                    {
                        uint8_t u8Voice = u8Ch - pxVoiceHandler->u8MidiCh;
                        handle_note_off_mono(pxVoiceHandler, u8Note, u8Velocity, u8Voice);
                        xRetval = midiOk;
                    }
                }
            }
        }
    }

    return xRetval;
}

midiStatus_t VOICE_update_mode(midi_voice_handler_t *pxVoiceHandler, midiMode_t eNewMode)
{
    ERR_ASSERT( pxVoiceHandler != NULL );
    ERR_ASSERT( eNewMode <  MidiModeNum );

    midiStatus_t xRetval = midiBadParam;

    if ( (pxVoiceHandler != NULL) && (eNewMode < MidiModeNum) )
    {
        pxVoiceHandler->eMode = eNewMode;

        clear_voice_list(pxVoiceHandler);

        xRetval = midiOk;
    }

    return xRetval;
}

midiStatus_t VOICE_update_ch(midi_voice_handler_t *pxVoiceHandler, uint8_t u8NewCh)
{
    ERR_ASSERT( pxVoiceHandler != NULL );
    ERR_ASSERT( u8NewCh <=  MIDI_CHANNEL_MAX_VALUE );

    midiStatus_t xRetval = midiBadParam;

    if ( (pxVoiceHandler != NULL) && (u8NewCh <=  MIDI_CHANNEL_MAX_VALUE) )
    {
        pxVoiceHandler->u8MidiCh = u8NewCh;

        xRetval = midiOk;
    }

    return xRetval;
}

/* EOF */
