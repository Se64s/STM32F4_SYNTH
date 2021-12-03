/**
 * @file    audio_hal.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to implement HAL for I2S interface.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_HAL_H__
#define __AUDIO_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_common.h"

/* Exported types ------------------------------------------------------------*/

/** Audio lib async events */
typedef enum {
    AUDIO_HAL_EVENT_TX_DONE       = 0U,
    AUDIO_HAL_EVENT_HALF_TX_DONE  = 1U,
    AUDIO_HAL_EVENT_ERROR         = 2U,
    AUDIO_HAL_EVENT_NUM,
} audio_hal_event_t;

/** User callback for async events */
typedef void (*audio_hal_event_cb)(audio_hal_event_t event);

/* Exported constants --------------------------------------------------------*/

/* Uncomment to enable GPIO toggle for transfer time tracking */
#define AUDIO_TRANSFER_TRACE

/* Disable codec isr while updating the buffer */
#define AUDIO_DISABLE_ISR

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Enable I2S peripheral and async callback function.
 * 
 * @param pEventCB pointer to handler async events.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_HAL_init(audio_hal_event_cb pEventCB);

/**
 * @brief Deinit I2S peripheral.
 * 
 */
void AUDIO_HAL_deinit(void);

/**
 * @brief Send data buffer over i2s peripheral.
 * 
 * @param pu16Data Data array to send.
 * @param u16DataLen Data len to send.
 * @return audio_ret_t Operation result.
 */
audio_ret_t AUDIO_HAL_send_buffer(uint16_t *pu16Data, uint16_t u16DataLen);

/**
 * @brief Disable isr events for i2s peripheral.
 * 
 * @param bIsrEnable ISR state.
 */
void AUDIO_HAL_isr_ctrl(bool bIsrEnable);

/**
 * @brief 
 * 
 * @param bSetState 
 */
void AUDIO_HAL_gpio_ctrl(bool bSetState);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_FILTER_H__ */

/* EOF */
