/**
 * @file    stm32f4xx_sys_usart_adc.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   HAL implementation for USART peripheral for STM32f4xx family.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Private includes ----------------------------------------------------------*/
#include "sys_usart.h"
#include "stm32f4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/

/* Hanlder definition for each interface */
typedef struct usart_handler
{
    UART_HandleTypeDef      *handler;
    usart_event_cb          event_cb;
} usart_handler_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

usart_handler_t handler_usart_0 = 
{
    .handler = &huart2,
    .event_cb = NULL
};

usart_handler_t handler_usart_1 = 
{
    .handler = &huart3,
    .event_cb = NULL
};

usart_handler_t* usart_handler_list[SYS_USART_NUM] = {
    &handler_usart_0,
    &handler_usart_1,
};

/* Private function prototypes -----------------------------------------------*/

void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        sys_error_handler();
    }
}

void MX_USART3_UART_Init(void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 31250U;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        sys_error_handler();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(uartHandle->Instance==USART2)
    {
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

        HAL_NVIC_SetPriority(USART2_IRQn, 10, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    else if(uartHandle->Instance==USART3)
    {
        /* USART3 clock enable */
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART3 GPIO Configuration
        PC5     ------> USART3_RX
        PB10     ------> USART3_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USART3 DMA Init */
        /* USART3_RX Init */
        hdma_usart3_rx.Instance = DMA1_Stream1;
        hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart3_rx.Init.Mode = DMA_NORMAL;
        hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
        {
            sys_error_handler();
        }

        __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

        /* USART3_TX Init */
        hdma_usart3_tx.Instance = DMA1_Stream3;
        hdma_usart3_tx.Init.Channel = DMA_CHANNEL_4;
        hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart3_tx.Init.Mode = DMA_NORMAL;
        hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
        {
            sys_error_handler();
        }

        __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);

        __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);

        HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 10, 0);
        HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 10, 0);
        HAL_NVIC_SetPriority(USART3_IRQn, 10, 0);

        HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
        HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
    if(uartHandle->Instance==USART2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
    else if(uartHandle->Instance==USART3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
        PC5     ------> USART3_RX
        PB10     ------> USART3_TX
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_5);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

        /* USART3 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmarx);
        HAL_DMA_DeInit(uartHandle->hdmatx);

        HAL_NVIC_DisableIRQ(DMA1_Stream1_IRQn);
        HAL_NVIC_DisableIRQ(DMA1_Stream3_IRQn);
        HAL_NVIC_DisableIRQ(USART3_IRQn);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    usart_handler_t * pHandler = NULL;

    if ( huart->Instance == USART2 )
    {
        pHandler = usart_handler_list[SYS_USART_0]; 
    }
    else if ( huart->Instance == USART3 )
    {
        pHandler = usart_handler_list[SYS_USART_1];
    }

    if ( pHandler != NULL )
    {
        if ( pHandler->event_cb != NULL )
        {
            pHandler->event_cb(SYS_USART_EVENT_TX_DONE);
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    usart_handler_t * pHandler = NULL;

    if ( huart->Instance == USART2 )
    {
        pHandler = usart_handler_list[SYS_USART_0]; 
    }
    else if ( huart->Instance == USART3 )
    {
        pHandler = usart_handler_list[SYS_USART_1];
    }

    if ( pHandler != NULL )
    {
        if ( pHandler->event_cb != NULL )
        {
            pHandler->event_cb(SYS_USART_EVENT_RX_DONE);
        }
    }
}

void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
{
    usart_handler_t * pHandler = NULL;

    if ( huart->Instance == USART2 )
    {
        pHandler = usart_handler_list[SYS_USART_0]; 
    }
    else if ( huart->Instance == USART3 )
    {
        pHandler = usart_handler_list[SYS_USART_1];
    }

    if ( pHandler != NULL )
    {
        if ( pHandler->event_cb != NULL )
        {
            pHandler->event_cb(SYS_USART_EVENT_RX_IDLE);
        }
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    usart_handler_t * pHandler = NULL;

    if ( huart->Instance == USART2 )
    {
        pHandler = usart_handler_list[SYS_USART_0]; 
    }
    else if ( huart->Instance == USART3 )
    {
        pHandler = usart_handler_list[SYS_USART_1];
    }

    if ( pHandler != NULL )
    {
        if ( pHandler->event_cb != NULL )
        {
            pHandler->event_cb(SYS_USART_EVENT_ERROR);
        }
    }
}


/* Public functions ----------------------------------------------------------*/

sys_state_t sys_usart_init(sys_usart_id_t eId, usart_event_cb pEventCallback)
{
    ERR_ASSERT(eId < SYS_USART_NUM);

    if ( eId == SYS_USART_0)
    {
        MX_USART2_UART_Init();
    }
    else if ( eId == SYS_USART_1)
    {
        MX_USART3_UART_Init();
    }

    if (pEventCallback != NULL)
    {
        usart_handler_list[eId]->event_cb = pEventCallback;
    }

    return SYS_SUCCESS;
}

sys_state_t sys_usart_deinit(sys_usart_id_t eId)
{
    ERR_ASSERT(eId < SYS_USART_NUM);

    HAL_UART_DeInit(usart_handler_list[eId]->handler);

    usart_handler_list[eId]->event_cb = NULL;

    return SYS_SUCCESS;
}

sys_state_t sys_usart_send(sys_usart_id_t eId, uint8_t *pu8Data, uint16_t u16LenData)
{
    ERR_ASSERT(eId < SYS_USART_NUM);

    sys_state_t eRetval = SYS_SUCCESS;

    if ( eId == SYS_USART_0)
    {
        HAL_StatusTypeDef eHalResult = HAL_UART_Transmit_IT(usart_handler_list[eId]->handler, pu8Data, u16LenData);

        if ( eHalResult != HAL_OK )
        {
            eRetval = (eHalResult == HAL_BUSY) ? SYS_BUSY : SYS_ERROR;
        }
    }
    else if ( eId == SYS_USART_1)
    {
        HAL_StatusTypeDef eHalResult = HAL_UART_Transmit_DMA(usart_handler_list[eId]->handler, pu8Data, u16LenData);

        if ( eHalResult != HAL_OK )
        {
            eRetval = (eHalResult == HAL_BUSY) ? SYS_BUSY : SYS_ERROR;
        }
    }

    return eRetval;
}

sys_state_t sys_usart_read(sys_usart_id_t eId, uint8_t *pu8Data, uint16_t u16MaxLen)
{
    ERR_ASSERT(eId < SYS_USART_NUM);

    sys_state_t eRetval = SYS_SUCCESS;

    if ( eId == SYS_USART_0)
    {
        HAL_StatusTypeDef eHalResult = HAL_UART_Receive_IT(usart_handler_list[eId]->handler, pu8Data, u16MaxLen);

        if ( eHalResult != HAL_OK )
        {
            eRetval = (eHalResult == HAL_BUSY) ? SYS_BUSY : SYS_ERROR;
        }
    }
    else if ( eId == SYS_USART_1)
    {
        HAL_StatusTypeDef eHalResult = HAL_UART_Receive_DMA(usart_handler_list[eId]->handler, pu8Data, u16MaxLen);

        if ( eHalResult != HAL_OK )
        {
            eRetval = (eHalResult == HAL_BUSY) ? SYS_BUSY : SYS_ERROR;
        }
    }

    return eRetval;
}

uint16_t sys_usart_get_read_count(sys_usart_id_t eId)
{
    ERR_ASSERT(eId < SYS_USART_NUM);

    uint16_t u16RedData = 0U;

    if ( eId == SYS_USART_0)
    {
        u16RedData = usart_handler_list[eId]->handler->RxXferCount;
    }
    else if ( eId == SYS_USART_1)
    {
        u16RedData = usart_handler_list[eId]->handler->RxXferSize - usart_handler_list[eId]->handler->hdmarx->Instance->NDTR;
    }

    return u16RedData;
}

/* EOF */
