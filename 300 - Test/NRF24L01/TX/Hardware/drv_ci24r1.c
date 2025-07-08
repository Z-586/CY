#include  "drv_ci24r1.h"
/*********TX*************

DrvCi24r1Init(MODE_TX);//����RFΪ����ģʽ

***********RX************

DrvCi24r1Init(MODE_RX);//����RFΪ����ģʽ
Ci24R1_Write_Cmd(SELIRQ);//ѡ��DATA�������IRQֵ
CI24R1_DATA_IN();

************************/
	
static const unsigned char Iint_ADDR[ADR_WIDTH]= {0x00,0x1A,0xB1,0x21,0x01}; // //Ĭ�Ͻڵ��ַ
uint8_t INIT_ADDR[ADR_WIDTH]={0x00,0x1A,0xB1,0x21,0x01}; //�ڵ��ַ

void CI24R1_DATA_IN(void){

	GPIO_InitTypeDef GPIO_InitStruct = {0}; 					
	__HAL_RCC_GPIOB_CLK_ENABLE();											
	GPIO_InitStruct.Pin = CI24R1_DATA_PIN;						
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;						
	GPIO_InitStruct.Pull = GPIO_PULLUP;								
	HAL_GPIO_Init(CI24R1_DATA_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin( CI24R1_DATA_PORT, CI24R1_DATA_PIN,GPIO_PIN_SET);
}

void CI24R1_DATA_OUT(void){

	GPIO_InitTypeDef GPIO_InitStruct = {0};					
	__HAL_RCC_GPIOB_CLK_ENABLE();											
	GPIO_InitStruct.Pin = CI24R1_DATA_PIN;						
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;				
	GPIO_InitStruct.Pull = GPIO_PULLUP;			
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		
	HAL_GPIO_Init(CI24R1_DATA_PORT, &GPIO_InitStruct);	
	
	HAL_GPIO_WritePin( CI24R1_DATA_PORT, CI24R1_DATA_PIN,GPIO_PIN_SET);
}

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
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};					
	__HAL_RCC_GPIOB_CLK_ENABLE();											
	GPIO_InitStruct.Pin = CI24R1_CLK_PIN | CI24R1_NSS_PIN | CI24R1_MOSI_PIN;						
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;				
	GPIO_InitStruct.Pull = GPIO_PULLUP;	
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;	
	HAL_GPIO_Init(CI24R1_DATA_PORT, &GPIO_InitStruct);	
//	GPIO_InitStruct.Pin =  CI24R1_MOSI_PIN;	
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		
//	HAL_GPIO_Init(CI24R1_DATA_PORT, &GPIO_InitStruct);	
	
	CI24R1_CLK_HIGH();
	CI24R1_NSS_HIGH();
	CI24R1_DATA_HIGH();
}

uint8_t DrvCi24r1IsExist(void)
{
	uint8_t sta = 1;
	uint8_t tx_addr[5] = {0xA5,0xA5,0xA5,0xA5,0xA5};
	uint8_t rx_addr[5] = {0};
	
	Ci24R1_Write_Cmd(SELSPI);
	Ci24R1_Write_Cmd(CE_OFF);
	Ci24R1_Write_Cmd(SELSPI);
	Ci24R1_Write_Buf(W_REGISTER + TX_ADDR, tx_addr, 5);     		// д�뷢�͵�ַ
	
	Ci24R1_Read_Buf(R_REGISTER + TX_ADDR,rx_addr,5);
	
	for(uint8_t i = 0; i < 5; i++)
	{
		if(tx_addr[i] != rx_addr[i] )
		{
			sta = 0;
			break;
		}
	}
	return sta;
}

uint8_t DrvCi24r1Init( Ci24r1Mode_e Mode )
{
	uint8_t sta = 0;
	
	DrvCi24r1GpioInit();
	
	if(DrvCi24r1IsExist())
	{
		sta = 1;
	
		Ci24R1_Write_Reg(CE_OFF,0xff);

#if (PACKET_LEN != 0)
		Ci24R1_Write_Reg(W_REGISTER+RX_PW_P0,PACKET_LEN); //�̶�����
		Ci24R1_Write_Reg(W_REGISTER + FEATURE, 0x07 );
		Ci24R1_Write_Reg(W_REGISTER + DYNPD, 0x00); 
#else
		Ci24R1_Write_Reg(W_REGISTER + DYNPD, 0x01); //P0��̬����
		Ci24R1_Write_Reg(W_REGISTER + FEATURE, 0x07 );//������̬���� 
#endif
		
		if(Mode == MODE_TX)
		{
			Ci24R1_Write_Reg( W_REGISTER + CONFIG, 	 ( 0 << MASK_TX_DS) | //�������
													( 0 << MASK_MAX_RT) |//����ش�
													( 1 <<MASK_RX_DR ) |	//0:�����ж� 1 :�ر�
													( 0 << CRCO ) |
													( 1 << EN_CRC ) |     //ʹ��CRC 1���ֽ�
													( 1 << PWR_UP ) );    //�����豸
			DrvCi24r1SetMode(MODE_TX);
		}
		else
		{
			Ci24R1_Write_Reg( W_REGISTER + CONFIG, 	 ( 1 << MASK_TX_DS) | //�������
													 ( 1 << MASK_MAX_RT) |//����ش�
													 ( 0 <<MASK_RX_DR ) |	//0:�����ж� 1 :�ر�
													 ( 0 << CRCO ) |
													 ( 1 << EN_CRC ) |     //ʹ��CRC 1���ֽ�
													 ( 1 << PWR_UP ) );    //�����豸
			DrvCi24r1SetMode(MODE_RX);
		}
		

		Ci24R1_Write_Reg(W_REGISTER+EN_AA,( 1 << ENAA_P0 ));					//1��ʹ�� 0��ʧ�� ͨ��0���Զ�Ӧ��
	
		Ci24R1_Write_Reg(W_REGISTER+EN_RXADDR,( 1 << ERX_P0 ));				//ʹ��ͨ��0�Ľ��յ�ַ
		Ci24R1_Write_Reg(W_REGISTER+SETUP_AW,AW_5BYTES);					//���ý��յ�ַ���Ϊ5���ֽڣ�
		Ci24R1_Write_Reg(W_REGISTER+SETUP_RETR, 
						 ARD_750US |( REPEAT_CNT & 0x0F ) );         	//�ط�ʱ��ʹ���
		Ci24R1_Write_Reg(W_REGISTER+RF_CH,2);						//����RFƵ��Ϊ2402MHz
		Ci24R1_Write_Reg(W_REGISTER+RF_SETUP,0x27);					//1M 3db0x03
		if(Mode == MODE_TX){
			Ci24R1_Write_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t *)Iint_ADDR,ADR_WIDTH);	//д��RX�ڵ��ַ
			Ci24R1_Write_Buf(W_REGISTER+TX_ADDR,(uint8_t *)Iint_ADDR,ADR_WIDTH); //д��TX�ڵ��ַ
		}
		else{
			Ci24R1_Write_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t *)INIT_ADDR,ADR_WIDTH);	//д��RX�ڵ��ַ
			Ci24R1_Write_Buf(W_REGISTER+TX_ADDR,(uint8_t *)INIT_ADDR,ADR_WIDTH); //д��TX�ڵ��ַ
		}	
		Ci24R1_Write_Reg(CE_ON,0xff);
	}
	
	return sta;
}

uint8_t Ci24R1RxPacket(uint8_t *rx_buf)
{
	uint8_t status;
	uint16_t rxtimeout;
	Ci24R1_Write_Cmd(SELIRQ);
	CI24R1_DATA_IN();
	while (CI24R1_READ()){
			rxtimeout++;
		if(rxtimeout>0xfff0)
		{
			rxtimeout=0;
			break;
		}
	} 
	CI24R1_DATA_OUT(); 
	Ci24R1_Write_Cmd(SELSPI);
	status = Ci24R1_Read_Reg(R_REGISTER+STATUS);
	Ci24R1_Write_Reg(W_REGISTER+STATUS,status);
	if(status&RX_OK)
	{
		if((status&RX_P_NO)!=0x0e)                         //�ж�RX_FIFO�Ƿ�Ϊ�գ���Ϊ�ռ�����
		{
#if (PACKET_LEN == 0)
			uint8_t rx_len = Ci24R1_Read_Reg(R_REGISTER + R_RX_PL_WID);
			Ci24R1_Read_Buf(R_RX_PAYLOAD,rx_buf,rx_len);
#else
			Ci24R1_Read_Buf(R_RX_PAYLOAD,rx_buf,PACKET_LEN);
#endif
			Ci24R1_Write_Reg(W_REGISTER + STATUS,status);         		//������״̬�Ĵ����ж�RX_FIFO��־λ
		}	
		Ci24R1_Write_Reg(FLUSH_RX,0xff);							//���RX_FIFO
		return 0;

	}
	return 1;
}

uint8_t DrvCi24r1Send(uint8_t* buf, uint8_t cnt)
{
	uint8_t status;
	uint16_t txtimeout=0;

	Ci24R1_Write_Reg(CE_OFF,0xff);
	Ci24R1_Write_Cmd(SELSPI);
	DrvCi24r1SetMode(MODE_TX);
	Ci24R1_Write_Reg(FLUSH_TX,0xff);							//���TX FIFO��ֵ
	

	#if (PACKET_LEN == 0)
		Ci24R1_Write_Buf(W_TX_PAYLOAD,buf,cnt);   
	#else
		Ci24R1_Write_Buf(W_TX_PAYLOAD,buf,PACKET_LEN);   
	#endif

		
	Ci24R1_Write_Reg(CE_ON,0xff);													//CE���ߣ�Si24R1��ʼ����
	Ci24R1_Write_Cmd(SELIRQ);
	CI24R1_DATA_IN();
    
	while ( CI24R1_READ())
	{
		txtimeout++;
		if(txtimeout>0xfff0)
		{
			txtimeout=0;
			break;
		}
	};                 //һֱ�ȴ�IRQ���ű����ͣ�Si24R1��IRQ�͵�ƽ��Ч��
      
    CI24R1_DATA_OUT();                                          	
	Ci24R1_Write_Cmd(SELSPI);
	status = Ci24R1_Read_Reg(R_REGISTER+STATUS); 
	Ci24R1_Write_Reg(W_REGISTER+STATUS,status);						//���MAX_TX��TX_OK�ж�
	if(status&MAX_TX)
	{
		Ci24R1_Write_Reg(FLUSH_TX,0xff);							//���TX FIFO��ֵ
		return MAX_TX;
	}
	if(status&TX_OK)
	{
		return TX_OK;
	}
	return 0xff;  													//����ԭ����ʧ��
}

