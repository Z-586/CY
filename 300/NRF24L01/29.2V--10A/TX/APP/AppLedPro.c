#include "main.h"


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void LED_Init(void);
uint8_t GetErrCnt(void);


//TimerHandle_t m_ble_blue_timer;        	    /**< Definition of battery timer. */
static TimerHandle_t m_io_led_timer;        	/**< Definition of battery timer. */
static TimerHandle_t m_ble_blue_timer;
static TimerHandle_t m_ble_red_timer;
static TimerHandle_t m_ble_green_timer;
static TimerHandle_t m_ble_green_red_timer;     /**< Definition of battery timer. */


//void IO_Led_Open(void)
//{
//	GPIO_ResetBits(GPIOC, IO_LED);
//	//nrf_gpio_pin_set(BLUE_LED);
//}

//void IO_Led_Close(void)
//{
//	GPIO_SetBits(GPIOC, IO_LED);
//	//nrf_gpio_pin_clear(BLUE_LED);
//}


//void IO_Led_Toggle(void)
//{
//	if (GPIO_ReadOutputDataBit(GPIOC, IO_LED)) {
//		GPIO_ResetBits(GPIOC, IO_LED);
//	} else {
//		GPIO_SetBits(GPIOC, IO_LED);
//	}
//	//nrf_gpio_pin_toggle(BLUE_LED);
//}


/**@brief Function for handling the Battery measurement timer time-out.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] xTimer Handler to the timer that called this function.
 *                   You may get identifier given to the function xTimerCreate using pvTimerGetTimerID.
 */
static void IO_Led_handler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
	//HAL_IWDG_Refresh(&hiwdg);

    //µÆÉÁË¸
	//IO_Led_Toggle();
}


void Ble_Blue_Open(void)
{
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
}

void Ble_Blue_Close(void)
{
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
}


void Ble_Blue_Toggle(void)
{	
	HAL_GPIO_TogglePin(LED_B_GPIO_Port,LED_B_Pin);
}


void Ble_Red_Open(void)
{
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
}

void Ble_Red_Close(void)
{
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
}


void Ble_Red_Toggle(void)
{
	HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin);
}

void Ble_Green_Open(void)
{
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
}

void Ble_Green_Close(void)
{
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
}


void Ble_Green_Toggle(void)
{
	HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
}


/**@brief Function for handling the Battery measurement timer time-out.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] xTimer Handler to the timer that called this function.
 *                   You may get identifier given to the function xTimerCreate using pvTimerGetTimerID.
 */
static void BleBlue_handler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
    //µÆÉÁË¸
	Ble_Blue_Toggle();
}


/**@brief Function for handling the Battery measurement timer time-out.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] xTimer Handler to the timer that called this function.
 *                   You may get identifier given to the function xTimerCreate using pvTimerGetTimerID.
 */
static void BleRed_handler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
    //µÆÉÁË¸
	Ble_Red_Toggle();
	//Debug_Printf("Ble_Red_Toggle\r\n");
}


/**@brief Function for handling the Battery measurement timer time-out.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] xTimer Handler to the timer that called this function.
 *                   You may get identifier given to the function xTimerCreate using pvTimerGetTimerID.
 */
static void BleGreen_handler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
    //µÆÉÁË¸
	Ble_Green_Toggle();
}


/**@brief Function for handling the Battery measurement timer time-out.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] xTimer Handler to the timer that called this function.
 *                   You may get identifier given to the function xTimerCreate using pvTimerGetTimerID.
 */
static void BleGreenRed_handler(TimerHandle_t xTimer)
{
	static uint8_t led_flag = 0;
	
    UNUSED_PARAMETER(xTimer);
	if (led_flag) {
		led_flag = 0;
		Ble_Red_Open();
		Ble_Green_Close();
	} else {
		led_flag = 1;
		Ble_Red_Close();
		Ble_Green_Open();
	}
}



/**@brief Function for handling the Battery measurement timer time-out.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] xTimer Handler to the timer that called this function.
 *                   You may get identifier given to the function xTimerCreate using pvTimerGetTimerID.
 */
//static void BlePeriodTimer_handler(TimerHandle_t xTimer)
//{
//    UNUSED_PARAMETER(xTimer);
//	
//	if (g_BleLedStat == BLE_DISCONNECT) {
//		m_dac_cnt++;
//		if (m_dac_cnt > 3) {
//			m_dac_cnt = 0;
//		}
//		switch (m_dac_cnt) {
//			case 0:
//				xTimerChangePeriod(m_ble_period_timer, 200, 0);
//				break;
//			case 1:

//				break;
//			case 2:

//				break;
//			case 3:
//				xTimerChangePeriod(m_ble_period_timer, 2000, 0);
//				break;
//		}
//	}
//}




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
void LedProc_task(void *pvParameters)
{
	unsigned portBASE_TYPE uxHighWaterMark;
	uint8_t _init_led_stat = 0xff;
	uint8_t _tx_system_stat = 0;
	uint8_t _led_stat = 0;

	m_io_led_timer 		    = xTimerCreate("IO_LED",  	         500,  pdTRUE,  NULL, IO_Led_handler);
	m_ble_blue_timer  		= xTimerCreate("BLE_BLUE_LED",   	1000,  pdTRUE,  NULL, BleBlue_handler);
	m_ble_red_timer         = xTimerCreate("BLE_RED_LED",   	 200,  pdTRUE,  NULL, BleRed_handler);
	m_ble_green_timer       = xTimerCreate("BLE_GREEN_LED",   	1000,  pdTRUE,  NULL, BleGreen_handler);
	m_ble_green_red_timer   = xTimerCreate("GREEN_RED_LED",   	1000,  pdTRUE,  NULL, BleGreenRed_handler);
	
	if(pdPASS != xTimerStart(m_io_led_timer, 0)) {
		//APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}
//	APPL_LOG("LedProc_task Creating...\r\n");
	uxHighWaterMark = uxTaskGetStackHighWaterMark( LedProcTask_Handler );
	Debug_Printf("LedProcTask_Handler:%ld.\r\n", uxHighWaterMark);
	Debug_Printf("_DATE_ is:%s\r\n", __DATE__);
	Debug_Printf("_TIME_ is:%s\r\n", __TIME__);
	while(1)
	{ 
		taskENTER_CRITICAL();  			//½øÈëÁÙ½çÇø
		_tx_system_stat = g_tx_system_stat;
		_led_stat       = TXparameter.led_stat;
		taskEXIT_CRITICAL();            //ÍË³öÁÙ½çÇø
		Debug_Printf("GetErrCnt:%d.\r\n", GetErrCnt());
		Debug_Printf("TXparameter.led_stat:%d.\r\n", _led_stat);
		Debug_Printf("Init_led_stat:%d.\r\n", _init_led_stat);
		Debug_Printf("g_tx_system_stat:%d.\r\n", _tx_system_stat);
		if (_init_led_stat != _led_stat) {
			_init_led_stat = _led_stat;
			switch (_led_stat) {
				case 0: //
					xTimerStop(m_ble_red_timer, 0);
					xTimerStop(m_ble_green_timer, 0);
					xTimerStop(m_ble_green_red_timer, 0);
					Ble_Red_Close();
					Ble_Green_Close();
					if(pdPASS != xTimerStart(m_ble_blue_timer, 0)) {
						//APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
					}				
					break;
				case 1: //				
					xTimerStop(m_ble_blue_timer, 0);
					xTimerStop(m_ble_red_timer, 0);
					xTimerStop(m_ble_green_timer, 0);
					xTimerStop(m_ble_green_red_timer, 0);
					Ble_Blue_Close();
					Ble_Green_Close();
					Ble_Red_Open();
					break;
				case 2: //
					xTimerStop(m_ble_blue_timer, 0);
					xTimerStop(m_ble_red_timer, 0);
					xTimerStop(m_ble_green_timer, 0);
					xTimerStop(m_ble_green_red_timer, 0);
					Ble_Blue_Close();
					Ble_Red_Close();
					Ble_Green_Open();
					break;
				case 3: //
					xTimerStop(m_ble_blue_timer, 0);
					xTimerStop(m_ble_green_timer, 0);
					xTimerStop(m_ble_green_red_timer, 0);
					Ble_Blue_Close();
					Ble_Green_Close();
					if(pdPASS != xTimerStart(m_ble_red_timer, 0)) {
						//APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
					}
					break;
				case 4: //
					xTimerStop(m_ble_blue_timer, 0);
					xTimerStop(m_ble_red_timer, 0);
					xTimerStop(m_ble_green_timer, 0);
					Ble_Blue_Close();
					if(pdPASS != xTimerStart(m_ble_green_red_timer, 0)) {
						//APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
					}
					break;
				case 5: //
					xTimerStop(m_ble_blue_timer, 0);
					xTimerStop(m_ble_red_timer, 0);
					xTimerStop(m_ble_green_red_timer, 0);
					Ble_Blue_Close();
					Ble_Red_Close();
					if(pdPASS != xTimerStart(m_ble_green_timer, 0)) {
						//APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
					}
					break;
			}
		}
		vTaskDelay(500);
	}
}  
