/**
 * @file    sys_mcu.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL layer for MCU handling.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_MCU_H
#define __SYS_MCU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Basic hardware initialization (systick, pll and NVIC).
 * 
 * @return sys_state_t 
 *      SYS_SUCCESS - all hardware initiated successfully.
 *      SYS_ERROR   - error on initialization.
 */
sys_state_t sys_mcu_init(void);

/**
 * @brief Get current system tick.
 * 
 * @return uint32_t current system tick. 
 */
uint32_t sys_mcu_get_systick(void);

/**
 * @brief Reset MCU.
 * 
 */
void sys_mcu_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_HAL_MCU_H */

/* EOF */
