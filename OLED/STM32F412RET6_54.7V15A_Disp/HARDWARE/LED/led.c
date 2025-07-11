#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK NANO STM32F4开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2019/3/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2019-2029
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
   
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE();           	//开启GPIOC时钟
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
    GPIO_Initure.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;	//PC6 PC7 PC8
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	//高速
	
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
}


