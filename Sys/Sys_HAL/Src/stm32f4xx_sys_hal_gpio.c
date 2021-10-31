/**
 * @file    stm32f4xx_sys_hal_gpio.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL implementation for GPIO peripheral for STM32f4xx family.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "sys_gpio.h"
#include "stm32f4xx_hal.h"

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

/* Public functions ----------------------------------------------------------*/

sys_state_t sys_gpio_init(sys_gpio_port_id_t ePortId, sys_gpio_pin_id_t ePinId)
{
    ERR_ASSERT(ePortId < SYS_GPIO_PORT_NUM);
    ERR_ASSERT(ePinId < SYS_GPIO_PIN_NUM);

    MX_GPIO_Init();

    return SYS_SUCCESS;
}

/* EOF */
