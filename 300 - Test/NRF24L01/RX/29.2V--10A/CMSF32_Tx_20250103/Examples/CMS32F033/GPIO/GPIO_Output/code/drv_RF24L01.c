/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   NRF24L01配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */
  
  
#include "drv_RF24L01.h"


const char *g_ErrorString = "RF24L01 is not find !...";
unsigned char INIT_ADDR[5]= {0x00,0x1A,0xB1,0xB1,0x01}; //节点地址

/**
  * @brief :NRF24L01检测
  * @param :无
  * @note  :无
  * @retval:无
  */ 
unsigned char NRF24L01_Check( void )
{
	uint8_t buf[5]={ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	unsigned char buf1[5];
	uint8_t i;
	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR , buf, 5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf1, 5); //读出写入的地址   
	
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;	
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}


/**
  * @brief :NRF24L01写寄存器
  * @param :无
  * @note  :地址在设备中有效
  * @retval:读写状态
  */
//封装SPI写寄存器，regaddr:要写的寄存器，data:写入寄存器的值
unsigned char NRF24L01_Write_Reg(unsigned char regaddr,unsigned char data)
{
	unsigned char status;
	
	RF24L01_SET_CS_LOW( );							//片选
	status = drv_spi_read_write_byte(regaddr); 		//发送寄存器号
	drv_spi_read_write_byte(data);            		//写入寄存器的值
	RF24L01_SET_CS_HIGH( );					//取消片选
	
	return(status);       		         //返回状态值
}


/**
  * @brief :NRF24L01读寄存器
  * @param :
           @Addr:寄存器地址
  * @note  :地址在设备中有效
  * @retval:读取的数据
  */
//封装SPI读寄存器值 ，regaddr:要读的寄存器
uint8_t NRF24L01_Read_Reg( uint8_t regaddr )
{
	unsigned char reg_val;
	
	RF24L01_SET_CS_LOW( );                		//使能SPI传输
	drv_spi_read_write_byte(regaddr);     		//发送寄存器号
	reg_val = drv_spi_read_write_byte(0XFF);	//读取寄存器内容
	RF24L01_SET_CS_HIGH( );						//取消片选
	
	return(reg_val);                 			//返回状态值
}


//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
unsigned char NRF24L01_Read_Buf(unsigned char regaddr,unsigned char *pBuf,unsigned char datalen)
{
	unsigned char status;
	uint8_t btmp;
	
	RF24L01_SET_CS_LOW( );									//片选
	status = drv_spi_read_write_byte(regaddr);   			//发送寄存器值(位置),并读取状态值
    for( btmp = 0; btmp < datalen; btmp ++ )
    {
        *( pBuf + btmp ) = drv_spi_read_write_byte( 0xFF );	//读数据
    }
	 RF24L01_SET_CS_HIGH( );								//取消片选
	return status;                        					//返回读到的状态值
}


/**
  * @brief :NRF24L01写指定长度的数据
  * @param :
  *			@reg:地址
  *			@pBuf:写入的数据地址
  *			@len:数据长度
  * @note  :数据长度不超过255，地址在设备中有效
  * @retval:写状态
  */
//在指定位置写指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
unsigned char NRF24L01_Write_Buf(unsigned char regaddr, unsigned char *pBuf, unsigned char datalen)
{
	unsigned char status;
	uint8_t i;
	
	 RF24L01_SET_CS_LOW( );										//片选
	status = drv_spi_read_write_byte(regaddr);                	//发送寄存器值(位置),并读取状态值
    for( i = 0; i < datalen; i ++ )
    {
        drv_spi_read_write_byte( *( pBuf + i ) );				//写数据
    }
	RF24L01_SET_CS_HIGH( );										//取消片选
	return status;                                       		//返回读到的状态值
}


//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
unsigned char NRF24L01_TxPacket(unsigned char *txbuf)
{
	unsigned char state;
	
	RF24L01_SET_CE_LOW( );
	//NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//写数据到TX BUF  32个字节
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_Ten_WIDTH);	//写数据到TX BUF  32个字节
	RF24L01_SET_CE_HIGH( );									//启动发送
	while(RF24L01_GET_IRQ_STATUS() != 0);                   //等待发送完成
	state=NRF24L01_Read_Reg(STATUS);                     	//读取状态寄存器的值
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);      	//清除TX_DS或MAX_RT中断标志
	if(state&MAX_TX)                                     	//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);               	//清除TX FIFO寄存器
		return MAX_TX;
	}
	if(state&TX_OK)                                      //发送完成
	{
		return TX_OK;
	}
	return 0xff;                                         //其他原因发送失败
}


//启动NRF24L01发送一次数据
//rxbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
unsigned char NRF24L01_RxPacket(unsigned char *rxbuf)
{
	unsigned char state;
	
	state = NRF24L01_Read_Reg(STATUS);                //读取状态寄存器的值
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); //清除TX_DS或MAX_RT中断标志
	if(state&RX_OK)                                 //接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_Ten_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //清除RX FIFO寄存器
		return 0;
	}
	return 1;                                      //没收到任何数据
}


//该函数初始化NRF24L01到RX模式
//numofslave:接收地址的低字节
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
void RX_Mode(unsigned char numofslave)
{
	INIT_ADDR[0] = numofslave;
	RF24L01_SET_CE_LOW( );
	//写RX节点地址
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)INIT_ADDR, RX_ADR_WIDTH);
	//使能通道0的自动应答
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);
	//使能通道0的接收地址
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);
	//设置RF通信频率（频率计算公式：2400+RF_CH（MHZ））
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);
	//选择通道0的有效数据宽度
	//NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_Ten_WIDTH);
	//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	//NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0A);
	//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX接收模式
	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x0f);
	//CE为高,进入接收模式
	RF24L01_SET_CE_HIGH( );			
}


//该函数初始化NRF24L01到TX模式
//numofslave:发送地址的低字节
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,
//选择RF频道,波特率和LNA HCURR PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发送.
void TX_Mode(unsigned char numofslave)
{
	INIT_ADDR[0] = numofslave;
	RF24L01_SET_CE_LOW( );
	//写TX节点地址
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(unsigned char*)INIT_ADDR,TX_ADR_WIDTH);
	//设置TX节点地址,主要为了使能ACK
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)INIT_ADDR,RX_ADR_WIDTH);
	//使能通道0的自动应答
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);
	//使能通道0的接收地址
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);
	//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);
	//设置RF通道为40
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);
	//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
//	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0A);
	//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX发送模式,开启所有中断
	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);
	// CE为高,10us后启动发送
	RF24L01_SET_CE_HIGH( );	
}


 /**
  * @brief :RF24L01引脚初始化
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Gpio_Init( void )
{
	//??CE?? ???? 
	SYS_SET_IOCFG(IOP01CFG,SYS_IOCFG_P01_GPIO);				//初始化CE P01
	GPIO_CONFIG_IO_MODE(RF24L01_CE_GPIO_PORT,RF24L01_CE_GPIO_PIN,GPIO_MODE_OUTPUT);
	GPIO0->DO_f.P1 = 1;										//初始化状态设置为高	
	
	//??IRQ?? ????
	SYS_SET_IOCFG(IOP00CFG,SYS_IOCFG_P00_GPIO);				//初始化IRQ P00		
	GPIO_CONFIG_IO_MODE(RF24L01_IRQ_GPIO_PORT,RF24L01_IRQ_GPIO_PIN,GPIO_MODE_INPUT_WITH_PULL_UP);	
	
	RF24L01_SET_CE_LOW( );		//??24L01
	RF24L01_SET_CS_HIGH( );		//??SPI??
}


 /**
  * @brief :NRF24L01模块初始化
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Init( void )
{
	RF24L01_SET_CE_LOW();
	RF24L01_SET_CS_HIGH();
	//delay_us(100);
}
