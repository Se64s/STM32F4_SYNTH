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

#include "midi_common.h"

/* Exported types ------------------------------------------------------------*/

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
