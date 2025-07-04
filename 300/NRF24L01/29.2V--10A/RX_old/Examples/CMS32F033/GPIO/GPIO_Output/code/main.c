/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/****************************************************************************/
/** \file main.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "cms32f033.h"
#include "stdio.h"
#include <string.h>
#include "drv_spi.h"
#include "drv_RF24L01.h"
#include "demo_gpio.h"
#include "demo_uart.h"
#include "demo_opa.h"
#include "demo_pga.h"
#include "demo_adc1.h"
#include "demo_acmp.h"
#include "CRC1.h"
#include "delay.h"
#include "Temp.h"

uint16_t I_Set(float I_set_number){
	return 4096.0*16.816*4.0/200.0/5.0*I_set_number;
}

uint16_t V_Set(float V_set_number){
	return 4096.0*6.8/206.8/5.0*V_set_number;
}

#define I_MIN I_Set(9.5 - Temp_State) 
#define I_Min I_Set(7.0- Temp_State) 
#define I_MAX I_Set(10.2 - Temp_State) 
#define I_Max I_Set(12)  
#define I_Fall I_Set(0.5)  

#define V_MAX V_Set(29.1)
#define Temp_MAX 85  
#define V_Max V_Set(31.2)  
#define Check_V_MIN V_Set(16)  
#define Check_V_MAX V_Set(30.5)  




void check_test(void);
void Check_State(void);
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/
/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/
uint8_t tx_buf[10] = {0};
uint8_t tx_status = 0;
uint8_t rx_buf[10] = {0};
uint8_t rx_status = 0;

uint8_t I_State = 0;
uint8_t State = 0;

volatile uint32_t AdcCompareValue = 0;
volatile uint32_t ADC30Value = 0;
volatile uint32_t ADC24Value = 0;

uint8_t system_start = 0;
uint8_t ACMP_Status = 0;
uint8_t I_envage = 20;
uint8_t I_envage_time = 0;

uint8_t V_envage = 20;
uint8_t V_envage_time = 0;

uint8_t V_I_envage_time = 0;
uint8_t V_envage_state = 0;
/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/

/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/
static uint32_t ADC_Value = 0; 
static uint32_t ADC_V_Value = 0; 
float Temp_Value = 0; 
static uint16_t Time = 0;
static uint8_t  Temp_State = 0;
static uint8_t  TX_RX_State = 0;
extern unsigned char INIT_ADDR[5]; //节点地址
uint16_t CRC_Value = 0;


uint16_t Time_Out = 0;
uint16_t Time_Count = 0;

uint16_t Delay_Time_Out = 0;
uint16_t Delay_Time_Count = 0;
uint16_t ADC_Real_V_Value = 0; 
uint16_t ADC_Real_I_Value = 0; 


uint32_t Verge(uint16_t Adc){
	uint32_t ADC_Verge_Value = 0; 
	if(Adc == ADC1_CH_10){
		for(int i = 0 ; i < 20 ; i++){
			if(!ADC1_IS_BUSY())
			{	
				ADC1_Go();
				while(ADC1_IS_BUSY());
			}
			ADC_Real_V_Value = ADC1_GetResult(ADC1_CH_10);
			ADC_Verge_Value += ADC_Real_V_Value;
			if(ADC_Real_V_Value > V_Max){
				Delay_Time_Out = 0;
				Delay_Time_Count = 0;
//				GPIO2->DO_f.P1 = 0;	
				while(1){
					tx_buf[0] = 1;
					tx_buf[1] = 4;
					tx_buf[8] = usCRC16(tx_buf,8)>>8;
					tx_buf[9] = usCRC16(tx_buf,8);
					tx_status = NRF24L01_TxPacket(tx_buf);
					if(tx_status == 0X20)  
					{
						Delay_Time_Out++;
					}
					Delay_Time_Count++;	
					if(Delay_Time_Out%10 == 0)
						GPIO2->DO_f.P6 = 1;	
					if(Delay_Time_Count%15 == 0)
						GPIO2->DO_f.P6 = 1;	
					delay_xms(1);
				}
			
			}
			delay_us(20);		
		}
	}else if(Adc == ADC1_CH_30){
		for(int i = 0 ; i < 20 ; i++){
			if(!ADC1_IS_BUSY())
			{	
				ADC1_Go();
				while(ADC1_IS_BUSY());
			}
			ADC_Real_I_Value = ADC1_GetResult(ADC1_CH_30);
			ADC_Verge_Value += ADC_Real_I_Value;
			if(ADC_Real_I_Value > I_Max){
				Delay_Time_Out = 0;
				Delay_Time_Count = 0;
//				GPIO2->DO_f.P1 = 0;	
				while(1){
					tx_buf[0] = 1;
					tx_buf[1] = 3;
					tx_buf[8] = usCRC16(tx_buf,8)>>8;
					tx_buf[9] = usCRC16(tx_buf,8);
					tx_status = NRF24L01_TxPacket(tx_buf);
					if(tx_status == 0X20)  
					{
						Delay_Time_Out++;
					}
					Delay_Time_Count++;	
					if(Delay_Time_Out%10 == 0)
						GPIO2->DO_f.P6 = 1;	
					if(Delay_Time_Count%15 == 0)
						GPIO2->DO_f.P6 = 1;	
					delay_xms(1);					
				}
			}
			delay_us(20);		
		}	
	}
	return 	ADC_Verge_Value/20;
}




/*****************************************************************************
 ** \brief	 main
 **
 ** \param [in]  none   
 **
 ** \return 0
*****************************************************************************/
int main(void)
{		

	SYS_DisableIOCFGProtect();			/*关闭IOCONFIG写保护*/
	SYS_DisableGPIO0Protect();			/*关闭GPIO0的相关寄存器写保护*/
	SYS_DisableGPIO1Protect();			/*关闭GPIO1的相关寄存器写保护*/
	SYS_DisableGPIO2Protect();			/*关闭GPIO2的相关寄存器写保护*/
	SYS_DisableGPIO3Protect();			/*关闭GPIO3的相关寄存器写保护*/
	SYS_DisableGPIO4Protect();	 		/*关闭GPIO4的相关寄存器写保护*/
	
	SYS_ConfigHSI(SYS_CLK_HSI_48M);		/*设置内部高速时钟为48Mhz*/
	SYS_EnableHSI();					/*开启高速时钟*/
	SYS_ConfigAHBClock(SYS_CLK_SEL_HSI,SYS_CLK_DIV_1);	/*设置AHB时钟为高速时钟的1分频*/
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);					/*设置APB时钟为AHB时钟的1分频*/
	
	delay_init();
	
	GPIO_Config();
	
	drv_spi_init( );
	UART_UART1_Config();
	delay_xms(200);
	
	SYS_SET_IOCFG(IOP21CFG,SYS_IOCFG_P21_GPIO);				//初始化 P21
	GPIO_CONFIG_IO_MODE(GPIO2, GPIO_PIN_1, GPIO_MODE_OUTPUT);
	GPIO2->DO_f.P1 = 0;										//低断，高通
	//保护引脚  过温、过压、过流
	SYS_SET_IOCFG(IOP26CFG,SYS_IOCFG_P26_GPIO);				//初始化 P26
	GPIO_CONFIG_IO_MODE(GPIO2, GPIO_PIN_6, GPIO_MODE_OUTPUT);
	GPIO2->DO_f.P6 = 0;										//低关，高开
	
	OPA0_Config();
	PGA_PGA1_Config();
	ADC1_Software_Trigger_Mode();
	ACMP_ACMP0_Config();				/*ACMP0功能*/
	
	//RF24L01引脚初始化
	NRF24L01_Gpio_Init( );		
	NRF24L01_Init( );
	NRF24L01_Check( );
	//检测nRF24L01
	if(NRF24L01_Check( ) == 0){
		//printf("NRF24L01  is  ok!\n\r");
	}
	else{
		//printf("NRF24L01  is  error!\n\r");
	}
	while(1)
	{
		check_test();
		if(GPIO2->DO_f.P1 == 1){
			break;
		}
	}
	RX_Mode(0);
	while(1)
	{
		if(TX_RX_State == 0){
			rx_status = NRF24L01_RxPacket(rx_buf);
			if(rx_status == 0x00){
				CRC_Value = usCRC16(rx_buf,8);
				if(CRC_Value == ((((uint16_t)rx_buf[8])<<8)|rx_buf[9])){
					memcpy(INIT_ADDR,rx_buf,5);
					Time_Count++;
				}
			}
			Time_Out++;
			if(Time_Out % 100 == 0){
				if(Time_Count != 0){
 					TX_Mode(0);
					TX_RX_State = 1;
				}
				Time_Out = 0;
			}			
		}else{
			ACMP_Status = ACMP_GetResult(ACMP0);
			ADC_V_Value = Verge(ADC1_CH_10);
			ADC_Value   = Verge(ADC1_CH_30);
			tx_buf[0] = 0xAA;	
			if (ACMP_Status == 0) {
				if(V_envage_state == 0){
					tx_buf[1] = 0;
					if(ADC_Value < I_MIN ){
						if(ADC_Value < I_Min ){
							State = 0;
						}
						if(State == 0)tx_buf[0] = 0;	
					}else if(ADC_Value >I_MAX){ 
						I_State ++;
						if(I_State % 4 == 0){
							State = 1;I_State = 0;
						}
						tx_buf[0] = 1;
					}
				}else{	
					if(ADC_V_Value < V_MAX){
						if(ADC_Value < I_Set(2)){
							V_envage_state = 0;
							State = 0;
						}
					}
					if(ADC_V_Value < V_Set(28.0)){
						V_envage_state = 0;
						State = 0;
					}
				}
			} else {
				V_envage_state = 1;	
				tx_buf[0] = 1;
				tx_buf[1] = 1;
			}
			Check_State();
			tx_status = NRF24L01_TxPacket(tx_buf);
			if(tx_status == 0X20)  //NRF24L01模块发送数据并判断是否发送成功
			{
				//printf("Send is ok!\r\n");
			}
		}
		delay_xms(1);
	}
}

float I_Float(uint32_t AD_I){
	float Value = (float)AD_I;
	return Value*5.0/4.0*200.0/16.816/4096.0;
}


static uint8_t Flag_65 = 0,Flag_70 = 0,Flag_75 = 0,Flag_80 = 0;
static uint8_t Set_ADC_State = 0;
void Check_State(void){
	Time++;
	TX_TEMPER_1();
	if(Temp_Value >Temp_MAX ){
		tx_buf[1] = 2;
	}	
	if(Temp_Value > 0){
		tx_buf[2] = (uint16_t)(Temp_Value*10)>>8;
		tx_buf[3] = (uint16_t)(Temp_Value*10);
	}else{
		tx_buf[2] = ((uint16_t)(-Temp_Value*10)>>8) | 0x80;
		tx_buf[3] = (uint16_t)(-Temp_Value*10);
	}
	tx_buf[4] = ((uint16_t)(V_Float(ADC_V_Value)*1000))>>8;
	tx_buf[5] = ((uint16_t)(V_Float(ADC_V_Value)*1000));
	tx_buf[6] = ((uint16_t)(I_Float(ADC_Value)*1000))>>8;
	tx_buf[7] = ((uint16_t)(I_Float(ADC_Value)*1000));
	tx_buf[8] = usCRC16(tx_buf,8)>>8;
	tx_buf[9] = usCRC16(tx_buf,8);
	if(Time%200 == 0){
		Time = 0;
/***********/
		if(Temp_Value > 65){
			Flag_65 = 1;
			if(Temp_Value >70){
				Flag_70 = 1;
				if(Temp_Value >75){
					Flag_75 = 1;
					if(Temp_Value >80){
						Flag_80 = 1;
					}
				}
			}
		}	
		if(Flag_80 == 1){
			if(Temp_Value <= 75){
				//4A
				Temp_State = 6;
				if(Set_ADC_State != 80)
					ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(4)); 
				Flag_80 = 0;
				Set_ADC_State = 80;
			}else{
				//2A			
				Temp_State = 8;
				if(Set_ADC_State != 81)
					ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(2)); 
				Set_ADC_State = 81;				
			}		
		}else{
			if(Flag_75 == 1){
				if(Temp_Value <= 70){
					//6A
					Temp_State = 4;
					if(Set_ADC_State != 75)
						ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(6));
					Flag_75 = 0;
					Set_ADC_State = 75;
				}else{
					//4A
					Temp_State = 6;
					if(Set_ADC_State != 76)
						ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(4)); //2688 10A              // 0x00
					Set_ADC_State = 76;				
				}		
			}else{
				if(Flag_70 == 1){
					if(Temp_Value <= 65){
						//8A
						Temp_State = 2;
						if(Set_ADC_State != 70)
							ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(8)); //2688 10A              // 0x00
						Flag_70 = 0;
						Set_ADC_State = 70;
					}else{
						//6A
						Temp_State = 4;
						if(Set_ADC_State != 71)
							ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(6)); //2688 10A              // 0x00
						Set_ADC_State = 71;				
					}		
				}else{
					if(Flag_65 == 1){
						if(Temp_Value <= 60){
							//10A
							Temp_State = 0;
							if(Set_ADC_State != 65)
								ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(10)); //2688 10A              // 0x00
							Flag_65 = 0;
							Set_ADC_State = 65;
						}else{
							//8A
							Temp_State = 2;
							if(Set_ADC_State != 66)
								ADC1_ConfigCompare(ADC1_CMP_NOT_LESS_THAN_RESULT, 50, ADC1_CH_30, I_Set(8)); //2688 10A              // 0x00
							Set_ADC_State = 66;				
						}
					}
				}
			}
		}

/***********/
//		printf("ADDR:%.2x %.2x %.2x %.2x %.2x \r\n",INIT_ADDR[0],INIT_ADDR[1],INIT_ADDR[2],INIT_ADDR[3],INIT_ADDR[4]);
		printf("ACMP_Status ：%d Temp : %0.2f V_Value : %0.2fV  VAD %d I_Value : %0.3fA  I_Value : %d \r\n",ACMP_Status,Temp_Value,V_Float(ADC_V_Value),ADC_V_Value,I_Float(ADC_Value),ADC_Value);
	}
	if(ADC_V_Value > V_MAX){
		if(ADC_Value < I_Fall){
			tx_buf[1] = 5;
			tx_buf[8] = usCRC16(tx_buf,8)>>8;
			tx_buf[9] = usCRC16(tx_buf,8);
		}
	}
}

void check_test(void){
	delay_xms(100);
	if(!ADC1_IS_BUSY())
	{	
		ADC1_Go();
		while(ADC1_IS_BUSY());
		ADC_V_Value = Verge(ADC1_CH_10);
		ADC_Value = Verge(ADC1_CH_30);
		TX_TEMPER_1();
	}
	if(Temp_Value>-20 && Temp_Value <45 ){
		if(ADC_V_Value > Check_V_MIN && ADC_V_Value < Check_V_MAX){
			if(ADC_Value < 100){
				GPIO2->DO_f.P1 = 1;
			}				
		}			
	}
	//printf("Temp : %0.2f V_Value : %dV  I_Value : %d \r\n",Temp_Value,ADC_V_Value,ADC_Value);
} 

