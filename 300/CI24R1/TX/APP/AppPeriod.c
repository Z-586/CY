
#include "main.h"

typedef struct {
    uint16_t tick;
    uint8_t  code;
    uint8_t  flags;
} keyDev_t;

/*******************************************************************************
*
*       Global functions & vars
*
********************************************************************************
*/

void Reset_Variable(void);
void ClrErrCnt(void);

TimerHandle_t   TemperatureAlarm_Handle;				//
TimerHandle_t   ErrorStatus_Handle;						//
TimerHandle_t   FanCtrl_Handle;						    //

#define IR_IN1 HAL_GPIO_ReadPin(CTL_FAN_SPEED_GPIO_Port, CTL_FAN_SPEED_Pin)

int16_t pwm_value        = 0;
uint32_t hall_ref        = 0;

/*******************************************************************************
*
*       Static functions & vars
*
********************************************************************************
*/
static uint8_t m_new_status_A  = 0;
static uint8_t m_success_cnt_A = 0;
static uint8_t m_error_cnt_A   = 0;


static  uint8_t  time_up_flag    = 0;	//上升沿标志位
static  uint8_t  time_dowm_flag  = 0;	//下降沿标志位
static  uint32_t time_up_num     = 0;	//上升沿计数
static  uint32_t time_dowm_num   = 0;	//下降沿计数
static	uint8_t  error_temperature   = 0;
static  uint8_t  err_temperature_cnt = 0;
static  int32    last_temperature    = 9999;
static  uint8_t  m_fanCtrl_stage     = 0;
static  uint8_t  fan_error_cnt       = 0;
///////////////////////////////////////////

const float RTD_TAB_PT100[37] =   // 表格是以5度为一步，即-55, -50, -45.....
{
	437139.00,326906.00,246574.00,187518.00,143739.00,111025.00,86390.00,67699.60,    // -55 ~ -20

	53417.70,42428.70,33916.40,27280.00,22073.40,17963.80,14701.00,12095.70,          // -15 ~ 20

	10000.00,8313.00,6946.07,5829.24,4914.20,4161.00,3538.18,3020.93,                 //  25 ~ 60

	2589.51,2228.22,1924.43,1668.02,1451.00,1266.06,1108.44,973.50,                   //  65 ~ 100
	
	857.575,757.675,671.317,596.441,531.331
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
		return 999;
	}
	if (fR < RTD_TAB_PT100[36])        			// 电阻值小于表格最值低于量程下限。
	{
		return 999;
	}
	
	cBottom = 0; 
	cTop    = 36;
	for (i = 18; (cTop - cBottom) != 1;)        // 2分法查表。
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
	fTem = ( ((fR - fLowRValue)*5) / (fHighRValue - fLowRValue) ) + iTem;        // 表格是以5度为一步的。
																			   // 两点内插进行运算。
	return fTem;
}


/*
* 功能 :
* 描述 :
* 函数 :
*/
static void Tx_ADC_PFC(void)
{ 
	uint32_t adc_pfc = 0;
	
	adc_pfc = Get_Adc_Average(ADC_CHANNEL_15, 20);
	adc_pfc = adc_pfc*3300/4096;
	adc_pfc = adc_pfc*205;
	TXparameter.Tx_U_PFC = adc_pfc;
}


/*
* 功能 : 采集电压
* 描述 ：为了补偿误差自拟采样值与真实电压的函数，(ADC_average0(HAL_ADC_CHN_0,250) 是求平均值
*/
static void TX_ADC_Hall(void)
{
	uint32_t tx_hall = 0;
	
	tx_hall = Get_Adc_Average(ADC_CHANNEL_5, 50);
	tx_hall = tx_hall*3300/4096;
	tx_hall = tx_hall*2;
	if (tx_hall < hall_ref) {
		tx_hall = 0;
	} else {
		tx_hall = (tx_hall-hall_ref);
		tx_hall = tx_hall*5/2;	
	}
	TXparameter.TX_hall = tx_hall;
}


/*
* 功能 : 采集温度T1,T2,...T6
* 描述 ：ADC_average4(HAL_ADC_CHN_4,50)是求平均值函数
*/
static void TX_TEMPER_1(void)
{
	float total_val = 0;
	float resistance = 0;
	uint32_t total_adc = 0;
	total_adc = Get_Adc_Average(ADC_CHANNEL_7, 20);
	total_val = (total_adc*3.3)/4096;
	resistance = ((5-total_val)*1000)/(total_val);
	TXparameter.TX_T1 = CalculateTemperature(resistance);
}

/***********************************************
* 功能 ：计算效率
************************************************/
void EfficiencyCalculation(void)
{                                            
	uint32_t power_out;
	
	power_out = (RXparameter.RX_I*RXparameter.RX_Uout)/TXparameter.TX_hall;
	power_out = power_out*100;
	TXparameter.efficiency = power_out/TXparameter.Tx_U_PFC;
	TXparameter.TX_Power = (TXparameter.TX_hall*TXparameter.Tx_U_PFC)/1000000;
	RXparameter.RX_Power = (RXparameter.RX_I*RXparameter.RX_Uout)/1000000;
}


/*************************************************
* 功能 : 采集电流，电压，温度
**************************************************/
void Adc_deal(void)
{
	Tx_ADC_PFC();       	//
	TX_ADC_Hall();	
}

void Fan_Speed_Proc(int32 t1)
{
	float calc_value = 0;
	
	// 25度 50% 166, 35度 25% 250, 45度0% 329  y = -2.5x + 112.5 y = -3.26x + 329.83
	calc_value = (float)t1*2.5f;
	if (calc_value > 112.5f) {
		TXparameter.fan_duty = 0;
	} else {
		TXparameter.fan_duty = 112.5f - calc_value;
		if (TXparameter.fan_duty > 50) {
			TXparameter.fan_duty = 50;
		}
	}
	pwm_value = TXparameter.fan_duty*6800/100;
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,pwm_value);
	vTaskDelay(100);
	calc_value = TXparameter.fan_duty*3.26f;
	TXparameter.calc_speed = 329.83f - calc_value;
}

void Fan_Operation_Proc(void)
{
	if (TXparameter.TX_T1 > 25) {
		if (abs(last_temperature - TXparameter.TX_T1) >= 2) {
			last_temperature = TXparameter.TX_T1;
			Fan_Speed_Proc(TXparameter.TX_T1);
		}
	} else {
		__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,6800-1);
		m_fanCtrl_stage  = 0;
		TXparameter.fan_duty = 0;
		TXparameter.fan_speed = 0;
	}
	if (TXparameter.TX_T1 > 25) {
		TXparameter.fan_speed = 1000000/time_up_num; //频率
		switch(m_fanCtrl_stage) {
			case 0:
				if( xTimerIsTimerActive( FanCtrl_Handle ) != pdFALSE ) {
					// xTimer is active, do something.
					xTimerStop(FanCtrl_Handle, 0);
				} 
				// xTimer is not active, do something else.
				xTimerStart(FanCtrl_Handle, 0);	
				Debug_Printf("Fan ctrl stage:%d.\r\n", m_fanCtrl_stage);
				m_fanCtrl_stage = 1;						
				break;
			case 1:
				if (TXparameter.fan_speed > 90) {  // 100
					if( xTimerIsTimerActive( FanCtrl_Handle ) != pdFALSE ) {
						// xTimer is active, do something.
						xTimerStop(FanCtrl_Handle, 0);
					}
					fan_error_cnt = 0;
					m_fanCtrl_stage = 2;
				} else {
					Debug_Printf("Fan ctrl stage:%d, fan_speed:%d.\r\n", m_fanCtrl_stage, TXparameter.fan_speed);	
				}			
				break;
			case 2:
				if (TXparameter.fan_speed <= 90) {  // 100
					fan_error_cnt++;
					if (fan_error_cnt >= 10) {
						m_fanCtrl_stage  = 0;
						TXparameter.system_stat2 = 15;
						Debug_Printf("Fan error, fan_speed:%d.\r\n", TXparameter.fan_speed);					
					}
				} else {
					fan_error_cnt = 0;
				}
				break;			
		}			
	}
}

void Check_TemperatureCurrent_Status(void)
{
	if (error_temperature == 0) {	
		if (TXparameter.TX_T1 >= TX_HIGH_TEMP_ERR) {   			    // 1300 90°C
			if (err_temperature_cnt >= 3) {
				// error
				error_temperature = 1;		
			} else {
				err_temperature_cnt++;
			}
		} else if (TXparameter.TX_T1 <= TX_LOW_TEMP_ERR) {
			if (err_temperature_cnt >= 3) {
				// error
				error_temperature = 2;		
			} else {
				err_temperature_cnt++;
			}
		}	
	} else if (error_temperature == 1) {
		if (TXparameter.TX_T1 < TX_HIGH_TEMP_NORMAL) {
			err_temperature_cnt = 0;
			error_temperature = 0;
		}	
	} else if (error_temperature == 2) {
		if (TXparameter.TX_T1 > TX_LOW_TEMP_NORMAL) {
			err_temperature_cnt = 0;
			error_temperature = 0;
		}	
	}
	if (TXparameter.TX_T1 >= TX_TEMP_OPEN_fAN) {
//		GPIO_SetBits(GPIOB, GPIO_Pin_1);
	}	
	else if (TXparameter.TX_T1 <= TX_TEMP_CLOSE_fAN) {
//		GPIO_ResetBits(GPIOB, GPIO_Pin_1);	
	}		
}


void Timer1Operation(void)
{

}


void CLOSE_OPERATION(void)
{
	TXparameter.power_on = 0;
	PwmFreq = Set_140K;
	hhrtim1.Instance->sTimerxRegs[0].PERxR = PwmFreq;
	HAL_HRTIM_WaveformCounterStop(&hhrtim1, HRTIM_TIMERID_TIMER_A); //开启子定时器A
	Reset_Variable();
}


void PhotoelectricSignalOperation(void)
{
	//m_new_status_A = 1;
	if (HAL_GPIO_ReadPin(CTL_GD_GPIO_Port, CTL_GD_Pin) == 0) {
		m_new_status_A = 1;
	} else {
		m_new_status_A = 0;
	}
	if (m_new_status_A == 1) {
		m_error_cnt_A = 1;
		if (m_success_cnt_A < 5) {
			m_success_cnt_A++;
		} else {
			if (g_tx_system_stat == STATE_FLAGE_DEFAULT) {
				// 打开系统
				if (TXparameter.power_on == 0) {
					TXparameter.power_on = 1;					
				}				
			}
		}			
	} else {
		m_success_cnt_A = 1;
		if (m_error_cnt_A < 10) {
			m_error_cnt_A++;
		} else {
			CLOSE_OPERATION();
			g_system_status = 0;
			TXparameter.efficiency = 0;
			ClrErrCnt();
			g_tx_system_stat = STATE_FLAGE_DEFAULT;
			if( xTimerIsTimerActive( TemperatureAlarm_Handle ) != pdFALSE ) {
				// xTimer is active, do something.
				xTimerStop(TemperatureAlarm_Handle, 0);
			}
			if( xTimerIsTimerActive( ErrorStatus_Handle ) != pdFALSE ) {
				// xTimer is active, do something.
				xTimerStop(ErrorStatus_Handle, 0);
			}
		}
	}
}

//回调函数.
void FanCtrlCallback(TimerHandle_t xTimer)
{
	m_fanCtrl_stage  = 0;
	TXparameter.system_stat2 = 15;
	Debug_Printf("Fan error, fan_speed:%d.\r\n", TXparameter.fan_speed);	
}

//回调函数.
void TemperatureAlarmCallback(TimerHandle_t xTimer)
{
	g_tx_system_stat = STATE_FLAGE_DEFAULT;
}


//回调函数.
void ErrorStatusCallback(TimerHandle_t xTimer)
{
	g_tx_system_stat = STATE_FLAGE_DEFAULT;
}


/**@brief Reads an advertising report and checks if a uuid is present in the service list.
 *
 * @details The function is able to search for 16-bit, 32-bit and 128-bit service uuids. 
 *          To see the format of a advertisement packet, see 
 *          https://www.bluetooth.org/Technical/AssignedNumbers/generic_access_profile.htm
 *
 * @param[in]   p_target_uuid The uuid to search fir
 * @param[in]   p_adv_report  Pointer to the advertisement report.
 *
 * @retval      true if the UUID is present in the advertisement report. Otherwise false  
 */
void Period_task(void *pvParameters)
{
	uint8_t _ready1 = 0;
	uint8_t _fault1 = 0;
	
	// 硬件端口初始化   
	TemperatureAlarm_Handle  = xTimerCreate("Temperature TimeOut", 30000,  pdFALSE, NULL, TemperatureAlarmCallback); //600000
	ErrorStatus_Handle       = xTimerCreate("Error Status",         5000,  pdFALSE, NULL, ErrorStatusCallback);  
	FanCtrl_Handle           = xTimerCreate("Fan Control",          2000,  pdFALSE, NULL, FanCtrlCallback); 
	vTaskDelay(100);
	
	hall_ref = Get_Adc_Average(ADC_CHANNEL_2, 20);
	hall_ref = hall_ref*3300/4096;
	hall_ref = hall_ref*3/2;
	while(1){

		Debug_Printf("AppPeriod!!!\r\n");
		vTaskDelay(200);
	
	}
	while (1)
	{
		TX_TEMPER_1();        //采集温度1
		Adc_deal();
		if ((TXparameter.Tx_U_PFC > 370000) && (TXparameter.Tx_U_PFC <= 400000)) {
			_ready1 = HAL_GPIO_ReadPin(GPIOA, FLAG_READY_Pin);
			_fault1 = HAL_GPIO_ReadPin(GPIOB, FLAG_FAULT_Pin);
			if ((_ready1 == 1) && (_fault1 == 1)) {
//				PhotoelectricSignalOperation();	//	
				if (TXparameter.TX_T1 == 999) {
					TXparameter.system_stat2 = ERR_TX_TEMP;	
					g_tx_system_stat = STATE_FLAGE_ERROR;	
				} else {
					PhotoelectricSignalOperation();	//	
					Check_TemperatureCurrent_Status();	
					if (error_temperature) {
						TXparameter.system_stat2 = PEND_TX_TEMP;	
						g_tx_system_stat = STATE_FLAGE_SUSPEND;	
					}									
				}
			} else {
				TXparameter.system_stat2 = ERR_RDY_FUT;
				g_tx_system_stat = STATE_FLAGE_ERROR;
			}			
		} else {
			g_tx_system_stat = STATE_FLAGE_ERROR;
			TXparameter.system_stat2 = ERR_PFC_U;
		}
		vTaskDelay(100);
	}
}

// 定时器2中断函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)	
{
	 if (htim->Instance == TIM2){
		 if(__HAL_TIM_GET_ITSTATUS(&htim2, TIM_IT_CC1) == SET){
			if(IR_IN1&&time_up_flag == 0)								//第一次上升
			{
				time_up_flag = 1;
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_FALLING);// 改变捕获极性为下降沿捕获
				__HAL_TIM_SetCounter(&htim2,0);// 计数清零，从头开始计
			}
			else if(IR_IN1==0&&time_dowm_flag==0)						//下降
			{
				time_dowm_num = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);// 读取捕获计数，这个时间即为上升沿持续的时间
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_RISING);// 设置输入捕获通道的极性为上升沿
				time_dowm_flag=1;
				time_dowm_num = time_dowm_num;
			}
			else if(IR_IN1&&time_dowm_flag==1)							//第二次之后上升
			{        
				time_up_num = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);// 读取捕获计数，这个时间即为上升沿持续的时间
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_FALLING);// 改变捕获极性为下降沿捕获
				time_dowm_flag = 0;
				__HAL_TIM_SetCounter(&htim2,0);// 计数清零，从头开始计
			} 
			__HAL_TIM_CLEAR_IT(&htim2,TIM_CHANNEL_2);	// 清除输入捕获通道的中断标志位
		 } 	 
	 }
	
//	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)     				// 定时器4通道1输入捕获中断
//	{
//        if(IR_IN1&&time_up_flag == 0)								//第一次上升
//        {
//            time_up_flag = 1;
//			TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Falling); 	// 改变捕获极性为下降沿捕获
//			TIM_SetCounter(TIM4, 0);								// 计数清零，从头开始计
//        }
//        else if(IR_IN1==0&&time_dowm_flag==0)						//下降
//        {
//            time_dowm_num = TIM_GetCapture1(TIM4); 					// 读取捕获计数，这个时间即为上升沿持续的时间
//            TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Rising);  	// 设置输入捕获通道的极性为上升沿
//            time_dowm_flag=1;
//			time_dowm_num = time_dowm_num;
//        }
//        else if(IR_IN1&&time_dowm_flag==1)							//第二次之后上升
//        {        
//            time_up_num = TIM_GetCapture1(TIM4); 					// 读取捕获计数，这个时间即为上升沿持续的时间
//            TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Falling);  	// 改变捕获极性为下降沿捕获
//            time_dowm_flag = 0;
//            TIM_SetCounter(TIM4, 0);								// 计数清零，从头开始计  

//        } 
//		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);     				// 清除输入捕获通道的中断标志位
//	}	
}
