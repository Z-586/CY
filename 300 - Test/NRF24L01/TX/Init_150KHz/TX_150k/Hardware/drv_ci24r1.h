#ifndef __DRV_CI24R1_H
#define __DRV_CI24R1_H

#include "gpio.h"

#define	PACKET_LEN			        10
        
#define REPEAT_CNT			        15          //重复次数
#define ADR_WIDTH    	        	5   	    //5个字节的发送/接收地址长度
       
#define MAX_TX  			        0x10
#define TX_OK				        0x20
#define RX_OK				        0x40
#define RX_P_NO				        0X0E        //接收FIFO为空标志

#define CI24R1_CLK_PORT				GPIOB       //
#define	CI24R1_CLK_PIN				GPIO_PIN_7

#define CI24R1_MOSI_PORT			GPIOB       //
#define	CI24R1_MOSI_PIN				GPIO_PIN_6
		
#define CI24R1_NSS_PORT				GPIOB       //
#define	CI24R1_NSS_PIN				GPIO_PIN_9

#define CI24R1_DATA_PORT			CI24R1_MOSI_PORT			
#define	CI24R1_DATA_PIN				CI24R1_MOSI_PIN

#define CI24R1_CLK_HIGH()			HAL_GPIO_WritePin( CI24R1_CLK_PORT, CI24R1_CLK_PIN,GPIO_PIN_SET)
#define	CI24R1_CLK_LOW()			HAL_GPIO_WritePin( CI24R1_CLK_PORT, CI24R1_CLK_PIN,GPIO_PIN_RESET)

#define CI24R1_NSS_HIGH()			HAL_GPIO_WritePin( CI24R1_NSS_PORT, CI24R1_NSS_PIN,GPIO_PIN_SET)
#define	CI24R1_NSS_LOW()			HAL_GPIO_WritePin( CI24R1_NSS_PORT, CI24R1_NSS_PIN,GPIO_PIN_RESET)

#define CI24R1_DATA_HIGH()			HAL_GPIO_WritePin( CI24R1_MOSI_PORT, CI24R1_MOSI_PIN,GPIO_PIN_SET)
#define	CI24R1_DATA_LOW()			HAL_GPIO_WritePin( CI24R1_MOSI_PORT, CI24R1_MOSI_PIN,GPIO_PIN_RESET)

#define CI24R1_READ()				HAL_GPIO_ReadPin(CI24R1_DATA_PORT,CI24R1_DATA_PIN)


//======================================================================
//                        SPI commands (Si24R1)
//======================================================================
#define R_REGISTER    		0x00  	// Define Read command to register
#define W_REGISTER   		0x20  	// Define Write command to register
#define R_RX_PAYLOAD 		0x61  	// Define Read RX payload register address
#define W_TX_PAYLOAD 		0xA0  	// Define Write TX payload register address
#define FLUSH_TX    		0xE1  	// Define Flush TX register command
#define FLUSH_RX    		0xE2  	// Define Flush RX register command
#define REUSE_TX_PL 		0xE3  	// Define Reuse TX payload register command
#define R_RX_PL_WID     	0x60  	// Define Read RX payload width for the R_RX_PAYLOAD in the RX FIFO
#define W_ACK_PAYLOAD   	0xA8	// Define Write Payload to be transmitted together with ACK packet on PIPExxx xxx valid in the range from 000 to 101)
#define W_TX_PAYLOAD_NOACK	0xB0	// Define Write TX Disables AUTOACK on this specific packet
#define NOP         		0xFF  	// Define No Operation, might be used to read status register

//======================================================================
//                     SPI commands (Ci24R1) (newly increased!!!!)
#define CE_ON         	    0x70        //′ò?aCE  CE=1,CE_STATE=1
#define CE_OFF 	            0x71		//1?±?CE  CE=0,CE_STATE=0
#define SELSPI	            0x74		//????DATAòy???aSPI1|?ü
#define SELIRQ	            0x75		//????DATAòy???aê?3?IRQ?μ
//======================================================================

//======================================================================
//                    SPI registers address(Si24R1)
//======================================================================
#define CONFIG      0x00  // 'Config' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address
#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define RPD         0x09  // 'Received Power Detector' register address  equal to Si24R1 is "CD"(Carrier Detecotr)
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address
#define BandWidth_TX 0x18  
#define DYNPD       0x1C  // 'Enable dynamic paload length' register address
#define FEATURE     0x1D  //  Feature Register address

//位定义
#define MASK_RX_DR   	6 
#define MASK_TX_DS   	5 
#define MASK_MAX_RT  	4 
#define EN_CRC       	3 
#define CRCO         	2 
#define PWR_UP       	1 
#define PRIM_RX      	0 

#define ENAA_P5      	5 
#define ENAA_P4      	4 
#define ENAA_P3      	3 
#define ENAA_P2      	2 
#define ENAA_P1      	1 
#define ENAA_P0      	0 

#define ERX_P5       	5 
#define ERX_P4       	4 
#define ERX_P3       	3 
#define ERX_P2      	2 
#define ERX_P1       	1 
#define ERX_P0       	0 

#define AW_RERSERVED 	0x0 
#define AW_3BYTES    	0x1
#define AW_4BYTES    	0x2
#define AW_5BYTES    	0x3

#define ARD_250US    	(0x00<<4)
#define ARD_500US    	(0x01<<4)
#define ARD_750US    	(0x02<<4)
#define ARD_1000US   	(0x03<<4)
#define ARD_2000US   	(0x07<<4)
#define ARD_4000US   	(0x0F<<4)
#define ARC_DISABLE   	0x00
#define ARC_15        	0x0F

#define CONT_WAVE     	7 
#define RF_DR_LOW     	5 
#define PLL_LOCK      	4 
#define RF_DR_HIGH    	3 
//bit2-bit1:
//#define PWR_18DB  		(0x00<<1)
//#define PWR_12DB  		(0x01<<1)
//#define PWR_6DB   		(0x02<<1)
//#define PWR_0DB   		(0x03<<1)

#define RX_DR         	6 
#define TX_DS         	5 
#define MAX_RT        	4 
//for bit3-bit1, 
#define TX_FULL_0     	0 

#define TX_REUSE      	6 
#define TX_FULL_1     	5 
#define TX_EMPTY      	4 
//bit3-bit2, reserved, only '00'
#define RX_FULL       	1 
#define RX_EMPTY      	0 

#define DPL_P5        	5 
#define DPL_P4        	4 
#define DPL_P3        	3 
#define DPL_P2        	2 
#define DPL_P1        	1 
#define DPL_P0        	0 

#define EN_DPL        	2 
#define EN_ACK_PAY    	1 
#define EN_DYN_ACK    	0 
#define IRQ_ALL  ( (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) )

typedef enum 
{
	MODE_TX = 0,
	MODE_RX
}Ci24r1Mode_e;

void CI24R1_DATA_IN(void);
void CI24R1_DATA_OUT(void);

void SPI1_WriteByte(uint8_t	src);
uint8_t	SPI1_ReadByte(void);
void Ci24R1_Write_Reg(uint8_t reg,uint8_t value);
uint8_t	Ci24R1_Read_Reg(uint8_t reg);
void Ci24R1_Write_Cmd(uint8_t cmd);
void Ci24R1_Write_Buf(uint8_t reg,uint8_t *pBuf,uint8_t length);
void Ci24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t length);
void DrvCi24r1SetMode( Ci24r1Mode_e Mode );
void DrvCi24r1GpioInit(void);
uint8_t DrvCi24r1IsExist(void);
uint8_t DrvCi24r1Init( Ci24r1Mode_e Mode );
uint8_t DrvCi24r1Send(uint8_t* buf, uint8_t cnt);
uint8_t Ci24R1RxPacket(uint8_t *rx_buf);

#endif

