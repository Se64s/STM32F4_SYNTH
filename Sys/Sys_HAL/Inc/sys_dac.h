/**
 * @file    sys_dac.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL interface for DAC peripheral.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_DAC_H__
#define __SYS_DAC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief DAC Peripheral.
 * 
 */
typedef enum {
    SYS_DAC_0   = 0,
    SYS_DAC_NUM,
} sys_dac_id_t;

/**
 * @brief DAC Channels.
 * 
 */
typedef enum {
    SYS_DAC_CH_0   = 0,
    SYS_DAC_CH_1   = 1,
    SYS_DAC_CH_NUM,
} sys_dac_ch_id_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init DAC peripheral.
 * 
 * @param eId peripheral id.
 * @return sys_state_t operation result.
 */
sys_state_t sys_dac_init(sys_dac_id_t eId);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_DAC_H__ */

/* EOF */
