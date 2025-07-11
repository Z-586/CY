#include "RGB.h"
#include "delay.h"



void RGB_LED_Write0(void)
{
	RGB_LED_HIGH;
//	__nop();
//	for(i=0;i<num;i++)
//	{
//		
//	}
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();

	RGB_LED_LOW;
//	for(i=0;i<num1;i++)
//	{
//		__nop();
//	}
	//delay_us(1);
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	/*__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();*/
}

void RGB_LED_Write1(void)
{
	for(i=0;i<num1;i++)
	{
		__nop();
	}
	//delay_us(1);
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	/*__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();*/
	//__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	//__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	//__nop();__nop();
	RGB_LED_LOW;
	for(i=0;i<num;i++)
	{
		__nop();
	}
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
}

void RGB_LED_Reset(void)
{
	RGB_LED_LOW;
	delay_us(400);
}

void RGB_LED_Write_Byte(uint8_t byte)
{
	uint8_t i;

	for(i=0;i<8;i++)
		{
			if(byte&0x80)
				{
					RGB_LED_Write1();
			}
			else
				{
					RGB_LED_Write0();
			}
		byte <<= 1;
	}
}

void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue)
{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
}

void RGB_LED_Write_24Bits_t(uint8_t green,uint8_t red,uint8_t blue,uint8_t t)
{
	uint8_t i;
	for(i=0;i<t;i++)
	{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
	delay_us(1);
	}
}

void RGB_LED_Red(uint8_t t)
{
	 uint8_t i;
	for(i=0;i<t;i++)
		{
			RGB_LED_Write_24Bits(0, 0xff, 0);
	}
}

void RGB_LED_Green(uint8_t t)
{
	uint8_t i;

	for(i=0;i<t;i++)
		{
			RGB_LED_Write_24Bits(0xff, 0, 0);
	}
}

void RGB_LED_Blue(uint8_t t)
{
	uint8_t i;

	for(i=0;i<t;i++)
		{
			RGB_LED_Write_24Bits(0, 0, 0xff);
	}
}

void RGB_LED_YHONG(uint8_t t)
{
	uint8_t i;

	for(i=0;i<t;i++)
		{
			RGB_LED_Write_24Bits(0, 0xff, 0xff);
	}
}
