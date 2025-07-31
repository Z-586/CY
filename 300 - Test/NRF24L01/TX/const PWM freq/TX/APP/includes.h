#ifndef INCLUDES_H
#define INCLUDES_H

/* ----------------------- FreeRTOS includes ----------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* ----------------------- HardWare includes ----------------------------------*/

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"
#include <stdio.h>

/* ----------------------- Platform includes --------------------------------*/
#include "main.h"

/* ----------------------- Modbus includes ----------------------------------*/

/* ----------------------- FatFS includes ----------------------------------*/

/* ----------------------- Segger_RTT includes ----------------------------------*/

/* ----------------------- APP includes ----------------------------------*/



/************************************************
//.
************************************************/
typedef struct {
	uint8_t type;
	uint8_t addr;
	char cmdcode[8];
	uint16_t air_buf[4];
} AirMsg;


typedef struct {
	char lightId[8];
	char status[8];
} LightMsg;


/************************************************
//全局变量声明.
************************************************/


/************************************************
//操作系统变量声明.
************************************************/
extern QueueHandle_t NetMsg_Queue;										//服务器接收数据
extern QueueHandle_t PanelRcvMsg_Queue;									//灯面板接收数据
extern QueueHandle_t AirRcvMsg_Queue;									//温控器接收数据
extern QueueHandle_t SetLightMsg_Queue;									//按键配对灯设备(用于测试)
extern QueueHandle_t SetQRcodeMsg_Queue;								//按键配对二维码
extern QueueHandle_t AirQuery_Queue;									//温控器扫描数据
extern QueueHandle_t IrdaRcvMsg_Queue;									//红外控制数据
extern QueueHandle_t LightRspMsg_Queue;									//灯应答数据
extern QueueHandle_t LightSendMsg_Queue;								//灯发送数据
extern QueueHandle_t Light433MRcvMsg_Queue;								//灯便利贴数据
extern QueueHandle_t SetTVMsg_Queue;									//电视上传码库数据


extern SemaphoreHandle_t AirRspSemaphore;								//温控器二值信号量句柄
extern SemaphoreHandle_t TVRspSemaphore;								//电视机二值信号量句柄
extern SemaphoreHandle_t PanelRspSemaphore;								//面板二值信号量句柄
extern SemaphoreHandle_t SetLightSemaphore;								//二值信号量句柄
extern SemaphoreHandle_t SetQRcodeSemaphore;							//设置二维码二值信号量句柄
extern SemaphoreHandle_t AirOperateSemaphore;							//二值信号量句柄
extern SemaphoreHandle_t QRcodeSemaphore;								//二值信号量句柄
extern SemaphoreHandle_t LightRspSemaphore;								//二值信号量句柄

//互斥信号量句柄
extern SemaphoreHandle_t AirMutexSemaphore;								//温控器互斥信号量句柄
extern SemaphoreHandle_t ToServerMutexSemaphore;						//服务器发送数据互斥信号量句柄
extern SemaphoreHandle_t LightMutexSemaphore;							//灯面板互斥信号量句柄


extern TaskHandle_t NetTask_Handler;									//任务句柄								
extern TaskHandle_t PanelTask_Handler;									//任务句柄
//extern TaskHandle_t RcvFromAirTask_Handler;							//任务句柄
extern TaskHandle_t AirObjTask_Handler;									//任务句柄
extern TaskHandle_t UploadMsg_task_Handler;								//任务句柄
//extern TaskHandle_t KeyScan_task_Handler;								//任务句柄
extern TaskHandle_t SetLight_task_Handler;								//任务句柄
extern TaskHandle_t SetQRcode_task_Handler;								//任务句柄
extern TaskHandle_t IrdaTask_Handler;									//任务句柄
extern TaskHandle_t LightObj_task_Handler;								//任务句柄
extern TaskHandle_t SetTV_task_Handler;									//任务句柄
extern TaskHandle_t Light433M_task_Handler;								//任务句柄

extern TaskHandle_t FullbridgeCtlTask_Handler;							//任务句柄
extern TaskHandle_t VoltRegTask_Handler;								//任务句柄
extern TaskHandle_t DAC_CtlTask_Handler;								//任务句柄

/************************************************
//全局函数声明.
************************************************/
uint16_t usCRC16( uint8_t * pucFrame, uint16_t usLen );
void Get_CPU_ID(void);

#endif /* INCLUDES_H */
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
