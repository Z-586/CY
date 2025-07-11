#include "includes.h"

static uint8_t rx_status = 0;
static uint8_t rx_buf[10];
static uint8_t tx_buf[10];
static uint32_t count_time = 0;
static uint8_t Stop_state = 0;
static uint16_t count_delay_time = 0;
extern unsigned char INIT_ADDR[5]; //节点地址
//static const unsigned char Iint_ADDR[5]= {0x00,0x1A,0xB1,0xB1,0x01}; // //默认节点地址

uint8_t tx_status = 0;
uint8_t tx_time_status = 0;
static uint16_t rx_error_time = 0;

static TimerHandle_t m_parse_date_timer;        	/**< Definition of battery timer. */
float Rx_Temp = 0;
uint16_t CRC_Value = 0;
uint8_t NRF_IS_OK_STATE = 0;
static uint8_t STM32_ID[12] ={0};
uint8_t TX_RX_status = 0;
static uint8_t pwm_start_flag = 0;
static const unsigned char Iint_ADDR[5]= {0x00,0x1A,0xB1,0xB1,0x01}; // //默认节点地址
void EfficiencyCalculation(void);
void Adc_deal(void);

void Reset_Variable(void)
{
	RXparameter.RX_I = 0;
	RXparameter.RX_Uout = 0;
	RXparameter.RX_Uin = 0;
	RXparameter.RX_T1 = 0;
	TXparameter.efficiency = 0;
	
	TXparameter.system_stat1 = 0;
//	GPIO_ResetBits(GPIOC, SWITCH_IO); 
}


/**@brief Function for handling the Battery measurement timer time-out.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] xTimer Handler to the timer that called this function.
 *                   You may get identifier given to the function xTimerCreate using pvTimerGetTimerID.
 */
static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};


static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

uint16_t usCRC16( uint8_t * pucFrame, uint16_t usLen )
{
    uint8_t         ucCRCHi = 0xFF;
    uint8_t         ucCRCLo = 0xFF;
    int             iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( uint8_t )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}


static void parse_date_handler(TimerHandle_t xTimer)
{
	if(TX_RX_status == 1){
		CRC_Value = usCRC16(rx_buf,8);
		if(CRC_Value == ((((uint16_t)rx_buf[8])<<8)|rx_buf[9])) {
			RXparameter.RX_Uout = ((((uint16_t)rx_buf[4])<<8)|rx_buf[5]);
			RXparameter.RX_I = ((((uint16_t)rx_buf[6])<<8)|rx_buf[7]);
			RXparameter.RX_T1  = (((uint16_t)rx_buf[2])<<8)|rx_buf[3];
		}
		Debug_Printf("%d,%d\r\nR_T:%d °C, R_V:%dV, R_I:%dA, TXparameter.system_stat2:%d\r\n",\
		TX_RX_status,PwmFreq, RXparameter.RX_T1, RXparameter.RX_Uout, RXparameter.RX_I, TXparameter.system_stat2);
	}
}

uint32_t NRF_ID(const uint8_t *data, uint8_t length) {
    uint32_t hash = 0;
    for (uint8_t i = 0; i < length; i++) {
        hash = (hash << 4) + data[i];
        uint32_t g = hash & 0xF0000000;
        if (g != 0) {
            hash ^= g >> 24;
            hash ^= g;
        }
    }
    return hash;
}


void Get_CPU_ID(void){
	//获取CPU唯一ID
	uint32_t chipId[4] = {0};		
	chipId[0] =*(volatile unsigned long *)(0x1fff7590); //按全字（32位）读取
	chipId[1] =*(volatile unsigned long *)(0x1fff7594);
	chipId[2] =*(volatile unsigned long *)(0x1fff7598);
	
	STM32_ID[0] = (uint8_t)(chipId[0] & 0x000000FF);
	STM32_ID[1] = (uint8_t)((chipId[0] & 0xFF00) >>8);
	STM32_ID[2] = (uint8_t)((chipId[0] & 0xFF0000) >>16);
	STM32_ID[3] = (uint8_t)((chipId[0] & 0xFF000000) >>24);
	
	STM32_ID[4] = (uint8_t)(chipId[1] & 0xFF);
	STM32_ID[5] = (uint8_t)((chipId[1] & 0xFF00) >>8);
	STM32_ID[6] = (uint8_t)((chipId[1] & 0xFF0000) >>16);
	STM32_ID[7] = (uint8_t)((chipId[1] & 0xFF000000) >>24);
	
	STM32_ID[8] = (uint8_t)(chipId[2] & 0xFF);
	STM32_ID[9] = (uint8_t)((chipId[2] & 0xFF00) >>8);
	STM32_ID[10] = (uint8_t)((chipId[2] & 0xFF0000) >>16);
	STM32_ID[11] = (uint8_t)((chipId[2] & 0xFF000000) >>24);
	chipId[3] = NRF_ID(STM32_ID,12);
	
	INIT_ADDR[1] = (uint8_t)(chipId[3] & 0x000000FF);
	INIT_ADDR[2] = (uint8_t)((chipId[3] & 0xFF00) >>8);
	INIT_ADDR[3] = (uint8_t)((chipId[3] & 0xFF0000) >>16);
	INIT_ADDR[4] = (uint8_t)((chipId[3] & 0xFF000000) >>24);
	memcpy(tx_buf,INIT_ADDR,5);
	tx_buf[8] = usCRC16(tx_buf,8)>>8;
	tx_buf[9] = usCRC16(tx_buf,8);
}

void BlePro_task(void *pvParameters)
{	
	m_parse_date_timer = xTimerCreate("parse_date", 500, pdTRUE, NULL, parse_date_handler);
	if(pdPASS != xTimerStart(m_parse_date_timer, 0)){}
	NRF24L01_Init();
	//检测nRF24L01
	if(NRF24L01_Check( ) == 0){
		NRF_IS_OK_STATE = 1;
		Debug_Printf("NRF24L01  is  ok!\n\r");
	}
	else{
		Debug_Printf("NRF24L01  is  error!\n\r");
		g_tx_system_stat = STATE_FLAGE_ERROR;
		vTaskDelay(1000);
		if(NRF24L01_Check( ) == 0){
			NRF_IS_OK_STATE = 1;
		}
		else{
			g_tx_system_stat = STATE_FLAGE_ERROR;
			vTaskDelay(5000);
			if(NRF24L01_Check( ) == 0)
				NRF_IS_OK_STATE = 1;
			else{
				g_tx_system_stat = STATE_FLAGE_ERROR;
				TXparameter.system_stat2 = ERR_NRF24L01;
				vTaskDelay(5000);
			}
		}
	}
	TX_Mode(0);		
	vTaskDelay(2000);
	PwmFreq = Set_140K;	
	while(1)
	{
		if (TXparameter.power_on == 1) {
			if(pwm_start_flag == 0){
				Start_PWM();
				for (int i = 1; i < 46; i++) {
					hhrtim1.Instance->sTimerxRegs[0].PERxR = PwmFreq;
					hhrtim1.Instance->sTimerxRegs[0].CMP1xR = (PwmFreq * (100 - i)) / 100;
					hhrtim1.Instance->sTimerxRegs[0].CMP3xR = (PwmFreq * i) / 100;
					vTaskDelay(20);
				}
				Get_CPU_ID();
				pwm_start_flag = 1;
			}
			if(TX_RX_status == 0){
				if(Stop_state == 0){
					tx_status = NRF24L01_TxPacket(tx_buf);
					if(tx_status == 0X20)  //NRF24L01模块发送数据并判断是否发送成功
					{
						RX_Mode(0);	
						TX_RX_status = 1;
						g_tx_system_stat = STATE_FLAGE_CHARGING;
						vTaskDelay(10);
					}else{
						if(PwmFreq == Set_140K)
							count_time++;
						if(count_time % 500 == 0){
							if(PwmFreq < Set_100K){
								PwmFreq = PwmFreq + 10;
								hhrtim1.Instance->sTimerxRegs[0].PERxR = PwmFreq;
								hhrtim1.Instance->sTimerxRegs[0].CMP1xR = (PwmFreq * (100 - 46)) / 100;
								hhrtim1.Instance->sTimerxRegs[0].CMP3xR = (PwmFreq * 46) / 100;
							}else{
								count_delay_time++;
								if(count_delay_time % 200 == 0){
									count_time = 0;
									count_delay_time = 0;
									Stop_state = 1;
									Stop_PWM();
								}								
							}			
						}
					}
				}
			}else{
				rx_status = NRF24L01_RxPacket(rx_buf);
				if(rx_status == 0x00)  //NRF24L01模块接收数据并判断是否接收成功
				{
					rx_error_time = 0;
					if (rx_buf[0] == 0) {
						// 减小频率,增加功率
						if (PwmFreq < Set_88K) {
							PwmFreq = PwmFreq + 1;
						}else{
							PwmFreq = Set_88K;
						}	
					} else if (rx_buf[0] == 1){
						// 增大频率,减小功率
						if (PwmFreq > Set_140K) {
							PwmFreq = PwmFreq - 1;
						}else{
							PwmFreq = Set_140K;
						}	
					}else if (rx_buf[0] == 2) {
						// 减小频率,增加功率
						if (PwmFreq < Set_88K) {
							PwmFreq = PwmFreq + 5;
						}else{
							PwmFreq = Set_88K;
						}	
					}
					hhrtim1.Instance->sTimerxRegs[0].PERxR = PwmFreq;
					hhrtim1.Instance->sTimerxRegs[0].CMP1xR = (PwmFreq * (100 - 46)) / 100;
					hhrtim1.Instance->sTimerxRegs[0].CMP3xR = (PwmFreq * 46) / 100;
					TXparameter.system_stat2 = rx_buf[1];
					if (TXparameter.system_stat2 == SYS_BATT_FULL) {
						g_tx_system_stat = STATE_FLAGE_BATT_FULL;
					} else if ((TXparameter.system_stat2 == ERR_I_Max)||(TXparameter.system_stat2 == ERR_V_Max)) {
						g_tx_system_stat = STATE_FLAGE_ERROR;
					} else if ((TXparameter.system_stat2 == SYS_CC) || (TXparameter.system_stat2 == SYS_CV)) {
						g_tx_system_stat = STATE_FLAGE_CHARGING;
					} else if (TXparameter.system_stat2 == PEND_RX_TEMPER) {
						g_tx_system_stat = STATE_FLAGE_SUSPEND;
					}
				}
				rx_error_time++;
				if(rx_error_time % 1000 == 0){
					PwmFreq = Set_140K;
					Stop_PWM();
					Reset_Variable();
					pwm_start_flag = 0;
					memcpy(INIT_ADDR,Iint_ADDR,5);
					memcpy(INIT_ADDR,Iint_ADDR,5);
					TX_Mode(0);			
					TX_RX_status = 0;
					count_delay_time = 0;
					count_time = 0;
					Stop_state = 0;
					g_tx_system_stat = STATE_FLAGE_DEFAULT;
					vTaskDelay(1000);
				}
			}
			vTaskDelay(1);
		} else {
			if(pwm_start_flag == 1){
				pwm_start_flag = 0;
				memcpy(INIT_ADDR,Iint_ADDR,5);
				memcpy(INIT_ADDR,Iint_ADDR,5);
				TX_Mode(0);			
				TX_RX_status = 0;
				count_delay_time = 0;
				count_time = 0;
				Stop_state = 0;
			}
			vTaskDelay(100);
		}
	}
}  

