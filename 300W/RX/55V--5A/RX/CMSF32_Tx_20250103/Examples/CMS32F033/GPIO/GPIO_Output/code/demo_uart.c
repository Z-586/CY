/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/****************************************************************************/
/** \file demo_uart.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "demo_uart.h"
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/

/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

/*****************************************************************************
 ** \brief	UART_UART1_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_UART1_Config(uint32_t Baud)
{
	/*
	1.设置UART 模式
	*/
	UART_ConfigRunMode(UART1, Baud, UART_WLS_8, UART_PARITY_NONE,UART_STOP_BIT_1);
	
	UART_EnableAuto(UART1);
	UART_EnableFIFO(UART1,FIFO_L0_1BYTE, FIFO_L2_8BYTE, FIFO_RST_EN, FIFO_RST_EN);	
	
	/*
	2.设置UART 时钟
	*/	
	SYS_EnablePeripheralClk(SYS_CLK_UART1_MSK);
	/*
	3.设置UART 接口
	*/	
	SYS_SET_IOCFG(IOP32CFG,SYS_IOCFG_P32_RXD1);	
	SYS_SET_IOCFG(IOP34CFG,SYS_IOCFG_P34_TXD1);	

	/*
	4.设置UART 中断
	*/	
	
	UART_EnableTHREInt(UART1);
	UART_EnableRBRInt(UART1);
	NVIC_SetPriority(UART1_IRQn,3);	
	NVIC_EnableIRQ(UART1_IRQn); 
}

#define DEBUG_ON 0

void Debug_Printf(char *format, ...)
{
#if DEBUG_ON
	char  buf_str[256];
	va_list   v_args;

	va_start(v_args, format);
	(void)vsnprintf((char       *)&buf_str[0],
				   (size_t      ) sizeof(buf_str),
				   (char const *) format,
								  v_args);
	va_end(v_args);

	/* 互斥信号量 */
	printf("%s", buf_str);
#endif
}


