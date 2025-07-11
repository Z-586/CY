#ifndef _RGB_H
#define _RGB_H

#include "stm32f4xx.h"

#define 	RGB_LED 	     GPIO_PIN_4
#define		RGB_LED_HIGH     HAL_GPIO_WritePin(GPIOA, RGB_LED, GPIO_PIN_SET )
#define 	RGB_LED_LOW		 HAL_GPIO_WritePin(GPIOA, RGB_LED, GPIO_PIN_RESET )

void RGB_LED_Init(void);
void RGB_LED_Write0(void);
void RGB_LED_Write1(void);
void RGB_LED_Reset(void);
void RGB_LED_Write_Byte(uint8_t byte);
void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue);
void RGB_LED_Write_24Bits_t(uint8_t green,uint8_t red,uint8_t blue,uint8_t t);
void RGB_LED_Red(uint8_t t);
void RGB_LED_Green(uint8_t t);
void RGB_LED_Blue(uint8_t t);
void RGB_LED_YHONG(uint8_t t);

#endif

