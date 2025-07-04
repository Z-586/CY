#include "includes.h"


///*******************************************************************************
//*
//*       Static typedefs & defines & misc
//*
//********************************************************************************
//*/
static uint8_t m_err_cnt = 0;

extern TimerHandle_t    TemperatureAlarm_Handle;
extern TimerHandle_t    ErrorStatus_Handle;

///////////////////////////////////////////////////////////////////////////////////////
void AddErrCnt(void)
{
	taskENTER_CRITICAL();           //进入临界区
	m_err_cnt++;
	taskEXIT_CRITICAL();            //退出临界区
}


uint8_t GetErrCnt(void)
{
	uint8_t _err_cnt = 0;
	
	taskENTER_CRITICAL();           //进入临界区
	_err_cnt = m_err_cnt;
	taskEXIT_CRITICAL();            //退出临界区
	
	return _err_cnt;
}


void ClrErrCnt(void)
{
	taskENTER_CRITICAL();           //进入临界区
	m_err_cnt = 0;
	taskEXIT_CRITICAL();            //退出临界区
}


void TxSystemStatus(uint8_t status)
{
	uint8_t _status = 0;
	uint8_t _system_stat2 = 0;
	
	_status = status;
	switch (_status) {
		case STATE_FLAGE_DEFAULT:
			if (TXparameter.system_stat2 == ERR_RDY_FUT) {
				vTaskDelay(10);
				HAL_GPIO_WritePin(GPIOC, FLAG_RST_Pin, GPIO_PIN_RESET);
				vTaskDelay(100);
				HAL_GPIO_WritePin(GPIOC, FLAG_RST_Pin, GPIO_PIN_SET);
			}
			TXparameter.led_stat = 0;
			TXparameter.system_stat2 = SYS_NORMAL;
			break;
		case STATE_FLAGE_BATT_FULL:
			m_err_cnt = 0;
			TXparameter.led_stat = 2;
			break;
		case STATE_FLAGE_ERROR:
			if( xTimerIsTimerActive( TemperatureAlarm_Handle ) != pdFALSE ) {
				// xTimer is active, do something.
				xTimerStop(TemperatureAlarm_Handle, 0);
			}
//			CLOSE_OPERATION();
			if (GetErrCnt() >= 3) {	
				TXparameter.led_stat = 3;	
			} else {
				if( xTimerIsTimerActive( ErrorStatus_Handle ) != pdFALSE ) {
					// xTimer is active, do something.
				} else {
					// xTimer is not active, do something else.
					AddErrCnt();
					if (GetErrCnt() < 3) {
						xTimerStart(ErrorStatus_Handle, 0);	
						TXparameter.led_stat = 0;					
					}
				}				
			}
			break;
		case STATE_FLAGE_CHARGING: // 恒压 恒流  充满
			_system_stat2 = TXparameter.system_stat2;
			if (_system_stat2 == 0) {
				TXparameter.led_stat = 1;        // 恒流
			} else if (_system_stat2 == 1) {
				TXparameter.led_stat = 5;        // 恒压
			} else {
				TXparameter.led_stat = 1;        // 默认给恒流
			}
			break;	
		case STATE_FLAGE_SUSPEND:
//			CLOSE_OPERATION();
			TXparameter.led_stat = 4;	
			if( xTimerIsTimerActive( TemperatureAlarm_Handle ) != pdFALSE ) {
				// xTimer is active, do something.
			} else {
				// xTimer is not active, do something else.
				xTimerStart(TemperatureAlarm_Handle, 0);				
			}			
			break;
	} 
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
//创建FaultHandler任务
void FaultHandler_task(void *pvParameters)
{
	while(1){

		Debug_Printf("AppFaultHandler!!!\r\n");
		vTaskDelay(200);
	
	}
	
	while(1)
	{
		TxSystemStatus(g_tx_system_stat);
//		Debug_Printf("%d,%d\r\nR_T:%d °C, R_V:%dV, R_I:%dA, TXparameter.system_stat2:%d\r\n",\
		TX_RX_status,PwmFreq, RXparameter.RX_T1, RXparameter.RX_Uout, RXparameter.RX_I, TXparameter.system_stat2);
//		Debug_Printf("ADDR:--------%.2x----%.2x------%.2x----%.2x-----%.2x \r\n",\
//		INIT_ADDR[0],INIT_ADDR[1],INIT_ADDR[2],INIT_ADDR[3],INIT_ADDR[4]);

		vTaskDelay(500);
	}
} 

