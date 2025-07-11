#ifndef _LCD_H
#define _LCD_H

#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

//#include "FONT.h"

#define TFT_SPI1_SCK_Pin 				GPIO_PIN_5
#define TFT_SPI1_MISO_Pin 				GPIO_PIN_6
#define TFT_SPI1_MOSI_Pin 				GPIO_PIN_7

#define TFT_DC_RS_Pin 					GPIO_PIN_13
#define TFT_CS_Pin 						GPIO_PIN_15
#define TFT_RESET_Pin 					GPIO_PIN_14
#define TFT_VCC_EN_Pin 					GPIO_PIN_12

#define TFT_VCC_EN_GPIO_Port 			GPIOB
#define TFT_CS_GPIO_Port 				GPIOB
#define TFT_DC_RS_GPIO_Port 			GPIOB
#define TFT_RESET_GPIO_Port 			GPIOB


//#define TFT_SPI1_SCK_GPIO_Port 			GPIOA
//#define TFT_SPI1_MISO_GPIO_Port 		GPIOA
//#define TFT_SPI1_MOSI_GPIO_Port 		GPIOA




//#define TFT_T_IRQ_Pin 					GPIO_PIN_7
//#define TFT_T_IRQ_GPIO_Port 			GPIOA
//#define TFT_T_DO_Pin 					GPIO_PIN_5
//#define TFT_T_DO_GPIO_Port 				GPIOA
//#define TFT_T_DIN_Pin 					GPIO_PIN_6
//#define TFT_T_DIN_GPIO_Port 			GPIOA
//#define TFT_T_CS_Pin 					GPIO_PIN_13
//#define TFT_T_CS_GPIO_Port 				GPIOC
//#define TFT_T_CLK_Pin 					GPIO_PIN_4
//#define TFT_T_CLK_GPIO_Port 			GPIOA

//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				//LCD ID
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	 wramcmd;		//开始写gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL  	 3//定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 320
#define LCD_H 480

//TFTLCD部分外要调用的函数		   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 

//#define LED  13        //背光控制引脚
//#define CS   15       //片选引脚
//#define RS   14       //寄存器/数据选择引脚  
//#define RST  12       //复位引脚

//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
//#define	LCD_LED PBout(LED) //LCD背光    		 
//#define LCD_CS  PBout(CS)
//#define LCD_RS  PBout(RS)
//#define LCD_RST PBout(RST)
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

#define LCD_LED_SET    HAL_GPIO_WritePin(TFT_VCC_EN_GPIO_Port, TFT_VCC_EN_Pin, GPIO_PIN_SET);
#define LCD_LED_RESET  HAL_GPIO_WritePin(TFT_VCC_EN_GPIO_Port, TFT_VCC_EN_Pin, GPIO_PIN_RESET);

#define	LCD_CS_SET     HAL_GPIO_WritePin(TFT_CS_GPIO_Port,     TFT_CS_Pin,     GPIO_PIN_SET);
#define	LCD_RS_SET	   HAL_GPIO_WritePin(TFT_DC_RS_GPIO_Port,  TFT_DC_RS_Pin,  GPIO_PIN_SET);
#define	LCD_RST_SET	   HAL_GPIO_WritePin(TFT_RESET_GPIO_Port,  TFT_RESET_Pin,  GPIO_PIN_SET);			
			    
#define	LCD_CS_CLR     HAL_GPIO_WritePin(TFT_CS_GPIO_Port,     TFT_CS_Pin,     GPIO_PIN_RESET);
#define	LCD_RS_CLR	   HAL_GPIO_WritePin(TFT_DC_RS_GPIO_Port,  TFT_DC_RS_Pin,  GPIO_PIN_RESET);
#define	LCD_RST_CLR	   HAL_GPIO_WritePin(TFT_RESET_GPIO_Port,  TFT_RESET_Pin,  GPIO_PIN_RESET);


				


//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 		0XFFE0
#define GBLUE		0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)
	
void SPIM_TXEn(SPI_TypeDef* SPIx);
void SPIM_RXEn(SPI_TypeDef* SPIx);
//unsigned int SPIMReadWriteByte(SPI_TypeDef* SPIx, unsigned char tx_data);
void SPI1_Init(void);
void LCD_GPIOInit(void);

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t Color);	 
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x,uint16_t y);//画点
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //读点
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);

uint16_t LCD_RD_DATA(void);//读取LCD数据									    
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WR_DATA(uint8_t data);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
uint16_t LCD_ReadRAM(void);		   
uint16_t LCD_BGR2RGB(uint16_t c);
void LCD_SetParam(void);
void Lcd_WriteData_16Bit(uint16_t Data);
void LCD_direction(uint8_t direction );

void GUI_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2, uint16_t color);
void _draw_circle_8(int xc, int yc, int x, int y, u16 c);
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
static void _swap(u16 *a, u16 *b);
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode);
u32 mypow(u8 m,u8 n);
void LCD_ShowNum(u16 x,u16 y,u16 fc, u16 bc,u32 num,u8 len,u8 size);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p);
void Gui_Drawbmp16_1(u16 x,u16 y,const unsigned char *p);
	
#endif

