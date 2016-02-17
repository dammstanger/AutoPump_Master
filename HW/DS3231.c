/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��DS3231.c
 * ��	��	��ʵʱʱ��DS3231����
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.0.160129
 * ������ϵ	��AutoPumpV2 
 * ��汾	����
 * ����ʱ��	��2016.1.29
 * ���༭	��2016.1.29
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "DS3231.h"
#include "UART_51.h"

/****************************�궨��***********************************************/
#define DS3231_SLADDR			0xD0 		//�ӻ���ַ		

#define DS3231_SEC				0x00 		//��
#define DS3231_MIN				0x01 		//��
#define DS3231_HOUR				0x02 		//ʱ
#define DS3231_WEEK				0x03 		//��
#define DS3231_DATE				0x04 		//��
#define DS3231_MON_CEN			0x05 		//��
#define DS3231_YEAR				0x06 		//��
#define DS3231_ALM1SEC			0x07 		//����1
#define DS3231_ALM1MIN			0x08 		//����1
#define DS3231_ALM1HOUR			0x09 		//����1
#define DS3231_ALM1DAY_WEK		0x0A 		//����1
#define DS3231_ALM2MIN			0x0B		//����2
#define DS3231_ALM2HOUR			0x0C 		//����2
#define DS3231_ALM2DAY_WEK		0x0D 		//����2
#define DS3231_CTL				0x0E 		//���ƼĴ���
#define DS3231_STA				0x0F 		//״̬�Ĵ���
#define DS3231_AOFFSET			0x10 		//�����ϻ�����
#define DS3231_TMP_H			0x11 		//�¶�8��λ bit7-0
#define DS3231_TMP_L			0x12 		//�¶�2��λ bit7 6

/****************************��������*********************************************/

/****************************��������*********************************************/
sbit DS3231_INT	= P3^4;
sbit DS3231_RST	= P3^5;

STC_TIME RTCtime={0,0,0,1,5,1,2016,0};
/****************************��������*********************************************/

/********************************************************************************
 * ��������DS3231_Init()
 * ����  ��DS3231��ʼ��
 * ����  ��
 * ����  ��-
 * ����  ��-
 ********************************************************************************/
void DS3231_Init(void)  
{
	IIC_WriteReg(DS3231_SLADDR,DS3231_CTL,0);		//��ʹ���жϣ����1kHz����
}


/********************************************************************************
 * ��������DS3231_ReadTime()
 * ����  ��DS3231��ʱ��
 * ����  ��
 * ����  ��-
 * ����  ��-
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
 * ��������DS3231_SetTime()
 * ����  ��DS3231����ʱ��
 * ����  ��
 * ����  ��-
 * ����  ��-
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




