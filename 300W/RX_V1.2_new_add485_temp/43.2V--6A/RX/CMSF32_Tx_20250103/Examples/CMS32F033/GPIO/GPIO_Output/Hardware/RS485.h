#ifndef _RS485_H__
#define _RS485_H__

#include "cms32f033.h"
#include "stdio.h"
#include "demo_uart.h"
#include <string.h>
#include "fmc.h"



#define Temp_ERROR 	    	0x06	//温度错误
#define Temp_Over_ERROR 	0x07	//过温
#define V_Low_ERROR 	    0x08	//自检电压过低
#define V_Over_ERROR 		0x09	//自检电压过高
#define I_Over_ERROR 		0x0A	//自检电流过高
#define nrf2_4_ERROR 		0x0B	//通讯故障



typedef uint16_t u16;
typedef uint32_t u32;

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

struct RX_parameter {
	uint32_t 	RX_I;
	uint32_t 	RX_Uout;
	int32_t 	RX_T1;
	uint16_t    State;
	uint32_t    RX_Power;
};
extern R_Buffer Buffer_Rx;

extern struct RX_parameter RXparameter;
uint16_t usCRC16( uint8_t * pucFrame, uint16_t usLen );
void RS485_Init(void);
void RS485_Anylize(void);

#endif 

