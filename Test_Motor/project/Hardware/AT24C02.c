#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "Delay.h"

#define AT24C02_ADDRESS		0xA0

void AT24C02_Init(void)
{
	MyI2C_Init();
}

void AT24C02_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
	Delay_ms(5);
}

void at24cxx_write_byte(uint8_t reg, int8_t *pdata, uint16_t size)
{
    uint16_t i;
    for(i = 0; i < size; i++)
        AT24C02_WriteReg(reg+i, pdata[i]);
}

uint8_t AT24C02_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

void at24cxx_read_byte(uint8_t reg, int8_t *pdata, uint16_t size)
{
    uint8_t i;
    /* 接收数据 */
    for(i = 0; i < size; i++)
    {
        pdata[i] = AT24C02_ReadReg(reg+i);
    }
}

uint8_t at24c02_check(void)
{
	uint8_t temp;
	temp=AT24C02_ReadReg(255);
	if(temp==0X55)return 0;		   
	else
	{
		AT24C02_WriteReg(255,0X55);
	    temp=AT24C02_ReadReg(255);	  
		if(temp==0X55)return 0;
	}
	return 1;		
}