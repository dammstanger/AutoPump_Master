/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：Sensor.c
 * 描	述	：对传感器数据进行处理的应用
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.1.160128
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.1.28
 * 最后编辑	：2016.1.28
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "globaldefine.h"
#include "ADC_51.h"
#include "UART_51.h"
#include "Transmit.h"
#include "Sensor.h"
#include "dataopt.h"
#include "LCD_GUI.h"
#include "YXD19264D_51.h"
#include "DS18B20.h"
/****************************宏定义***********************************************/

/****************************变量定义*********************************************/
PAG_DATA sensor_data={0,0,0,0,0x02,0x44,0x01,0xff,};
TMPDAT	tmp_data={8888,8888};
uchar g_level_per=50;
/********************************************************************************
 * 函数名：TemperDatHandle()
 * 描述  ：温度数据处理
 * 输入  ：-
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void TemperDatHandle()
{
	tmp_data.tmp2 = DS18B20_ReadTemperature(1);
	SendString("temp2:\r\n");									//调试信息时候用
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
//	if(tmp_data.tmp1>5000) tmp_data.tmp1 = 8888;		//默认没有零下的情况，50度以上显示错误88.88度
	if(tmp_data.tmp2>5000) tmp_data.tmp2 = 8888;			//默认没有零下的情况，50度以上显示错误88.88度
}

/********************************************************************************
 * 函数名：LevelADCHandle()
 * 描述  ：液位模拟量数据处理
 * 输入  ：-
 * 返回  ：-
 * 调用  ：-
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
 * 函数名：LevelDatHandle()
 * 描述  ：液位数据处理
 * 输入  ：-
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void LevelDatHandle()
{
	static bool flg_ful_emty = 0;
	static uint max=0x3ff,min=0x9a;
	uint level_adc;
	
	float k,b;

	//获取液位模拟量
	level_adc = LevelADCHandle();
	
	//通过液位开关校准液位传感器
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
	else if((sensor_data.possw&PEC0)==0)			//零液位点暂未检测
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
