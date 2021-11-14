/**
 * @file    midi_lib.h
 * @author  Sebastian Del Moral
 * @brief   Library for handling and parsing midi messages.
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MIDI_LIB_H
#define __MIDI_LIB_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>

/* Exported types ------------------------------------------------------------*/

/** Midi modes */
typedef enum {
    MidiMode1 = 0x00,       /**< Omni-on Poly. */
    MidiMode2,              /**< Omni-on Mono. */
    MidiMode3,              /**< Omni-off Poly. */
    MidiMode4,              /**< Omni-off Mono. */
    MidiModeNum,            /**< Number of defined modes. */
} midiMode_t;

/** Defined midi lib status */
typedef enum {
    midiOk = 0x00,          /**< Midi operation OK. */
    midiSysExcBuffFull,     /**< Execution buffer full. */
    midiHandleNewState,     /**< New state to handle. */
    midiError,              /**< Error on processing. */
} midiStatus_t;

/** CB for system exclusive messages */
typedef void (*sys_ex_cb_t)(uint8_t *pdata, uint32_t len_data);

/** CB for handle 1 data message */
typedef void (*data1_cb_t)(uint8_t cmd, uint8_t data);

/** CB for handle 2 data message */
typedef void (*data2_cb_t)(uint8_t cmd, uint8_t data0, uint8_t data1);

/** CB for handle rt message */
typedef void (*rt_cb_t)(uint8_t rt_data);

/** Internal control variables use by midi handlers */
typedef struct midi_ctrl_vars {
    uint8_t     tmp_running_status;
    uint8_t     tmp_data_1;
    uint8_t     tmp_data_2;
    uint32_t    i_data_sys_ex;
    uint8_t     *sys_ex_buff;
    uint32_t    sys_ex_size;
    uint32_t    fsm_state;
} midi_ctrl_vars_t;

/** Midi interface control structure */
typedef struct midi_handler {
    // User CB
    sys_ex_cb_t         midi_sys_ex_cb;
    data1_cb_t          midi_data_1_cb;
    data2_cb_t          midi_data_2_cb;
    rt_cb_t             midi_rt_cb;
    // Internal status variables - not modify by the user
    midi_ctrl_vars_t    ctrl_var;
} midi_handler_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/

/* Midi data value not valid */
#define MIDI_DATA_NOT_VALID         0xFF

/* MIDI maximun channel value */
#define MIDI_CHANNEL_MAX_VALUE      0x0F

/* MIDI maximun program value */
#define MIDI_PROGRAM_MAX_VALUE      0x0F

/* MIDI maximun BANK value */
#define MIDI_BANK_MAX_VALUE         0x0F

/* MIDI status cmd with 1 bytes */
#define MIDI_STATUS_PROG_CHANGE     0xC0
#define MIDI_STATUS_CH_PRESS        0xD0
#define MIDI_STATUS_TIME_CODE       0xF1
#define MIDI_STATUS_SONG_SELECT     0xF3

/* MIDI status cmd with 2 bytes */
#define MIDI_STATUS_NOTE_OFF        0x80
#define MIDI_STATUS_NOTE_ON         0x90
#define MIDI_STATUS_POLY_PRESS      0xA0
#define MIDI_STATUS_CC              0xB0
#define MIDI_STATUS_PITCH_BEND      0xE0
#define MIDI_STATUS_SONG_POS        0xF2

/* Mask to get channel from status messages */
#define MIDI_STATUS_CH_MASK         0x0F
#define MIDI_STATUS_CMD_MASK        0xF0

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
#define MIDI_RT_MASK                0xF8
#define MIDI_RT_CLK                 0xF8
#define MIDI_RT_START               0xFA
#define MIDI_RT_CONT                0xFB
#define MIDI_RT_STOP                0xFC
#define MIDI_RT_RESET               0xFF

/* MIDI SYS_EX */
#define MIDI_STATUS_SYS_EX_START    0xF0
#define MIDI_STATUS_SYS_EX_END      0xF7

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  Init library FSM and setup callback functions for rx parsing.
 * @param  midi_handler: Handler to initiate.
 * @param  sys_ex_buff: pointer to system exclusive buffer.
 * @param  buff_size: system exckusive buffer size.
 * @param  cb_sys_ex: callback function for sysEx messages.
 * @param  cb_msg_data1: callback function for 1 data messages.
 * @param  cb_msg_data2: callback function for 2 data messages.
 * @param  cb_msg_rt: callback function for real time messages
 * @retval Operation status.
 */
midiStatus_t midi_init(
    midi_handler_t *midi_handler,
    uint8_t *sys_ex_buff,
    uint32_t buff_size,
    sys_ex_cb_t cb_sys_ex,
    data1_cb_t cb_msg_data1,
    data2_cb_t cb_msg_data2,
    rt_cb_t cb_msg_rt);

/**
 * @brief  Update midi rx_fsm.
 * @param  midi_handler: Handler to use.
 * @param  data_rx: input byte to update midi_rx fsm.
 * @retval Operation result.
 */
midiStatus_t midi_update_fsm(midi_handler_t *midi_handler, uint8_t data_rx);

/**
 * @brief  Set rx_fsm into reset state.
 * @param  midi_handler: Handler to use.
 * @retval Operation result.
 */
midiStatus_t midi_reset_fsm(midi_handler_t *midi_handler);

#ifdef __cplusplus
}
#endif

#endif /* __MIDI_LIB_H */

/* EOF */
