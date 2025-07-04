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
/** \file demo_opa.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "demo_opa.h"
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
 ** \brief	OPA0_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  OPA0_Config(void)
{
	
	/*
	(1)��OAPģ��ʱ��
	*/
	SYS_EnablePeripheralClk(SYS_CLK_OPA_PGA_MSK);
	
	/*
	(2)����OPAģ������ģʽ
	*/		
	OPA_ConfigRunMode(OPA0,OPA_FIL_OPA);			/*�˷�ģʽ*/
	/*
	(3)����OP1��������
	*/
	OPA_ConfigPositive(OPA0, OPA_POSSEL_P);
	
	/*
	(4)����OP1��������
	*/
	OPA_ConfigNegative(OPA0, OPA_NEGSEL_N);	
	/*
	(5)����OP1�����
	*/
	OPA_ConfigOutput(OPA0, OPA_OUTSEL_O);		
	
	/*
	(6)����OP IO��
	*/
	SYS_SET_IOCFG(IOP23CFG,SYS_IOCFG_P23_OP0O);
	SYS_SET_IOCFG(IOP24CFG,SYS_IOCFG_P24_OP0N);	
	SYS_SET_IOCFG(IOP25CFG,SYS_IOCFG_P25_OP0P);	
	
	/*
	(7)����OP1
	*/
	OPA_Start(OPA0);
}


