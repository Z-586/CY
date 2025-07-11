/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

#define USART_REC_LEN  200  					//�����������ֽ��� 200

#define RXBUFFERSIZE   1 						//�����С

extern uint8_t  aRxBuffer_1[RXBUFFERSIZE];		//HAL��USART����Buffer
extern uint8_t  USART1_RX_BUF[USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART1_RX_STA;         			//����״̬���

extern uint8_t  aRxBuffer_2[RXBUFFERSIZE];		//HAL��USART����Buffer
extern uint8_t  USART2_RX_BUF[USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART2_RX_STA;         			//����״̬���

/*---------------------------------------------- �������� ---------------------------------------*/

void USART1_Init(void);	// USART1��ʼ������
void USART2_Init(void);	// USART2��ʼ������

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
