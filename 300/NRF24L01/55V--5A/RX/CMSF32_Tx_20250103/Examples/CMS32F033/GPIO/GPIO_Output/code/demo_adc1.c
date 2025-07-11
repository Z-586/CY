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
/** \file demo_adc1.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "demo_adc1.h"
extern uint16_t I_Set(float I_set_number);

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
 ** \brief	ADC1_Software_Trigger_Mode
 **			�������ADC1ת��
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC1_Software_Trigger_Mode(void)
{
	/*
	(1)����ADC1ʱ��
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	ADC1_ConfigRunMode(ADC1_CONVERT_CONTINUOUS,ADC1_CLK_DIV_8,ADC1_HOLD_10P5_CLK);			/*ʹ�ܵ�ͨ��ת��*/
	ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(Target_I)); //2688 10A              // 0x00
	//ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, 4000);               // 0x00
	/*
	(2)����ADC1ͨ��ʹ��
	*/	
	ADC1_EnableScanChannel(ADC1_CH_30_MSK|ADC1_CH_10_MSK|ADC1_CH_9_MSK);		/*������ͨ��ѡ��AN30 */	
	SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_AN10);					/*�ر�P36�����ֹ���*/
	SYS_SET_IOCFG(IOP14CFG,SYS_IOCFG_P14_AN9);	
	ADC1_SetAN30Channel(ADC1_CH_30_PGA1O);
	
	ADC1_EnableCompare();
//	/*
//	(4)����ADC1�ж�
//	*/		
//	
//	ADC1_EnableChannelInt(ADC1_CH_13_MSK);						/*��AN13ת���ж�*/	
//	NVIC_EnableIRQ(ADC1_IRQn);
//	/*
//	(5)�������ȼ�
//	*/	
//	NVIC_SetPriority(ADC1_IRQn,2);					/*���ȼ�0~3�� 0��ߡ�3���*/	
	/*
	(6)����ADC1
	*/		
	ADC1_Start();
	/*
	(7)����ADC1У׼
	*/		
	ADC1_StartAdjust();
	/*
	(8)ʹ��ADC1У׼ֵ��ADCת��
	*/	
	ADC1_EnableAdjust();
	
	/*
	(9)����GPIO���
	*/
//	SYS_SET_IOCFG(IOP15CFG, SYS_IOCFG_P15_GPIO);	
//	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_5,GPIO_MODE_OUTPUT);			/*ָʾADC1�ж�*/
//	GPIO1->DO_f.P5 = 0;		
}

void ADC1_Software_P44_Mode(void)
{
	/*
	(1)����ADC1ʱ��
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	ADC1_ConfigRunMode(ADC1_CONVERT_CONTINUOUS,ADC1_CLK_DIV_8,ADC1_HOLD_10P5_CLK);			/*ʹ�ܵ�ͨ��ת��*/
	ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(Target_I)); //2688 10A              // 0x00
	//ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, 4000);               // 0x00
	/*
	(2)����ADC1ͨ��ʹ��
	*/	
	ADC1_EnableScanChannel(ADC1_CH_30_MSK|ADC1_CH_10_MSK|ADC1_CH_27_MSK|ADC1_CH_9_MSK);		/*������ͨ��ѡ��AN30 */	
	SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_AN10);					/*�ر�P36�����ֹ���*/
	SYS_SET_IOCFG(IOP14CFG,SYS_IOCFG_P14_AN9);	
	SYS_SET_IOCFG(IOP44CFG,SYS_IOCFG_P44_AN27);	
	ADC1_SetAN30Channel(ADC1_CH_30_PGA1O);
	
	ADC1_EnableCompare();
//	/*
//	(4)����ADC1�ж�
//	*/		
//	
//	ADC1_EnableChannelInt(ADC1_CH_13_MSK);						/*��AN13ת���ж�*/	
//	NVIC_EnableIRQ(ADC1_IRQn);
//	/*
//	(5)�������ȼ�
//	*/	
//	NVIC_SetPriority(ADC1_IRQn,2);					/*���ȼ�0~3�� 0��ߡ�3���*/	
	/*
	(6)����ADC1
	*/		
	ADC1_Start();
	/*
	(7)����ADC1У׼
	*/		
	ADC1_StartAdjust();
	/*
	(8)ʹ��ADC1У׼ֵ��ADCת��
	*/	
	ADC1_EnableAdjust();
	
	/*
	(9)����GPIO���
	*/
//	SYS_SET_IOCFG(IOP15CFG, SYS_IOCFG_P15_GPIO);	
//	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_5,GPIO_MODE_OUTPUT);			/*ָʾADC1�ж�*/
//	GPIO1->DO_f.P5 = 0;		
}

