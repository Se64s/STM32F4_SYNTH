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
#include "stm32f4xx_hal.h"

/* Private types -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private functions prototypes ----------------------------------------------*/

/**
 * @brief Handler for critical errors.
 * 
 */
void sys_error_handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

/* EOF */
