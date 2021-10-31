/**
 * @file    stm32f4xx_sys_hal_tim.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL implementation for TIM peripheral for STM32f4xx family.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "sys_tim.h"
#include "stm32f4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim6;

/* Private function prototypes -----------------------------------------------*/

/* TIM6 init function */
void MX_TIM6_Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 0;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = 65535;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
        sys_error_handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
    {
        sys_error_handler();
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if(tim_baseHandle->Instance==TIM6)
    {
        /* TIM6 clock enable */
        __HAL_RCC_TIM6_CLK_ENABLE();
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if(tim_baseHandle->Instance==TIM6)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM6_CLK_DISABLE();
    }
}

/* Public functions ----------------------------------------------------------*/

sys_state_t sys_tim_init(sys_tim_id_t eId)
{
    ERR_ASSERT(eId < SYS_TIM_NUM);

    if (eId == SYS_TIM_0)
    {
        MX_TIM6_Init();
    }

    return SYS_SUCCESS;
}

/* EOF */
