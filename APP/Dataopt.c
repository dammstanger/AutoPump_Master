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

/****************************����ͷ�ļ�*******************************************/
#include "Dataopt.h"
#include "UART_51.h"
#include "EEPROM_51.h"
/****************************�궨��***********************************************/

/****************************��������***********************************************/
STR_EEPDAT const defaultdat={1,123456,1,30,100,0x28d,0x95,AUTO};		//Ĭ������
STR_EEPDAT g_savedat={0};

/********************************************************************************
 * ��������DAT_InitDat()
 * ����  �����ݳ�ʼ��
 * ����  ��-
 * ����  ��
 * ����  ���ⲿ����
 ********************************************************************************/
void DAT_InitDat(bool defaultset)
{
	if(defaultset)
	{
		g_savedat = defaultdat;
		DAT_SaveDat(S_ALL,g_savedat);
	}
	else
		DAT_LoadDat(&g_savedat);
	SendByteASCII(g_savedat.s_needkey);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_keyword>>24);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_keyword>>16);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_keyword>>8);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_keyword);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_baklit_on);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_startl);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_stopl);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_adcmax>>8);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_adcmax);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_adcmin>>8);
	SendString("\r\n");
	SendByteASCII(g_savedat.s_adcmin);
	SendString("\r\n");
	SendByteASCII(g_savedat.mode);
	SendString("\r\n");
}

/********************************************************************************
 * ��������DAT_LoadDat()
 * ����  ����EEPROM��������
 * ����  ��-
 * ����  ��
 * ����  ���������
 ********************************************************************************/
void DAT_LoadDat(STR_EEPDAT *dat)
{
	uchar buf[4],i;
	
	dat->s_needkey = IAP_ReadByte(EEPROM_SECT1);
	buf[0] = IAP_ReadByte(EEPROM_SECT1+1);		//���ֽ���ǰ
	buf[1] = IAP_ReadByte(EEPROM_SECT1+2);
	buf[2] = IAP_ReadByte(EEPROM_SECT1+3);
	buf[3] = IAP_ReadByte(EEPROM_SECT1+4);
	dat->s_keyword = 0;
	for(i=0;i<4;i++)
	{
		dat->s_keyword = dat->s_keyword<<8;
		dat->s_keyword += buf[i];
	}
	dat->s_baklit_on = IAP_ReadByte(EEPROM_SECT1+5);
	dat->s_startl = IAP_ReadByte(EEPROM_SECT1+6);
	dat->s_stopl = IAP_ReadByte(EEPROM_SECT1+7);
	buf[0] = IAP_ReadByte(EEPROM_SECT1+8);
	buf[1] = IAP_ReadByte(EEPROM_SECT1+9);
	dat->s_adcmax = (((uint)buf[0])<<8)+buf[1];
	buf[0] = IAP_ReadByte(EEPROM_SECT1+10);
	buf[1] = IAP_ReadByte(EEPROM_SECT1+11);
	dat->s_adcmin = (((uint)buf[0])<<8)+buf[1];
	dat->mode = IAP_ReadByte(EEPROM_SECT1+12);
}


/********************************************************************************
 * ��������DAT_SaveDat()
 * ����  ���������ݵ�EEPROM
 * ����  ��-
 * ����  ��
 * ����  ��-
 ********************************************************************************/
 void DAT_SaveDat(uchar item,STR_EEPDAT dat)
{
	STR_EEPDAT temp;
	if(item)
	{
		DAT_LoadDat(&temp);

		switch(item)
		{
			case S_KEYSW :{
				temp.s_needkey = dat.s_needkey;
			}break;
			case S_KEY :{
				temp.s_keyword = dat.s_keyword;
			}break;
			case S_BL :{
				temp.s_baklit_on = dat.s_baklit_on;
			}break;
			case S_STARTL :{
				temp.s_startl = dat.s_startl;
			}break;
			case S_STOPL :{
				temp.s_stopl = dat.s_stopl;
			}break;
			case S_ADCMAX :{
				temp.s_adcmax = dat.s_adcmax;
			}break;
			case S_ADCMIN :{
				temp.s_adcmin = dat.s_adcmin;
			}break;
			case S_MODE :{
				temp.mode = dat.mode;
			}break;
		}
	}
	else {
		temp = dat;					//�������ݶ�д��
	}
	//д��ǰ���������
	do
	{
		IAP_EraseSector(EEPROM_SECT1);
	}while(IAP_EEPROMCheck(SECTOR_1));
	//��������д��
	IAP_WriteByte(EEPROM_SECT1,temp.s_needkey);
	IAP_WriteByte(EEPROM_SECT1+1,temp.s_keyword>>24);
	IAP_WriteByte(EEPROM_SECT1+2,temp.s_keyword>>16);
	IAP_WriteByte(EEPROM_SECT1+3,temp.s_keyword>>8);
	IAP_WriteByte(EEPROM_SECT1+4,temp.s_keyword);
	IAP_WriteByte(EEPROM_SECT1+5,temp.s_baklit_on);
	IAP_WriteByte(EEPROM_SECT1+6,temp.s_startl);
	IAP_WriteByte(EEPROM_SECT1+7,temp.s_stopl);
	IAP_WriteByte(EEPROM_SECT1+8,temp.s_adcmax>>8);
	IAP_WriteByte(EEPROM_SECT1+9,temp.s_adcmax);
	IAP_WriteByte(EEPROM_SECT1+10,temp.s_adcmin>>8);
	IAP_WriteByte(EEPROM_SECT1+11,temp.s_adcmin);
	IAP_WriteByte(EEPROM_SECT1+12,temp.mode);
	SendString("EEPROM data is saved .\r\n");
	
}
/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/
