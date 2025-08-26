/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include <stdarg.h>
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int _io_putchar(int ch)
#else 
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__*/
    
// 重定向C语言中的printf函数 
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart5, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
/* USER CODE END 0 */

UART_HandleTypeDef huart5;
uint8_t  USART5_RX_BUF[USART_REC_LEN]; 
uint8_t  aRxBuffer_5[1];			//HAL库使用的串口接收缓冲

UART_HandleTypeDef huart3;

extern R_Buffer Buffer_Rx;
extern T_Buffer Buffer_Tx;
   

/* UART5 init function */
void MX_UART5_Init(uint32_t BAUD)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = BAUD;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */
  HAL_UART_Receive_IT(&huart5, (uint8_t *)aRxBuffer_5, 1);	//该函数会开启接收中断	
  __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);       	    //使能IDLE中断
  /* USER CODE END UART5_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART5;
    PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    GPIO_InitStruct.Pin = RS485_DEBUG_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_UART5;
    HAL_GPIO_Init(RS485_DEBUG_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RS485_DEBUG_RX_Pin;
    HAL_GPIO_Init(RS485_DEBUG_RX_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN UART5_MspInit 1 */
	HAL_NVIC_EnableIRQ(UART5_IRQn);
	HAL_NVIC_SetPriority(UART5_IRQn, 7, 0);
  /* USER CODE END UART5_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = RS485_PM_TX_Pin|RS485_PM_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();

    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    HAL_GPIO_DeInit(RS485_DEBUG_TX_GPIO_Port, RS485_DEBUG_TX_Pin);
    HAL_GPIO_DeInit(RS485_DEBUG_RX_GPIO_Port, RS485_DEBUG_RX_Pin);

  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOC, RS485_PM_TX_Pin|RS485_PM_RX_Pin);

  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}


void Debug_Printf(char *format, ...)
{
#if DEBUG_ON
	char  buf_str[256];
	va_list   v_args;

	va_start(v_args, format);
	(void)vsnprintf((char       *)&buf_str[0],
				   (size_t      ) sizeof(buf_str),
				   (char const *) format,
								  v_args);
	va_end(v_args);

	/* 互斥信号量 */
	xSemaphoreTake(MutexPrintfHandle, portMAX_DELAY );
	printf("%s", buf_str);
	xSemaphoreGive(MutexPrintfHandle);
#endif

}


/***************************************************************************//**
* @brief  USART5_SendAtStr(uint8_t * pArray, uint32_t len)
* @param  pData

* @retval None.
* Notes   None.
********************************************************************************
*/
void USART5_CommSendAtStr(uint8_t * pArray, uint32_t len)
{
	//HAL_UART_Transmit(&huart5,pArray,len,0xFFFF);
	HAL_UART_Transmit(&huart5, (uint8_t *)pArray, len, 100);	// 发送单字节数据
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == UART5) {
		Buffer_Rx.buf[Buffer_Rx.len++] = aRxBuffer_5[0];
		HAL_UART_Receive_IT(&huart5, (uint8_t *)aRxBuffer_5, 1);
	} 
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{  
	BaseType_t xHigherPriorityTaskWoken;
	
	HAL_UART_IRQHandler(&huart5);	//调用HAL库中断处理公用函数	
	
	if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE)) {
		__HAL_UART_CLEAR_IDLEFLAG(&huart5);
		if (Buffer_Rx.len > 2) {
	//		USART2_CommSendAtStr(Buffer_Rx.buf, Buffer_Rx.len);
	//		memset(Buffer_Rx.buf, 0, UART1_BUF_LENGTH);
	//		Buffer_Rx.len = 0;
			//释放二值信号量
			xSemaphoreGiveFromISR(RS485DataSemaphore, &xHigherPriorityTaskWoken);	//释放二值信号量
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);								//如果需要的话进行一次任务切换			
		}
	}
}

/* USER CODE END 1 */
