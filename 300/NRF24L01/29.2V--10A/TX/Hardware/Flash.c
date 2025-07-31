#include "Flash.h"


static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}

uint32_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(__IO uint32_t*)faddr; 
}


#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:双字(64位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr, u64 *pBuffer, u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i = 0;i < NumToWrite; i++)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,WriteAddr, pBuffer[i]) == HAL_OK)
		{
			WriteAddr += 8;//地址增加8.
		}		
	}  
}

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif

#define FLASH_USER_START_ADDR    ((uint32_t)0X0800F800 )   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR      ((uint32_t)(0X0800F800 +32))   /* End @ of user Flash area */

static FLASH_EraseInitTypeDef EraseInitStruct;

uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
uint32_t Address = 0, PageError = 0;
static u64 Flash_Date[4] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};//目前暂定 可以写32字节数据

//STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t*)TX_Buf, 3);

void IN16_TO_IN64(u16* Buff,u16 Len){//目前暂定 可以写32字节数据
	uint8_t Buf_Len = (uint8_t)((Len+3)/4);
	for(int i = 0; i < Buf_Len; i++){
		Flash_Date[i] = ((u64)Buff[i*4+3] << 48) |((u64)Buff[i*4 +2] << 32) |((u64)Buff[i*4 + 1] << 16) |(u64)Buff[i*4] ;
	}
}

void STMFLASH_Write(u32 Write_Addr, u16* D_Buff,u16 Lenth)	
{
	uint32_t data32 = 0,DATA32 = 0;
	uint8_t Len_flag = 0,ErroR = 0;
	IN16_TO_IN64(D_Buff,Lenth);
	HAL_FLASH_Unlock();

	/* Clear OPTVERR bit set on virgin samples */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

	/* Get the 1st page to erase */
	FirstPage = GetPage(FLASH_USER_START_ADDR);

	/* Get the number of pages to erase from 1st page */
	NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;

	/* Get the bank */
	BankNumber = GetBank(FLASH_USER_START_ADDR);

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks       = BankNumber;
	EraseInitStruct.Page        = FirstPage;
	EraseInitStruct.NbPages     = NbOfPages;
	ErroR = 10;
	while(ErroR --){
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK)
		{
			ErroR = 0;
		}
	}
	Address = Write_Addr;
	Len_flag = 0;
	while (Address < FLASH_USER_END_ADDR)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Flash_Date[Len_flag]) == HAL_OK)
		{
			Address = Address + 8;  /* increment to next double word*/
			Len_flag++;
		}
	}
	HAL_FLASH_Lock();//上锁
	Address = Write_Addr;
//	DATA32 = (u32)Flash_Date[0];
//	data32 = *(__IO uint32_t *)Address;
//	if (data32 != DATA32)
//	{
//		Debug_Printf("Flash write is error!!!");
//	}else	
//		Debug_Printf("Flash write is ok!!!");

}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	uint8_t Lenth = (NumToRead+1)/2 ;
	u32 Date_buff[10] = {0};//暂定为10
	for(i = 0; i < Lenth; i++)
	{
		Date_buff[i] = STMFLASH_ReadHalfWord(ReadAddr);	//读取4个字节.
		ReadAddr += 4;									//偏移4个字节.	
		pBuffer[i*2]   = (u16)Date_buff[i];
		pBuffer[i*2+1] = Date_buff[i] >> 16;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 Write_Addr,u16 *WriteData,u16 lenth)   	
{
	STMFLASH_Write(Write_Addr,WriteData,lenth);			//写入两个字 
}

