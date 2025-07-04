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
//ȫ�ֱ�������.
************************************************/


/************************************************
//����ϵͳ��������.
************************************************/
extern QueueHandle_t NetMsg_Queue;										//��������������
extern QueueHandle_t PanelRcvMsg_Queue;									//������������
extern QueueHandle_t AirRcvMsg_Queue;									//�¿�����������
extern QueueHandle_t SetLightMsg_Queue;									//������Ե��豸(���ڲ���)
extern QueueHandle_t SetQRcodeMsg_Queue;								//������Զ�ά��
extern QueueHandle_t AirQuery_Queue;									//�¿���ɨ������
extern QueueHandle_t IrdaRcvMsg_Queue;									//�����������
extern QueueHandle_t LightRspMsg_Queue;									//��Ӧ������
extern QueueHandle_t LightSendMsg_Queue;								//�Ʒ�������
extern QueueHandle_t Light433MRcvMsg_Queue;								//�Ʊ���������
extern QueueHandle_t SetTVMsg_Queue;									//�����ϴ��������


extern SemaphoreHandle_t AirRspSemaphore;								//�¿�����ֵ�ź������
extern SemaphoreHandle_t TVRspSemaphore;								//���ӻ���ֵ�ź������
extern SemaphoreHandle_t PanelRspSemaphore;								//����ֵ�ź������
extern SemaphoreHandle_t SetLightSemaphore;								//��ֵ�ź������
extern SemaphoreHandle_t SetQRcodeSemaphore;							//���ö�ά���ֵ�ź������
extern SemaphoreHandle_t AirOperateSemaphore;							//��ֵ�ź������
extern SemaphoreHandle_t QRcodeSemaphore;								//��ֵ�ź������
extern SemaphoreHandle_t LightRspSemaphore;								//��ֵ�ź������

//�����ź������
extern SemaphoreHandle_t AirMutexSemaphore;								//�¿��������ź������
extern SemaphoreHandle_t ToServerMutexSemaphore;						//�������������ݻ����ź������
extern SemaphoreHandle_t LightMutexSemaphore;							//����廥���ź������


extern TaskHandle_t NetTask_Handler;									//������								
extern TaskHandle_t PanelTask_Handler;									//������
//extern TaskHandle_t RcvFromAirTask_Handler;							//������
extern TaskHandle_t AirObjTask_Handler;									//������
extern TaskHandle_t UploadMsg_task_Handler;								//������
//extern TaskHandle_t KeyScan_task_Handler;								//������
extern TaskHandle_t SetLight_task_Handler;								//������
extern TaskHandle_t SetQRcode_task_Handler;								//������
extern TaskHandle_t IrdaTask_Handler;									//������
extern TaskHandle_t LightObj_task_Handler;								//������
extern TaskHandle_t SetTV_task_Handler;									//������
extern TaskHandle_t Light433M_task_Handler;								//������

extern TaskHandle_t FullbridgeCtlTask_Handler;							//������
extern TaskHandle_t VoltRegTask_Handler;								//������
extern TaskHandle_t DAC_CtlTask_Handler;								//������

/************************************************
//ȫ�ֺ�������.
************************************************/
uint16_t usCRC16( uint8_t * pucFrame, uint16_t usLen );
void Get_CPU_ID(void);

#endif /* INCLUDES_H */
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
