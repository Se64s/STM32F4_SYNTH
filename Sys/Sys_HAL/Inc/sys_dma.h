/**
 * @file    sys_dma.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL interface for DMA peripheral.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_DMA_H__
#define __SYS_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief DMA channels.
 * 
 */
typedef enum {
    SYS_DMA_0   = 0,
    SYS_DMA_1   = 1,
    SYS_DMA_NUM,
} sys_dma_id_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init DMA peripheral.
 * 
 * @param eId peripheral id.
 * @return sys_state_t operation result.
 */
sys_state_t sys_dma_init(sys_dma_id_t eId);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_DMA_H__ */

/* EOF */
