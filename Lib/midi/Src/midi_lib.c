/**
 * @file    midi_lib.c
 * @author  Sebastian Del Moral
 * @brief   Library for handling and parsing midi messages.
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "midi_lib.h"

/* Private typedef -----------------------------------------------------------*/

/** List of defined states used in rx_fsm */
typedef enum {
    wait_byte_init = 0,
    wait_byte_first_data,
    wait_byte_second_data,
    wait_byte_sys_ex,
    wait_byte_data,
    dispatch_status,
    handle_1_data_msg,
    handle_2_data_msg,
    handle_sys_ex_msg,
    handle_rt_msg,
} midi_rx_state_t;

/* Private define ------------------------------------------------------------*/

#define MIDI_STATUS_MASK (1 << 7)
#define MIDI_STATUS_RT_MASK (0xF8)

#define MIDI_IS_STATUS(data) (((data)&MIDI_STATUS_MASK) == MIDI_STATUS_MASK)
//#define MIDI_IS_RT(data)				(((data) & MIDI_STATUS_RT_MASK) == MIDI_STATUS_RT_MASK)
#define MIDI_IS_RT(data) ((((data)&MIDI_STATUS_RT_MASK) == MIDI_STATUS_RT_MASK) && ((data) != MIDI_STATUS_SYS_EX_START) && ((data) != MIDI_STATUS_SYS_EX_END))
#define MIDI_STATUS_GET_CH(status) (((status)&MIDI_STATUS_CH_MASK) >> 0)
#define MIDI_STATUS_GET_CMD(status) (((status)&MIDI_STATUS_CMD_MASK) >> 4)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static midiStatus_t state_handler_wait_byte_init(midi_handler_t *midi_handler, uint8_t rx_byte);
static midiStatus_t state_handler_wait_byte_first_data(midi_handler_t *midi_handler, uint8_t rx_byte);
static midiStatus_t state_handler_wait_byte_second_data(midi_handler_t *midi_handler, uint8_t rx_byte);
static midiStatus_t state_handler_wait_byte_sys_ex(midi_handler_t *midi_handler, uint8_t rx_byte);
static midiStatus_t state_handler_wait_byte_data(midi_handler_t *midi_handler, uint8_t rx_byte);
static midiStatus_t state_handler_dispatch_status(midi_handler_t *midi_handler, uint8_t rx_byte);

/* Private function ----------------------------------------------------------*/

static midiStatus_t state_handler_wait_byte_init(midi_handler_t *midi_handler, uint8_t rx_byte)
{
    midiStatus_t retval = midiOk;

    if ( MIDI_IS_STATUS(rx_byte) )
    {
        if ( MIDI_IS_RT(rx_byte) )
        {
            if ( midi_handler->midi_rt_cb != NULL )
            {
                midi_handler->midi_rt_cb(rx_byte);
            }
        }
        else
        {
            midi_handler->ctrl_var.fsm_state = dispatch_status;
            retval = midiHandleNewState;
        }
    }

    return retval;
}

static midiStatus_t state_handler_wait_byte_first_data(midi_handler_t *midi_handler, uint8_t rx_byte)
{
    midiStatus_t retval = midiOk;

    if ( MIDI_IS_STATUS(rx_byte) )
    {
        if ( MIDI_IS_RT(rx_byte) )
        {
            if ( midi_handler->midi_rt_cb != NULL )
            {
                midi_handler->midi_rt_cb(rx_byte);
            }
        }
        else
        {
            midi_handler->ctrl_var.fsm_state = dispatch_status;
            retval = midiHandleNewState;
        }
    }
    else
    {
        midi_handler->ctrl_var.tmp_data_1 = rx_byte;
        midi_handler->ctrl_var.fsm_state = wait_byte_second_data;
    }

    return retval;
}

static midiStatus_t state_handler_wait_byte_second_data(midi_handler_t *midi_handler, uint8_t rx_byte)
{
    midiStatus_t retval = midiOk;

    if (MIDI_IS_STATUS(rx_byte))
    {
        if ( MIDI_IS_RT(rx_byte) )
        {
            if ( midi_handler->midi_rt_cb != NULL )
            {
                midi_handler->midi_rt_cb(rx_byte);
            }
        }
        else
        {
            midi_handler->ctrl_var.fsm_state = dispatch_status;
            retval = midiHandleNewState;
        }
    }
    else
    {
        midi_handler->ctrl_var.tmp_data_2 = rx_byte;

        if ( midi_handler->midi_data_2_cb != NULL )
        {
            midi_handler->midi_data_2_cb(
                midi_handler->ctrl_var.tmp_running_status, 
                midi_handler->ctrl_var.tmp_data_1, 
                midi_handler->ctrl_var.tmp_data_2);
        }

        midi_handler->ctrl_var.fsm_state = wait_byte_first_data;
        midi_handler->ctrl_var.tmp_data_1 = 0U;
        midi_handler->ctrl_var.tmp_data_2 = 0U;
    }

    return retval;
}

static midiStatus_t state_handler_wait_byte_sys_ex(midi_handler_t *midi_handler, uint8_t rx_byte)
{
    midiStatus_t retval = midiOk;

    if ( MIDI_IS_STATUS(rx_byte) )
    {
        if ( MIDI_IS_RT(rx_byte) )
        {
            if ( midi_handler->midi_rt_cb != NULL )
            {
                midi_handler->midi_rt_cb(rx_byte);
            }
        }
        else
        {
            if ( rx_byte == MIDI_STATUS_SYS_EX_END )
            {
                /* Handle end of sys_ex */
                if ( midi_handler->midi_sys_ex_cb != NULL )
                {
                    if ( midi_handler->ctrl_var.sys_ex_buff != NULL )
                    {
                        midi_handler->midi_sys_ex_cb(
                                                    midi_handler->ctrl_var.sys_ex_buff, 
                                                    midi_handler->ctrl_var.i_data_sys_ex);
                    }
                    // midi_handler->ctrl_var.i_data_sys_ex = 0U
                }
            }
            midi_handler->ctrl_var.fsm_state = dispatch_status;
            retval = midiHandleNewState;
        }
    }
    else
    {
        /* Handle data byte for sys_ex */
        if ( midi_handler->ctrl_var.i_data_sys_ex < midi_handler->ctrl_var.sys_ex_size )
        {
            midi_handler->ctrl_var.sys_ex_buff[midi_handler->ctrl_var.i_data_sys_ex++] = rx_byte;
        }
        else
        {
            midi_handler->ctrl_var.i_data_sys_ex = 0U;
            midi_handler->ctrl_var.fsm_state = (uint32_t)wait_byte_init;
            retval = midiSysExcBuffFull;
        }
    }

    return retval;
}

static midiStatus_t state_handler_wait_byte_data(midi_handler_t *midi_handler, uint8_t rx_byte)
{
    midiStatus_t retval = midiOk;

    if ( MIDI_IS_STATUS(rx_byte) )
    {
        if ( MIDI_IS_RT(rx_byte) )
        {
            if ( midi_handler->midi_rt_cb != NULL )
            {
                midi_handler->midi_rt_cb(rx_byte);
            }
        }
        else
        {
            midi_handler->ctrl_var.fsm_state = (uint32_t)dispatch_status;
            retval = midiHandleNewState;
        }
    }
    else
    {
        if ( midi_handler->midi_data_1_cb != NULL )
        {
            midi_handler->midi_data_1_cb(midi_handler->ctrl_var.tmp_running_status, rx_byte);
        }
    }

    return retval;
}

static midiStatus_t state_handler_dispatch_status(midi_handler_t *midi_handler, uint8_t rx_byte)
{
    midiStatus_t retval = midiError;

    /* Check if 1 data cmd */
    if (((rx_byte & MIDI_STATUS_CMD_MASK) == MIDI_STATUS_PROG_CHANGE) ||
        ((rx_byte & MIDI_STATUS_CMD_MASK) == MIDI_STATUS_CH_PRESS) ||
        (rx_byte == MIDI_STATUS_TIME_CODE) ||
        (rx_byte == MIDI_STATUS_SONG_SELECT))
    {
        midi_handler->ctrl_var.fsm_state = (uint32_t)wait_byte_data;
        retval = midiOk;
    }
    /* Check if 2 data cmd */
    else if (((rx_byte & MIDI_STATUS_CMD_MASK) == MIDI_STATUS_NOTE_OFF) ||
             ((rx_byte & MIDI_STATUS_CMD_MASK) == MIDI_STATUS_NOTE_ON) ||
             ((rx_byte & MIDI_STATUS_CMD_MASK) == MIDI_STATUS_POLY_PRESS) ||
             ((rx_byte & MIDI_STATUS_CMD_MASK) == MIDI_STATUS_CC) ||
             ((rx_byte & MIDI_STATUS_CMD_MASK) == MIDI_STATUS_PITCH_BEND) ||
             (rx_byte == MIDI_STATUS_SONG_POS))
    {
        midi_handler->ctrl_var.fsm_state = (uint32_t)wait_byte_first_data;
        retval = midiOk;
    }
    /* Check if sys ex cmd */
    else if ( rx_byte == MIDI_STATUS_SYS_EX_START )
    {
        midi_handler->ctrl_var.i_data_sys_ex = 0U;
        midi_handler->ctrl_var.fsm_state = (uint32_t)wait_byte_sys_ex;
        retval = midiOk;
    }
    /* If not defined, jump to start */
    else
    {
        midi_handler->ctrl_var.fsm_state = (uint32_t)wait_byte_init;
    }

    /* Save running status */
    if ( retval == midiOk )
    {
        midi_handler->ctrl_var.tmp_running_status = rx_byte;
    }

    return retval;
}

/* ---------------------------------------------------------------------------*/

/** State Handler definitions */
midiStatus_t (*rx_state_table[])(midi_handler_t *midi_handler, uint8_t rx_byte) = {
    state_handler_wait_byte_init,
    state_handler_wait_byte_first_data,
    state_handler_wait_byte_second_data,
    state_handler_wait_byte_sys_ex,
    state_handler_wait_byte_data,
    state_handler_dispatch_status
};

/* Public function -----------------------------------------------------------*/

/* Init library FSM and setup callback functions for rx parsing */
midiStatus_t midi_init( midi_handler_t *midi_handler,
                        uint8_t *sys_ex_buff,
                        uint32_t buff_size,
                        sys_ex_cb_t cb_sys_ex,
                        data1_cb_t cb_msg_data1,
                        data2_cb_t cb_msg_data2,
                        rt_cb_t cb_msg_rt )
{
    midiStatus_t retval = midiError;

    if ( midi_handler != NULL )
    {
        /* Init private cb handlers */
        if ( cb_sys_ex != NULL )
        {
            midi_handler->midi_sys_ex_cb = cb_sys_ex;
        }

        if ( cb_msg_data1 != NULL )
        {
            midi_handler->midi_data_1_cb = cb_msg_data1;
        }

        if ( cb_msg_data2 != NULL )
        {
            midi_handler->midi_data_2_cb = cb_msg_data2;
        }

        if ( cb_msg_rt != NULL )
        {
            midi_handler->midi_rt_cb = cb_msg_rt;
        }

        midi_handler->ctrl_var.fsm_state = (uint32_t)wait_byte_init;

        midi_handler->ctrl_var.tmp_running_status = 0U;
        midi_handler->ctrl_var.tmp_data_1 = 0U;
        midi_handler->ctrl_var.tmp_data_2 = 0U;

        midi_handler->ctrl_var.sys_ex_buff = sys_ex_buff;
        midi_handler->ctrl_var.sys_ex_size = buff_size;
        midi_handler->ctrl_var.i_data_sys_ex = 0U;

        retval = midiOk;
    }

    return retval;
}

/* Update midi rx_fsm */
midiStatus_t midi_update_fsm(midi_handler_t *midi_handler, uint8_t data_rx)
{
    midiStatus_t retval = midiError;

    if ( midi_handler != NULL )
    {
        do {
            retval = rx_state_table[midi_handler->ctrl_var.fsm_state](midi_handler, data_rx);
        } while (retval == midiHandleNewState);
    }

    return retval;
}

/* Set rx_fsm into reset state */
midiStatus_t midi_reset_fsm(midi_handler_t *midi_handler)
{
    midiStatus_t retval = midiOk;

    if ( midi_handler != NULL )
    {
        midi_handler->ctrl_var.fsm_state = (uint32_t)wait_byte_init;
        midi_handler->ctrl_var.tmp_running_status = 0U;
        midi_handler->ctrl_var.tmp_data_1 = 0U;
        midi_handler->ctrl_var.tmp_data_2 = 0U;
        midi_handler->ctrl_var.i_data_sys_ex = 0U;

        retval = midiOk;
    }

    return retval;
}

/* EOF */
