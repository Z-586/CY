#include "includes.h"


/*******************************************************************************
*
*       Static typedefs & defines & misc
*
********************************************************************************
*/
//uint32_t rx_u = 0;
uint16_t RX_Uout_H = 0;
uint16_t RX_Uout_L = 0;
uint16_t RX_Iout   = 0;
uint16_t RX_Power  = 0;
uint16_t TX_Buf[3] = {0,0,0};
uint16_t TX_Status = 0;
uint16_t eeprom_flag = 0;

void CommRcvTimeOutCallback(TimerHandle_t xTimer);
void Comm485TimeOutCallback(TimerHandle_t xTimer);
void BleConnectTimeOutCallback(TimerHandle_t xTimer);


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
R_Buffer Buffer_Rx;
T_Buffer Buffer_Tx;
uint16_t startRegAddr;
uint16_t RegNum;
uint16_t calCRC;
uint16_t *Modbus_HoldReg[10];//???????.

#define FLASH_SAVE_ADDR  0X0800FC00  //0X08020000 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

/**
* @brief 
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


void SoftReset(void)
{
	__set_FAULTMASK(1); // 关闭所有中端
	NVIC_SystemReset(); // 复位
}


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


void Modbus_RegMap(void)
{
	//?????????.
	Modbus_HoldReg[0] 	= (uint16_t *)&RX_Iout;					// RX_Iout
	Modbus_HoldReg[1] 	= (uint16_t *)&RX_Uout_H;				// RX_Uout_H
	Modbus_HoldReg[2] 	= (uint16_t *)&RX_Uout_L;				// RX_Uout_L 
	Modbus_HoldReg[3] 	= (uint16_t *)&RX_Power;				// RX_Power 
	Modbus_HoldReg[4] 	= (uint16_t *)&TX_Status;	            // status
	Modbus_HoldReg[5] 	= (uint16_t *)&TX_Buf[1];	            // baud
	Modbus_HoldReg[6] 	= (uint16_t *)&TX_Buf[0];	            // addr_485
	
	RXparameter.RX_I    = 0;
	RXparameter.RX_Uout = 0;
	RX_Power            = 0;
	RS485_Addr          = 1;
	baud                = 2;	
	
	RX_Iout   =  RXparameter.RX_I;
	RX_Uout_H = (RXparameter.RX_Uout >> 16);
	RX_Uout_L = (RXparameter.RX_Uout & 0xFFFF);
	TX_Buf[0] = RS485_Addr;
	TX_Buf[1] = baud;	
	
	STMFLASH_Read(FLASH_SAVE_ADDR, (u16*)TX_Buf, 3);
	eeprom_flag = TX_Buf[2];
	if (eeprom_flag == 0xffff) {
		RS485_Addr = 1;
		baud = 2;
	} else {
		RS485_Addr = TX_Buf[0];
		baud = TX_Buf[1];	
	}
}



//Modbus???03????
//??????
void Modbus_03_Solve(void)
{
	RegNum = ((((uint16_t)Buffer_Rx.buf[4])<<8) | Buffer_Rx.buf[5]);//???????.
	if((startRegAddr + RegNum) < 1000) { //?????+??????.
		Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
		Buffer_Tx.buf[1] = Buffer_Rx.buf[1];
		Buffer_Tx.buf[2] = RegNum*2;
		for(int i = 0;i < RegNum; i++) {
			Buffer_Tx.buf[3+i*2] = ((*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF); 	//?????.
			Buffer_Tx.buf[4+i*2] = (*Modbus_HoldReg[startRegAddr+i]&0xFF);			//?????.
		}
		calCRC = usCRC16(Buffer_Tx.buf, RegNum*2+3);
		Buffer_Tx.buf[RegNum*2+3] = calCRC&0xFF;
		Buffer_Tx.buf[RegNum*2+4] = (calCRC>>8)&0xFF;
		USART2_CommSendAtStr(Buffer_Tx.buf, RegNum*2+5);
		//drv_uart_tx(Buffer_Tx.buf, RegNum*2+5);
	} else { //?????+??????.
		Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
		Buffer_Tx.buf[1] = (Buffer_Rx.buf[1]|0x80);
		Buffer_Tx.buf[2] = 0x02; //error code.
		USART2_CommSendAtStr(Buffer_Tx.buf, 3);
		//drv_uart_tx(Buffer_Tx.buf, 3);
	}
}


//Modbus???06????
//????????
void Modbus_06_Solve(void)
{
	*Modbus_HoldReg[startRegAddr]  = ((uint16_t)Buffer_Rx.buf[4])<<8;			//?????.
	*Modbus_HoldReg[startRegAddr] |= Buffer_Rx.buf[5];							//?????.

	Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
	Buffer_Tx.buf[1] = Buffer_Rx.buf[1];
	Buffer_Tx.buf[2] = Buffer_Rx.buf[2];
	Buffer_Tx.buf[3] = Buffer_Rx.buf[3];
	Buffer_Tx.buf[4] = Buffer_Rx.buf[4];
	Buffer_Tx.buf[5] = Buffer_Rx.buf[5];

	calCRC = usCRC16(Buffer_Tx.buf, 6);
	Buffer_Tx.buf[6] = (calCRC&0xFF);
	Buffer_Tx.buf[7] = ((calCRC>>8)&0xFF);
	USART2_CommSendAtStr(Buffer_Tx.buf, 8);
	//drv_uart_tx(Buffer_Tx.buf, 8);
}


void RS485_Service(void)
{
	u16 recCRC;
	
	if (Buffer_Rx.buf[0] == RS485_Addr) { //
		if((Buffer_Rx.buf[1] == 03) || (Buffer_Rx.buf[1] == 06)) {
			startRegAddr = ((((u16)Buffer_Rx.buf[2])<<8) | Buffer_Rx.buf[3]);//
			if(startRegAddr < 1000) {
				calCRC = usCRC16(Buffer_Rx.buf, Buffer_Rx.len-2);//
				recCRC = Buffer_Rx.buf[Buffer_Rx.len-2]|(((u16)Buffer_Rx.buf[Buffer_Rx.len-1])<<8);//
				if(calCRC == recCRC)//CRC
				{
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					switch(Buffer_Rx.buf[1])
					{
						case 03:
						{
							Modbus_03_Solve();
							break;
						}
						case 06:
						{
							Modbus_06_Solve();
							break;
						}
					}
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				} else { //CRC
					Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
					Buffer_Tx.buf[1] = Buffer_Rx.buf[1]|0x80;
					Buffer_Tx.buf[2] = 0x04; //error code
					USART2_CommSendAtStr(Buffer_Tx.buf, 3);
					//drv_uart_tx(Buffer_Tx.buf, 3);
					return;
				}        
			} else { //
				Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
				Buffer_Tx.buf[1] = Buffer_Rx.buf[1]|0x80;
				Buffer_Tx.buf[2] = 0x02; //error code
				USART2_CommSendAtStr(Buffer_Tx.buf, 3);
				//drv_uart_tx(Buffer_Tx.buf, 3);
				return;
			}                                                
		} else { //
			Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
			Buffer_Tx.buf[1] = Buffer_Rx.buf[1]|0x80;
			Buffer_Tx.buf[2] = 0x01; //error code
			USART2_CommSendAtStr(Buffer_Tx.buf, 3);
			//drv_uart_tx(Buffer_Tx.buf, 3);
			return;
		}
		if ((startRegAddr == 0x0005) && (Buffer_Rx.buf[1] == 06)) {
			TX_Buf[2] = 0xAAAA;
			baud = TX_Buf[1];
			STMFLASH_Write(FLASH_SAVE_ADDR, (u16*)TX_Buf, 3);
			SoftReset();
		} else if ((startRegAddr == 0x0006) && (Buffer_Rx.buf[1] == 06)) {
			TX_Buf[2] = 0xAAAA;
			RS485_Addr = TX_Buf[0];
			STMFLASH_Write(FLASH_SAVE_ADDR, (u16*)TX_Buf, 3);
		} 
	} else {
		return;
	}                         
}


u32 Setting_baud(uint8_t no)
{
	uint32_t baud_32 = 0;
	
	switch (no) {
		case 0:
			baud_32 = 2400;
			break;
		case 1:
			baud_32 = 4800;
			break;
		case 2:
			baud_32 = 9600;
			break;
		case 3:
			baud_32 = 19200;
			break;
		case 4:
			baud_32 = 38400;
			break;
		case 5:
			baud_32 = 57600;
			break;
		case 6:
			baud_32 = 115200;
			break;
		default:
			baud_32 = 9600;
			break;			
	}
	return baud_32;
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
void RS485_Proc_task(void *pvParameters)
{	
	unsigned portBASE_TYPE uxHighWaterMark;
	
	APPL_LOG("RS485_Proc_task Creating...\r\n");	
	uxHighWaterMark = uxTaskGetStackHighWaterMark( BleCommTask_Handler );
	APPL_LOG("RS485ProcTask_Handler:%ld.\r\n", uxHighWaterMark);
	
	Modbus_RegMap();
	Uart_Debug_Init(Setting_baud(baud));
	while(1)
	{
		xSemaphoreTake(RS485DataSemaphore, portMAX_DELAY);
	//	RXparameter.RX_Uout = 50000;
	//	RXparameter.RX_I = 10000;
		taskENTER_CRITICAL();           //进入临界区
		RX_Uout_H = (RXparameter.RX_Uout >> 16);
		RX_Uout_L = (RXparameter.RX_Uout & 0xFFFF);
		RX_Iout = RXparameter.RX_I;
		RX_Power = RXparameter.RX_Uout*RXparameter.RX_I/1000000;
		if (g_tx_system_stat == STATE_FLAGE_BATT_FULL) {
			TX_Status = SYS_BATT_FULL;
		} else {
			TX_Status = TXparameter.system_stat2;
		}
		taskEXIT_CRITICAL();            //退出临界区
		RS485_Service();
		memset(Buffer_Rx.buf, 0, UART1_BUF_LENGTH);
		Buffer_Rx.len = 0;  
	}
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{  
	BaseType_t xHigherPriorityTaskWoken;
	unsigned char ch;
	
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET) {	   			//??????
		USART_ReceiveData(USART2); 
	} 

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {		   		//USART????
        ch = (USART_ReceiveData(USART2));
		if(Buffer_Rx.len > 128){		   //????????
			return;
		}
		Buffer_Rx.buf[Buffer_Rx.len++] = ch;
	} else if(USART_GetFlagStatus(USART2, USART_FLAG_IDLE) != RESET) {		//USART????
		USART_ReceiveData(USART2);
		xSemaphoreGiveFromISR(RS485DataSemaphore, &xHigherPriorityTaskWoken);		//????
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);								//??????????????
	}		
}
