#include "delay.h"

static uint8_t  fac_us = 0;							//us延时倍乘数			   
//static uint16_t fac_ms = 0;							//ms延时倍乘数

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/
void delay_init()
{
	uint32_t reload;
	
	fac_us  = SystemCoreClock/3/1000000;					//不论是否使用OS,fac_us都需要使用
	reload  = SystemCoreClock/3/1000000;					//每秒钟的计数次数 单位为M  
	reload *= 1000000/1000;									//根据configTICK_RATE_HZ设定溢出时间
															//reload为24位寄存器,最大值:16777216,在72M下,约合0.233s左右	
//	fac_ms  = 1000/1000;									//代表OS可以延时的最少单位	   

	//SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   			//开启SYSTICK中断
	SysTick->LOAD  = reload; 								//每1/configTICK_RATE_HZ秒中断一次	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   			//开启SYSTICK    
}


void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt = 0;
	uint32_t reload = SysTick->LOAD;		//LOAD的值	    	 
	ticks 	   = nus*fac_us; 				//需要的节拍数 
	told	   = SysTick->VAL;     		    //刚进入时的计数器值
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow < told) tcnt += told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt += reload-tnow+told;	    
			told = tnow;
			if(tcnt >= ticks)break;				//时间超过/等于要延迟的时间,则退出.
		}  
	}									    
}


void delay_xms(uint32_t nms)
{
	uint32_t i;
	for(i = 0;i < nms; i++) delay_us(1000);
}

