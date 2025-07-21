#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Serial.h"
#include "LED.h"
#include "AT24C02.h"

void Get_Change_Puls(int8_t x_pos,int8_t y_pos,int8_t z_pos);
void Motor_start(void);
void Motor_Go_Zero(void);
void Zero_Action(void);

//单位 ： mm
#define X_Max 40
#define X_Min -40

#define Y_Max 40
#define Y_Min -40

#define Z_Max 50
#define Z_Min 0

int8_t X_Y_Z_Position[6];
int8_t Check_X_Y_Z_Position[6];

uint8_t  CRC8_Value;

static uint8_t X_Go_Zero_Flag = 0;
static uint8_t Y_Go_Zero_Flag = 0;
static uint8_t Z_Go_Zero_Flag = 0;

static BitAction X_DIR_Flag = 0;
static BitAction Y_DIR_Flag = 0;
static BitAction Z_DIR_Flag = 0;

static uint8_t Timer_Flag = 0;

static int16_t X_Puls_Count = 0;
static int16_t Y_Puls_Count = 0;
static int16_t Z_Puls_Count = 0;

static int16_t X_Puls_number = 0;
static int16_t Y_Puls_number = 0;
static int16_t Z_Puls_number = 0;

static int16_t X_New_Puls_number = 0;
static int16_t Y_New_Puls_number = 0;
static int16_t Z_New_Puls_number = 0;

static int16_t X_Will_GO_Count = 0;
static int16_t Y_Will_GO_Count = 0;
static int16_t Z_Will_GO_Count = 0;

static uint8_t X_IS_OK = 0;
static uint8_t Y_IS_OK = 0;
static uint8_t Z_IS_OK = 0;

static uint8_t AT24C02_Flag = 0;
static uint8_t AT24C02_Check_Flag = 0;
int main(void)
{
	Timer_Init();
	Serial_Init();
	LED_Init();
	GD_Init();
	Motor_Init();
	AT24C02_Init();
	AT24C02_Flag = at24c02_check();
	AT24C02_Flag = at24c02_check();
	X_DIR_Flag = GPIO_ReadOutputDataBit(GPIOA, X_DIR_IO);
	Y_DIR_Flag = GPIO_ReadOutputDataBit(GPIOA, Y_DIR_IO);
	Z_DIR_Flag = GPIO_ReadOutputDataBit(GPIOA, Z_DIR_IO);
	while(1);
	while (1)
	{
		if(Timer_Flag == 1){
			if((X_Go_Zero_Flag == 2) && (Y_Go_Zero_Flag == 2) && (Z_Go_Zero_Flag == 2)){
				if (Serial_GetRxFlag() == 1)
				{
					CRC8_Value = CRC8(Serial_RxPacket,15);
					if(Serial_RxPacket[0] == 0xAA){
						if(Serial_RxPacket[15] == CRC8_Value){
							// x y z有个函数，计算绝对位置脉冲数
							X_Y_Z_Position[0] = (int8_t)Serial_RxPacket[1];
							X_Y_Z_Position[1] = (int8_t)Serial_RxPacket[2];
							X_Y_Z_Position[2] = (int8_t)Serial_RxPacket[3];
							Get_Change_Puls(X_Y_Z_Position[0],X_Y_Z_Position[1],X_Y_Z_Position[2]);
							//TIM_SetAutoreload(TIM2,1000);//1000 1ms
						}
					}else{
						Serial_SendArray(Serial_RxPacket, 16);			
					}
				}
				if(X_IS_OK == 1 && Y_IS_OK == 1 && Z_IS_OK == 1){
					at24cxx_write_byte(0,X_Y_Z_Position,6);
					Delay_ms(100);
					at24cxx_read_byte(0,Check_X_Y_Z_Position,6);
					for(int i =0;i < 6; i++){
						if(Check_X_Y_Z_Position[i] != X_Y_Z_Position[i]){
							AT24C02_Check_Flag++;
						}
						if(AT24C02_Check_Flag == 6){
							X_IS_OK = 0;
							Y_IS_OK = 0;
							Z_IS_OK = 0;
							AT24C02_Check_Flag = 0;
						}
					}
				}
			}else if((X_Go_Zero_Flag == 3) || (Y_Go_Zero_Flag == 3) || (Z_Go_Zero_Flag == 3)){
				//记录当前位置，报警告
			
			}
			else{
				if(AT24C02_Flag == 0){
					at24cxx_read_byte(0,X_Y_Z_Position,6);
					Delay_ms(50);
					X_Puls_number = X_Y_Z_Position[0]*4*32/1.8;
					Y_Puls_number = X_Y_Z_Position[1]*4*32/1.8;
					Z_Puls_number = X_Y_Z_Position[2]*4*32/1.8;
					
					X_DIR_Flag = X_Y_Z_Position[3];
					Y_DIR_Flag = X_Y_Z_Position[4];
					Z_DIR_Flag = X_Y_Z_Position[5];
					if (GPIO_ReadOutputDataBit(GPIOA, X_DIR_IO) != X_DIR_Flag)
						Motor_Dir(X_DIR_IO,X_DIR_Flag);
					if (GPIO_ReadOutputDataBit(GPIOA, Y_DIR_IO) != Y_DIR_Flag)
						Motor_Dir(Y_DIR_IO,Y_DIR_Flag);
					if (GPIO_ReadOutputDataBit(GPIOA, Z_DIR_IO) != Z_DIR_Flag)
						Motor_Dir(Z_DIR_IO,Z_DIR_Flag);	
					
					X_Go_Zero_Flag = 2;
					Y_Go_Zero_Flag = 2;
					Z_Go_Zero_Flag = 2;
					LED1_Turn();//读取存储数据
				}else{
					Zero_Action();
					Motor_start();
					LED2_Turn();
				}			
			}
			Timer_Flag = 0;
		}
	}
}

void Get_Change_Puls(int8_t x_pos,int8_t y_pos,int8_t z_pos){	
	X_New_Puls_number = x_pos*4*32/1.8;
	Y_New_Puls_number = y_pos*4*32/1.8;
	Z_New_Puls_number = z_pos*4*32/1.8;
}

void Motor_Go_Zero(void)
{
	X_Puls_number = 0;
	Y_Puls_number = 0;
	Z_Puls_number = 0;
	X_New_Puls_number = 0;
	Y_New_Puls_number = 0;
	Z_New_Puls_number = 0;
}

uint8_t GD_X_Open_number = 0;
uint8_t GD_Y_Open_number = 0;
uint8_t GD_Z_Open_number = 0;

void Motor_GD_Check(void){
	//X
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0) {
		GD_X_Open_number++;
		if(GD_X_Open_number > 10){
			if(X_Go_Zero_Flag == 0)
				X_Go_Zero_Flag = 1;
			if(X_Go_Zero_Flag == 2)
				X_Go_Zero_Flag = 3;
			GD_X_Open_number = 10;
		}
	}else{
		GD_X_Open_number = 0;
		if(X_Go_Zero_Flag == 3)
			X_Go_Zero_Flag = 2;
	}
	//Y
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) {
		GD_Y_Open_number++;
		if(GD_Y_Open_number > 10){
			if(Y_Go_Zero_Flag == 0)
				Y_Go_Zero_Flag = 1;
			else if(Y_Go_Zero_Flag == 2)
				Y_Go_Zero_Flag = 3;
			GD_X_Open_number = 10;
		}
	}else{
		GD_Y_Open_number = 0;
		if(Y_Go_Zero_Flag == 3)
			Y_Go_Zero_Flag = 2;

	}
	//Z
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0) {
		GD_Z_Open_number++;
		if(GD_Z_Open_number > 10){
			if(Z_Go_Zero_Flag == 0)
				Z_Go_Zero_Flag = 1;
			else if(Z_Go_Zero_Flag == 2)
				Z_Go_Zero_Flag = 3;
			GD_X_Open_number = 10;
		}
	}else{
		GD_Z_Open_number = 0;
		if(Z_Go_Zero_Flag == 3)
			Z_Go_Zero_Flag = 2;
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Timer_Flag = 1;
		if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void Zero_Action(void){	
	//X
	if(X_Go_Zero_Flag == 0){
		if (GPIO_ReadOutputDataBit(GPIOA, X_DIR_IO) != 0){
			Motor_Dir(X_DIR_IO,0);
			X_DIR_Flag = 0;
		}			
		if (GPIO_ReadOutputDataBit(GPIOA, X_Step_IO) == 0)
			GPIO_SetBits(GPIOA, X_Step_IO);
		else
			GPIO_ResetBits(GPIOA, X_Step_IO);
	}
	//Y
	if(Y_Go_Zero_Flag == 0){
		if (GPIO_ReadOutputDataBit(GPIOA, Y_DIR_IO) != 0){
			Motor_Dir(Y_DIR_IO,0);
			Y_DIR_Flag = 0;
		}
		if (GPIO_ReadOutputDataBit(GPIOA, Y_Step_IO) == 0)
			GPIO_SetBits(GPIOA, Y_Step_IO);
		else
			GPIO_ResetBits(GPIOA, Y_Step_IO);
	}
	//Z
	if(Z_Go_Zero_Flag == 0){
		if (GPIO_ReadOutputDataBit(GPIOA, Z_DIR_IO) != 0){
			Motor_Dir(Z_DIR_IO,0);
			Z_DIR_Flag = 0;
		}
		if (GPIO_ReadOutputDataBit(GPIOA, Z_Step_IO) == 0)
			GPIO_SetBits(GPIOA, Z_Step_IO);
		else
			GPIO_ResetBits(GPIOA, Z_Step_IO);
	}
	
	if(X_Go_Zero_Flag == 1){
		if(X_Puls_number == 0)
			X_Puls_number = X_Max*4*32/1.8;
	}
	if(Y_Go_Zero_Flag == 1){
		if(Y_Puls_number == 0)
			Y_Puls_number = Y_Max*4*32/1.8;
	}
	if(Z_Go_Zero_Flag == 1){
		if(Z_Puls_number == 0)
			Z_Puls_number = Z_Max*4*32/1.8;
	}
}

void Motor_start(void)
{	
	X_Will_GO_Count = X_New_Puls_number - X_Puls_number;
	Y_Will_GO_Count = Y_New_Puls_number - Y_Puls_number;
	Z_Will_GO_Count = Z_New_Puls_number - Z_Puls_number;	
	if(X_Go_Zero_Flag == 1 ||X_Go_Zero_Flag == 2){
		if(X_Will_GO_Count != 0){
			if(X_Will_GO_Count< 0){
				X_Will_GO_Count = -X_Will_GO_Count;
				X_DIR_Flag = !X_DIR_Flag;
				if (GPIO_ReadOutputDataBit(GPIOA, X_DIR_IO) != X_DIR_Flag)
					Motor_Dir(X_DIR_IO,X_DIR_Flag);//反向
			}
			
			if (GPIO_ReadOutputDataBit(GPIOA, X_Step_IO) == 0){
				GPIO_SetBits(GPIOA, X_Step_IO);
				X_Puls_Count ++ ;
				if(X_Puls_Count >= X_Will_GO_Count){
					X_Puls_Count = 0;
					X_Puls_number = X_New_Puls_number;
					X_Y_Z_Position[3] = X_DIR_Flag;
					GPIO_ResetBits(GPIOA, X_Step_IO);
					X_Go_Zero_Flag = 2;
					X_IS_OK = 1;
				}
			}
			else
				GPIO_ResetBits(GPIOA, X_Step_IO);
		}
	}
	//Y
	if(Y_Go_Zero_Flag == 1 || Y_Go_Zero_Flag == 2){
		if(Y_Will_GO_Count != 0){
			if(Y_Will_GO_Count< 0){
				Y_Will_GO_Count = -Y_Will_GO_Count;
				Y_DIR_Flag = !Y_DIR_Flag;
				if (GPIO_ReadOutputDataBit(GPIOA, Y_DIR_IO) != Y_DIR_Flag)
					Motor_Dir(Y_DIR_IO,Y_DIR_Flag);//反向
			}
			if (GPIO_ReadOutputDataBit(GPIOA, Y_Step_IO) == 0){
				GPIO_SetBits(GPIOA, Y_Step_IO);
				Y_Puls_Count ++ ;
				if(Y_Puls_Count >= Y_Puls_number){
					Y_Puls_Count = 0;
					Y_Puls_number = Y_New_Puls_number;
					X_Y_Z_Position[4] = Y_DIR_Flag;
					GPIO_ResetBits(GPIOA, Y_Step_IO);
					Y_Go_Zero_Flag = 2;
					Y_IS_OK = 1;
				}
			}
			else
				GPIO_ResetBits(GPIOA, Y_Step_IO);
		}
	}
	//Z
	if(Z_Go_Zero_Flag == 1 ||Z_Go_Zero_Flag == 2){
		if(Z_Will_GO_Count != 0){
			if(Z_Will_GO_Count < 0){
				Z_Will_GO_Count = -Z_Will_GO_Count;
				Z_DIR_Flag = !Z_DIR_Flag;
				if (GPIO_ReadOutputDataBit(GPIOA, Z_DIR_IO) != Z_DIR_Flag)
					Motor_Dir(Z_DIR_IO,Z_DIR_Flag);//反向			
			}
			if (GPIO_ReadOutputDataBit(GPIOA, Z_Step_IO) == 0){
				GPIO_SetBits(GPIOA, Z_Step_IO);
				Z_Puls_Count ++ ;
				if(Z_Puls_Count >= Z_Will_GO_Count){
					Z_Puls_Count = 0;
					Z_Puls_number = Z_New_Puls_number;
					X_Y_Z_Position[5] = Z_DIR_Flag;
					GPIO_ResetBits(GPIOA, Z_Step_IO);
					Z_Go_Zero_Flag = 2;
					Z_IS_OK = 1;
				}
			}
			else
				GPIO_ResetBits(GPIOA, Z_Step_IO);	
		}
	}
}
