#ifndef _RS485_H__
#define _RS485_H__

#include "cms32f033.h"
#include "stdio.h"
#include "demo_uart.h"
#include <string.h>
extern struct RX_parameter RXparameter;
uint16_t usCRC16( uint8_t * pucFrame, uint16_t usLen );

#endif 

