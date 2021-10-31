/**
 * @file    sys_error.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Header for system error handler.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_ERROR_H
#define __SYS_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "user_assert.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init error log interface.
 * 
 */
void sys_error_init(void);

/**
 * @brief Handler for fatal errors.
 * 
 */
void sys_error_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_ERROR_H */

/* EOF */
