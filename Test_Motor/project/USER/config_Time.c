#include "config_time.h"
#include "config_led.h"
#include "config_usart1.h"
void TIME4_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE); //时钟定时器6使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到arr
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 psc
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4,	TIM_IT_Update,	ENABLE ); //使能TIM1更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4外设	
//	TIM_Cmd(TIM4, DISABLE);  //失能TIM4外设	
}

unsigned char star_time = 0 ;  //计时开始变量
unsigned char recv_time_cnt;  //定时计数器
extern unsigned char recv_cnt;
extern unsigned char recv_flag;
void TIM4_IRQHandler(void)   //TIM1中断函数
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM1中断发生与否:TIM1中断源 
		{	
   
		if(star_time == 1)
		{ 
			recv_time_cnt++; //1、累加定时时间计数器
      if(recv_time_cnt>MAX_REV_TIME)//2、判断时间是否超过了设定的最大的时间阈值，
				                            //   超过则说明等待一段时间后没有新的数据到来
			                              //   则判断一包数据接收完毕
			{
				recv_time_cnt = 0;//3、清除定时计数器，处理数据 清楚nuffer（放在数据处理之后）
				recv_cnt = 0;
				recv_flag = 1; //接收完成标志位置1
			}
		}
  		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIM1的中断待处理位:TIM1中断源 
		}
		
}

