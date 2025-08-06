#ifndef __AT24C02_H
#define __AT24C02_H

void AT24C02_Init(void);
void at24cxx_write_byte(uint8_t reg, int8_t *pdata, uint16_t size);
void at24cxx_read_byte(uint8_t reg, int8_t *pdata, uint16_t size);
uint8_t at24c02_check(void);
void AT24C02_WriteReg(uint8_t RegAddress, uint8_t Data);

#endif
