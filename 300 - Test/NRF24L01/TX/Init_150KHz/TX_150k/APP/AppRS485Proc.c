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
uint16_t TX_Buf[4] = {0,0,0,0};
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

#define FLASH_SAVE_ADDR ((uint32_t)0X0800F800 )//0X08020000 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)


/**
* @brief 
*/
void SoftReset(void)
{
	__disable_irq(); // 关闭所有中端
	NVIC_SystemReset(); // 复位
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
	RS485_Addr          = 255;
	baud                = 6;	
	
	RX_Iout   =  RXparameter.RX_I;
	RX_Uout_H = (RXparameter.RX_Uout >> 16);
	RX_Uout_L = (RXparameter.RX_Uout & 0xFFFF);
	TX_Buf[0] = RS485_Addr;
	TX_Buf[1] = baud;	
	
	STMFLASH_Read(FLASH_SAVE_ADDR, (uint16_t*)TX_Buf, 3);
	eeprom_flag = TX_Buf[2];
	if (eeprom_flag == 0xffff) {
		RS485_Addr = 255;
		baud = 6;
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
		USART5_CommSendAtStr(Buffer_Tx.buf, RegNum*2+5);
		//drv_uart_tx(Buffer_Tx.buf, RegNum*2+5);
	} else { //?????+??????.
		Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
		Buffer_Tx.buf[1] = (Buffer_Rx.buf[1]|0x80);
		Buffer_Tx.buf[2] = 0x02; //error code.
		USART5_CommSendAtStr(Buffer_Tx.buf, 3);
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
	USART5_CommSendAtStr(Buffer_Tx.buf, 8);
	//drv_uart_tx(Buffer_Tx.buf, 8);
}


void RS485_Service(void)
{
	uint16_t recCRC;
	
	if (Buffer_Rx.buf[0] == RS485_Addr) { //
		if((Buffer_Rx.buf[1] == 03) || (Buffer_Rx.buf[1] == 06)) {
			startRegAddr = ((((uint16_t)Buffer_Rx.buf[2])<<8) | Buffer_Rx.buf[3]);//
			if(startRegAddr < 1000) {
				calCRC = usCRC16(Buffer_Rx.buf, Buffer_Rx.len-2);//
				recCRC = Buffer_Rx.buf[Buffer_Rx.len-2]|(((uint16_t)Buffer_Rx.buf[Buffer_Rx.len-1])<<8);//
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
					USART5_CommSendAtStr(Buffer_Tx.buf, 3);
					//drv_uart_tx(Buffer_Tx.buf, 3);
					return;
				}        
			} else { //
				Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
				Buffer_Tx.buf[1] = Buffer_Rx.buf[1]|0x80;
				Buffer_Tx.buf[2] = 0x02; //error code
				USART5_CommSendAtStr(Buffer_Tx.buf, 3);
				//drv_uart_tx(Buffer_Tx.buf, 3);
				return;
			}                                                
		} else { //
			Buffer_Tx.buf[0] = Buffer_Rx.buf[0];
			Buffer_Tx.buf[1] = Buffer_Rx.buf[1]|0x80;
			Buffer_Tx.buf[2] = 0x01; //error code
			USART5_CommSendAtStr(Buffer_Tx.buf, 3);
			//drv_uart_tx(Buffer_Tx.buf, 3);
			return;
		}
		if ((startRegAddr == 0x0005) && (Buffer_Rx.buf[1] == 06)) {
			TX_Buf[2] = 0xAABB;
			baud = TX_Buf[1];
			STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t*)TX_Buf, 3);
			SoftReset();
		} else if ((startRegAddr == 0x0006) && (Buffer_Rx.buf[1] == 06)) {
			TX_Buf[2] = 0xAAAA;
			RS485_Addr = TX_Buf[0];
			STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t*)TX_Buf, 3);
		} 
	} else {
		return;
	}                         
}


uint32_t Setting_baud(uint8_t no)
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
			baud_32 = 115200;
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
	Modbus_RegMap();
	MX_UART5_Init(Setting_baud(baud));
	//Uart_Debug_Init(Setting_baud(baud));
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




