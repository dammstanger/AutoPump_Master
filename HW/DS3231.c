/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：DS3231.c
 * 描	述	：实时时钟DS3231驱动
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.0.160129
 * 从属关系	：AutoPumpV2 
 * 库版本	：无
 * 创建时间	：2016.1.29
 * 最后编辑	：2016.1.29
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "DS3231.h"
#include "UART_51.h"

/****************************宏定义***********************************************/
#define DS3231_SLADDR			0xD0 		//从机地址		

#define DS3231_SEC				0x00 		//秒
#define DS3231_MIN				0x01 		//分
#define DS3231_HOUR				0x02 		//时
#define DS3231_WEEK				0x03 		//周
#define DS3231_DATE				0x04 		//天
#define DS3231_MON_CEN			0x05 		//月
#define DS3231_YEAR				0x06 		//年
#define DS3231_ALM1SEC			0x07 		//闹钟1
#define DS3231_ALM1MIN			0x08 		//闹钟1
#define DS3231_ALM1HOUR			0x09 		//闹钟1
#define DS3231_ALM1DAY_WEK		0x0A 		//闹钟1
#define DS3231_ALM2MIN			0x0B		//闹钟2
#define DS3231_ALM2HOUR			0x0C 		//闹钟2
#define DS3231_ALM2DAY_WEK		0x0D 		//闹钟2
#define DS3231_CTL				0x0E 		//控制寄存器
#define DS3231_STA				0x0F 		//状态寄存器
#define DS3231_AOFFSET			0x10 		//晶振老化补偿
#define DS3231_TMP_H			0x11 		//温度8高位 bit7-0
#define DS3231_TMP_L			0x12 		//温度2低位 bit7 6

/****************************变量声明*********************************************/

/****************************变量定义*********************************************/
sbit DS3231_INT	= P3^4;
sbit DS3231_RST	= P3^5;

STC_TIME RTCtime={0,0,0,1,5,1,2016,0};
/****************************函数声明*********************************************/

/********************************************************************************
 * 函数名：DS3231_Init()
 * 描述  ：DS3231初始化
 * 输入  ：
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void DS3231_Init(void)  
{
	IIC_WriteReg(DS3231_SLADDR,DS3231_CTL,0);		//不使能中断，输出1kHz方波
}


/********************************************************************************
 * 函数名：DS3231_ReadTime()
 * 描述  ：DS3231读时间
 * 输入  ：
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void DS3231_ReadTime(void)  
{
	uchar temp;
	temp = IIC_ReadReg(DS3231_SLADDR,DS3231_SEC);
	RTCtime.sec = ((temp&0xf0)>>4)*10+(temp&0x0f);
	temp = IIC_ReadReg(DS3231_SLADDR,DS3231_MIN);
	RTCtime.min = ((temp&0xf0)>>4)*10+(temp&0x0f);
	temp = IIC_ReadReg(DS3231_SLADDR,DS3231_HOUR);
	RTCtime.hour = ((temp&0xf0)>>4)*10+(temp&0x0f);
	temp = IIC_ReadReg(DS3231_SLADDR,DS3231_DATE);
	RTCtime.date = ((temp&0xf0)>>4)*10+(temp&0x0f);
	RTCtime.week = IIC_ReadReg(DS3231_SLADDR,DS3231_WEEK);
	temp = IIC_ReadReg(DS3231_SLADDR,DS3231_MON_CEN);
	RTCtime.month = ((temp&0x10)>>4)*10+(temp&0x0f);
	temp = IIC_ReadReg(DS3231_SLADDR,DS3231_YEAR);
	RTCtime.year = 2000+((temp&0xf0)>>4)*10+(temp&0x0f);
	
}

/********************************************************************************
 * 函数名：DS3231_SetTime()
 * 描述  ：DS3231设置时间
 * 输入  ：
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void DS3231_SetTime(STC_TIME timedat)  
{
	uchar temp;
	temp = ((timedat.sec/10)<<4)+(timedat.sec%10);
	IIC_WriteReg(DS3231_SLADDR,DS3231_SEC,temp);
	temp = ((timedat.min/10)<<4)+(timedat.min%10);
	IIC_WriteReg(DS3231_SLADDR,DS3231_MIN,temp);
	temp = ((timedat.hour/10)<<4)+(timedat.hour%10);
	IIC_WriteReg(DS3231_SLADDR,DS3231_HOUR,temp);
	temp = ((timedat.date/10)<<4)+(timedat.date%10);
	IIC_WriteReg(DS3231_SLADDR,DS3231_DATE,temp);
	IIC_WriteReg(DS3231_SLADDR,DS3231_WEEK,timedat.week);
	temp = ((timedat.month/10)<<4)+(timedat.month%10);
	IIC_WriteReg(DS3231_SLADDR,DS3231_MON_CEN,temp);
	temp = (((timedat.year-2000)/10)<<4)+(timedat.year%10);
	IIC_WriteReg(DS3231_SLADDR,DS3231_YEAR,temp);
}


/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/




