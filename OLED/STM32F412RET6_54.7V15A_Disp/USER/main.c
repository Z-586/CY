#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "can.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
#include "RGB.h"
#include "lcd.h"
#include "pic2.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


/************************************************
 DEFINE VAR
************************************************/
#define START_TASK_PRIO   			1
#define START_STK_SIZE    			128
TaskHandle_t StartTask_Handler;
void start_task(void*pvParameters);

#define LCD_TASK_PRIO               3
#define LCD_STK_SIZE                200
TaskHandle_t LCDTask_Handler;
void LCD_task(void*p_arg);

#define CONTROL_TASK_PRIO           5
#define CONTROL_STK_SIZE            200
TaskHandle_t ControlTask_Handler;
void Control_task(void*p_arg);

#define COMM_TASK_PRIO              8
#define COMM_STK_SIZE               200
TaskHandle_t CommTask_Handler;
void Comm_task(void*p_arg);

SemaphoreHandle_t 	Uart2Semaphore;
SemaphoreHandle_t 	MutexPrintfHandle;
TimerHandle_t	    CommDataTimeOut_Handle;					//单次定时器句柄,创建单次定时器,用于面板超时接收数据
void CommDataTimeOutCallback(TimerHandle_t xTimer);

//TimerHandle_t	Comm2RcvTimeOut_Handle;						//单次定时器句柄,创建单次定时器,用于面板超时接收数据
//void Comm2RcvTimeOutCallback(TimerHandle_t xTimer);

//TimerHandle_t	Comm1RcvTimeOut_Handle;						//单次定时器句柄,创建单次定时器,用于面板超时接收数据
//void Comm1RcvTimeOutCallback(TimerHandle_t xTimer);




#define UOUT_MIN 				40000
#define UOUT_MAX 				56000


#define RED_LED   				GPIO_PIN_8			
#define GREEN_LED       	    GPIO_PIN_7
#define BLUE_LED       	        GPIO_PIN_6	

#define UNUSED_VARIABLE(X)  	((void)(X))
#define UNUSED_PARAMETER(X) 	UNUSED_VARIABLE(X)

ADC_HandleTypeDef ADC1_Handler;//ADC句柄
float    temperature_1  = 0.0f;
float    temperature_2  = 0.0f;

uint32_t  tx_u_disp = 0;
float  tx_i_disp = 0;
float  rx_u_disp = 0;
float  rx_i_disp = 0;
uint32_t tx_u = 0;
uint32_t tx_i = 0;
uint32_t rx_u = 0;
uint32_t rx_i = 0;
uint32_t rx_e = 0;
uint32_t rx_p = 0;
uint8_t  system_status = 0;
uint8_t  g_led_stat = 0;

char buf_i[5];
char buf_u[5];
char buf_e[3];
char buf_p[3];


void Debug_Printf(char *format, ...)
{
	char  buf_str[128];
	va_list   v_args;

	va_start(v_args, format);
	(void)vsnprintf((char       *)&buf_str[0],
				   (size_t      ) sizeof(buf_str),
				   (char const *) format,
								  v_args);
	va_end(v_args);

	/* 互斥信号量 */
	xSemaphoreTake(MutexPrintfHandle, portMAX_DELAY );
	printf("%s", buf_str);
	xSemaphoreGive(MutexPrintfHandle);
}


void MY_ADC_Init(void)
{ 
    ADC1_Handler.Instance                   = ADC1;
    ADC1_Handler.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;     //4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution            = ADC_RESOLUTION_12B;           //12位模式
    ADC1_Handler.Init.DataAlign             = ADC_DATAALIGN_RIGHT;          //右对齐
    ADC1_Handler.Init.ScanConvMode          = DISABLE;                      //非扫描模式
    ADC1_Handler.Init.EOCSelection          = DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode    = DISABLE;                		//关闭连续转换
    ADC1_Handler.Init.NbrOfConversion       = 1;                         	//1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode = DISABLE;             			//禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion   = 0;                     		//不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv      = ADC_SOFTWARE_START;       	//软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests = DISABLE;             			//关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                                 			//初始化 
}


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            			//使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();						//开启GPIOA时钟
	
    GPIO_Initure.Pin  = GPIO_PIN_0|GPIO_PIN_1;          //PA0和PA1
    GPIO_Initure.Mode = GPIO_MODE_ANALOG;     			//模拟
    GPIO_Initure.Pull = GPIO_NOPULL;          			//不带上下拉
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
}


//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel      = ch;                              //通道
    ADC1_ChanConf.Rank         = 1;                               //第1个序列，序列1
    ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC1_ChanConf.Offset       = 0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);          //通道配置
	
    HAL_ADC_Start(&ADC1_Handler);                                 //开启ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                  //轮询转换
 
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	              //返回最近一次ADC1规则组的转换结果
}

//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t = 0; t < times; t++) {
		temp_val += Get_Adc(ch);
		vTaskDelay(5);
	}
	return temp_val/times;
}


const float RTD_TAB_PT100[33] =   // 表格是以5度为一步，即-55, -50, -45.....
{
	437139.00,326906.00,246574.00,187518.00,143739.00,111025.00,86390.00,67699.60,    // -55 ~ -20

	53417.70,42428.70,33916.40,27280.00,22073.40,17963.80,14701.00,12095.70,          // -15 ~ 20

	10000.00,8313.00,6946.07,5829.24,4914.20,4161.00,3538.18,3020.93,                 //  25 ~ 60

	2589.51,2228.22,1924.43,1668.02,1451.00,1266.06,1108.44,973.50,                   //  65 ~ 100
	
	857.575
};


/***********************************************************************
 *FunName:        float CalculateTemperature(float fR)
 *
 *In:                fR -> PT100的电阻值。
 *                        
 *Out:                fTem -> 测得的温度值。               
 *
*Discription: 将电阻值查表算出温度值。
 *
 *Notes:         采用2分查找法。          ************************************************************************/
float CalculateTemperature(float fR)
{
	float fTem;
	float fLowRValue;
	float fHighRValue;        
	int   iTem;
	uint8_t i;
	//uint8_t cLimite = 0xFF;
	uint8_t cBottom, cTop;
 
	if (fR > RTD_TAB_PT100[0])                	// 电阻值大于表格最大值，超出量程上限 。
	{
		return 0;
	}
	if (fR < RTD_TAB_PT100[32])        			// 电阻值小于表格最值低于量程下限。
	{
		return 0;
	}
	
	cBottom = 0; 
	cTop    = 32;
	for (i = 16; (cTop - cBottom) != 1;)        // 2分法查表。
	{
		if (fR > RTD_TAB_PT100[i])
		{
			cTop = i;
			i = (cTop + cBottom) / 2;
		}
		else if (fR < RTD_TAB_PT100[i])
		{
			 cBottom = i;
			 i = (cTop + cBottom) / 2;
		}
		else
		{
			iTem = (uint32_t)i * 5 - 55;
			fTem = (float)iTem;
			return fTem;
		}
	}
	iTem = (uint32_t)i * 5 - 55;
	fLowRValue  = RTD_TAB_PT100[cBottom];
	fHighRValue = RTD_TAB_PT100[cTop];
	fTem = ( ((fR - fLowRValue)*5) / (fHighRValue - fLowRValue) ) + iTem;       	// 表格是以5度为一步的。
																					// 两点内插进行运算。
	return fTem;
}


void user1_LCD_Display(void)
{
	Show_Str(121, 20, WHITE, BLACK, "卡珀林智能科技有限公司", 32, 1);
	Gui_Drawbmp16(0, 0, gImage_logo_120x80);
}


void user2_LCD_Display(void)
{
//	rx_i = 10000;
//	rx_u = 35000;
//	tx_i = 666;
//	tx_u = 380000;
//	rx_e = 100;
	rx_i_disp = rx_i;
	sprintf(buf_i,"%5.2f", rx_i_disp/1000);
	Show_Str(10-8,  98, WHITE, BLACK, "输出电流:", 32, 0);
	Show_Str(159-13, 98, WHITE, BLACK, (uint8_t *)buf_i, 32, 0);
	Show_Str(243-17, 98, WHITE, BLACK, "A", 32, 0); //246
	
	rx_u_disp = rx_u;
	sprintf(buf_u,"%5.2f", rx_u_disp/1000);
	Show_Str(10-8, 163, WHITE, BLACK, "输出电压:", 32, 0);
	Show_Str(159-13,163, WHITE, BLACK, (uint8_t *)buf_u, 32, 0);
	Show_Str(243-17,163, WHITE, BLACK, "V", 32, 0);
	
	tx_i_disp = tx_i;
	sprintf(buf_i,"%3.2f", tx_i_disp/1000);
	Show_Str(245,  98, WHITE, BLACK, "输入电流:", 32, 0);
	Show_Str(389, 98, WHITE, BLACK, (uint8_t *)buf_i, 32, 0);
	Show_Str(453, 98, WHITE, BLACK, "A", 32, 0); //246
	
	tx_u_disp = tx_u;
	sprintf(buf_u,"%3d", tx_u_disp/1000);
	Show_Str(245,  163, WHITE, BLACK, "输入电压:", 32, 0);
	Show_Str(389+16, 163, WHITE, BLACK, (uint8_t *)buf_u, 32, 0);
	Show_Str(453, 163, WHITE, BLACK, "V", 32, 0); //246
	
	sprintf(buf_e,"%3d", rx_e);
	Show_Str(10, 227, WHITE, BLACK, "效率:", 32, 0);
	Show_Str(159,227, WHITE, BLACK, (uint8_t *)buf_e, 32, 0); 
	Show_Str(212,227, WHITE, BLACK, "%", 32, 0);
	
	rx_p = rx_i*rx_u/1000000;
	sprintf(buf_p,"%03d", rx_p);
	Show_Str(250,227, WHITE, BLACK, "功率:", 32, 0);
	Show_Str(399,227, WHITE, BLACK, (uint8_t *)buf_p, 32, 0);
	Show_Str(452,227, WHITE, BLACK, "W", 32, 0);
}


void user3_LCD_Display(void)
{
	Show_Str(50, 270, WHITE, BLACK, "电池电量:", 32, 0);
	LCD_DrawRectangle(206, 270, 402, 310); // 20 246
}


//IO初始化
void IO_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();               //开启GPIOB时钟
	
    GPIO_Initure.Pin = (GPIO_PIN_8);			//PA8
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull = GPIO_NOPULL;          	//
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	//高速
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
    GPIO_Initure.Pin = (GPIO_PIN_5);			//PB5
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull = GPIO_NOPULL;          	//
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	//高速
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);	
}


void Open_Fan(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}


void Close_Fan(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}


/* USER CODE BEGIN 0 */
void CommDataTimeOutCallback(TimerHandle_t xTimer)
{
	xTimer = xTimer;
	
	tx_i = 0;
	tx_u = 0;
	rx_e = 0;
	rx_i = 0;
	rx_u = 0;
	rx_p = 0;
	system_status = 0;
	g_led_stat = 0;
}


int main(void)
{
    HAL_Init();                    	      	// 初始化HAL库    
    Stm32_Clock_Init(96, 12, 2, 2);      	// 设置时钟,100Mhz
    //delay_init(96);                     	// 初始化延时函数
    LED_Init();  						  	// 初始化LED 
	LCD_Init();
	USART1_Init();							// USART1初始化	
	USART2_Init();							// USART2初始化
	MY_ADC_Init();                  		// 初始化ADC1通道1
	IO_Init();
	//MX_CAN1_Init();

	xTaskCreate((TaskFunction_t    )start_task,
	            (const char*       )"start_task",
							(uint16_t          )START_STK_SIZE,
							(void*             )NULL,
							(UBaseType_t       )START_TASK_PRIO,
							(TaskHandle_t*     )&StartTask_Handler);
	vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();     //进入临界区
	// 创建LCD任务
	xTaskCreate((TaskFunction_t    )LCD_task,
	            (const char*       )"LCD_task",
							(uint16_t          )LCD_STK_SIZE,
							(void*             )NULL,
							(UBaseType_t       )LCD_TASK_PRIO,
							(TaskHandle_t*     )&LCDTask_Handler);
	// 创建Control任务
	xTaskCreate((TaskFunction_t    )Control_task,
	            (const char*       )"Control_task",
							(uint16_t          )CONTROL_STK_SIZE,
							(void*             )NULL,
							(UBaseType_t       )CONTROL_TASK_PRIO,
							(TaskHandle_t*     )&ControlTask_Handler);
	// 创建Comm任务
	xTaskCreate((TaskFunction_t    )Comm_task,
	            (const char*       )"Comm_task",
							(uint16_t          )COMM_STK_SIZE,
							(void*             )NULL,
							(UBaseType_t       )COMM_TASK_PRIO,
							(TaskHandle_t*     )&CommTask_Handler);							
							
	MutexPrintfHandle   	= 	xSemaphoreCreateMutex();
    Uart2Semaphore 		    =	xSemaphoreCreateBinary();	

	CommDataTimeOut_Handle  = xTimerCreate("RS485 Timeout",       3000,  pdFALSE, NULL, CommDataTimeOutCallback);							
//	Comm2RcvTimeOut_Handle	= xTimerCreate("RS485 Communication",  100,  pdFALSE, NULL, Comm2RcvTimeOutCallback);
//	Comm1RcvTimeOut_Handle	= xTimerCreate("RS485 Communication",  100,  pdFALSE, NULL, Comm1RcvTimeOutCallback);
							
	vTaskDelete(StartTask_Handler);       //删除开始任务
	taskEXIT_CRITICAL();
}


void LCD_task(void *pvParameters)
{	
	uint8_t disp_flag = 0;
	uint32_t uout_step = 0;
	
	uout_step = (UOUT_MAX-UOUT_MIN)/4;
	//Gui_Drawbmp16_1(0, 0, gImage_logo_black);
	//vTaskDelay(4000);
	LCD_Clear(BLACK);//清全屏黑色
	
	while(1)
	{
		user1_LCD_Display();
		user2_LCD_Display();
		user3_LCD_Display();
		switch (system_status) {
			case 0:
				disp_flag = 0;
				break;
			case 1:// 15000mV ~ 25200mV 
				if (rx_u > 0 && rx_u < UOUT_MIN) {
					disp_flag = 1;
				} else if (rx_u >= UOUT_MIN && rx_u < (1*uout_step + UOUT_MIN)) {
					disp_flag = 2;
				} else if (rx_u >= (1*uout_step + UOUT_MIN) && rx_u < (2*uout_step + UOUT_MIN)) {
					disp_flag = 3;
				} else if (rx_u >= (2*uout_step + UOUT_MIN) && rx_u < (3*uout_step + UOUT_MIN)) {
					disp_flag = 4;
				} else if (rx_u >= (3*uout_step + UOUT_MIN) && (rx_u < UOUT_MAX)) {
					disp_flag = 5;
				} 
				break;
			case 2:
				disp_flag = 10;
				break;
		}
		switch (disp_flag) {
			case 1:
				LCD_DrawFillRectangle(250-40, 274, 246+35-40, 306, WHITE); // 24 
				LCD_DrawFillRectangle(285-40, 274, 285+35-40, 306, BLACK); // 50+22
				LCD_DrawFillRectangle(324-40, 274, 324+35-40, 306, BLACK); // 76+22
				LCD_DrawFillRectangle(363-40, 274, 363+35-40, 306, BLACK); //102+22
				LCD_DrawFillRectangle(402-40, 274, 402+35-40, 306, BLACK); // 128+22
				vTaskDelay(500);
				LCD_DrawFillRectangle(250-40, 274, 246+35-40, 306, BLACK); // 24 
				break;
			case 2:
				LCD_DrawFillRectangle(250-40, 274, 246+35-40, 306, WHITE); // 24 
				LCD_DrawFillRectangle(285-40, 274, 285+35-40, 306, WHITE); // 50+22
				LCD_DrawFillRectangle(324-40, 274, 324+35-40, 306, BLACK); // 76+22
				LCD_DrawFillRectangle(363-40, 274, 363+35-40, 306, BLACK); //102+22
				LCD_DrawFillRectangle(402-40, 274, 402+35-40, 306, BLACK); // 128+22
				vTaskDelay(500);
				LCD_DrawFillRectangle(285-40, 274, 285+35-40, 306, BLACK); // 50+22
				break;
			case 3:
				LCD_DrawFillRectangle(250-40, 274, 246+35-40, 306, WHITE); // 24 
				LCD_DrawFillRectangle(285-40, 274, 285+35-40, 306, WHITE); // 50+22
				LCD_DrawFillRectangle(324-40, 274, 324+35-40, 306, WHITE); // 76+22
				LCD_DrawFillRectangle(363-40, 274, 363+35-40, 306, BLACK); //102+22
				LCD_DrawFillRectangle(402-40, 274, 402+35-40, 306, BLACK); // 128+22
				vTaskDelay(500);
				LCD_DrawFillRectangle(324-40, 274, 324+35-40, 306, BLACK); // 76+22
				break;
			case 4:
				LCD_DrawFillRectangle(250-40, 274, 246+35-40, 306, WHITE); // 24 
				LCD_DrawFillRectangle(285-40, 274, 285+35-40, 306, WHITE); // 50+22
				LCD_DrawFillRectangle(324-40, 274, 324+35-40, 306, WHITE); // 76+22
				LCD_DrawFillRectangle(363-40, 274, 363+35-40, 306, WHITE); //102+22
				LCD_DrawFillRectangle(402-40, 274, 402+35-40, 306, BLACK); // 128+22
				vTaskDelay(500);
				LCD_DrawFillRectangle(363-40, 274, 363+35-40, 306, BLACK); //102+22
				break;
			case 5:
				LCD_DrawFillRectangle(250-40, 274, 246+35-40, 306, WHITE); // 24 
				LCD_DrawFillRectangle(285-40, 274, 285+35-40, 306, WHITE); // 50+22
				LCD_DrawFillRectangle(324-40, 274, 324+35-40, 306, WHITE); // 76+22
				LCD_DrawFillRectangle(363-40, 274, 363+35-40, 306, WHITE); //102+22
				LCD_DrawFillRectangle(402-40, 274, 402+35-40, 306, WHITE); // 128+22
				vTaskDelay(500);
				LCD_DrawFillRectangle(402-40, 274, 402+35-40, 306, BLACK); // 128+22
				break;
			case 10:
				LCD_DrawFillRectangle(250-40, 274, 246+35-40, 306, WHITE); // 24 
				LCD_DrawFillRectangle(285-40, 274, 285+35-40, 306, WHITE); // 50+22
				LCD_DrawFillRectangle(324-40, 274, 324+35-40, 306, WHITE); // 76+22
				LCD_DrawFillRectangle(363-40, 274, 363+35-40, 306, WHITE); //102+22
				LCD_DrawFillRectangle(402-40, 274, 402+35-40, 306, WHITE); // 128+22
				break;
			case 0:
				LCD_DrawFillRectangle(250-40, 274, 246+35-40, 306, BLACK); // 24 
				LCD_DrawFillRectangle(285-40, 274, 285+35-40, 306, BLACK); // 50+22
				LCD_DrawFillRectangle(324-40, 274, 324+35-40, 306, BLACK); // 76+22
				LCD_DrawFillRectangle(363-40, 274, 363+35-40, 306, BLACK); //102+22
				LCD_DrawFillRectangle(402-40, 274, 402+35-40, 306, BLACK); // 128+22
				break;
		}
		vTaskDelay(500);
	}
}


void Control_task(void *pvParameters)
{
	u16 adcx = 0;
	float total_val = 0.0f;
	float resistance = 0.0f;

	while(1)
	{
		adcx = Get_Adc_Average(ADC_CHANNEL_0, 20);			//获取通道1的转换值，20次取平均
		total_val = (float)adcx*(3.3/4096);          		//获取计算后的带小数的实际电压值，比如3.1111
		resistance = ((3.3f-total_val)*1000)/(total_val);
		temperature_1 = CalculateTemperature(resistance);
		adcx = Get_Adc_Average(ADC_CHANNEL_1, 20);			//获取通道1的转换值，20次取平均
		total_val = (float)adcx*(3.3/4096);          		//获取计算后的带小数的实际电压值，比如3.1111
		resistance = ((3.3f-total_val)*1000)/(total_val);
		temperature_2 = CalculateTemperature(resistance);
		if ((temperature_1 > 30) || (temperature_2 > 30)) {
			Open_Fan();
		} else if ((temperature_1 < 25) && (temperature_2 < 25)) {
			Close_Fan();	
		} 
		//LED0 = 0;
		vTaskDelay(200);
		Debug_Printf("temperature_1:%5.1f, temperature_2:%5.1f.\r\n", temperature_1, temperature_2);
		//LED0 = 1;
	}
}


void Comm_task(void *pvParameters)
{
	while(1)
	{
		xSemaphoreTake(Uart2Semaphore, portMAX_DELAY);
		if (USART2_RX_BUF[0] == 0x3A) {
			//Debug_Printf("hello world.\r\n");
			xTimerStop(CommDataTimeOut_Handle, 0);
			rx_u = ((USART2_RX_BUF[1] << 16) | (USART2_RX_BUF[2] << 8) | USART2_RX_BUF[3]);
			rx_i = ((USART2_RX_BUF[4] << 8) | USART2_RX_BUF[5]);
			rx_e = USART2_RX_BUF[6];
			rx_p = ((USART2_RX_BUF[7] << 8) | USART2_RX_BUF[8]);
			system_status = USART2_RX_BUF[9];
			g_led_stat = USART2_RX_BUF[10];
			tx_u = ((USART2_RX_BUF[11] << 16) | (USART2_RX_BUF[12] << 8) | USART2_RX_BUF[13]);
			tx_i = ((USART2_RX_BUF[14] << 8) | USART2_RX_BUF[15]);
			xTimerStart(CommDataTimeOut_Handle, 0);
		}
		memset(USART2_RX_BUF, 0, USART_REC_LEN);
		USART2_RX_STA = 0;
	}
}
	
