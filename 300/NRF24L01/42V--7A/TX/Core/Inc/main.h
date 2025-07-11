/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

//#include "drv_ci24r1.h"
#include "drv_rf24L01.h"
#include "drv_spi.h"
#include "hrtim.h"

#include "AppBlePro.h"
#include "AppFaultHandler.h"
/* USER CODE END Includes */
//#pragma diag_suppress 1035
/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OSC_IN_L_Pin GPIO_PIN_14
#define OSC_IN_L_GPIO_Port GPIOC
#define OSC_OUT_L_Pin GPIO_PIN_15
#define OSC_OUT_L_GPIO_Port GPIOC
#define OSC_IN_H_Pin GPIO_PIN_0
#define OSC_IN_H_GPIO_Port GPIOF
#define OSC_OUT_H_Pin GPIO_PIN_1
#define OSC_OUT_H_GPIO_Port GPIOF
/*****ADC*****/
#define ADC0_Pin GPIO_PIN_1
#define ADC0_GPIO_Port GPIOC
#define ADC1_Pin GPIO_PIN_2
#define ADC1_GPIO_Port GPIOA
#define ADC_5V_Pin GPIO_PIN_1
#define ADC_5V_GPIO_Port GPIOA
#define ADC5_Pin GPIO_PIN_15
#define ADC5_GPIO_Port GPIOB
#define ADC6_Pin GPIO_PIN_14
#define ADC6_GPIO_Port GPIOB
/*****ADC*****/
#define CTL_FAN_PWM_Pin GPIO_PIN_15
#define CTL_FAN_PWM_GPIO_Port GPIOA
#define CTL_FAN_SPEED_Pin GPIO_PIN_3
#define CTL_FAN_SPEED_GPIO_Port GPIOB
#define FLAG_READY_Pin GPIO_PIN_12
#define FLAG_READY_GPIO_Port GPIOB
#define FLAG_FAULT_Pin GPIO_PIN_13
#define FLAG_FAULT_GPIO_Port GPIOB
#define FLAG_RST_Pin GPIO_PIN_10
#define FLAG_RST_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_5
#define LED_B_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_6
#define LED_G_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_7
#define LED_R_GPIO_Port GPIOA
#define PWM_1_Pin GPIO_PIN_8
#define PWM_1_GPIO_Port GPIOA
#define PWM_2_Pin GPIO_PIN_9
#define PWM_2_GPIO_Port GPIOA
#define CTL_GD_Pin GPIO_PIN_4
#define CTL_GD_GPIO_Port GPIOC
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define RS485_PM_TX_Pin GPIO_PIN_10
#define RS485_PM_TX_GPIO_Port GPIOC
#define RS485_PM_RX_Pin GPIO_PIN_11
#define RS485_PM_RX_GPIO_Port GPIOC
#define RS485_DEBUG_TX_Pin GPIO_PIN_12
#define RS485_DEBUG_TX_GPIO_Port GPIOC
#define RS485_DEBUG_RX_Pin GPIO_PIN_2
#define RS485_DEBUG_RX_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
//参数设置
#define RS485_ADDR			1
#define DEBUG_ON            1

#define Set_140K  19428 //SET_PWM_Freq(140000)
#define Set_88K   30909//SET_PWM_Freq(88000)
#define Set_100K  27200//SET_PWM_Freq(100000)
#define Set_86K   31627//SET_PWM_Freq(86000)
#define Set_85K   32000//SET_PWM_Freq(85000)
#define Set_95K  28631//SET_PWM_Freq(100000)

//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define STATE_FLAGE_DEFAULT			0
#define STATE_FLAGE_CHARGING		1
#define STATE_FLAGE_BATT_FULL		2
#define STATE_FLAGE_ERROR			3
#define STATE_FLAGE_SUSPEND			4
//RS485
#define UART1_BUF_LENGTH    128
typedef struct
{
	uint8_t  buf[UART1_BUF_LENGTH];
	uint16_t len;
} T_Buffer;


typedef struct
{
	uint8_t  buf[UART1_BUF_LENGTH];
	uint16_t len;
} R_Buffer;


//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define RX_HIGH_TEMP_ERR			90		//1803
#define RX_HIGH_TEMP_NORMAL			70		//1661

#define RX_LOW_TEMP_ERR				-35		//1803
#define RX_LOW_TEMP_NORMAL			-30		//1661

#define TX_TEMP_OPEN_fAN			30		//55		//480	//510
#define TX_TEMP_CLOSE_fAN			25		//45		//380	//400

#define TX_HIGH_TEMP_ERR			80		//80   	    //900   //90
#define TX_HIGH_TEMP_NORMAL			55		//65		//550	//70

#define TX_LOW_TEMP_ERR				-35		//80   	    //900   //90
#define TX_LOW_TEMP_NORMAL			-30		//65		//550	//70


//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// 恒压,小于满载电流的80% 充满,小于满载电流的10%
// 系统满载电流为2.5A,电压72V
// 此系统特例,充满电流设置为0.5A
#define RECHARGE_I_FULL				500				//500
#define RECHARGE_U_FULL				28500			//500



//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define EFF_HIGH_POWER_I_DETECTION  2000 

#define EFF_HIGH_POWER_MIN_1	    65
#define EFF_HIGH_POWER_MAX_1	    95

#define EFF_HIGH_POWER_MIN_2		75
#define EFF_HIGH_POWER_MAX_2	    95
				


//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define SYS_NORMAL   			0xff
#define SYS_CC                  0
#define SYS_CV                  1
#define PEND_RX_TEMPER          2
#define ERR_I_Max             	3
#define ERR_V_Max               4
#define SYS_BATT_FULL           5
#define ERR_PFC_U    			10
#define ERR_RDY_FUT  			11
#define PEND_TX_TEMP            17
#define ERR_TX_TEMP             22
#define ERR_RX_TEMP             23
#define ERR_NRF24L01			0x15
//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define UNUSED_VARIABLE(X)  		((void)(X))
#define UNUSED_PARAMETER(X) 		UNUSED_VARIABLE(X)
#define UNUSED_RETURN_VALUE(X) 		UNUSED_VARIABLE(X)

#define UART_TX_BUF_SIZE        256                             /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE        256                             /**< UART RX buffer size. */

//1 
//任务优先级
#define START_TASK_PRIO			 					1	
//任务堆栈大小
#define START_STK_SIZE 		  						50  
//任务函数
void start_task(void *pvParameters);												

//1
//任务优先级
#define LedProc_TASK_PRIO                  			4
//任务堆栈大小
#define LedProc_STK_SIZE                     		200  
//任务函数
void LedProc_task(void *pvParameters);

//2
//任务优先级
#define RS485Proc_TASK_PRIO			 			         18
//任务堆栈大小
#define RS485Proc_STK_SIZE 					           250 
//任务函数
void RS485_Proc_task(void *pvParameters);	

//3
//任务优先级
#define Period_TASK_PRIO                  			8
//任务堆栈大小
#define Period_STK_SIZE                     		250  
//任务函数
void Period_task(void *pvParameters);	

//4
//任务优先级
#define FaultHandler_TASK_PRIO                  	6  //12
//任务堆栈大小
#define FaultHandler_STK_SIZE                     250  
//任务函数
void FaultHandler_task(void *pvParameters);

//7
//任务优先级
#define BlePro_TASK_PRIO                  			15
//任务堆栈大小
#define BlePro_STK_SIZE                     		250  
//任务函数
void BlePro_task(void *pvParameters);

/************************************************
//操作系统变量
************************************************/
extern TaskHandle_t 					StartTask_Handler;			    //任务句柄	1
extern TaskHandle_t 					TestTask_Handler;				//任务句柄  2
extern TaskHandle_t 					LedProcTask_Handler;			//任务句柄  3
extern TaskHandle_t           			Period_task_Handler;			//任务句柄  3
extern TaskHandle_t 					FaultHandlerTask_Handler;       //任务句柄  4
extern TaskHandle_t 					BleCommTask_Handler;			//任务句柄  5
extern TaskHandle_t 					RcvPro2Task_Handler;            //任务句柄  6
extern TaskHandle_t 					BleProTask_Handler;				//任务句柄  7
extern TaskHandle_t 					RS485ProcTask_Handler;				//任务句柄  7

extern SemaphoreHandle_t 			BleProcSemaphore;				//蓝牙处理信号量
extern SemaphoreHandle_t      		BleSendSemaphore;				//蓝牙发送信号量
extern SemaphoreHandle_t      		RcvProc2Semaphore;
extern SemaphoreHandle_t      		RS485DataSemaphore;
extern SemaphoreHandle_t 			MutexPrintfHandle;

//全局变量声明
extern struct TX_parameter 		TXparameter ;
extern struct RX_parameter 		RXparameter ;
extern uint8_t       		    g_tx_system_stat;
extern uint8_t					g_system_status;
extern uint8_t	                RS485_Addr;
extern uint8_t                  baud;
extern uint16_t                 PwmFreq;
extern uint8_t 					NRF_IS_OK_STATE;

extern HRTIM_HandleTypeDef hhrtim1;
extern IWDG_HandleTypeDef hiwdg;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim8;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart3;

uint32_t SET_PWM_Freq(uint32_t Freq);
void Debug_Printf(char *format, ...);
uint32_t Get_Adc_Average(unsigned long ch, uint16_t times);
void CLOSE_OPERATION(void);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
