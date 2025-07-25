#ifndef __config_IO_H
#define __config_IO_H	 
#include "sys.h"



/////////////////////////输出控制////////////////////////////////
#define LED1_ON          GPIO_ResetBits(GPIOB,GPIO_Pin_10)      
#define LED1_OFF         GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define LED1 PBout(10)

#define LED2_ON          GPIO_ResetBits(GPIOB,GPIO_Pin_11)      
#define LED2_OFF         GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define LED2 PBout(11)

#define X_DIR_IO GPIO_Pin_10	//B10
#define Y_DIR_IO GPIO_Pin_11	//B11
#define Z_DIR_IO GPIO_Pin_7		//B3
#define X_Step_IO GPIO_Pin_2	//A2
#define Y_Step_IO GPIO_Pin_3	//A3
#define Z_Step_IO GPIO_Pin_6	//A15

void GPIO_IO_Config(void);//初始化
void Motor_Dir(uint16_t IO_Port,BitAction x_Dir);
void GD_Init(void);

		 				    
#endif
