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
 **			ʵ��ACMP0�ıȽϹ���
 **			��1�����˵�ѹ(IO)>���˵�ѹ(�ڲ���׼1.2V) ���1(�ߵ�ƽ)
 **			��2�����˵�ѹ(IO)<���˵�ѹ(�ڲ���׼1.2V) ���0(�͵�ƽ)
 **			��3�������0���䵽1 �������������ж�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ACMP_ACMP0_Config(void)
{
	//uint32_t AdjResult;

	/*
	(1)����ACMPģ��ʱ��
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ACMP_MSK);	
	/*
	(2)��ȡACMP0У������ֵ
	 */	
	//AdjResult = ACMP_EnableAutoAdjust(ACMP0);		
	
	/*
	(3)����ACMP0 ��������
	 */
	ACMP_ConfigPositive(ACMP0,ACMP_POSSEL_P0);
	/*
	(4)����ACMP0 �������� 
	*/  //55V
	ACMP_ConfigNegative(ACMP0,ACMP_NEGSEL_VREF_BG_K, 8);		/*����ѡ���ڲ�1.2V��׼��ѹ*/
	/*
	(5)���õ�ѹ����	 						
	*/
	//ACMP_EnableCADJAdjust(ACMP0, AdjResult);	
	
	/*
	 (6)�����˲����
	*/
	 ACMP_EnableFilter(ACMP0, ACMP_NFCLK_257_TSYS);
	 /*
	 (7)�رշ������
	 */
	 ACMP_DisableReverseOutput(ACMP0);
	 /*
	 (8)�ر��¼����
	 */
	 ACMP_DisableEventOut(ACMP0);

	 /*
	 (9)�����ж�
	 */
	 ACMP_ConfigEventAndIntMode(ACMP0,ACMP_EVENT_INT_BOTH_EDGE);	/*�����ش����ж����¼����жϴ�����ʽ���¼�������ʽ����*/	
	 ACMP_EnableInt(ACMP0);									
	 NVIC_EnableIRQ(ACMP_IRQn);
	/*
	(10)�����ж����ȼ�
	*/	 	 
	 NVIC_SetPriority(ACMP_IRQn,2);

	/*
	(11)���ó���
	*/	 	 
	ACMP_ConfigHYS(ACMP0,ACMP_HYS_0_TO_1,ACMP_HYS_S_0); 	
	
	/*
	(12)����ACMP IO��
	*/
	SYS_SET_IOCFG(IOP13CFG,SYS_IOCFG_P13_C0P0);			/*����P13ΪACMP0 ��������*/
	//SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_C0P2);			/*����P13ΪACMP0 ��������*/
	//SYS_SET_IOCFG(IOP13CFG,SYS_IOCFG_P43_C0N);			/*����P43ΪACMP0 ��������*/
	//SYS_SET_IOCFG(IOP26CFG,SYS_IOCFG_P26_C0O);			/*����P26ΪACMP0 ���*/
	/*
	(13)����ACMP0
	*/
  	ACMP_Start(ACMP0);	
	
	/*
	(14)P12����Ϊ�������ģʽ��ָʾACMP0���ж�						
	*/
//	SYS_SET_IOCFG(IOP12CFG,SYS_IOCFG_P12_GPIO);					/*����P12ΪGPIOģʽ*/	
//	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_2,GPIO_MODE_OUTPUT);		/*����P12Ϊ�������ģʽ*/
//	GPIO1->DO_f.P2 = 0;											/*P12����͵�ƽ*/		
	
}




