#ifndef Delay_H__
#define Delay_H__

#include "cms32f033.h"
#include "stdio.h"

void delay_init(void);
void delay_us(uint32_t nus);
void delay_xms(uint32_t nms);


#endif
