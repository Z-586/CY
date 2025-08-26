/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "semphr.h"

UART_HandleTypeDef huart1;  				//UART_HandleTypeDef 结构体变量
uint8_t  aRxBuffer_1[RXBUFFERSIZE];			//HAL库使用的串口接收缓冲
uint8_t  USART1_RX_BUF[USART_REC_LEN];      //接收缓冲,最大USART_REC_LEN个字节.
uint16_t USART1_RX_STA = 0;       			//接收状态标记	

UART_HandleTypeDef huart2;  				// UART_HandleTypeDef 结构体变量
uint8_t  aRxBuffer_2[RXBUFFERSIZE];			//HAL库使用的串口接收缓冲
uint8_t  USART2_RX_BUF[USART_REC_LEN];      //接收缓冲,最大USART_REC_LEN个字节.
uint16_t USART2_RX_STA = 0;       			//接收状态标记	


extern SemaphoreHandle_t Uart2Semaphore;

/*************************************************************************************************
*	函 数 名:	HAL_UART_MspInit
*	入口参数:	huart - UART_HandleTypeDef定义的变量，即表示定义的串口
*	返 回 值:	无
*	函数功能:	初始化串口引脚
*	说    明:	无		
*************************************************************************************************/


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	if(huart->Instance == USART1) {
		__HAL_RCC_GPIOA_CLK_ENABLE();					//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();					//使能USART1时钟

		GPIO_InitStruct.Pin       = GPIO_PIN_9|GPIO_PIN_10;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	} else if(huart->Instance == USART2) {
		__HAL_RCC_GPIOA_CLK_ENABLE();					//使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE();					//使能USART2时钟

		GPIO_InitStruct.Pin = GPIO_PIN_2;				//PA2
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;			//复用推挽输出
		GPIO_InitStruct.Pull = GPIO_PULLUP;				//上拉
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;		//高速
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;	//复用为USART2
		HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);	   		//初始化PA2

		GPIO_InitStruct.Pin = GPIO_PIN_3;				//PA3
		HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);	   		//初始化PA3	

		HAL_NVIC_EnableIRQ(USART2_IRQn);				//使能USART2中断通道
		HAL_NVIC_SetPriority(USART2_IRQn, 13, 0);	   		
	}
}


/*************************************************************************************************
*	函 数 名:	USART1_Init
*	入口参数:	无
*	返 回 值:	无
*	函数功能:	初始化串口配置
*	说    明:	无		 
*************************************************************************************************/
void USART1_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;   //无硬件流控
	huart1.Init.Mode = UART_MODE_TX_RX;
	if (HAL_UART_Init(&huart1) != HAL_OK) {

	}
//	HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer_1, RXBUFFERSIZE);	//该函数会开启接收中断
//	__HAL_UART_CLEAR_IDLEFLAG(&huart1);  								//清除IDLE标志
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);       					//使能IDLE中断
}


/*************************************************************************************************
*	函 数 名:	USART2_Init
*	入口参数:	无
*	返 回 值:	无
*	函数功能:	初始化串口配置
*	说    明:	无		 
*************************************************************************************************/
void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;   //无硬件流控
	huart2.Init.Mode = UART_MODE_TX_RX;
	if (HAL_UART_Init(&huart2) != HAL_OK) {

	}
	HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer_2, RXBUFFERSIZE);	//该函数会开启接收中断
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);  								//清除IDLE标志
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);       					//使能IDLE中断
}


//串口接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) {									//如果是串口1
		USART1_RX_BUF[USART1_RX_STA++] = aRxBuffer_1[0];
		if(USART1_RX_STA > (USART_REC_LEN-1)) {
			USART1_RX_STA = 0;									
		}
	} else if(huart->Instance == USART2) {							//如果是串口2
		USART2_RX_BUF[USART2_RX_STA++] = aRxBuffer_2[0];
		if(USART2_RX_STA > (USART_REC_LEN-1)) {
			USART2_RX_STA = 0;								
		}
	}
}


//串口接收空闲中断回调函数，这个是从hal里面新加的
void HAL_UART_IdleCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken;
	
    if(huart->Instance == USART1) {  
  
    } else if(huart->Instance == USART2) {  
        //一般要在这里判断一下，是否已经接收到数据，防止误触发
        //经测试发现在开机刚开始使能idle中断的时候，会触发一次idle中断。
        if(USART2_RX_STA >= 1) {
            //已经触发空闲中断回调，剩下内容由用户操作
			//释放二值信号量
			xSemaphoreGiveFromISR(Uart2Semaphore, &xHigherPriorityTaskWoken);	//释放二值信号量
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);						//如果需要的话进行一次任务切换
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//			HAL_UART_Transmit(&huart2, (uint8_t *)USART2_RX_BUF, USART2_RX_STA, 0xffff);
//			USART2_RX_STA = 0;
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        }    
    }
}


//串口接收错误中断，下一节会说明为啥要有这个
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(HAL_UART_GetError(huart) & HAL_UART_ERROR_PE){		/*!< Parity error            */
		//奇偶校验错误
		__HAL_UART_CLEAR_PEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_NE){ /*!< Noise error             */
		//噪声错误
		__HAL_UART_CLEAR_NEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_FE){ /*!< Frame error             */
		//帧格式错误
		__HAL_UART_CLEAR_FEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_ORE){ /*!< Overrun error           */
		//数据太多串口来不及接收错误
		__HAL_UART_CLEAR_OREFLAG(huart);
	}
    //当这个串口发生了错误，一定要在重新使能接收中断
    if(huart ->Instance == USART1){
		HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer_1, RXBUFFERSIZE);	//该函数会开启接收中断
	} else if(huart ->Instance == USART2){
		HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer_2, RXBUFFERSIZE);	//该函数会开启接收中断
	}
    //其他串口......
}


//串口2中断服务程序
void USART2_IRQHandler(void)                	
{ 
    uint32_t timeout = 0;
    uint32_t maxDelay = 0x1FFFF;
	
	HAL_UART_IRQHandler(&huart2);	//调用HAL库中断处理公用函数
	
    timeout = 0;
    while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY) /* 等待就绪 */
    {
        timeout++;                              /* 超时处理 */
        if(timeout > maxDelay)
        {
            break;
        }
    }
     
    timeout = 0;
    /* 一次处理完成之后，重新开启中断并设置RxXferCount为1 */
    while (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer_2, RXBUFFERSIZE) != HAL_OK) {
        timeout++;                              /* 超时处理 */
        if (timeout > maxDelay)
        {
            break;
        }
    }
}


/*************************************************************************************************
*	函 数 名:	fputc
*	入口参数:	ch - 要输出的字符 ，  f - 文件指针（这里用不到）
*	返 回 值:	正常时返回字符，出错时返回 EOF（-1）
*	函数功能:	重定向 fputc 函数，目的是使用 printf 函数
*	说    明:	无		
*************************************************************************************************/
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);	// 发送单字节数据
	return (ch);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
