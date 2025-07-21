/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup app_gpiote GPIOTE Handler
 * @{
 * @ingroup app_common
 *
 * @brief GPIOTE handler module.
 *
 * @details The GPIOTE handler allows several modules ("users") to share the GPIOTE interrupt,
 *          each user defining a set of pins able to generate events to the user.
 *          When a GPIOTE interrupt occurs, the GPIOTE interrupt handler will call the event handler
 *          of each user for which at least one of the pins generated an event.
 *
 *          The GPIOTE users are responsible for configuring all their corresponding pins, except
 *          the SENSE field, which should be initialized to GPIO_PIN_CNF_SENSE_Disabled.
 *          The SENSE field will be updated by the GPIOTE module when it is enabled or disabled,
 *          and also while it is enabled.
 *
 *          The module specifies on which pins events should be generated if the pin(s) goes
 *          from low->high or high->low or both directions.
 *
 * @note    Even if the application is using the @ref app_scheduler, the GPIOTE event handlers will
 *          be called directly from the GPIOTE interrupt handler.
 *
 * @warning If multiple users registers for the same pins the behavior for those pins are undefined.
 */

#ifndef MAIN_H__
#define MAIN_H__

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "AppBlePro.h"
#include "AppFaultHandler.h"
#include "AppBleComm.h"
#include "AppTest.h"
#include "AppRcvProc2.h"



//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////设置参数 START/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//extern  int                         g_DAC_Init;


//#define DAC_INIT_VALUE				900		   // 800 //1280
//#define DAC_HIGH_VALUE				900	


//参数设置
#define RS485_ADDR					1
#define DEBUG_ON                    0


//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define STATE_FLAGE_DEFAULT			0
#define STATE_FLAGE_CHARGING		1
#define STATE_FLAGE_BATT_FULL		2
#define STATE_FLAGE_ERROR			3
#define STATE_FLAGE_SUSPEND			4
//#define STATE_FLAGE_FAULT           5

//#define STATE_FLAGE_TOO_BIG		4
//#define STATE_FLAG_TOO_BIG_WAIT	5
//#define STATE_FLAG_BATT_FULL_WAIT	6
//#define STATE_FLAG_ERROR_WAIT		7

 
//#define LED_DEFAULT				0xff
//#define LED_BATT_FULL				3
//#define LED_SYS_ERR				4

//RS485
#define UART1_BUF_LENGTH    128
typedef struct
{
	uint8_t  buf[UART1_BUF_LENGTH];
	uint16_t len;
} T_Buffer;


typedef struct
{
	uint8_t  buf[UART1_BUF_LENGTH];
	uint16_t len;
} R_Buffer;


//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define RX_HIGH_TEMP_ERR			90		//1803
#define RX_HIGH_TEMP_NORMAL			70		//1661

#define RX_LOW_TEMP_ERR				-35		//1803
#define RX_LOW_TEMP_NORMAL			-30		//1661

#define TX_TEMP_OPEN_fAN			30		//55		//480	//510
#define TX_TEMP_CLOSE_fAN			25		//45		//380	//400

#define TX_HIGH_TEMP_ERR			80		//80   	    //900   //90
#define TX_HIGH_TEMP_NORMAL			55		//65		//550	//70

#define TX_LOW_TEMP_ERR				-35		//80   	    //900   //90
#define TX_LOW_TEMP_NORMAL			-30		//65		//550	//70


//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// 恒压,小于满载电流的80% 充满,小于满载电流的10%
// 系统满载电流为2.5A,电压72V
// 此系统特例,充满电流设置为0.5A
#define RECHARGE_I_FULL				500				//500
#define RECHARGE_U_FULL				28500			//500



//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define EFF_HIGH_POWER_I_DETECTION  2000 
//#define EFF_LOW_POWER_I_DETECTION   6000 

#define EFF_HIGH_POWER_MIN_1	    65
#define EFF_HIGH_POWER_MAX_1	    95

#define EFF_HIGH_POWER_MIN_2		75
#define EFF_HIGH_POWER_MAX_2	    95
				


//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define SYS_NORMAL   			0xff
#define SYS_CC                  0
#define SYS_CV                  1
#define PEND_RX_TEMPER          2
#define ERR_I_Max             	3
#define ERR_V_Max               4
#define SYS_BATT_FULL           5
#define ERR_PFC_U    			10
#define ERR_RDY_FUT  			11
#define PEND_TX_TEMP            17
#define ERR_TX_TEMP             22
#define ERR_RX_TEMP             23

//#define ERR_IOUT_SOFT          5
//#define ERR_ADC_IOUT           6
//#define ERR_RX_TEMPER          7
//#define ERR_RX_FAN     		 8
//#define ERR_CHARGE_UIN         9

//#define ERR_HALL_CHANGE     	12
//#define ERR_HALL_DETECTION  	12
//#define ERR_EFF_1     		13
//#define ERR_EFF_2     		14
//#define ERR_TX_FAN     		15
//#define SYS_BATT_FULL         16
//#define ERR_FIRST_CONN        18
//#define ERR_DIS_CONN          19
//#define ERR_POSITION_UIN      20
//#define ERR_INIT_UIN          21



//////////////////////////////////////////////////////
//////////////////////////////////////////////////////



//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define UNUSED_VARIABLE(X)  		((void)(X))
#define UNUSED_PARAMETER(X) 		UNUSED_VARIABLE(X)
#define UNUSED_RETURN_VALUE(X) 		UNUSED_VARIABLE(X)

#define UART_TX_BUF_SIZE        256                             /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE        256                             /**< UART RX buffer size. */

#define APPL_LOG                NRF_LOG_PRINTF                  /**< Debug logger macro that will be used in this file to do logging of debug information over UART. */

#define SCAN_INTERVAL           0x00A0                          /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW             0x0050                          /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_ACTIVE             1                               /**< If 1, performe active scanning (scan requests). */
#define SCAN_SELECTIVE          0                               /**< If 1, ignore unknown devices (non whitelisted). */
#define SCAN_TIMEOUT            0x0000                          /**< Timout when scanning. 0x0000 disables timeout. */

#define MIN_CONNECTION_INTERVAL MSEC_TO_UNITS(20, UNIT_1_25_MS) /**< Determines minimum connection interval in millisecond. */
#define MAX_CONNECTION_INTERVAL MSEC_TO_UNITS(75, UNIT_1_25_MS) /**< Determines maximum connection interval in millisecond. */
#define SLAVE_LATENCY           0                               /**< Determines slave latency in counts of connection events. */
#define SUPERVISION_TIMEOUT     MSEC_TO_UNITS(4000, UNIT_10_MS) /**< Determines supervision time-out in units of 10 millisecond. */

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

//参数设置
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// Output
//#define VC_VIN       			                    GPIO_Pin_9
//#define FAN_ENABLE   			                    GPIO_Pin_12
#define SWITCH_IO								    GPIO_Pin_13
#define RESET1										GPIO_Pin_12

#define RED_LED   				                	GPIO_Pin_5			
#define GREEN_LED       			            	GPIO_Pin_11
#define BLUE_LED       	                        	GPIO_Pin_10	


// Input
#define KEY_CTL       			                    GPIO_Pin_7
#define READY1										GPIO_Pin_0
#define FAULT1										GPIO_Pin_0

//Test
#define TEST_LED					                GPIO_Pin_12      

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////设置参数 END///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

//1 
//任务优先级
#define START_TASK_PRIO			 					1	
//任务堆栈大小
#define START_STK_SIZE 		  						50  
//任务函数
void start_task(void *pvParameters);												

////2
////任务优先级
//#define TEST_TASK_PRIO                  			2
////任务堆栈大小
//#define TEST_STK_SIZE                     			100  
////任务函数
//void Test_task(void *pvParameters);

//1
//任务优先级
#define LedProc_TASK_PRIO                  			4
//任务堆栈大小
#define LedProc_STK_SIZE                     		200  
//任务函数
void LedProc_task(void *pvParameters);

//2
//任务优先级
#define RS485Proc_TASK_PRIO			 			    18
//任务堆栈大小
#define RS485Proc_STK_SIZE 					        250 
//任务函数
void RS485_Proc_task(void *pvParameters);	

//3
//任务优先级
#define Period_TASK_PRIO                  			8
//任务堆栈大小
#define Period_STK_SIZE                     		250  
//任务函数
void Period_task(void *pvParameters);	

//4
//任务优先级
#define FaultHandler_TASK_PRIO                  	6  //12
//任务堆栈大小
#define FaultHandler_STK_SIZE                     	250  
//任务函数
void FaultHandler_task(void *pvParameters);

////5
////任务优先级
//#define BleComm_TASK_PRIO                  			10
////任务堆栈大小
//#define BleComm_STK_SIZE                     		250  
////任务函数
//void BleComm_task(void *pvParameters);

////6
////任务优先级
//#define RcvPro2_TASK_PRIO			 			    11
////任务堆栈大小
//#define RcvPro2_STK_SIZE 					        250 
////任务函数
//void RcvProc2_task(void *pvParameters);			

//7
//任务优先级
#define BlePro_TASK_PRIO                  			15
//任务堆栈大小
#define BlePro_STK_SIZE                     		250  
//任务函数
void BlePro_task(void *pvParameters);


#define BLE_CONNECTED     		1
#define BLE_DISCONNECT    		2



/************************************************
//操作系统变量
************************************************/
extern TaskHandle_t 					StartTask_Handler;			    //任务句柄	1
extern TaskHandle_t 					TestTask_Handler;				//任务句柄  2
extern TaskHandle_t 					LedProcTask_Handler;			//任务句柄  3
extern TaskHandle_t                     Period_task_Handler;			//任务句柄  3
extern TaskHandle_t 					FaultHandlerTask_Handler;       //任务句柄  4
extern TaskHandle_t 					BleCommTask_Handler;			//任务句柄  5
extern TaskHandle_t 					RcvPro2Task_Handler;            //任务句柄  6
extern TaskHandle_t 					BleProTask_Handler;				//任务句柄  7

extern SemaphoreHandle_t 				BleProcSemaphore;				//蓝牙处理信号量
extern SemaphoreHandle_t                BleSendSemaphore;				//蓝牙发送信号量
extern SemaphoreHandle_t                RcvProc2Semaphore;
extern SemaphoreHandle_t                RS485DataSemaphore;
//extern SemaphoreHandle_t 				MutexGlobalHandle;
//extern SemaphoreHandle_t 				MutexADCmHandle;
extern SemaphoreHandle_t 				MutexPrintfHandle;

//extern TimerHandle_t 					Control_DAC_timer;


//全局变量声明
extern struct TX_parameter 		TXparameter ;
extern struct RX_parameter 		RXparameter ;
extern uint8_t       		    g_tx_system_stat;
extern uint8_t					g_system_status;
extern uint8_t	                RS485_Addr;
extern uint8_t                  baud;
extern uint16_t                 PwmFreq;

//extern uint8_t                  g_system_power;
//extern uint8_t                  g_fanCtrl_stage;

//extern ble_nus_c_t         		g_ble_nus_c;                    /**< Instance of NUS service. Must be passed to all NUS_C API calls. */
//extern nrf_drv_wdt_channel_id 	g_channel_id;


void Adc_init(void);
void CLOSE_OPERATION(void);
void OPEN_OPERATION(void);
void Double_pwm_init(void);
void frequence_init(void);
void gpiote_init(void);
void ppi_set(void);
void timer1_init(void);
//void Update_Voltage(int dac_value);
void Debug_Printf(char *format, ...);
//void Control_DAC_handler(TimerHandle_t xTimer);
void IWDG_Feed(void);

//void SendBatteryStatus(void);
//void SendDisconnect(void);
//void SendTemperatureError(uint8_t tx_err, uint8_t rx_err);
//void SendMachinePosFail(void);
//void SendDutyValueReady(void);
//uint8_t Get_Random_Binary(uint8_t arry[]);


#endif // MAIN_H__

/** @} */
