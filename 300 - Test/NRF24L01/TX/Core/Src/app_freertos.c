/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void start_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
    xTaskCreate((TaskFunction_t )start_task,            	//任务函数
                (const char*    )"start_task",          	//任务名称
                (uint16_t       )START_STK_SIZE,        	//任务堆栈大小
                (void*          )NULL,                  	//传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,      		//任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   	//任务句柄

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

void RS485_Proc_task(void *pvParameters)
{
	
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void start_task(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	taskENTER_CRITICAL();

//    if (pdPASS != xTaskCreate((TaskFunction_t )LedProc_task,            
//                (const char*    )"LedProc_task",           
//                (uint16_t       )LedProc_STK_SIZE,
//                (void*          )NULL,                                
//                (UBaseType_t    )LedProc_TASK_PRIO,        
//                (TaskHandle_t*  )&LedProcTask_Handler)) {
//		
//	}
				
    if (pdPASS != xTaskCreate((TaskFunction_t )Period_task,
                (const char*    )"Period_task",
                (uint16_t       )Period_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Period_TASK_PRIO,
   				(TaskHandle_t*  )&Period_task_Handler)) {
					
	}
				
//    if (pdPASS != xTaskCreate((TaskFunction_t )FaultHandler_task,            
//                (const char*    )"FaultHandler_task",           
//                (uint16_t       )FaultHandler_STK_SIZE,
//                (void*          )NULL,                                
//                (UBaseType_t    )FaultHandler_TASK_PRIO,        
//                (TaskHandle_t*  )&FaultHandlerTask_Handler)) {
//						
//	} 
			
    if (pdPASS != xTaskCreate((TaskFunction_t )BlePro_task,            
                (const char*    )"BlePro_task",           
                (uint16_t       )BlePro_STK_SIZE,
                (void*          )NULL,                                
                (UBaseType_t    )BlePro_TASK_PRIO,        
                (TaskHandle_t*  )&BleProTask_Handler)) {
					
	}
				
//	if (pdPASS != xTaskCreate((TaskFunction_t )RS485_Proc_task,
//                (const char*    )"RS485_Proc_task",
//                (uint16_t       )RS485Proc_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )RS485Proc_TASK_PRIO,
//                (TaskHandle_t*  )&RS485ProcTask_Handler)) {
//					
//	}
				
	BleSendSemaphore 		  =	xSemaphoreCreateBinary();
	RS485DataSemaphore        =	xSemaphoreCreateBinary();
	RcvProc2Semaphore 	      =	xSemaphoreCreateBinary();
	MutexPrintfHandle         = xSemaphoreCreateMutex();
	
	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Task1 */
/**
* @brief Function implementing the APP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task1 */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

