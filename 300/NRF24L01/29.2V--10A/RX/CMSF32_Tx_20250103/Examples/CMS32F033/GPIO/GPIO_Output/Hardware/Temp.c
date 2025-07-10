#include "Temp.h"

///////////////////////////////////////////
extern float Temp_Value; 

const float RTD_TAB_PT100[37] =   // 表格是以5度为一步，即-55, -50, -45.....
{
	437139.00,326906.00,246574.00,187518.00,143739.00,111025.00,86390.00,67699.60,    // -55 ~ -20

	53417.70,42428.70,33916.40,27280.00,22073.40,17963.80,14701.00,12095.70,          // -15 ~ 20

	10000.00,8313.00,6946.07,5829.24,4914.20,4161.00,3538.18,3020.93,                 //  25 ~ 60

	2589.51,2228.22,1924.43,1668.02,1451.00,1266.06,1108.44,973.50,                   //  65 ~ 100
	
	857.575,757.675,671.317,596.441,531.331
};


/***********************************************************************
 *FunName:        float CalculateTemperature(float fR)
 *
 *In:                fR -> PT100的电阻值。
 *                        
 *Out:                fTem -> 测得的温度值。               
 *
*Discription: 将电阻值查表算出温度值。
 *
 *Notes:         采用2分查找法。          ************************************************************************/
float CalculateTemperature(float fR)
{
	float fTem;
	float fLowRValue;
	float fHighRValue;        
	int   iTem;
	uint8_t i;
	//uint8_t cLimite = 0xFF;
	uint8_t cBottom, cTop;
 
	if (fR > RTD_TAB_PT100[0])                	// 电阻值大于表格最大值，超出量程上限 。
	{
		return 999;
	}
	if (fR < RTD_TAB_PT100[36])        			// 电阻值小于表格最值低于量程下限。
	{
		return 999;
	}
	
	cBottom = 0; 
	cTop    = 36;
	for (i = 18; (cTop - cBottom) != 1;)        // 2分法查表。
	{
		if (fR > RTD_TAB_PT100[i])
		{
			cTop = i;
			i = (cTop + cBottom) / 2;
		}
		else if (fR < RTD_TAB_PT100[i])
		{
			 cBottom = i;
			 i = (cTop + cBottom) / 2;
		}
		else
		{
			iTem = (uint32_t)i * 5 - 55;
			fTem = (float)iTem;
			return fTem;
		}
	}
	iTem = (uint32_t)i * 5 - 55;
	fLowRValue  = RTD_TAB_PT100[cBottom];
	fHighRValue = RTD_TAB_PT100[cTop];
	fTem = ( ((fR - fLowRValue)*5) / (fHighRValue - fLowRValue) ) + iTem;        // 表格是以5度为一步的。
																			   // 两点内插进行运算。
	return fTem;
}

float V_Float(uint32_t AD_V){
	float Value = (float)AD_V;
	return Value*5.0*206.8/4096.0/6.8;
}

/*
* 功能 :
* 描述 :
* 函数 :
*/
void TX_TEMPER_1(void)
{
	float total_val = 0;
	float resistance = 0;
	uint32_t total_adc = 0;
	total_adc = ADC1_GetResult(ADC1_CH_9);
	total_val = (total_adc*5.0)/4096;
	resistance = ((5-total_val)*1000)/(total_val);
	Temp_Value = CalculateTemperature(resistance);

	//printf("ADC:  %d,resistance ： %.02f,temp ： %.02f\r\n",total_adc,total_val,Temp_Value);
}

//uint32_t Verge(uint16_t Adc){
//	uint32_t ADC_Verge_Value = 0; 
//	if(Adc == ADC1_CH_10){
//		for(int i = 0 ; i < 20 ; i++){
//			if(!ADC1_IS_BUSY())
//			{	
//				ADC1_Go();
//				while(ADC1_IS_BUSY());
//			}
//			ADC_Verge_Value += ADC1_GetResult(ADC1_CH_10);			
//			delay_us(20);		
//		}
//	}else if(Adc == ADC1_CH_30){
//		for(int i = 0 ; i < 20 ; i++){
//			if(!ADC1_IS_BUSY())
//			{	
//				ADC1_Go();
//				while(ADC1_IS_BUSY());
//			}
//			ADC_Verge_Value += ADC1_GetResult(ADC1_CH_30);
//			delay_us(20);		
//		}	
//	}
//	return 	ADC_Verge_Value/20;
//}

