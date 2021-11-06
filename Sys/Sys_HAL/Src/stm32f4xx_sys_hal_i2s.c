/**
 * @file    stm32f4xx_sys_hal_i2s.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL implementation for I2S peripheral for STM32f4xx family.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Private includes ----------------------------------------------------------*/
#include "sys_i2s.h"
#include "stm32f4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/

/* Hanlder definition for each interface */
typedef struct i2s_handler
{
    I2S_HandleTypeDef   *handler;
    i2s_event_cb        event_cb;
} i2s_handler_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_tx;

i2s_handler_t handler_i2s_0 = {
    .handler = &hi2s2,
    .event_cb = NULL
};

i2s_handler_t *i2s_handler_list[SYS_I2S_NUM] = {
    &handler_i2s_0,
};

/* Private function prototypes -----------------------------------------------*/

void MX_I2S2_Init(void)
{
    hi2s2.Instance = SPI2;
    hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;
    hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_48K;
    hi2s2.Init.CPOL = I2S_CPOL_LOW;
    hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
    if (HAL_I2S_Init(&hi2s2) != HAL_OK)
    {
        sys_error_handler();
    }
}

void HAL_I2S_MspInit(I2S_HandleTypeDef* i2sHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if(i2sHandle->Instance==SPI2)
    {
        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1;
        PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
        PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLI2SP_DIV2;
        PeriphClkInitStruct.PLLI2S.PLLI2SM = 16;
        PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
        PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
        PeriphClkInitStruct.PLLI2SDivQ = 1;
        PeriphClkInitStruct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            sys_error_handler();
        }

        /* I2S2 clock enable */
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2S2 GPIO Configuration
        PB12     ------> I2S2_WS
        PB13     ------> I2S2_CK
        PB15     ------> I2S2_SD
        */
        GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* I2S2 DMA Init */
        /* SPI2_TX Init */
        hdma_spi2_tx.Instance = DMA1_Stream4;
        hdma_spi2_tx.Init.Channel = DMA_CHANNEL_0;
        hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_spi2_tx.Init.Mode = DMA_CIRCULAR;

        hdma_spi2_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
        {
            sys_error_handler();
        }

        __HAL_LINKDMA(i2sHandle,hdmatx,hdma_spi2_tx);

        HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 10, 0);
        HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
    }
}

void HAL_I2S_MspDeInit(I2S_HandleTypeDef* i2sHandle)
{
    if(i2sHandle->Instance==SPI2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**I2S2 GPIO Configuration
        PB12     ------> I2S2_WS
        PB13     ------> I2S2_CK
        PB15     ------> I2S2_SD
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_15);

        /* I2S2 DMA DeInit */
        HAL_DMA_DeInit(i2sHandle->hdmatx);
    }
}

/* HAL callbacks ------------------------------------------------------------*/

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    i2s_handler_t * pHandler = NULL;

    if ( hi2s->Instance == SPI2 )
    {
        pHandler = i2s_handler_list[SYS_I2S_0]; 
    }

    if ( pHandler != NULL )
    {
        if ( pHandler->event_cb != NULL )
        {
            pHandler->event_cb(SYS_I2S_EVENT_TX_DONE);
        }
    }
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    i2s_handler_t * pHandler = NULL;

    if ( hi2s->Instance == SPI2 )
    {
        pHandler = i2s_handler_list[SYS_I2S_0]; 
    }

    if ( pHandler != NULL )
    {
        if ( pHandler->event_cb != NULL )
        {
            pHandler->event_cb(SYS_I2S_EVENT_HALF_TX_DONE);
        }
    }
}

void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
    i2s_handler_t * pHandler = NULL;

    if ( hi2s->Instance == SPI2 )
    {
        pHandler = i2s_handler_list[SYS_I2S_0]; 
    }

    if ( pHandler != NULL )
    {
        if ( pHandler->event_cb != NULL )
        {
            pHandler->event_cb(SYS_I2S_EVENT_ERROR);
        }
    }
}

/* Public functions ----------------------------------------------------------*/

sys_state_t sys_i2s_init(sys_i2s_id_t eId, i2s_event_cb pEventCallback)
{
    ERR_ASSERT(eId < SYS_I2S_NUM);

    if ( eId == SYS_I2S_0)
    {
        MX_I2S2_Init();
    }

    if (pEventCallback != NULL)
    {
        i2s_handler_list[eId]->event_cb = pEventCallback;
    }

    return SYS_SUCCESS;
}

sys_state_t sys_i2s_deinit(sys_i2s_id_t eId)
{
    ERR_ASSERT(eId < SYS_I2S_NUM);

    HAL_I2S_DeInit(i2s_handler_list[eId]->handler);

    i2s_handler_list[eId]->event_cb = NULL;

    return SYS_SUCCESS;
}

sys_state_t sys_i2s_send(sys_i2s_id_t eId, uint16_t *pu16Data, uint16_t u16DataLen)
{
    ERR_ASSERT(eId < SYS_I2S_NUM);

    sys_state_t eRetval = SYS_SUCCESS;

    if ( eId == SYS_I2S_0)
    {
        HAL_StatusTypeDef eHalResult = HAL_I2S_Transmit_DMA(i2s_handler_list[eId]->handler, pu16Data, u16DataLen);

        if ( eHalResult != HAL_OK )
        {
            eRetval = (eHalResult == HAL_BUSY) ? SYS_BUSY : SYS_ERROR;
        }
    }

    return eRetval;
}

sys_state_t sys_i2s_isr_ctrl(sys_i2s_id_t eId, bool bState)
{
    ERR_ASSERT(eId < SYS_I2S_NUM);

    sys_state_t eRetval = SYS_PARAM_ERROR;

    if ( eId == SYS_I2S_0)
    {
        if ( bState )
        {
            HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
        }
        else
        {
            HAL_NVIC_DisableIRQ(DMA1_Stream4_IRQn);
        }

        eRetval = SYS_SUCCESS;
    }

    return eRetval;
}

/* EOF */
