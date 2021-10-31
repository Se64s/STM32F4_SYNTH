/**
 * @file    sys_tim.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL interface for TIM peripheral.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_TIM_H__
#define __SYS_TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief TIM channels.
 * 
 */
typedef enum {
    SYS_TIM_0   = 0,
    SYS_TIM_NUM,
} sys_tim_id_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init TIM peripheral.
 * 
 * @param eId peripheral id.
 * @return sys_state_t operation result.
 */
sys_state_t sys_tim_init(sys_tim_id_t eId);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_TIM_H__ */

/* EOF */
