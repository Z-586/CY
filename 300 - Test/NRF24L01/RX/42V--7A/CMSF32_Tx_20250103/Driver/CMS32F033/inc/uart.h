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

/*****************************************************************************/
/** \file uart.h
**
** History:
** 
*****************************************************************************/
#ifndef __UART_H_
#define __UART_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "CMS32F033.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
/*----------------------------------------------------------------------------
 **UART 字长
-----------------------------------------------------------------------------*/
#define		UART_WLS_5			(0x00UL)		/*5位字长*/
#define		UART_WLS_6			(0x01UL)		/*6位字长*/
#define		UART_WLS_7			(0x02UL)		/*7位字长*/		
#define		UART_WLS_8			(0x03UL)		/*8位字长*/
/*----------------------------------------------------------------------------
 **UART 停止位
-----------------------------------------------------------------------------*/
#define		UART_STOP_BIT_1			(0x00UL)		/*1位停止位*/
#define		UART_STOP_BIT_2			(0x01UL<<UART_LCR_SBS_Pos)		/*2位停止位*/
/*----------------------------------------------------------------------------
 **UART 奇偶校验
-----------------------------------------------------------------------------*/
#define		UART_PARITY_NONE		(0x00UL)		/*无奇偶校验*/
#define		UART_PARITY_ODD			(0x00UL|(0x1<<UART_LCR_PEN_Pos))	/*奇偶校验*/
#define		UART_PARITY_EVEN		((0x01UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))	/*偶偶校验*/
#define		UART_PARITY_FORCE_1		((0x02UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))		/*校验位强制为1*/
#define		UART_PARITY_FORCE_0		((0x03UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))		/*校验位强制为0*/

/*----------------------------------------------------------------------------
 **UART FIFO Level
-----------------------------------------------------------------------------*/
#define 	FIFO_L0_1BYTE			(0x0UL)		
#define 	FIFO_L1_4BYTE			(0x1UL)		
#define 	FIFO_L2_8BYTE			(0x2UL)		
#define 	FIFO_L3_14BYTE			(0x3UL)		
/*----------------------------------------------------------------------------
 **UART FIFO Reset 
-----------------------------------------------------------------------------*/
#define 	FIFO_RST_EN				(0x1UL)			
#define 	FIFO_RST_DIS			(0x0UL)		

/*----------------------------------------------------------------------------
 **UART XON XOFF
-----------------------------------------------------------------------------*/
#define		UART_USE_XON1_XOFF1		(0x02)
#define		UART_USE_XON2_XOFF2		(0x01)
#define		UART_USE_BOTH_XON_XOFF		(0x03)		/*XON1&XON2,XOFF1&XOFF2*/

							
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/

/*****************************************************************************
 ** \brief	UART_ConfigRunMode
 **			配置异步串口运行模式
 ** \param [in] uart: UART0、UART1
 **				Baudrate:波特率
 **				WordLength:(1)UART_WLS_5
 **							(2)UART_WLS_6
 **							(3)UART_WLS_7
 **							(4)UART_WLS_8
 **				Parity:	(1)UART_PARITY_NONE
 **						(2)UART_PARITY_ODD
 **						(3)UART_PARITY_EVEN
 **						(4)UART_PARITY_FORCE_1
 **					 	(5)UART_PARITY_FORCE_0
 **				StopBit:(1)UART_STOP_BIT_1
 **						(2)UART_STOP_BIT_2
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_ConfigRunMode(UART_T* uart,uint32_t Baudrate,uint32_t WordLength, 
						uint32_t Parity, uint32_t StopBit);

/*****************************************************************************
 ** \brief	UART_WriteTHR
 **			写发送保持寄存器
 ** \param [in] uart: UART0、UART1
 **				value: 8bit
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_WriteTHR(UART_T* uart, uint32_t value);

/*****************************************************************************
 ** \brief	UART_ReadRBR
 **			读接收缓存寄存器
 ** \param [in] uart: UART0、UART1
 ** \return  8bit data
 ** \note	
*****************************************************************************/
uint32_t  UART_ReadRBR(UART_T* uart);

/*****************************************************************************
 ** \brief	UART_EnableCTSInt
 **			开启CTS中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableCTSInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableCTSInt
 **			关闭CTS中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableCTSInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableRTSInt
 **			开启RTS中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableRTSInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableRTSInt
 **			关闭RTS中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableRTSInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableXOFInt
 **			开启XOFF中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableXOFInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableXOFInt
 **			关闭XOF中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableXOFInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableModemInt
 **			开启Modem中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableModemInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableModemInt
 **			关闭Modem中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableModemInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableRxLineInt
 **			开启接收线状态中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableRxLineInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableRxLineInt
 **			关闭接收线状态中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableRxLineInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableTHREInt
 **			开启发送保持器空中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableTHREInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableTHREInt
 **			关闭发送保持器空中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableTHREInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableRBRInt
 **			开启接收数据有效中断和接收定时器溢出中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_EnableRBRInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableRBRInt
 **			关闭接收数据有效中断和接收定时器溢出中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_DisableRBRInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableAuto
 **			开启xx高级功能
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_EnableAuto(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableAuto
 **			关闭xx高级功能
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_DisableAuto(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableFIFO
 **			开启FIFO功能
 ** \param [in] uart: UART0、UART1
 ** 			TxLevel: (1)FIFO_L0_1BYTE (2)FIFO_L1_4BYTE (3)FIFO_L2_8BYTE (4)FIFO_L3_14BYTE
 ** 			RxLevel: (1)FIFO_L0_1BYTE (2)FIFO_L1_4BYTE (3)FIFO_L2_8BYTE (4)FIFO_L3_14BYTE
 **				TxFIFOReset: (1)FIFO_RST_EN (2)FIFO_RST_DIS
 **				RxFIFOReset: (1)FIFO_RST_EN (2)FIFO_RST_DIS
 ** \return  none
 ** \note	FCR寄存器为只写寄存器，读取FCR的数据为无效值
*****************************************************************************/
void UART_EnableFIFO(UART_T* uart, uint32_t TxFIFOLvel, uint32_t RxFIFOLvel, uint32_t TxFIFOReset, uint32_t RxFIFOReset);
/*****************************************************************************
 ** \brief	UART_DisableFIFO
 **			关闭FIFO功能
 ** \param [in] uart: UART0、UART1
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_DisableFIFO(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableRTS
**			开启硬件接收流控制
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_EnableRTS(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableRTS
 **			关闭硬件接收流控制
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_DisableRTS(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableCTS
**			开启硬件发送流控制
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_EnableCTS(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableCTS
 **			关闭硬件发送流控制
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_DisableCTS(UART_T* uart);

/*****************************************************************************
 ** \brief	UART_GetCTSState
 **			获取CTS输入管脚状态
 ** \param [in] uart: UART0、UART1
 ** \return 0：低电平   1：高电平
 ** \note	
*****************************************************************************/
uint32_t  UART_GetCTSState(UART_T* uart);

/*****************************************************************************
 ** \brief	UART_RTSSet
 **			RTS脚输出高电平
 ** \param [in] uart: UART0、UART1
 ** \return 
 ** \note	
*****************************************************************************/
void UART_RTSSet(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_RTSClean
 **			RTS脚输出低电平
 ** \param [in] uart: UART0、UART1
 ** \return 
 ** \note	
*****************************************************************************/
void UART_RTSClean(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_SetXON1AndXOFF1
 **			设置XON1和XOFF1
 ** \param [in] uart: UART0、UART1
 **				Xon1 ：8位值
 **				Xoff1: 8位值
 ** \return 
 ** \note	
*****************************************************************************/
void UART_SetXON1AndXOFF1(UART_T* uart, uint8_t Xon1, uint8_t Xoff1);
/*****************************************************************************
 ** \brief	UART_SetXON2AndXOFF2
 **			RTS脚输出低电平
 ** \param [in] uart: UART0、UART1
 **				Xon2 ：8位值
 **				Xoff2: 8位值
 ** \return 
 ** \note	
*****************************************************************************/
void UART_SetXON2AndXOFF2(UART_T* uart, uint8_t Xon2, uint8_t Xoff2);
/*****************************************************************************
 ** \brief	UART_EnableXONAndXOFF
 **			开启XON_XOFF模式
 ** \param [in] uart: UART0、UART1
 **				Mode: (1)UART_USE_XON1_XOFF1
 **					  (2)UART_USE_XON2_XOFF2
 **					  (3)UART_USE_BOTH_XON_XOFF
 **						当连续接收到的两位数据，第一个 数据与 XON1 相同，
 **                     第二个数据与 XON2 相同，则 XON 有效。 同理当连续接收到的两位数据，
 **                     第一个数据 与 XOFF1 相同，第二个数据与 XOFF2 相同，则 XOFF 有效。
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableXONAndXOFF(UART_T* uart, uint32_t Mode);
/*****************************************************************************
 ** \brief	UART_DisableXONAndXOFF
 **			关闭XON_XOFF模式
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableXONAndXOFF(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_GetXONAndXOFFState
 **			获取XON与XOFF状态
 ** \param [in] uart: UART0、UART1
 ** \return 0：接收到XON字符  1：接收到XOFF字符
 ** \note	
*****************************************************************************/
uint32_t  UART_GetXONAndXOFFState(UART_T* uart);


#ifdef __cplusplus
}
#endif

#endif /* __UART_H_ */

