#include "includes.h"


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
void TestLed_Open(void)
{
	//nrf_gpio_pin_clear(TEST_LED);
}

void TestLed_Close(void)
{
	//nrf_gpio_pin_set(TEST_LED);
}

void TestLed_Toggle(void)
{
	//nrf_gpio_pin_toggle(TEST_LED);
}


//创建Test任务
void Test_task(void *pvParameters)
{
	uint32_t cnt = 0;
	APPL_LOG("Test_task Creating...\r\n");
	
	while(1)
	{
		//nrf_drv_wdt_channel_feed(g_channel_id);  //喂狗操作 
		APPL_LOG("Cnt:%d\r\n", cnt);
		cnt++;
		vTaskDelay(3600000);//1h
	}
} 





//创建LED2任务
//void led2_task(void *pvParameters)
//{
//	APPL_LOG("LED2 CREATE...\r\n");
//	while(1)
//	{
//		//APPL_LOG("LED2...\r\n");
//		LED2_Open();
//		vTaskDelay(500);
//		LED2_Close();
//		vTaskDelay(500);
//	}
//}  

