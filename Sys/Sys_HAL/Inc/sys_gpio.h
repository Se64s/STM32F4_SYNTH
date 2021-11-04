/**
 * @file    sys_gpio.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL interface for GPIO peripheral.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_GPIO_H__
#define __SYS_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief GPIO port.
 * 
 */
typedef enum {
    SYS_GPIO_0   = 0,
    SYS_GPIO_NUM,
} sys_gpio_port_id_t;

/**
 * @brief GPIO modes.
 * 
 */
typedef enum {
    SYS_GPIO_MODE_OUT = 1,
    SYS_GPIO_MODE_NUM,
} sys_gpio_mode_t;

/**
 * @brief GPIO defined states.
 * 
 */
typedef enum {
    SYS_GPIO_STATE_RESET  = 0,
    SYS_GPIO_STATE_SET    = 1,
    SYS_GPIO_STATE_NUM,
} sys_gpio_state_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init GPIO peripheral.
 * 
 * @param ePortId port id
 * @param ePinId pin id
 * @return sys_state_t operation result.
 */
sys_state_t sys_gpio_init(sys_gpio_port_id_t ePortId, sys_gpio_mode_t eMode);

/**
 * @brief Set state in gpio in output mode
 * 
 * @param ePortId gpio id
 * @param eState new state to set
 */
void sys_gpio_set_level(sys_gpio_port_id_t ePortId, sys_gpio_state_t eState);

#ifdef __cplusplus
}
#endif
#endif /*__SYS_GPIO_H__ */

/* EOF */
