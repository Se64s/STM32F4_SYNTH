/**
 * @file    sys_adc.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL interface for ADC peripheral.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_ADC_H__
#define __SYS_ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief ADC channel.
 * 
 */
typedef enum {
    SYS_ADC_0   = 0,
    SYS_ADC_NUM,
} sys_adc_id_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init ADC peripheral.
 * 
 * @param eId peripheral id.
 * @return sys_state_t operation result.
 */
sys_state_t sys_adc_init(sys_adc_id_t eId);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_ADC_H__ */

/* EOF */
