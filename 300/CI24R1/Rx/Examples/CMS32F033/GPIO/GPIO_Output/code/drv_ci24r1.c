#include  "drv_ci24r1.h"

uint8_t INIT_ADDR[ADR_WIDTH]={0x00,0x1A,0xB1,0x21,0x01}; //节点地址

void DrvSpiDelay(uint8_t delay)
{
	while(delay--)
	{

	}
}


void SPI1_WriteByte(uint8_t	src)
{
	uint8_t i=0;
	CI24R1_CLK_LOW();
		
	CI24R1_DATA_OUT();
	
	for(i=0;i<8;i++)
	{
		CI24R1_CLK_LOW();

		if(src&0x80)
		{
			CI24R1_DATA_HIGH();
		}
		else
		{	
			CI24R1_DATA_LOW();
		}
		
		CI24R1_CLK_HIGH();
		src=src<<1;
	}
	CI24R1_CLK_LOW();
}


uint8_t	SPI1_ReadByte(void)
{
	uint8_t i=0,RxData;
	
	CI24R1_DATA_IN();
	CI24R1_CLK_LOW();
	
	for(i=0;i<8;i++)
	{
		RxData = RxData<<1;
		CI24R1_CLK_HIGH();

		if(CI24R1_READ())
		{
			RxData|=0x01;
		}
		else
		{
			RxData&=0xfe;
		}
		CI24R1_CLK_LOW();
	}
	
	CI24R1_CLK_LOW();
	
	return RxData;
}


void Ci24R1_Write_Reg(uint8_t reg,uint8_t value)
{
	CI24R1_NSS_LOW();					
	SPI1_WriteByte(reg);

	SPI1_WriteByte(value);
	CI24R1_NSS_HIGH();
}


uint8_t	Ci24R1_Read_Reg(uint8_t reg)
{
	uint8_t	reg_val;
	CI24R1_NSS_LOW();
	SPI1_WriteByte(reg);
	reg_val = SPI1_ReadByte();
	CI24R1_NSS_HIGH();
	return	reg_val;
}

void Ci24R1_Write_Cmd(uint8_t cmd)
{
	CI24R1_NSS_LOW();					
	SPI1_WriteByte(cmd);
	CI24R1_NSS_HIGH();
}

void Ci24R1_Write_Buf(uint8_t reg,uint8_t *pBuf,uint8_t length)
{
	uint8_t	ctr;
	CI24R1_NSS_LOW();
	SPI1_WriteByte(reg);
	for(ctr=0;ctr<length;ctr++)
	{
		SPI1_WriteByte(*pBuf++);
	}
	CI24R1_NSS_HIGH();
		
}

void Ci24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t length)
{
	uint8_t  ctr;
	CI24R1_NSS_LOW();                                
	SPI1_WriteByte(reg);         					
	for(ctr=0;ctr<length;ctr++)
	{
		pBuf[ctr] = SPI1_ReadByte();				
	}
	CI24R1_NSS_HIGH();		
}

void DrvCi24r1SetMode( Ci24r1Mode_e Mode )
{
    uint8_t controlreg = 0;
	controlreg = Ci24R1_Read_Reg( CONFIG );
	
    if( Mode == MODE_TX )       
	{
		controlreg &= ~( 1<< PRIM_RX );
	}
    else 
	{
		controlreg |= ( 1<< PRIM_RX ); 
	}


    Ci24R1_Write_Reg(W_REGISTER +  CONFIG, controlreg );
}

void DrvCi24r1GpioInit(void)
{	
	//SPI引脚初始化  
	SYS_SET_IOCFG(IOP04CFG,SYS_IOCFG_P04_GPIO);				
	SYS_SET_IOCFG(IOP05CFG,SYS_IOCFG_P05_GPIO);				
	SYS_SET_IOCFG(IOP07CFG,SYS_IOCFG_P07_GPIO);				
	GPIO_CONFIG_IO_MODE(GPIO0, GPIO_PIN_4, GPIO_MODE_OUTPUT);
	GPIO_CONFIG_IO_MODE(GPIO0, GPIO_PIN_5, GPIO_MODE_OUTPUT);
	GPIO_CONFIG_IO_MODE(GPIO0, GPIO_PIN_7, GPIO_MODE_OUTPUT);
	GPIO0->DO_f.P4 = 1;										
	GPIO0->DO_f.P5 = 1;										
	GPIO0->DO_f.P7 = 1;										
	//拉高引脚
}

//SPI测试
uint8_t DrvCi24r1IsExist(void)
{
	uint8_t sta = 1;
	uint8_t tx_addr[5] = {0xA5,0xA5,0xA5,0xA5,0xA5};
	uint8_t rx_addr[5] = {0};
	
	Ci24R1_Write_Cmd(SELSPI);
	Ci24R1_Write_Cmd(CE_OFF);
	Ci24R1_Write_Cmd(SELSPI);
	Ci24R1_Write_Buf(W_REGISTER + TX_ADDR, tx_addr, 5); // 写入发送地址
	
	Ci24R1_Read_Buf(R_REGISTER + TX_ADDR,rx_addr,5);	//读取发送地址
	
	for(uint8_t i = 0; i < 5; i++)
	{
		if(tx_addr[i] != rx_addr[i] )//判断写入参数与读取参数是否一致
		{
			sta = 0;
			break;
		}
	}
	return sta;
}


//初始化CI24R1
uint8_t DrvCi24r1Init( Ci24r1Mode_e Mode )
{
	uint8_t sta = 0;
	
	DrvCi24r1GpioInit();//相关IO初始化
	
	if(DrvCi24r1IsExist())//SPI通信是否正常
	{
		sta = 1;
/**********对Ci24R1相关寄存器写入参数***********/		
		Ci24R1_Write_Reg(CE_OFF,0xff);

#if (PACKET_LEN != 0)
		Ci24R1_Write_Reg(W_REGISTER+RX_PW_P0,PACKET_LEN); //固定包长
		Ci24R1_Write_Reg(W_REGISTER + FEATURE, 0x07 );
		Ci24R1_Write_Reg(W_REGISTER + DYNPD, 0x00); 
#else
		Ci24R1_Write_Reg(W_REGISTER + DYNPD, 0x01); //P0动态负载
		Ci24R1_Write_Reg(W_REGISTER + FEATURE, 0x07 );//开启动态长度 
#endif
		
		if(Mode == MODE_TX)//为发送模式时
		{
			Ci24R1_Write_Reg( W_REGISTER + CONFIG, 	 ( 0 << MASK_TX_DS) | //发送完成
													( 0 << MASK_MAX_RT) |//最大重传
													( 1 <<MASK_RX_DR ) |	//0:接收中断 1 :关闭
													( 0 << CRCO ) |
													( 1 << EN_CRC ) |     //使能CRC 1个字节
													( 1 << PWR_UP ) );    //开启设备
			DrvCi24r1SetMode(MODE_TX);
		}
		else//为接收模式时
		{
			Ci24R1_Write_Reg( W_REGISTER + CONFIG, 	 ( 1 << MASK_TX_DS) | //发送完成
													 ( 1 << MASK_MAX_RT) |//最大重传
													 ( 0 <<MASK_RX_DR ) |	//0:接收中断 1 :关闭
													 ( 0 << CRCO ) |
													 ( 1 << EN_CRC ) |     //使能CRC 1个字节
													 ( 1 << PWR_UP ) );    //开启设备
			DrvCi24r1SetMode(MODE_RX);
		}
		

		Ci24R1_Write_Reg(W_REGISTER+EN_AA,( 1 << ENAA_P0 ));					//1：使能 0：失能 通道0的自动应答
	
		Ci24R1_Write_Reg(W_REGISTER+EN_RXADDR,( 1 << ERX_P0 ));				//使能通道0的接收地址
		Ci24R1_Write_Reg(W_REGISTER+SETUP_AW,AW_5BYTES);					//设置接收地址宽度为5个字节；
		Ci24R1_Write_Reg(W_REGISTER+SETUP_RETR, 
						 ARD_750US |( REPEAT_CNT & 0x0F ) );         	//重发时间和次数
		Ci24R1_Write_Reg(W_REGISTER+RF_CH,2);						//设置RF频率为2402MHz
		Ci24R1_Write_Reg(W_REGISTER+RF_SETUP,0x27);					//1M 3db
		Ci24R1_Write_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t *)INIT_ADDR,ADR_WIDTH);	//写入RX节点地址
		Ci24R1_Write_Buf(W_REGISTER+TX_ADDR,(uint8_t *)INIT_ADDR,ADR_WIDTH); //写入TX节点地址
		
		Ci24R1_Write_Reg(CE_ON,0xff);
	}
	
	return sta;
}
/***********RF接收函数****************/
uint8_t Ci24R1RxPacket(uint8_t *rx_buf)
{
	uint8_t status;
		
	Ci24R1_Write_Cmd(SELIRQ);
	CI24R1_DATA_IN();
	while (CI24R1_READ()); 
	CI24R1_DATA_OUT(); 
	Ci24R1_Write_Cmd(SELSPI);
	
	status = Ci24R1_Read_Reg(R_REGISTER+STATUS);//读取状态寄存器
	Ci24R1_Write_Reg(W_REGISTER+STATUS,status);
	
	if(status&RX_OK)
	{
		if((status&RX_P_NO)!=0x0e)                         //判断RX_FIFO是否为空，不为空继续读
		{
#if (PACKET_LEN == 0)
			uint8_t rx_len = Ci24R1_Read_Reg(R_REGISTER + R_RX_PL_WID);
			Ci24R1_Read_Buf(R_RX_PAYLOAD,rx_buf,rx_len);
#else
			Ci24R1_Read_Buf(R_RX_PAYLOAD,rx_buf,PACKET_LEN);
#endif
			Ci24R1_Write_Reg(W_REGISTER + STATUS,status);         		//继续读状态寄存器判断RX_FIFO标志位
		}	
		Ci24R1_Write_Reg(FLUSH_RX,0xff);							//清空RX_FIFO
		return 0;

	}
	return 1;
}
/*****************RF发送函数*******************/
uint8_t DrvCi24r1Send(uint8_t* buf, uint8_t cnt)
{
	uint8_t status;
	
	uint16_t txtimeout=0;
	
	Ci24R1_Write_Reg(CE_OFF,0xff);
	Ci24R1_Write_Cmd(SELSPI);
	DrvCi24r1SetMode(MODE_TX);
	Ci24R1_Write_Reg(FLUSH_TX,0xff);							//清除TX FIFO的值
	

	#if (PACKET_LEN == 0)
		Ci24R1_Write_Buf(W_TX_PAYLOAD,buf,cnt);   //写入数据负载
	#else
		Ci24R1_Write_Buf(W_TX_PAYLOAD,buf,PACKET_LEN);   
	#endif
	
	Ci24R1_Write_Reg(CE_ON,0xff);													//CE拉高，Si24R1开始发射
	Ci24R1_Write_Cmd(SELIRQ);
	CI24R1_DATA_IN();
	
	while (CI24R1_READ())//等待发送完成
	{
		
		txtimeout++;
		if(txtimeout>0xfff0)
		{
			txtimeout=0;
			break;
		}
		
	};                 //一直等待IRQ引脚被拉低（Si24R1的IRQ低电平有效）
  CI24R1_DATA_OUT();                                          	
	Ci24R1_Write_Cmd(SELSPI);
	status = Ci24R1_Read_Reg(R_REGISTER+STATUS); 
	Ci24R1_Write_Reg(W_REGISTER+STATUS,status);						//清楚MAX_TX或TX_OK中断
	if(status&MAX_TX)
	{
		Ci24R1_Write_Reg(FLUSH_TX,0xff);							//清除TX FIFO的值
		return MAX_TX;
	}
	if(status&TX_OK)
	{
		return TX_OK;
	}
	
	return 0xff;  													//其他原因发送失败
}

