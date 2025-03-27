#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//2.4寸/2.8寸/3.5寸/4.3寸 TFT液晶驱动
//支持驱动IC型号包括:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/SSD1289/1505/B505/C505/NT35310/NT35510等
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/2/11
//版本：V2.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//V4.0 20211110
//1��ȥ���󲿷ֲ���������IC��֧��
//2��������ST7789����IC��֧��
//3���Ż�����ṹ������Դ�룩
//////////////////////////////////////////////////////////////////////////////////



//LCD重要参数集
typedef struct  
{
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令
}_lcd_dev;

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色
extern u16  POINT_COLOR;//默认红色
extern u16  BACK_COLOR; //背景颜色.默认为白色


//////////////////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义----------------
#define	LCD_LED PBout(0) //LCD背光    		 PB0
//LCD地址结构体
typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;
//RS=A16
//NCE1=
//使用NOR/SRAM的 Bank1.sector1,地址位HADDR[27,26]=00(0x60000000|0x00<<24) A16=0x0001FFFE作为数据命令区分线
//注意设置时STM32内部会右移一位对其! 111110=0X3E
#define LCD_BASE        ((u32)(0x60000000 | 0x0001FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

//PB0~15,��Ϊ������
#define DATAOUT(x) GPIOB->ODR=x; //�������
#define DATAIN     GPIOB->IDR;   //��������

 
//////////////////////////////////////////////////////////////////////

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(u16 Color);	 												//清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//设置光标
void LCD_DrawPoint(u16 x,u16 y);											//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//快速画点
u16  LCD_ReadPoint(u16 x,u16 y); 											//读点
void Draw_Circle(u16 x0,u16 y0,u8 r);										//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,const char *p);		//显示一个字符串,12/16字体

void LCD_Init(void);                                                        //��ʼ��
void LCD_DisplayOn(void);                                                   //����ʾ
void LCD_DisplayOff(void);                                                  //����ʾ
void LCD_Clear(u16 Color);                                                  //����
void LCD_SetCursor(u16 Xpos, u16 Ypos);                                     //���ù��
void LCD_DrawPoint(u16 x, u16 y);                                           //����
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color);                           //���ٻ���
u16  LCD_ReadPoint(u16 x, u16 y);                                           //����
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r);                                 //��Բ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);                          //����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);                     //������
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);                   //��䵥ɫ
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color);            //���ָ����ɫ
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode);                  //��ʾһ���ַ�
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size);                   //��ʾһ������
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode);         //��ʾ ����
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p);   //��ʾһ���ַ���,12/16����

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_SSD_BackLightSet(u8 pwm);                          //SSD1963 �������
void LCD_Scan_Dir(u8 dir);                                  //������ɨ�跽��
void LCD_Display_Dir(u8 dir);                               //������Ļ��ʾ����
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height); //���ô���

//д���ݺ���
#define LCD_WR_DATA(data){\
        LCD_RS_SET;\
        LCD_CS_CLR;\
        DATAOUT(data);\
        LCD_WR_CLR;\
        LCD_WR_SET;\
        LCD_CS_SET;\
    }

//LCD�ֱ�������
#define SSD_HOR_RESOLUTION      800     //LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION      480     //LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH     1       //ˮƽ����
#define SSD_HOR_BACK_PORCH      46      //ˮƽǰ��
#define SSD_HOR_FRONT_PORCH     210     //ˮƽ����

#define SSD_VER_PULSE_WIDTH     1       //��ֱ����
#define SSD_VER_BACK_PORCH      23      //��ֱǰ��
#define SSD_VER_FRONT_PORCH     22      //��ֱǰ��
//���¼����������Զ�����
#define SSD_HT  (SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS (SSD_HOR_BACK_PORCH)
#define SSD_VT  (SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)


void LCD_ShowPicture(uint16_t
x,uint16_t y,uint16_t column,uint16_t
row,unsigned short *pic);

void LCD_ShowPic_Part(uint16_t column,uint16_t row,uint16_t pos_x, uint16_t pos_y,
uint16_t pa_x,uint16_t pa_y, uint16_t tar_col,uint16_t tar_row,unsigned short *pic);

void LCD_ShowBigMiss(uint16_t x,uint16_t y,uint16_t column,uint16_t row,
uint16_t pa_x,uint16_t pa_y, uint16_t pa_w,uint16_t pa_h,unsigned short *pic);

#endif













