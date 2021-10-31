/**
 * @file    sys_log.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   System logging interface.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_LOG_H__
#define __SYS_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Definition of logging interfaces.
 * 
 */
typedef enum {
    SYS_LOG_IF_0    = 0,
    SYS_LOG_IF_1    = 1,
    SYS_LOG_IF_2    = 2,
    SYS_LOG_IF_3    = 3,
    SYS_LOG_IF_4    = 4,
    SYS_LOG_IF_5    = 5,
    SYS_LOG_IF_6    = 6,
    SYS_LOG_IF_7    = 7,
    SYS_LOG_IF_NUM,
} sys_log_if_t;

/**
 * @brief System debug levels.
 * 
 */
typedef enum {
    SYS_LOG_LEVEL_NONE  = 0,
    SYS_LOG_LEVEL_ERR   = 1,
    SYS_LOG_LEVEL_INF   = 2,
    SYS_LOG_LEVEL_DBG   = 3,
    SYS_LOG_LEVEL_NUM,
} sys_log_lvl_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** Define support for printf */
#define LOG_MAP_PRINTF

/** Use mutex to protect common resources in RTOS environment*/
#define LOG_RTOS

#ifdef LOG_MAP_PRINTF
#include "stdarg.h"
#endif // LOG_MAP_PRINTF

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init all system logging intefaces.
 * 
 * @return sys_state_t SYS_SUCCESS if all ok.
 */
sys_state_t sys_log_init(void);

/**
 * @brief Show constant log message into logging interface.
 * 
 * @param if logging interface id.
 * @param lvl logging level.
 * @param msg constatnt string to show.
 */
void sys_log_msg(sys_log_if_t if_id, sys_log_lvl_t lvl, char * msg);

#ifdef LOG_MAP_PRINTF
/**
 * @brief Log message using printf style.
 * 
 * @param if logging interface id.
 * @param lvl logging level.
 * @param fmt variable format.
 * @param ... 
 */
void sys_log_print(sys_log_if_t if_id, sys_log_lvl_t lvl, char *fmt, ...);
#endif // LOG_MAP_PRINTF

/**
 * @brief Set logging level into selected interface.
 * 
 * @param if Interface to modify output level.
 * @param lvl Output logging level to show.
 */
void sys_log_set_output_level(sys_log_if_t if_id, sys_log_lvl_t lvl);

/**
 * @brief Set global state for all logging interfaces
 * 
 * @param log_state: true for enabling logging, false for disable logging.
 */
void sys_log_set_global_log(bool log_state);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_LOG_H__ */

/* EOF */
