#include "stm32f10x.h"                  // Device header

void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//GPIOC端口时钟,使能  
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
//X_Dir B10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ; //LED1 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                      //IO模式推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                     //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);                                //GPIO配置初始化
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);           //端口配置为高电平
//Y_Dir B11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; //LED2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                      //IO模式推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);                                //GPIO配置初始化
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);           //端口配置为高电平
	
//Z_Dir B3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);                                
	GPIO_ResetBits(GPIOB,GPIO_Pin_3); 

//X_Step A2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);  

//Y_Step A3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                
	GPIO_ResetBits(GPIOA,GPIO_Pin_3); 

////Z_Step A15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);  

}

//0 正向	
//1 负向
void Motor_Dir(uint16_t IO_Port,uint8_t x_Dir){
	GPIO_WriteBit(GPIOB,IO_Port,(BitAction)x_Dir);
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
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);//接收
	//GPIO_SetBits(GPIOB, GPIO_Pin_5);//发送
}
