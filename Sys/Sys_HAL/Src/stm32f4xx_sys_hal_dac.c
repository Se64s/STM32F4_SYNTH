/**
 * @file    stm32f4xx_sys_hal_dac.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL implementation for DAC peripheral for STM32f4xx family.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Private includes ----------------------------------------------------------*/
#include "sys_dac.h"
#include "stm32f4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;
DMA_HandleTypeDef hdma_dac2;

/* Private function prototypes -----------------------------------------------*/

/* DAC init function */
void MX_DAC_Init(void)
{
    DAC_ChannelConfTypeDef sConfig = {0};

    /** DAC Initialization
     */
    hdac.Instance = DAC;
    if (HAL_DAC_Init(&hdac) != HAL_OK)
    {
        sys_error_handler();
    }

    /** DAC channel OUT1 config
     */
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
    {
        sys_error_handler();
    }

    /** DAC channel OUT2 config
     */
    if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
    {
        sys_error_handler();
    }
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(dacHandle->Instance==DAC)
    {
        /* DAC clock enable */
        __HAL_RCC_DAC_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**DAC GPIO Configuration
        PA4     ------> DAC_OUT1
        PA5     ------> DAC_OUT2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* DAC DMA Init */
        /* DAC1 Init */
        hdma_dac1.Instance = DMA1_Stream5;
        hdma_dac1.Init.Channel = DMA_CHANNEL_7;
        hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_dac1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_dac1.Init.Mode = DMA_NORMAL;
        hdma_dac1.Init.Priority = DMA_PRIORITY_LOW;
        hdma_dac1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_dac1) != HAL_OK)
        {
        sys_error_handler();
        }

        __HAL_LINKDMA(dacHandle,DMA_Handle1,hdma_dac1);

        /* DAC2 Init */
        hdma_dac2.Instance = DMA1_Stream6;
        hdma_dac2.Init.Channel = DMA_CHANNEL_7;
        hdma_dac2.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_dac2.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_dac2.Init.MemInc = DMA_MINC_ENABLE;
        hdma_dac2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_dac2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_dac2.Init.Mode = DMA_NORMAL;
        hdma_dac2.Init.Priority = DMA_PRIORITY_LOW;
        hdma_dac2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_dac2) != HAL_OK)
        {
        sys_error_handler();
        }

        __HAL_LINKDMA(dacHandle,DMA_Handle2,hdma_dac2);
    }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{
    if(dacHandle->Instance==DAC)
    {
        /* Peripheral clock disable */
        __HAL_RCC_DAC_CLK_DISABLE();

        /**DAC GPIO Configuration
        PA4     ------> DAC_OUT1
        PA5     ------> DAC_OUT2
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);

        /* DAC DMA DeInit */
        HAL_DMA_DeInit(dacHandle->DMA_Handle1);
        HAL_DMA_DeInit(dacHandle->DMA_Handle2);
    }
}

/* Public functions ----------------------------------------------------------*/

sys_state_t sys_dac_init(sys_dac_id_t eId)
{
    ERR_ASSERT(eId < SYS_DAC_NUM);

    if (eId == SYS_DAC_0)
    {
        MX_DAC_Init();
    }

    return SYS_SUCCESS;
}

/* EOF */
