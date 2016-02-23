
/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：DS18B20.h
 * 描	述	：DS18B20驱动	应用于STC12C5A60S2	11.0592MHz
 *            
 * 实验平台	：自控水泵V1.0  60S2 11.0592MHz
 * 硬件连接	：
 * 版 	本	：V0.1.160214
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.1.28
 * 最后编辑	：2016.1.28
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

#ifndef _DS18B20_H__
#define _DS18B20_H__
/****************************包含头文件*******************************************/
#include "globaldefine.h"
#include "UART_51.h"

/****************************宏定义***********************************************/
//多传感器模式
#define MULTI_SENSOR					0

//REGISTER COMMANDS
#define REGISTER_9_BITS  0x1F
#define REGISTER_10_BITS 0x3F
#define REGISTER_11_BITS 0x5F
#define REGISTER_12_BITS 0x7F
//ROM COMMANDS
#define ROM_Search_Cmd   0xF0				 //搜索 ROM
#define ROM_Read_Cmd     0x33				 //读ROM
#define ROM_Match_Cmd    0x55				 //符合 ROM	多个
#define ROM_Skip_Cmd     0xCC				 //跳过 ROM
#define ROM_AlarmSearch_Cmd 0xEC			 //告警搜索命令
//DS18b20 FUNCTION COMMANDS
#define Convert_T         0x44
#define Write_Scratchpad  0x4E
#define Read_Scratchpad   0xBE
#define Copy_Scratchpad   0x48
#define Recall_EEPROM     0x88
#define Read_PowerSupply  0x84

/****************************变量声明*********************************************/
#if MULTI_SENSOR
extern uchar RomID[2][8];
#else
extern uchar RomID[1][8];
#endif

/****************************变量定义*********************************************/

/****************************函数声明*********************************************/
void delay_18B20(uint i);
void DS18B20_Init();
uchar ReadOneChar();
void WriteOneChar(uchar dat);
uint DS18B20_ReadTemperature(uchar NO);
char DS18B20_SearchRomID();
void DS18B20_Read_RomID(char *id);

void SendTemp(uint temp);
void Send_RomID();
void SendROMID(unsigned char n);


#endif

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


