/**
 * @file    sys_log.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   System log interface.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "sys_log.h"
#include "swo.h"
#include "stm32f4xx_hal.h"

#ifdef LOG_MAP_PRINTF
#include "string.h"
#include "printf.h"
#endif // LOG_MAP_PRINTF

#ifdef LOG_RTOS
#include "cmsis_os.h"
#endif // LOG_RTOS

/* Private typedef -----------------------------------------------------------*/

// Control logging structure.
typedef struct log_ctrl
{
    bool log_enable;                            // Enable logging on all if.
    sys_log_lvl_t log_lvl[SYS_LOG_IF_NUM];      // Log levels for each if.
} log_ctrl_t;

/* Private define ------------------------------------------------------------*/
#define SWO_TRACE_PORTS     ( 0x0000000F )
#define SWO_SPEED_HZ        ( 500000 )
#define SWO_TRACE_PORT      ( 0U )

#define DEFAULT_LOG_LVL     ( SYS_LOG_LEVEL_DBG )

#ifdef LOG_MAP_PRINTF
#define FORMAT_BUFF_SIZE    ( 64U )
#endif // LOG_MAP_PRINTF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// External clock freq to use in SWO cfg
extern uint32_t SystemCoreClock;

// Global control object
static log_ctrl_t g_log_data = { 0 };

// Debug string to identify debug level
const char * dbg_lvl_str[SYS_LOG_LEVEL_NUM] = {
    "NON: ",
    "ERR: ",
    "INF: ",
    "DBG: ",
};

// Debug string to identify logging interface
const char * dbg_if_str[SYS_LOG_IF_NUM] = {
    "IF0_",
    "IF1_",
    "IF2_",
    "IF3_",
    "IF4_",
    "IF5_",
    "IF6_",
    "IF7_",
};

#ifdef LOG_RTOS
osMutexId_t swoIfMutex;
#endif // LOG_MAP_PRINTF

#ifdef LOG_MAP_PRINTF
// General printf buffer
char pcFormatBuffer[FORMAT_BUFF_SIZE];
#endif // LOG_MAP_PRINTF

/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

sys_state_t sys_log_init(void)
{
    // Enable global logging
    g_log_data.log_enable = true;

    // Set initial log level for all interfaces
    for ( uint32_t i = 0U; i < SYS_LOG_IF_NUM; i++ )
    {
        g_log_data.log_lvl[i] = DEFAULT_LOG_LVL;
    }

    // Init HW associated with each if
    SWO_Init(SWO_TRACE_PORTS, SWO_SPEED_HZ, SystemCoreClock);

#ifdef LOG_RTOS
    swoIfMutex = osMutexNew(NULL);
    ERR_ASSERT(swoIfMutex != NULL);
#endif // LOG_RTOS

    return SYS_SUCCESS;
}

void sys_log_msg(sys_log_if_t if_id, sys_log_lvl_t lvl, char *msg)
{
    ERR_ASSERT(if_id < SYS_LOG_IF_NUM);
    ERR_ASSERT(lvl < SYS_LOG_LEVEL_NUM);

    if ( g_log_data.log_enable )
    {
        if ( g_log_data.log_lvl[if_id] >= lvl )
        {
#ifdef LOG_RTOS
            if ( osMutexAcquire(swoIfMutex, osWaitForever) == osOK )
            {
#endif // LOG_RTOS

            // Select inteface
            // Inteface 0-7 ITM
            if ( if_id <= SYS_LOG_IF_7 )
            {
                SWO_PrintString(dbg_if_str[if_id], if_id);
                SWO_PrintString(dbg_lvl_str[lvl], if_id);
                SWO_PrintString(msg, if_id);
                SWO_PrintString("\r\n", if_id);
            }

#ifdef LOG_RTOS
            osMutexRelease(swoIfMutex);
            }
#endif // LOG_RTOS
        }
    }
}

#ifdef LOG_MAP_PRINTF
void sys_log_print(sys_log_if_t if_id, sys_log_lvl_t lvl, char *fmt, ...)
{
    ERR_ASSERT(if_id < SYS_LOG_IF_NUM);
    ERR_ASSERT(lvl < SYS_LOG_LEVEL_NUM);

    if ( g_log_data.log_enable )
    {
        if ( g_log_data.log_lvl[if_id] >= lvl )
        {
#ifdef LOG_RTOS
            if ( osMutexAcquire(swoIfMutex, osWaitForever) == osOK )
            {
#endif // LOG_RTOS

            int32_t len_data = 0;
            va_list args;

            // Select inteface
            // Inteface 0-7 ITM
            if ( if_id <= SYS_LOG_IF_7 )
            {
                memset(pcFormatBuffer, 0U, FORMAT_BUFF_SIZE);
                len_data = snprintf(pcFormatBuffer, FORMAT_BUFF_SIZE, "%s%s", dbg_if_str[if_id], dbg_lvl_str[lvl]);
                SWO_PrintString(pcFormatBuffer, if_id);

                memset(pcFormatBuffer, 0U, len_data);
                va_start(args, fmt);
                len_data = vsnprintf(pcFormatBuffer, FORMAT_BUFF_SIZE, fmt, args);
                va_end(args);
                SWO_PrintString(pcFormatBuffer, if_id);
                SWO_PrintString("\n\r", if_id);
            }

#ifdef LOG_RTOS
            osMutexRelease(swoIfMutex);
            }
#endif // LOG_RTOS
        }
    }
}
#endif // LOG_MAP_PRINTF

void sys_log_set_output_level(sys_log_if_t if_id, sys_log_lvl_t lvl)
{
    ERR_ASSERT(if_id < SYS_LOG_IF_NUM);
    ERR_ASSERT(lvl < SYS_LOG_LEVEL_NUM);

    g_log_data.log_lvl[if_id] = lvl;
}

void sys_log_set_global_log(bool log_state)
{
    g_log_data.log_enable = log_state;
}

/* EOF */
