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
/** \file demo_acmp.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "demo_acmp.h"
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
 ** \brief	ACMP_ACMP0_Config
 **			实现ACMP0的比较功能
 **			（1）正端电压(IO)>负端电压(内部基准1.2V) 输出1(高电平)
 **			（2）正端电压(IO)<负端电压(内部基准1.2V) 输出0(低电平)
 **			（3）输出从0跳变到1 ，产生上升沿中断
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ACMP_ACMP0_Config(void)
{
	//uint32_t AdjResult;

	/*
	(1)开启ACMP模块时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ACMP_MSK);	
	/*
	(2)获取ACMP0校调调节值
	 */	
	//AdjResult = ACMP_EnableAutoAdjust(ACMP0);		
	
	/*
	(3)设置ACMP0 正端输入
	 */
	ACMP_ConfigPositive(ACMP0,ACMP_POSSEL_P0);
	/*
	(4)设置ACMP0 负端输入 
	*/  //55V
	ACMP_ConfigNegative(ACMP0,ACMP_NEGSEL_VREF_BG_K, 8);		/*负端选择内部1.2V基准电压*/
	/*
	(5)设置电压调节	 						
	*/
	//ACMP_EnableCADJAdjust(ACMP0, AdjResult);	
	
	/*
	 (6)开启滤波输出
	*/
	 ACMP_EnableFilter(ACMP0, ACMP_NFCLK_257_TSYS);
	 /*
	 (7)关闭反相输出
	 */
	 ACMP_DisableReverseOutput(ACMP0);
	 /*
	 (8)关闭事件输出
	 */
	 ACMP_DisableEventOut(ACMP0);

	 /*
	 (9)设置中断
	 */
	 ACMP_ConfigEventAndIntMode(ACMP0,ACMP_EVENT_INT_BOTH_EDGE);	/*边沿沿触发中断与事件，中断触发方式与事件触发方式共用*/	
	 ACMP_EnableInt(ACMP0);									
	 NVIC_EnableIRQ(ACMP_IRQn);
	/*
	(10)设置中断优先级
	*/	 	 
	 NVIC_SetPriority(ACMP_IRQn,2);

	/*
	(11)设置迟滞
	*/	 	 
	ACMP_ConfigHYS(ACMP0,ACMP_HYS_0_TO_1,ACMP_HYS_S_0); 	
	
	/*
	(12)配置ACMP IO口
	*/
	SYS_SET_IOCFG(IOP13CFG,SYS_IOCFG_P13_C0P0);			/*设置P13为ACMP0 正端输入*/
	//SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_C0P2);			/*设置P13为ACMP0 正端输入*/
	//SYS_SET_IOCFG(IOP13CFG,SYS_IOCFG_P43_C0N);			/*设置P43为ACMP0 负端输入*/
	//SYS_SET_IOCFG(IOP26CFG,SYS_IOCFG_P26_C0O);			/*设置P26为ACMP0 输出*/
	/*
	(13)开启ACMP0
	*/
  	ACMP_Start(ACMP0);	
	
	/*
	(14)P12设置为推挽输出模式，指示ACMP0的中断						
	*/
//	SYS_SET_IOCFG(IOP12CFG,SYS_IOCFG_P12_GPIO);					/*设置P12为GPIO模式*/	
//	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_2,GPIO_MODE_OUTPUT);		/*设置P12为推挽输出模式*/
//	GPIO1->DO_f.P2 = 0;											/*P12输出低电平*/		
	
}




