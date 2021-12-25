/**
 * @file    audio_hal.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to implement HAL for I2S interface.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_hal.h"

#include "sys_i2s.h"
#ifdef AUDIO_TRANSFER_TRACE
#include "sys_gpio.h"
#endif // AUDIO_TRANSFER_TRACE

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Sys HAL defines */
#define AUDIO_I2S                   ( SYS_I2S_0 )
#define AUDIO_TEST_PIN              ( SYS_GPIO_0 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Event callbacl handler */
audio_hal_event_cb pEventCallBack = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private function definition -----------------------------------------------*/

static void audio_i2s_cb(sys_i2s_event_t event)
{
    if ( pEventCallBack != NULL )
    {
        switch ( event )
        {
            case SYS_I2S_EVENT_TX_DONE:
                pEventCallBack(AUDIO_HAL_EVENT_TX_DONE);
                break;

            case SYS_I2S_EVENT_HALF_TX_DONE:
                pEventCallBack(AUDIO_HAL_EVENT_HALF_TX_DONE);
                break;

            case SYS_I2S_EVENT_ERROR:
                pEventCallBack(AUDIO_HAL_EVENT_ERROR);
                break;

            default:
                break;
        }
    }
}

/* Public function definition ------------------------------------------------*/

audio_ret_t AUDIO_HAL_init(audio_hal_event_cb pEventCB)
{
    ERR_ASSERT(pEventCB != NULL);

    audio_ret_t eRetVal = AUDIO_ERR;

    pEventCallBack = pEventCB;

    if ( sys_i2s_init(AUDIO_I2S, audio_i2s_cb) == SYS_SUCCESS
#ifdef AUDIO_TRANSFER_TRACE
        && sys_gpio_init(AUDIO_TEST_PIN, SYS_GPIO_MODE_OUT) == SYS_SUCCESS
#endif // AUDIO_TRANSFER_TRACE
    )
    {
        eRetVal = AUDIO_OK;
    }

    return eRetVal;
}

void AUDIO_HAL_deinit(void)
{
    sys_i2s_deinit(AUDIO_I2S);

    pEventCallBack = NULL;
}

audio_ret_t AUDIO_HAL_send_buffer(uint16_t *pu16Data, uint16_t u16DataLen)
{
    ERR_ASSERT(pu16Data != NULL);
    ERR_ASSERT(u16DataLen != 0U);

    audio_ret_t eRetVal = AUDIO_ERR;

    /* Start transfer */
    if ( sys_i2s_send(AUDIO_I2S, pu16Data, u16DataLen) == SYS_SUCCESS )
    {
        eRetVal = AUDIO_OK;
    }

    return eRetVal;
}

void AUDIO_HAL_isr_ctrl(bool bIsrEnable)
{
#ifdef AUDIO_DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, bIsrEnable);
#endif // AUDIO_DISABLE_ISR
}

void AUDIO_HAL_gpio_ctrl(bool bSetState)
{
#ifdef AUDIO_TRANSFER_TRACE
    if ( bSetState )
    {
        sys_gpio_set_level(AUDIO_TEST_PIN, SYS_GPIO_STATE_SET);
    }
    else
    {
        sys_gpio_set_level(AUDIO_TEST_PIN, SYS_GPIO_STATE_RESET);
    }
#endif // AUDIO_TRANSFER_TRACE
}

/* EOF */