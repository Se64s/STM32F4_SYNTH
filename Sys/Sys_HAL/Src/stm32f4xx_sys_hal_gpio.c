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

sys_state_t sys_gpio_init(sys_gpio_port_id_t ePortId, sys_gpio_mode_t eMode)
{
    ERR_ASSERT(ePortId < SYS_GPIO_NUM);
    ERR_ASSERT(eMode < SYS_GPIO_MODE_NUM);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if ( ePortId == SYS_GPIO_0 )
    {
        if (eMode == SYS_GPIO_MODE_OUT)
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();

            GPIO_InitStruct.Pin = GPIO_PIN_8;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        }
    }
    else if ( ePortId == SYS_GPIO_1 )
    {
        if (eMode == SYS_GPIO_MODE_OUT)
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();

            GPIO_InitStruct.Pin = GPIO_PIN_9;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        }
    }

    return SYS_SUCCESS;
}

void sys_gpio_set_level(sys_gpio_port_id_t ePortId, sys_gpio_state_t eState)
{
    ERR_ASSERT(ePortId < SYS_GPIO_NUM);
    ERR_ASSERT(eState < SYS_GPIO_STATE_NUM);

    if ( ePortId == SYS_GPIO_0 )
    {
        if ( eState == SYS_GPIO_STATE_RESET )
        {
            
            GPIOC->BSRR = (uint32_t)GPIO_PIN_8 << 16U;
        }
        else
        {
            GPIOC->BSRR = GPIO_PIN_8;
        }
    }
    else if ( ePortId == SYS_GPIO_1 )
    {
        if ( eState == SYS_GPIO_STATE_RESET )
        {
            
            GPIOC->BSRR = (uint32_t)GPIO_PIN_9 << 16U;
        }
        else
        {
            GPIOC->BSRR = GPIO_PIN_9;
        }
    }
}

/* EOF */
