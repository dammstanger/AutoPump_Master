/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��Sensor.c
 * ��	��	���Դ��������ݽ��д����Ӧ��
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.1.160128
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.1.28
 * ���༭	��2016.1.28
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "globaldefine.h"
#include "ADC_51.h"
#include "UART_51.h"
#include "Transmit.h"
#include "Sensor.h"
#include "dataopt.h"
#include "LCD_GUI.h"
#include "YXD19264D_51.h"
#include "DS18B20.h"
/****************************�궨��***********************************************/

/****************************��������*********************************************/
PAG_DATA sensor_data={0,0,0,0,0x02,0x44,0x01,0xff,};
TMPDAT	tmp_data={8888,8888};
uchar g_level_per=50;
/********************************************************************************
 * ��������TemperDatHandle()
 * ����  ���¶����ݴ���
 * ����  ��-
 * ����  ��-
 * ����  ��-
 ********************************************************************************/
void TemperDatHandle()
{
	tmp_data.tmp2 = DS18B20_ReadTemperature(1);
	SendString("temp2:\r\n");									//������Ϣʱ����
	SendTemp(tmp_data.tmp2);
	SendString("\r\n");
//	tmp_data.tmp1 = (uint)sensor_data.temp1_h<<8|(uint)sensor_data.temp1_l;
//	SendString("temp1:\r\n");
//	SendByteASCII(tmp_data.tmp1);
//	SendString("\r\n");
//	tmp_data.tmp2 = (uint)sensor_data.temp2_h<<8|(uint)sensor_data.temp2_l;
//	SendString("temp2:\r\n");
//	SendByteASCII(tmp_data.tmp2);
//	SendString("\r\n");
//	if(tmp_data.tmp1>5000) tmp_data.tmp1 = 8888;		//Ĭ��û�����µ������50��������ʾ����88.88��
	if(tmp_data.tmp2>5000) tmp_data.tmp2 = 8888;			//Ĭ��û�����µ������50��������ʾ����88.88��
}

/********************************************************************************
 * ��������LevelADCHandle()
 * ����  ��Һλģ�������ݴ���
 * ����  ��-
 * ����  ��-
 * ����  ��-
 ********************************************************************************/
uint LevelADCHandle()
{
	SendString("ADC data:\r\n");			
	SendByteASCII(sensor_data.press_h);
	SendByteASCII(sensor_data.press_l);
	SendString("\r\n");	
	
	return (uint)sensor_data.press_h<<8|(uint)sensor_data.press_l;;
}	


/********************************************************************************
 * ��������LevelDatHandle()
 * ����  ��Һλ���ݴ���
 * ����  ��-
 * ����  ��-
 * ����  ��-
 ********************************************************************************/
void LevelDatHandle()
{
	static bool flg_ful_emty = 0;
	static uint max=0x3ff,min=0x9a;
	uint level_adc;
	
	float k,b;

	//��ȡҺλģ����
	level_adc = LevelADCHandle();
	
	//ͨ��Һλ����У׼Һλ������
	if((sensor_data.possw&PEC100)==0)
	{
		g_level_per =100;
		if(!flg_ful_emty)
		{
			flg_ful_emty = 1;
			max = level_adc;
			g_savedat.s_adcmax = level_adc;
			DAT_SaveDat(S_ADCMAX,g_savedat);
		}
	}
	else if((sensor_data.possw&PEC0)==0)			//��Һλ����δ���
	{
		g_level_per =0;
		if(!flg_ful_emty)
		{
			flg_ful_emty = 1;
			min = level_adc;
			g_savedat.s_adcmin = level_adc;
			DAT_SaveDat(S_ADCMIN,g_savedat);
		}
	}
	else {																		//
		flg_ful_emty = 0;
		k = 100.0/(max-min);
		b = (k*min);
		g_level_per = (uchar)(k*level_adc + b);
		if(g_level_per<0) g_level_per = 0;
		if(g_level_per>100) g_level_per = 100;
	}
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/
