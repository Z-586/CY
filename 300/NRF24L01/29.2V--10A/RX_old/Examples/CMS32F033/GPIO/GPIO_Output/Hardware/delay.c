#include "delay.h"

static uint8_t  fac_us = 0;							//us��ʱ������			   
//static uint16_t fac_ms = 0;							//ms��ʱ������

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/
void delay_init()
{
	uint32_t reload;
	
	fac_us  = SystemCoreClock/3/1000000;					//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload  = SystemCoreClock/3/1000000;					//ÿ���ӵļ������� ��λΪM  
	reload *= 1000000/1000;									//����configTICK_RATE_HZ�趨���ʱ��
															//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��0.233s����	
//	fac_ms  = 1000/1000;									//����OS������ʱ�����ٵ�λ	   

	//SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   			//����SYSTICK�ж�
	SysTick->LOAD  = reload; 								//ÿ1/configTICK_RATE_HZ���ж�һ��	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   			//����SYSTICK    
}


void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt = 0;
	uint32_t reload = SysTick->LOAD;		//LOAD��ֵ	    	 
	ticks 	   = nus*fac_us; 				//��Ҫ�Ľ����� 
	told	   = SysTick->VAL;     		    //�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow < told) tcnt += told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt += reload-tnow+told;	    
			told = tnow;
			if(tcnt >= ticks)break;				//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}									    
}


void delay_xms(uint32_t nms)
{
	uint32_t i;
	for(i = 0;i < nms; i++) delay_us(1000);
}

