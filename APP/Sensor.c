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
#include "LCD_GUI.h"
#include "YXD19264D_51.h"
#include "DS18B20.h"
/****************************�궨��***********************************************/

/****************************��������*********************************************/
PAG_DATA sensor_data={0,0,0,0,0x02,0x44,0x01,0xff,};
TMPDAT	tmp_data={8888,8888};
char g_level_per=50;
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
//	SendString("temp2:\r\n");									//������Ϣʱ����
//	SendTemp(tmp_data.tmp2);
//	SendString("\r\n");
//	tmp_data.tmp1 = (uint)sensor_data.temp1_h<<8|(uint)sensor_data.temp1_l;
//	SendString("temp1:\r\n");
//	SendByteASCII(tmp_data.tmp1);
//	SendString("\r\n");
//	tmp_data.tmp2 = (uint)sensor_data.temp2_h<<8|(uint)sensor_data.temp2_l;
//	SendString("temp2:\r\n");
//	SendByteASCII(tmp_data.tmp2);
//	SendString("\r\n");
//	if(tmp_data.tmp1>5000) tmp_data.tmp1 = 8888;			//Ĭ��û�����µ������50��������ʾ����88.88��
	if(tmp_data.tmp2>5000) tmp_data.tmp2 = 8888;			//Ĭ��û�����µ������50��������ʾ����88.88��
}

/********************************************************************************
 * ��������PressDatHandle()
 * ����  ��ѹ�����ݴ���
 * ����  ��-
 * ����  ��-
 * ����  ��-
 ********************************************************************************/
uint PressDatHandle()
{
	uint press = (uint)sensor_data.press_h<<8|(uint)sensor_data.press_l;

	SendString("ADC data:\r\n");			
	SendByteASCII(sensor_data.press_h);
	SendByteASCII(sensor_data.press_l);
	SendString("\r\n");	
	
	return press;
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
	uint max=611,min=532,press;
	float k,b;
	
	press = PressDatHandle();
	k = -100.0/(max-min);
	b = -(k*max);
	g_level_per = (char)(k*press + b);
	
	if((sensor_data.possw&PEC100)==0)
	g_level_per =100;
	else if((sensor_data.possw&PEC0)==0)
	g_level_per =0;
	
	if(g_level_per<0) g_level_per = 0;
	if(g_level_per>100) g_level_per = 100;
	
	SendString("possw data:\r\n");			
	SendByteASCII(sensor_data.possw);
	SendString("\r\n");			
	SendString("flow data:\r\n");			
	SendByteASCII(sensor_data.flow);
	SendString("\r\n");	
}

/********************************************************************************
 * ��������SPI_ISR()
 * ����  ��SPI�жϷ�����
 * ����  ��-
 * ����  ��-
 * ����  ��-
 ********************************************************************************/

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/
