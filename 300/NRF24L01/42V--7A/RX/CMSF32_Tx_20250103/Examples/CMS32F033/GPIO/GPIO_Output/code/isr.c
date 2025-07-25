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
/** \file isr.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "cms32f033.h"
#include "stdio.h"
#include "RS485.h"

/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/
extern uint8_t system_start;
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


/****************************************************************************
 ** \brief	NMI_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void NMI_Handler(void)
{
	
}
/****************************************************************************
 ** \brief	HardFault_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void HardFault_Handler(void)
{
	
}

/****************************************************************************
 ** \brief	SVC_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SVC_Handler(void)
{
	
}

/****************************************************************************
 ** \brief	PendSV_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void PendSV_Handler(void)
{
	
}
/****************************************************************************
 ** \brief	SysTick_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SysTick_Handler(void)
{

}
           
/****************************************************************************
 ** \brief	GPIO0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO0_IRQHandler(void)
{
	;
}

/****************************************************************************
 ** \brief	GPIO1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO1_IRQHandler(void)
{
	if(GPIO_GetIntFlag(GPIO1,GPIO_PIN_2))
	{
		GPIO1->DO_f.P3 = ~GPIO1->DO_f.P3;			/*P13翻转*/
		GPIO_ClearIntFlag(GPIO1,GPIO_PIN_2);
	}		
}
/****************************************************************************
 ** \brief	GPIO2_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO2_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	GPIO3_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO3_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	GPIO4_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO4_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	CCP_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void CCP_IRQHandler(void)
{
	
}

/****************************************************************************
 ** \brief	WWDT_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void WWDT_IRQHandler(void)
{
		
}
/****************************************************************************
 ** \brief	EPWM_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void EPWM_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	ADC1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void ADC1_IRQHandler(void)
{
	;
}
/****************************************************************************
 ** \brief	ACMP_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void ACMP_IRQHandler(void)
{
	if(ACMP_GetIntFlag(ACMP0))
	{
		system_start = 1;
		ACMP_ClearIntFlag(ACMP0);
	}	
}
/****************************************************************************
 ** \brief	UART0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void UART0_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	UART1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
static volatile uint32_t U_IntID;
static volatile uint32_t U_LSRFlag;
static volatile uint32_t U_count;
extern uint8_t U_ReceiveData[32];
extern volatile uint32_t U_RxdFlag;
extern uint8_t RS485_FLAG;
extern uint8_t RS485_Lenth;

extern volatile uint32_t U_TxdFlag ;
void UART1_IRQHandler(void)
{
/*----------------------------------------------------------------------------------------*/	
	U_IntID = (UART1->IIR & 0x0F);		//获取中断事件ID。此处不能打断点，会导致状态位丢失，读取后IIR寄存器值将会改变 
	U_IntID = U_IntID >>1;
	U_LSRFlag = UART1->LSR;
	
	switch(U_IntID)								//处理事件
	{
		case 0:										//Modem 状态改变
			break;
		case 0x1:									//THR 寄存器为空 （发送后 THR的值会自动清除）		
			U_TxdFlag =1;		
		  break;
					
		case 0x2:									// FIFO接收到数据后，并等于设置的FIFO等级，则产生中断

			U_count =0;
			while(UART1->LSR & UART_LSR_RDR_Msk)
			{
				U_ReceiveData[U_count] = UART_ReadRBR(UART1);
				U_count++;
			}
			RS485_FLAG = 1;
			RS485_Lenth = U_count;
			memcpy(Buffer_Rx.buf, U_ReceiveData, U_count);
//			for(;U_count>0;U_count--)
//			{
//				UART_WriteTHR(UART0,U_ReceiveData[U_count]);				
//			}	
			break;		
		case 0x3:									//Rx line 状态变化 ->监测LSR[4:1]	

			if(U_LSRFlag & (1<<1))				//FIFO溢出
			{
					
			}	
			if(U_LSRFlag & (1<<2))				//PE
			{
				
			}	
			if(U_LSRFlag & (1<<3))				//FE
			{
				
			}	
			if(U_LSRFlag & (1<<4))				//BI
			{
				
			}	
			UART_EnableFIFO(UART1,FIFO_L0_1BYTE, FIFO_L0_1BYTE, FIFO_RST_DIS, FIFO_RST_EN);					
			break;		
		case 0x6:					//1帧数据的定时时间内未接收到新数据,触发FIFO 接收定时溢出中断	
	
			UART_EnableFIFO(UART1,FIFO_L0_1BYTE, FIFO_L2_8BYTE, FIFO_RST_DIS, FIFO_RST_EN);	/*清除掉接收FIFO*/ 
//			while(UART1->LSR & (0x1))			//或者将FIFO的数据读完。
//			{				
//				UART_ReadRBR(UART0);
//			}			
			break;		
	}
	
}

/****************************************************************************
 ** \brief	TIMER0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TIMER0_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	TIMER1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TIMER1_IRQHandler(void)
{
                       
}
/****************************************************************************
 ** \brief	WDT_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void WDT_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	I2C0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void I2C0_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	SSP0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SSP0_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	SYS_CHK_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SYS_CHK_IRQHandler(void)
{	

}



