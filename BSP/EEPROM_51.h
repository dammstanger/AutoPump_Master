/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��EEPROM.h
 * ��	��	��STC 60S2 Ƭ��EEPROM����
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.1.160218
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.2.18
 * ���༭	��2016.2.18
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/
#ifndef __EEPROM_51_H
#define __EEPROM_51_H
/****************************����ͷ�ļ�*******************************************/
#include "globaldefine.h"

/****************************�궨��***********************************************/
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


