#ifndef __LED_H
#define __LED_H

#define X_DIR_IO GPIO_Pin_10	//B10
#define Y_DIR_IO GPIO_Pin_11	//B11
#define Z_DIR_IO GPIO_Pin_3		//B3
#define X_Step_IO GPIO_Pin_2	//A2
#define Y_Step_IO GPIO_Pin_3	//A3
#define Z_Step_IO GPIO_Pin_15	//A15

void Motor_Init(void);
void GD_Init(void);
void Motor_Dir(uint16_t IO_Port,uint8_t x_Dir);

void LED_Init(void);

#endif
