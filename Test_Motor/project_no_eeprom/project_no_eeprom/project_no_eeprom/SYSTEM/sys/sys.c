#include "sys.h"
#include <stdio.h>  

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK  STM32开发板
//系统中断分组设置化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/10
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************  
/************************************************
************************************************/
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))  
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))  
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))  
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))  
#define TRCENA          0x01000000  
  
struct __FILE { int handle; /* Add whatever you need here */ };  
    FILE __stdout;  
    FILE __stdin;  
      
int fputc(int ch, FILE *f)   
{  
    if (DEMCR & TRCENA)   
    {  
        while (ITM_Port32(0) == 0);  
        ITM_Port8(0) = ch;  
    }  
    return(ch);  
}  


//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
