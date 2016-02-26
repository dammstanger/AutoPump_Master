/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��Dataopt.c
 * ��	��	��STC 60S2 �������ݱ������ȡ
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.1.160224
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.2.24
 * ���༭	��2016.2.24
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/
#ifndef __DATAOPT_H
#define __DATAOPT_H
/****************************����ͷ�ļ�*******************************************/
#include "globaldefine.h"

/****************************�궨��***********************************************/
#define S_ALL				0
#define S_KEYSW				1
#define S_KEY				2
#define S_BL				3
#define S_STARTL			4
#define S_STOPL				5
#define S_ADCMAX			6
#define S_ADCMIN			7
#define S_MODE				8
/****************************���Ͷ���*********************************************/
typedef struct{
	uchar s_needkey;
	unsigned long s_keyword;
	uchar s_baklit_on;
	uchar s_startl;
	uchar s_stopl;
	uint s_adcmax;
	uint s_adcmin;
	uchar mode;
}STR_EEPDAT;
/****************************��������*********************************************/
 extern STR_EEPDAT g_savedat;
extern STR_EEPDAT const defaultdat;
/****************************��������*********************************************/

/****************************��������*********************************************/
void DAT_SaveDat(uchar item,STR_EEPDAT dat);
void DAT_LoadDat(STR_EEPDAT *dat);
void DAT_InitDat(bool defaultset);



#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


