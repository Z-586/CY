/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   SPI配置H文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

#include "main.h"

#define 	__RF24L01_TX_TEST__							//**@@ 如果测试发送功能则需要定义该宏，如果测试接收则需要屏蔽该宏 **@@//


//SPI引脚定义
#define SPI_CLK_GPIO_PORT			GPIOB
#define SPI_CLK_GPIO_PIN			GPIO_PIN_7

#define SPI_MISO_GPIO_PORT			GPIOB
#define SPI_MISO_GPIO_PIN			GPIO_PIN_5

#define SPI_MOSI_GPIO_PORT			GPIOB
#define SPI_MOSI_GPIO_PIN			GPIO_PIN_6

#define SPI_NSS_GPIO_PORT			GPIOB
#define SPI_NSS_GPIO_PIN			GPIO_PIN_9

#define spi_set_nss_high( )			HAL_GPIO_WritePin(SPI_NSS_GPIO_PORT,  SPI_NSS_GPIO_PIN, GPIO_PIN_SET)
#define spi_set_nss_low( )			HAL_GPIO_WritePin(SPI_NSS_GPIO_PORT,  SPI_NSS_GPIO_PIN, GPIO_PIN_RESET)	
	
#ifdef __USE_SOFT_SPI_INTERFACE__			/** 只有使用软件SPI才需要的封装 */			

#define spi_set_clk_high( )			HAL_GPIO_WritePin(SPI_CLK_GPIO_PORT,  SPI_CLK_GPIO_PIN, GPIO_PIN_SET)						//时钟置高
#define spi_set_clk_low( )			HAL_GPIO_WritePin(SPI_CLK_GPIO_PORT,  SPI_CLK_GPIO_PIN, GPIO_PIN_RESET)	//时钟置低

#define spi_set_mosi_hight( )		HAL_GPIO_WritePin(SPI_MOSI_GPIO_PORT,  SPI_MOSI_GPIO_PIN, GPIO_PIN_SET)					//发送脚置高
#define spi_set_mosi_low( )			HAL_GPIO_WritePin(SPI_MOSI_GPIO_PORT,  SPI_MOSI_GPIO_PIN, GPIO_PIN_RESET)	//发送脚置低

#define spi_get_miso( )				HAL_GPIO_ReadPin(SPI_MISO_GPIO_PORT,SPI_MISO_GPIO_PIN)


void drv_spi_init( void );
uint8_t drv_spi_read_write_byte( uint8_t TxByte );
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );


#else									/** 只有使用硬件SPI时会使用 */


//SPI接口定义
#define SPI_PORT					SPI2						//SPI接口
#define SPI_PORT_CLK				RCC_APB1Periph_SPI2			//SPI时钟


void drv_spi_init( void );
uint8_t drv_spi_read_write_byte( uint8_t TxByte );
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );

#endif



#endif

