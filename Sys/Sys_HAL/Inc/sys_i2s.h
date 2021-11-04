/**
 * @file    sys_i2s.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL interface for I2S peripheral.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_I2S_H__
#define __SYS_I2S_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief I2S interface.
 * 
 */
typedef enum {
    SYS_I2S_0   = 0,
    SYS_I2S_NUM,
} sys_i2s_id_t;

/** I2S events */
typedef enum {
    SYS_I2S_EVENT_TX_DONE       = 0U,
    SYS_I2S_EVENT_HALF_TX_DONE  = 1U,
    SYS_I2S_EVENT_ERROR         = 2U,
    SYS_I2S_EVENT_NUM
} sys_i2s_event_t;

/** User callback for async events */
typedef void (*i2s_event_cb)(sys_i2s_event_t event);

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init i2s defined interface.
 * 
 * @param eId: Interface id.
 * @param pEventCallback: Callback function to handle async events
 * @return sys_state_t: Operation result.
 */
sys_state_t sys_i2s_init(sys_i2s_id_t eId, i2s_event_cb pEventCallback);

/**
 * @brief Deinit i2s interface.
 * 
 * @param eId: Interface id.
 * @return sys_state_t: Operation result.
 */
sys_state_t sys_i2s_deinit(sys_i2s_id_t eId);

/**
 * @brief Send data over i2s interface.
 * 
 * @param eId Interface Id.
 * @param pu16Data pointer with data to send.
 * @param u16DataLen number of elements to send.
 * @return sys_state_t operation result.
 */
sys_state_t sys_i2s_send(sys_i2s_id_t eId, uint16_t *pu16Data, uint16_t u16DataLen);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_I2S_H__ */

/* EOF */
