/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：EEPROM.h
 * 描	述	：STC 60S2 片内EEPROM驱动
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.1.160218
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.2.18
 * 最后编辑	：2016.2.18
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/
#ifndef __EEPROM_51_H
#define __EEPROM_51_H
/****************************包含头文件*******************************************/
#include "globaldefine.h"

/****************************宏定义***********************************************/
#define EEPROM_ADDR		0x0000
#define EEPROM_SECT1	0x0000
#define EEPROM_SECT2	0x0200

#define SECTOR_1		0x01
#define SECTOR_2		0x02
#define SECTOR_ALL		0x03

uchar IAP_ReadByte(uint addr);
void IAP_WriteByte(uint addr,uchar dat);
void IAP_EraseSector(uint addr);
uchar IAP_EEPROMCheck(uchar sector);

	 
#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


