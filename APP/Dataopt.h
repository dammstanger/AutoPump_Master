/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：Dataopt.c
 * 描	述	：STC 60S2 闪存数据保存与读取
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.1.160224
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.2.24
 * 最后编辑	：2016.2.24
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/
#ifndef __DATAOPT_H
#define __DATAOPT_H
/****************************包含头文件*******************************************/
#include "globaldefine.h"

/****************************宏定义***********************************************/
#define S_ALL				0
#define S_KEYSW				1
#define S_KEY				2
#define S_BL				3
#define S_STARTL			4
#define S_STOPL				5
#define S_ADCMAX			6
#define S_ADCMIN			7
#define S_MODE				8
/****************************类型定义*********************************************/
typedef struct{
	uchar s_needkey;									//设置需要密码
	unsigned long s_keyword;					//密码值
	uchar s_baklit_on;								//背光
	uchar s_startl;										//自动启动抽水的液位0-100
	uchar s_stopl;										//自动停止抽水的液位
	uint s_adcmax;										//模拟液位信号的最大值，
	uint s_adcmin;										//模拟液位信号的最小值，
	uchar mode;												//操作模式：自动/手动
}STR_EEPDAT;												//EEPROM存储的数据结构体
/****************************变量声明*********************************************/
extern STR_EEPDAT g_savedat;
extern STR_EEPDAT const defaultdat;
/****************************变量定义*********************************************/

/****************************函数声明*********************************************/
void DAT_SaveDat(uchar item,STR_EEPDAT dat);
void DAT_LoadDat(STR_EEPDAT *dat);
void DAT_InitDat(bool defaultset);



#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


