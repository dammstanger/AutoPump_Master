/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：globaldefine.h
 * 描	述	：
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

#ifndef __GLOBALDEFINE_H_
#define __GLOBALDEFINE_H_

/****************************包含头文件*******************************************/
#include <STC12C5A60S2.H>

/****************************宏定义***********************************************/
#define uchar	unsigned char 
#define uint	unsigned int

#define MANAUL		0x00
#define AUTO		0x01
//----------------------
#define STATUS		0x0e
#define FREE		0x00
#define WORK_SELF	0x02
#define WORK_OUT	0x04
#define SWOFF		0x06

/****************************类型定义***********************************************/
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/****************************变量定义*********************************************/
sbit LED1 = P2^6;
sbit LED2 = P5^1;

/****************************变量声明*********************************************/
extern char g_sysflag;
#endif
/******************* (C) COPYRIGHT 2015 DammStanger *****END OF FILE************/
