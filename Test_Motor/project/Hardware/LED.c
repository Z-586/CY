#include "stm32f10x.h"                  // Device header

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_0 | GPIO_Pin_12 | GPIO_Pin_1| GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_0 | GPIO_Pin_12 | GPIO_Pin_1| GPIO_Pin_4);
}

//0 正向	
//1 负向
void Motor_Dir(uint16_t IO_Port,BitAction x_Dir){
	GPIO_WriteBit(GPIOA,IO_Port,x_Dir);
}

void GD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	}
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	}
}
