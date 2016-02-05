/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：lcd_YXD19264D_51.h
 * 描	述	：YXD19264D液晶显示屏
 *                    
 * 实验平台	：51开发板
 * 硬件连接	：
 * 版 	本	：V0.0.150711
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2015.7.11
 * 最后编辑	：2015.7.11
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

#ifndef __LCD_YXD19264_51_H
#define __LCD_YXD19264_51_H

/****************************包含头文件*******************************************/
#include "globaldefine.h"
/****************************宏定义***********************************************/
//管脚定义
#define LCD_DATPORT    P0   
//片选
#define LCD_CS1_1	LCD_CS1 = 1  
#define LCD_CS1_0	LCD_CS1 = 0  

#define LCD_CS2_1	LCD_CS2 = 1  
#define LCD_CS2_0	LCD_CS2 = 0  

#define LCD_CS3_1	LCD_CS3 = 1  
#define LCD_CS3_0	LCD_CS3 = 0  
//D/I
#define LCD_DI_1	LCD_DI = 1
#define LCD_DI_0	LCD_DI = 0
//R_W
#define LCD_RW_1    LCD_RW = 1
#define LCD_RW_0    LCD_RW = 0
//EN
#define LCD_EN_1	LCD_EN = 1
#define LCD_EN_0	LCD_EN = 0

//复位 低电平复位 
#define LCD_RST_1   LCD_RST = 1
#define LCD_RST_0   LCD_RST = 0

#define LCD_Write_Con_Cmd    0
#define LCD_Write_Dis_Data   1


#define LCD_WriteData(PortVal)	LCD_DATPORT = PortVal	
#define LCD_ReadData()			LCD_DATPORT

#define LCD_U1		(uchar)0x01		//列控制器代号
#define LCD_U2		(uchar)0x02
#define LCD_U3		(uchar)0x04
#define LCD_UALL	(uchar)0x07		//选择所有
/*****************************************************************
         液晶模块指令集定义
*****************************************************************/
#define	DIS_OFF				0x3E      //关显示指令
#define	DIS_ON				0x3F      //开显示指令
#define	SET_STROW_BASE		0xC0      //设置显示起始行,+0~63=设置命令和设置行 指定显示屏从DDRAM中哪一行开始显示
#define	SET_STRPAG_BASE		0xB8     //设置页地址,+0~7=设置命令和设置页，8行为一页，64行即8页
#define	SET_STCOL_BASE		0x40     //设置列地址，+0~63=设置命令和设置选中芯片的列，

/*****************************************************************/

/****************************类型定义*********************************************/

/****************************变量声明*********************************************/
extern char code WordLib_CN[][32];
extern char code CharLib_ASCII[][16];
extern char code CharLib_SplLabel[][16];
extern char code Pic_Case[];

/****************************变量定义*********************************************/

/****************************函数声明*********************************************/

void LCD_Init(void);
void LCD_Fill1(void);
void LCD_Fill2(void);
void LCD_Write(uchar unit,uchar inst, uchar dat);
void LCD_Dis_Str(uchar row, uchar col, uchar *str);
void LCD_Clear(void);
void LCD_Reset(void);
void LCD_Dis_Frame(void);
void LCD_Dis_Char_16_16(uchar row,uchar col,uchar *dat, bool invert);
void LCD_Dis_Char_8_16(uchar row,uchar col,uchar *dat, bool invert);
void LCD_Dis_ASCIIStr(uchar row,uchar col,uchar *str, bool invert);
void LCD_Dis_Digital_float(uchar row, uchar col,long Dig_Data);
void LCD_Dis_Digital_int(uchar row, uchar col,long Dig_Data);

void LCD_Dis_Pict(uchar row, uchar col, uchar row_Pixel, uchar col_Pixel,uchar *Dis_Data);
void LCD_Clear_Region(uchar row, uchar col, uchar row_Pixel, uchar col_Pixel);
void LCD_Read_Region(uchar row, uchar col, uchar row_Page, uchar col_Pixel,uchar* buf);
void LCD_fill_Region(uchar row, uchar col, uchar row_Pixel, uchar col_Pixel);

void LCD_Clear_Graphics(uchar row, uchar col, uchar row_Pixel, uchar col_Pixel);
void LCD_Dis_Graphics(uchar row, uchar col, uchar row_Pixel, uchar col_Pixel, uchar *Dis_Data);
#endif


