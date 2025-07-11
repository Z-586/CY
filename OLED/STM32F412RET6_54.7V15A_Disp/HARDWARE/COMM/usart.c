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

UART_HandleTypeDef huart1;  				//UART_HandleTypeDef �ṹ�����
uint8_t  aRxBuffer_1[RXBUFFERSIZE];			//HAL��ʹ�õĴ��ڽ��ջ���
uint8_t  USART1_RX_BUF[USART_REC_LEN];      //���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART1_RX_STA = 0;       			//����״̬���	

UART_HandleTypeDef huart2;  				// UART_HandleTypeDef �ṹ�����
uint8_t  aRxBuffer_2[RXBUFFERSIZE];			//HAL��ʹ�õĴ��ڽ��ջ���
uint8_t  USART2_RX_BUF[USART_REC_LEN];      //���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART2_RX_STA = 0;       			//����״̬���	


extern SemaphoreHandle_t Uart2Semaphore;

/*************************************************************************************************
*	�� �� ��:	HAL_UART_MspInit
*	��ڲ���:	huart - UART_HandleTypeDef����ı���������ʾ����Ĵ���
*	�� �� ֵ:	��
*	��������:	��ʼ����������
*	˵    ��:	��		
*************************************************************************************************/


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	if(huart->Instance == USART1) {
		__HAL_RCC_GPIOA_CLK_ENABLE();					//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();					//ʹ��USART1ʱ��

		GPIO_InitStruct.Pin       = GPIO_PIN_9|GPIO_PIN_10;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	} else if(huart->Instance == USART2) {
		__HAL_RCC_GPIOA_CLK_ENABLE();					//ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();					//ʹ��USART2ʱ��

		GPIO_InitStruct.Pin = GPIO_PIN_2;				//PA2
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;			//�����������
		GPIO_InitStruct.Pull = GPIO_PULLUP;				//����
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;		//����
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;	//����ΪUSART2
		HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);	   		//��ʼ��PA2

		GPIO_InitStruct.Pin = GPIO_PIN_3;				//PA3
		HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);	   		//��ʼ��PA3	

		HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART2�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn, 13, 0);	   		
	}
}


/*************************************************************************************************
*	�� �� ��:	USART1_Init
*	��ڲ���:	��
*	�� �� ֵ:	��
*	��������:	��ʼ����������
*	˵    ��:	��		 
*************************************************************************************************/
void USART1_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;   //��Ӳ������
	huart1.Init.Mode = UART_MODE_TX_RX;
	if (HAL_UART_Init(&huart1) != HAL_OK) {

	}
//	HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer_1, RXBUFFERSIZE);	//�ú����Ὺ�������ж�
//	__HAL_UART_CLEAR_IDLEFLAG(&huart1);  								//���IDLE��־
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);       					//ʹ��IDLE�ж�
}


/*************************************************************************************************
*	�� �� ��:	USART2_Init
*	��ڲ���:	��
*	�� �� ֵ:	��
*	��������:	��ʼ����������
*	˵    ��:	��		 
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
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;   //��Ӳ������
	huart2.Init.Mode = UART_MODE_TX_RX;
	if (HAL_UART_Init(&huart2) != HAL_OK) {

	}
	HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer_2, RXBUFFERSIZE);	//�ú����Ὺ�������ж�
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);  								//���IDLE��־
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);       					//ʹ��IDLE�ж�
}


//���ڽ����жϻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) {									//����Ǵ���1
		USART1_RX_BUF[USART1_RX_STA++] = aRxBuffer_1[0];
		if(USART1_RX_STA > (USART_REC_LEN-1)) {
			USART1_RX_STA = 0;									
		}
	} else if(huart->Instance == USART2) {							//����Ǵ���2
		USART2_RX_BUF[USART2_RX_STA++] = aRxBuffer_2[0];
		if(USART2_RX_STA > (USART_REC_LEN-1)) {
			USART2_RX_STA = 0;								
		}
	}
}


//���ڽ��տ����жϻص�����������Ǵ�hal�����¼ӵ�
void HAL_UART_IdleCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken;
	
    if(huart->Instance == USART1) {  
  
    } else if(huart->Instance == USART2) {  
        //һ��Ҫ�������ж�һ�£��Ƿ��Ѿ����յ����ݣ���ֹ�󴥷�
        //�����Է����ڿ����տ�ʼʹ��idle�жϵ�ʱ�򣬻ᴥ��һ��idle�жϡ�
        if(USART2_RX_STA >= 1) {
            //�Ѿ����������жϻص���ʣ���������û�����
			//�ͷŶ�ֵ�ź���
			xSemaphoreGiveFromISR(Uart2Semaphore, &xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);						//�����Ҫ�Ļ�����һ�������л�
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//			HAL_UART_Transmit(&huart2, (uint8_t *)USART2_RX_BUF, USART2_RX_STA, 0xffff);
//			USART2_RX_STA = 0;
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        }    
    }
}


//���ڽ��մ����жϣ���һ�ڻ�˵��ΪɶҪ�����
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(HAL_UART_GetError(huart) & HAL_UART_ERROR_PE){		/*!< Parity error            */
		//��żУ�����
		__HAL_UART_CLEAR_PEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_NE){ /*!< Noise error             */
		//��������
		__HAL_UART_CLEAR_NEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_FE){ /*!< Frame error             */
		//֡��ʽ����
		__HAL_UART_CLEAR_FEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_ORE){ /*!< Overrun error           */
		//����̫�മ�����������մ���
		__HAL_UART_CLEAR_OREFLAG(huart);
	}
    //��������ڷ����˴���һ��Ҫ������ʹ�ܽ����ж�
    if(huart ->Instance == USART1){
		HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer_1, RXBUFFERSIZE);	//�ú����Ὺ�������ж�
	} else if(huart ->Instance == USART2){
		HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer_2, RXBUFFERSIZE);	//�ú����Ὺ�������ж�
	}
    //��������......
}


//����2�жϷ������
void USART2_IRQHandler(void)                	
{ 
    uint32_t timeout = 0;
    uint32_t maxDelay = 0x1FFFF;
	
	HAL_UART_IRQHandler(&huart2);	//����HAL���жϴ����ú���
	
    timeout = 0;
    while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY) /* �ȴ����� */
    {
        timeout++;                              /* ��ʱ���� */
        if(timeout > maxDelay)
        {
            break;
        }
    }
     
    timeout = 0;
    /* һ�δ������֮�����¿����жϲ�����RxXferCountΪ1 */
    while (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer_2, RXBUFFERSIZE) != HAL_OK) {
        timeout++;                              /* ��ʱ���� */
        if (timeout > maxDelay)
        {
            break;
        }
    }
}


/*************************************************************************************************
*	�� �� ��:	fputc
*	��ڲ���:	ch - Ҫ������ַ� ��  f - �ļ�ָ�루�����ò�����
*	�� �� ֵ:	����ʱ�����ַ�������ʱ���� EOF��-1��
*	��������:	�ض��� fputc ������Ŀ����ʹ�� printf ����
*	˵    ��:	��		
*************************************************************************************************/
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);	// ���͵��ֽ�����
	return (ch);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
