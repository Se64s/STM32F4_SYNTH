/**
 * @file    sys_common.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Common system functions.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_COMMON_H
#define __SYS_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "sys_error.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief System return values.
 * 
 */
typedef enum {
    SYS_SUCCESS     = 0,
    SYS_BUSY        = 1,
    SYS_PARAM_ERROR = 2,
    SYS_ERROR       = 3,
    SYS_NOT_INIT    = 4,
} sys_state_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SYS_COMMON_H */

/* EOF */
