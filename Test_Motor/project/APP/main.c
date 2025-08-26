#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Serial.h"
#include "LED.h"
#include "main.h"
#include "AT24C02.h"

void Get_Change_Puls(int8_t x_pos,int8_t y_pos,int8_t z_pos);
void Motor_start(void);
void Motor_Go_Zero(void);
void Zero_Action(void);
void Motor_GD_Check(void);
void Timer_Cal(void);

int8_t X_Y_Z_Position[6];
int8_t Check_X_Y_Z_Position[6];

uint8_t  CRC8_Value;

static uint8_t Motor_Run_Flag = 0;

static uint8_t X_Go_Zero_Flag = 0;
static uint8_t Y_Go_Zero_Flag = 0;
static uint8_t Z_Go_Zero_Flag = 0;

static uint8_t X_DIR_Flag = 0;
static uint8_t Y_DIR_Flag = 0;
static uint8_t Z_DIR_Flag = 0;

static uint8_t  Timer_Flag = 0;
static uint32_t Timer_Count = 0;
static uint32_t _1s_Count = 0;
static uint32_t _100ms_Count = 0;

static int32_t X_Puls_Count = 0;
static int32_t Y_Puls_Count = 0;
static int32_t Z_Puls_Count = 0;

static int32_t X_Puls_number = 0;
static int32_t Y_Puls_number = 0;
static int32_t Z_Puls_number = 0;

static int32_t X_New_Puls_number = 0;
static int32_t Y_New_Puls_number = 0;
static int32_t Z_New_Puls_number = 0;

static int32_t X_Will_GO_Count = 0;
static int32_t Y_Will_GO_Count = 0;
static int32_t Z_Will_GO_Count = 0;

static uint8_t X_IS_OK = 0;
static uint8_t Y_IS_OK = 0;
static uint8_t Z_IS_OK = 0;

static uint8_t AT24C02_Flag = 0;
static uint8_t AT24C02_suc_Flag = 0;
static uint8_t AT24C02_Check_Flag = 0;

int main(void)
{
	Timer_Init();
	LED_Init();
	Serial_Init();
	GD_Init();
	Motor_Init();
	AT24C02_Init();
	AT24C02_Flag = at24c02_check();
	Timer_Cal();
	while (1)
	{
		if(Timer_Flag == 1){
			if((X_Go_Zero_Flag == 2) && (Y_Go_Zero_Flag == 2) && (Z_Go_Zero_Flag == 2)){
				if (Serial_GetRxFlag() == 1)
				{
					CRC8_Value = CRC8(Serial_RxPacket,18);
					if(Serial_RxPacket[0] == 0xAA){
//						if(Serial_RxPacket[18] == CRC8_Value){
							if(Motor_Run_Flag == 1){
								// x y z有个函数，计算绝对位置脉冲数
								X_Y_Z_Position[0] = Serial_RxPacket[1];
								X_Y_Z_Position[1] = Serial_RxPacket[2];
								X_Y_Z_Position[2] = Serial_RxPacket[3];
								Get_Change_Puls(X_Y_Z_Position[0],X_Y_Z_Position[1],X_Y_Z_Position[2]);
								Motor_Run_Flag = 0;
	//							AT24C02_WriteReg(10,Motor_Run_Flag);
								Delay_ms(10);
							}							
//						}
					}else{
						GPIO_SetBits(GPIOB, GPIO_Pin_5);Delay_ms(1);
						Serial_SendArray(Serial_RxPacket, 19);
						GPIO_SetBits(GPIOB, GPIO_Pin_5);
					}
					Serial_RxFlag = 0;
				}
				if(X_IS_OK == 1 && Y_IS_OK == 1 && Z_IS_OK == 1){
					at24cxx_write_byte(0,X_Y_Z_Position,6);
					Delay_ms(100);
					at24cxx_read_byte(0,Check_X_Y_Z_Position,6);
					for(int i =0;i < 6; i++){
						if(Check_X_Y_Z_Position[i] == X_Y_Z_Position[i]){
							AT24C02_Check_Flag++;
						}
					}
					if(AT24C02_Check_Flag == 6){
						X_IS_OK = 0;
						Y_IS_OK = 0;
						Z_IS_OK = 0;
						AT24C02_Check_Flag = 0;
						Motor_Run_Flag = 1;
					}
					Motor_Run_Flag = 1;
				}				
			}else if((X_Go_Zero_Flag == 3) || (Y_Go_Zero_Flag == 3) || (Z_Go_Zero_Flag == 3)){
				//记录当前位置，报警告
			
			}
			else {
				if(AT24C02_Flag == 0){
					if(AT24C02_suc_Flag == 0){
						at24cxx_read_byte(0,X_Y_Z_Position,6);
						Delay_ms(50);
						for(int i = 0; i < 6; i++){
							if((uint8_t)X_Y_Z_Position[i] == 0xFF ){
								AT24C02_Check_Flag++;
							}
						}
						if(AT24C02_Check_Flag > 3){
							for(int i = 0; i < 6; i++)
								X_Y_Z_Position[i] = 0;
							Delay_ms(10);
							at24cxx_write_byte(0,X_Y_Z_Position,6);
							Delay_ms(10);
						}
						X_Puls_number = X_Y_Z_Position[0]*One_Run_Pulse/One_Run_Lenth;
						Y_Puls_number = X_Y_Z_Position[1]*One_Run_Pulse/One_Run_Lenth;
						Z_Puls_number = X_Y_Z_Position[2]*One_Run_Pulse/One_Run_Lenth;
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
						AT24C02_suc_Flag = 1;
						
						Motor_Run_Flag = 1;
					}
				}
			}
			if(Timer_Count % (_100ms_Count+1) == _100ms_Count){
				Motor_GD_Check();
			}
			if(Timer_Count % (_1s_Count+1) == _1s_Count){
				Timer_Count = 0;
				GPIO_SetBits(GPIOB, GPIO_Pin_5);
				Delay_ms(1);
				Serial_TxPacket[0] = 0xFF;
				Serial_TxPacket[1] = 0x03;
				Serial_TxPacket[2] = 0x00;
				Serial_TxPacket[3] = 0x00;
				Serial_TxPacket[4] = 0x00;
				Serial_TxPacket[5] = 0x07;
				Serial_TxPacket[6] = 0x11;
				Serial_TxPacket[7] = 0xD6;
				Serial_SendArray(Serial_TxPacket, 8);
				GPIO_ResetBits(GPIOB, GPIO_Pin_5);
				if(X_Go_Zero_Flag == 0)
					X_Go_Zero_Flag = 1;
				if(Y_Go_Zero_Flag == 0)
					Y_Go_Zero_Flag = 1;
				if(Z_Go_Zero_Flag == 0)
					Z_Go_Zero_Flag = 1;
			}
			if(Motor_Run_Flag == 0){
				Zero_Action();
				Motor_start();
			}
			Timer_Flag = 0;
		}
	}
}

void Timer_Cal(void){
	uint16_t PSC = TIM2->PSC;
	uint16_t ARR = TIM2->ARR;
	_1s_Count = 72000000/(PSC+1)/(ARR+1);
	_100ms_Count = _1s_Count / 10;
}

void Get_Change_Puls(int8_t x_pos,int8_t y_pos,int8_t z_pos){	
	X_New_Puls_number = x_pos*One_Run_Pulse/One_Run_Lenth;
	Y_New_Puls_number = y_pos*One_Run_Pulse/One_Run_Lenth;
	Z_New_Puls_number = z_pos*One_Run_Pulse/One_Run_Lenth;
	
	X_Will_GO_Count = X_New_Puls_number - X_Puls_number;
	Y_Will_GO_Count = Y_New_Puls_number - Y_Puls_number;
	Z_Will_GO_Count = Z_New_Puls_number - Z_Puls_number;
	if(X_Will_GO_Count< 0){
		X_Will_GO_Count = abs(X_Will_GO_Count);
		X_DIR_Flag = 1;
		Motor_Dir(X_DIR_IO,X_DIR_Flag);//反向
	}else{
		X_DIR_Flag = 0;
		Motor_Dir(X_DIR_IO,X_DIR_Flag);//正向
	}
	if(Y_Will_GO_Count< 0){
		Y_Will_GO_Count = abs(Y_Will_GO_Count);
		Y_DIR_Flag = 1;
		Motor_Dir(Y_DIR_IO,Y_DIR_Flag);//反向
	}else{
		Y_DIR_Flag = 0;
		Motor_Dir(Y_DIR_IO,Y_DIR_Flag);//正向
	}
	if(Z_Will_GO_Count< 0){
		Z_Will_GO_Count = abs(Z_Will_GO_Count);
		Z_DIR_Flag = 1;
		Motor_Dir(Z_DIR_IO,Z_DIR_Flag);//反向
	}else{
		Z_DIR_Flag = 0;
		Motor_Dir(Z_DIR_IO,Z_DIR_Flag);//正向
	}
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
//	printf("X_Flag : %d   Y_Flag : %d   Z_Flag : %d\r\n",X_Go_Zero_Flag,Y_Go_Zero_Flag,Z_Go_Zero_Flag);
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
		Timer_Count++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void Zero_Action(void){	
	//X
	if(X_Go_Zero_Flag == 0){
		if (GPIO_ReadOutputDataBit(GPIOB, X_DIR_IO) != 0){
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
		if (GPIO_ReadOutputDataBit(GPIOB, Y_DIR_IO) != 0){
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
		if (GPIO_ReadOutputDataBit(GPIOB, Z_DIR_IO) != 0){
			Motor_Dir(Z_DIR_IO,0);
			Z_DIR_Flag = 0;
		}
		if (GPIO_ReadOutputDataBit(GPIOA, Z_Step_IO) == 0)
			GPIO_SetBits(GPIOA, Z_Step_IO);
		else
			GPIO_ResetBits(GPIOA, Z_Step_IO);
	}
	
	if(X_Go_Zero_Flag == 1){
		if(X_Puls_number == 0){
			X_Puls_number = X_Max*One_Run_Pulse/One_Run_Lenth;
			X_Will_GO_Count = X_New_Puls_number - X_Puls_number;
			if(X_Will_GO_Count< 0){
				X_Will_GO_Count = abs(X_Will_GO_Count);
				X_DIR_Flag = 1;
				Motor_Dir(X_DIR_IO,X_DIR_Flag);//反向
			}else{
				X_DIR_Flag = 0;
				Motor_Dir(X_DIR_IO,X_DIR_Flag);//正向
			}
		}
	}
	if(Y_Go_Zero_Flag == 1){
		if(Y_Puls_number == 0){
			Y_Puls_number = Y_Max*One_Run_Pulse/One_Run_Lenth;
			Y_Will_GO_Count = Y_New_Puls_number - Y_Puls_number;
			if(Y_Will_GO_Count< 0){
				Y_Will_GO_Count = abs(Y_Will_GO_Count);
				Y_DIR_Flag = 1;
				Motor_Dir(Y_DIR_IO,Y_DIR_Flag);//反向
			}else{
				Y_DIR_Flag = 0;
				Motor_Dir(Y_DIR_IO,Y_DIR_Flag);//正向
			}
		}
	}
	if(Z_Go_Zero_Flag == 1){
		if(Z_Puls_number == 0){
			Z_Puls_number = Z_Max*One_Run_Pulse/One_Run_Lenth;
			Z_Will_GO_Count = Z_New_Puls_number - Z_Puls_number;
			if(Z_Will_GO_Count< 0){
				Z_Will_GO_Count = abs(Z_Will_GO_Count);
				Z_DIR_Flag = 1;
				Motor_Dir(Z_DIR_IO,Z_DIR_Flag);//反向
			}else{
				Z_DIR_Flag = 0;
				Motor_Dir(Z_DIR_IO,Z_DIR_Flag);//正向
			}
		}
	}
}

void Motor_start(void)
{	
	if(X_Go_Zero_Flag == 1 ||X_Go_Zero_Flag == 2){
		if(X_Will_GO_Count != 0){
			if (GPIO_ReadOutputDataBit(GPIOA, X_Step_IO) == 0){
				GPIO_SetBits(GPIOA, X_Step_IO);
				X_Puls_Count ++ ;
				if(X_Puls_Count >= X_Will_GO_Count){
					X_Puls_Count = 0;
					X_Puls_number = X_New_Puls_number;
					X_Y_Z_Position[3] = X_DIR_Flag;
					X_Go_Zero_Flag = 2;
					X_IS_OK = 1;
					X_Will_GO_Count = 0;
				}
			}
			else
				GPIO_ResetBits(GPIOA, X_Step_IO);
		}
	}
	//Y
	if(Y_Go_Zero_Flag == 1 || Y_Go_Zero_Flag == 2){
		if(Y_Will_GO_Count != 0){
			if (GPIO_ReadOutputDataBit(GPIOA, Y_Step_IO) == 0){
				GPIO_SetBits(GPIOA, Y_Step_IO);
				Y_Puls_Count ++ ;
				if(Y_Puls_Count >= Y_Puls_number){
					Y_Puls_Count = 0;
					Y_Puls_number = Y_New_Puls_number;
					X_Y_Z_Position[4] = Y_DIR_Flag;
					Y_Go_Zero_Flag = 2;
					Y_IS_OK = 1;
					Y_Will_GO_Count = 0;
				}
			}
			else
				GPIO_ResetBits(GPIOA, Y_Step_IO);
		}
	}
	//Z
	if(Z_Go_Zero_Flag == 1 ||Z_Go_Zero_Flag == 2){
		if(Z_Will_GO_Count != 0){
			if (GPIO_ReadOutputDataBit(GPIOA, Z_Step_IO) == 0){
				GPIO_SetBits(GPIOA, Z_Step_IO);
				Z_Puls_Count ++ ;
				if(Z_Puls_Count >= Z_Will_GO_Count){
					Z_Puls_Count = 0;
					Z_Puls_number = Z_New_Puls_number;
					X_Y_Z_Position[5] = Z_DIR_Flag;
					Z_Go_Zero_Flag = 2;
					Z_IS_OK = 1;
					Z_Will_GO_Count = 0;
				}
			}
			else
				GPIO_ResetBits(GPIOA, Z_Step_IO);	
		}
	}
}
