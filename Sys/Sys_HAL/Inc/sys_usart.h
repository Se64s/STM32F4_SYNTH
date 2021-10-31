/**
 * @file    sys_usart.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL interface for USART peripheral.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_USART_H__
#define __SYS_USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

/** Defined USART channels */
typedef enum {
    SYS_USART_0   = 0,
    SYS_USART_1   = 1,
    SYS_USART_NUM,
} sys_usart_id_t;

/** Defined USART events */
typedef enum {
    SYS_USART_EVENT_RX_IDLE         = 0U,
    SYS_USART_EVENT_TX_DONE         = 1U,
    SYS_USART_EVENT_RX_BUFF_FULL    = 2U,
    SYS_USART_EVENT_RX_DONE         = 3U,
    SYS_USART_EVENT_ERROR           = 4U,
    SYS_USART_EVENT_NUM
} sys_usart_event_t;

/** User callback for async events */
typedef void (*usart_event_cb)(sys_usart_event_t event);

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init USART peripheral.
 *
 * @param eId peripheral id.
 * @return sys_state_t operation result.
 */
sys_state_t sys_usart_init(sys_usart_id_t eId, usart_event_cb pEventCallback);

/**
 * @brief Deinit resources for usart peripheral.
 * 
 * @param eId:  Peripheral id to deinit.
 * @return sys_state_t: operation result.
 */
sys_state_t sys_usart_deinit(sys_usart_id_t eId);

/**
 * @brief Send data over selected interface.
 * 
 * @param eId: Interface to use.
 * @param pu8Data: pointer with data to send.
 * @param u16LenData: size of data to send.
 * @return sys_state_t: operation result.
 */
sys_state_t sys_usart_send(sys_usart_id_t eId, uint8_t *pu8Data, uint16_t u16LenData);

/**
 * @brief Inititate data read from selected interface.
 * 
 * @param eId: Interface to use.
 * @param pu8Data: Pointer where store read data.
 * @param u16MaxLen: Read size, when reached this value, the operation will stop.
 * @return sys_state_t: operation result.
 */
sys_state_t sys_usart_read(sys_usart_id_t eId, uint8_t *pu8Data, uint16_t u16MaxLen);

/**
 * @brief Get current number of bytes read in one interface.
 * 
 * @param eId: Interface to use.
 * @return uint16_t: Number of bytes read.
 */
uint16_t sys_usart_get_read_count(sys_usart_id_t eId);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_USART_H__ */

/* EOF */
