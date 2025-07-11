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

#define USART_REC_LEN  200  					//定义最大接收字节数 200

#define RXBUFFERSIZE   1 						//缓存大小

extern uint8_t  aRxBuffer_1[RXBUFFERSIZE];		//HAL库USART接收Buffer
extern uint8_t  USART1_RX_BUF[USART_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t USART1_RX_STA;         			//接收状态标记

extern uint8_t  aRxBuffer_2[RXBUFFERSIZE];		//HAL库USART接收Buffer
extern uint8_t  USART2_RX_BUF[USART_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t USART2_RX_STA;         			//接收状态标记

/*---------------------------------------------- 函数声明 ---------------------------------------*/

void USART1_Init(void);	// USART1初始化函数
void USART2_Init(void);	// USART2初始化函数

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
