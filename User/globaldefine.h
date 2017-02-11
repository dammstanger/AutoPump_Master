/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：globaldefine.h
 * 描	述	：
 *                    
 * 实验平台	：AutoPumpV2.0
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

#ifndef __GLOBALDEFINE_H_
#define __GLOBALDEFINE_H_

/****************************包含头文件*******************************************/
#include <STC12C5A60S2.H>

/****************************宏定义***********************************************/
#define uchar	unsigned char 
#define uint	unsigned int
//
#define MANAUL		0x00
#define AUTO		0x01

#define ON			0x00
#define OFF			0x01
//----------------------
#define WORK		0x80
#define FREE		0x00

#define STATUS		0x07					//所有异常
#define OUTSIDE		0x01					//邻家抽水
#define SWOFF		0x02					//抽水开关错误
#define OFFLINE		0x04					//与检测单元离线
#define SLEEP		0x08					//晚间睡眠模式

//抽水超时时间值
#define OVERTIME_MIN	36u					//单位分钟
/****************************类型定义***********************************************/
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
/****************************变量定义*********************************************/
sbit LED1 = P2^6;
sbit LED2 = P5^1;

/****************************变量声明*********************************************/
extern char g_sysflag;
extern bit g_homepage;				//主界面标记，0界面1 =1界面2

#endif
/******************* (C) COPYRIGHT 2015 DammStanger *****END OF FILE************/
