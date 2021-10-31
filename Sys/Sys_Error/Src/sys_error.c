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

/* Includes ------------------------------------------------------------------*/
#include "sys_error.h"
#include "sys_log.h"
#include "stm32f4xx_hal.h"

/* Private types -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/

#define ERROR_LOG_IF    ( SYS_LOG_IF_0 )

/* Private macro -------------------------------------------------------------*/
/* Private functions prototypes ----------------------------------------------*/

static void vAsserPrint(char *msg, uint32_t len);

/* Private functions definition ----------------------------------------------*/

/**
 * @brief Function to output assert information.
 * 
 * @param msg assert string.
 * @param len number of bytes of assert message.
 */
static void vAsserPrint(char *msg, uint32_t len)
{
    (void)len;

    /* Expected string terminated with '/0' */
    sys_log_msg(ERROR_LOG_IF, SYS_LOG_LEVEL_ERR, msg);
}

/* Public functions prototypes -----------------------------------------------*/

void sys_error_init(void)
{
    vErrorInit(vAsserPrint);
}

void sys_error_handler(void)
{
    __disable_irq();

    sys_log_msg(ERROR_LOG_IF, SYS_LOG_LEVEL_ERR, "Error_Handler reached!");

    while (1)
    {
    }
}

/* EOF */
