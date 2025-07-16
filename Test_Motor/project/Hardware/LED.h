#ifndef __LED_H
#define __LED_H

#define X_DIR_IO GPIO_Pin_11
#define Y_DIR_IO GPIO_Pin_12
#define Z_DIR_IO GPIO_Pin_4
#define X_Step_IO GPIO_Pin_8
#define Y_Step_IO GPIO_Pin_0
#define Z_Step_IO GPIO_Pin_1


void Motor_Init(void);
void GD_Init(void);
void Motor_Dir(uint16_t IO_Port,BitAction x_Dir);



void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Turn(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED2_Turn(void);

#endif
