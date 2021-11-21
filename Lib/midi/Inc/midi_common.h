/**
 * @file    midi_common.h
 * @author  Sebastián Del Moral
 * @brief   Common defines and constants used in midi modules.
 * @version 0.1
 * @date    2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MIDI_COMMON_H
#define __MIDI_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/

/** Midi modes */
typedef enum midiMode {
    MidiMode1 = 0x00,       /**< Omni-on Poly. */
    MidiMode2,              /**< Omni-on Mono. */
    MidiMode3,              /**< Omni-off Poly. */
    MidiMode4,              /**< Omni-off Mono. */
    MidiModeNum,            /**< Number of defined modes. */
} midiMode_t;

/** Defined midi lib status */
typedef enum midiStatus {
    midiOk = 0x00,          /**< Midi operation OK. */
    midiSysExcBuffFull,     /**< Execution buffer full. */
    midiHandleNewState,     /**< New state to handle. */
    midiBadParam,           /**< Bad parameter used. */
    midiError,              /**< Error on processing. */
} midiStatus_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define MIDI_CMD_GET_STATUS(cmd)    (uint8_t)( ( cmd ) & MIDI_STATUS_CMD_MASK )
#define MIDI_CMD_GET_CH(cmd)        (uint8_t)( ( cmd ) & MIDI_STATUS_CH_MASK )

/* Exported defines ----------------------------------------------------------*/
/* Midi data value not valid */
#define MIDI_DATA_NOT_VALID         0xFFU

/* MIDI maximun channel value */
#define MIDI_CHANNEL_MAX_VALUE      0x0FU

/* MIDI maximun program value */
#define MIDI_PROGRAM_MAX_VALUE      0x0FU

/* MIDI maximun BANK value */
#define MIDI_BANK_MAX_VALUE         0x0FU

/* MIDI status cmd with 1 bytes */
#define MIDI_STATUS_PROG_CHANGE     0xC0U
#define MIDI_STATUS_CH_PRESS        0xD0U
#define MIDI_STATUS_TIME_CODE       0xF1U
#define MIDI_STATUS_SONG_SELECT     0xF3U

/* MIDI status cmd with 2 bytes */
#define MIDI_STATUS_NOTE_OFF        0x80U
#define MIDI_STATUS_NOTE_ON         0x90U
#define MIDI_STATUS_POLY_PRESS      0xA0U
#define MIDI_STATUS_CC              0xB0U
#define MIDI_STATUS_PITCH_BEND      0xE0U
#define MIDI_STATUS_SONG_POS        0xF2U

/* Mask to get channel from status messages */
#define MIDI_STATUS_CH_MASK         0x0FU
#define MIDI_STATUS_CMD_MASK        0xF0U

/* CC types */
#define MIDI_CC_MOD                 0x01U
#define MIDI_CC_C15                 0x0FU
#define MIDI_CC_C20                 0x14U
#define MIDI_CC_C21                 0x15U
#define MIDI_CC_C22                 0x16U
#define MIDI_CC_C23                 0x17U
#define MIDI_CC_C24                 0x18U
#define MIDI_CC_C25                 0x19U
#define MIDI_CC_C26                 0x1AU
#define MIDI_CC_C27                 0x1BU
#define MIDI_CC_C52                 0x34U
#define MIDI_CC_C53                 0x35U
#define MIDI_CC_C54                 0x36U
#define MIDI_CC_C55                 0x37U
#define MIDI_CC_C56                 0x38U
#define MIDI_CC_C57                 0x39U
#define MIDI_CC_C58                 0x3AU
#define MIDI_CC_C59                 0x3BU
#define MIDI_CC_C60                 0x3CU
#define MIDI_CC_C61                 0x3DU
#define MIDI_CC_C62                 0x3EU
#define MIDI_CC_BRI                 0x4AU
#define MIDI_CC_HAR                 0x47U
#define MIDI_CC_ATT                 0x49U
#define MIDI_CC_NOTE_OFF            0x7BU

#define MIDI_CC_USER_0_INI          0x14U
#define MIDI_CC_USER_0_END          0x1FU

#define MIDI_CC_USER_1_INI          0x34U
#define MIDI_CC_USER_1_END          0x3FU

#define MIDI_CC_USER_2_INI          0x55U
#define MIDI_CC_USER_2_END          0x5AU

#define MIDI_CC_USER_3_INI          0x66U
#define MIDI_CC_USER_3_END          0x77U

/* MIDI RT */
#define MIDI_RT_MASK                0xF8U
#define MIDI_RT_CLK                 0xF8U
#define MIDI_RT_START               0xFAU
#define MIDI_RT_CONT                0xFBU
#define MIDI_RT_STOP                0xFCU
#define MIDI_RT_RESET               0xFFU

/* MIDI SYS_EX */
#define MIDI_STATUS_SYS_EX_START    0xF0U
#define MIDI_STATUS_SYS_EX_END      0xF7U

/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MIDI_COMMON_H */

/* EOF */
