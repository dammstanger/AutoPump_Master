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
#include "LCD_GUI.h"
#include "YXD19264D_51.h"
#include "DS18B20.h"
/****************************宏定义***********************************************/

/****************************变量定义*********************************************/
PAG_DATA sensor_data={0,0,0,0,0x02,0x44,0x01,0xff,};
TMPDAT	tmp_data={8888,8888};

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
//	if(tmp_data.tmp1>5000) tmp_data.tmp1 = 8888;			//默认没有零下的情况，50度以上显示错误88.88度
	if(tmp_data.tmp2>5000) tmp_data.tmp2 = 8888;			//默认没有零下的情况，50度以上显示错误88.88度
}

/********************************************************************************
 * 函数名：PressDatHandle()
 * 描述  ：压力数据处理
 * 输入  ：-
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void PressDatHandle()
{
	uint press = (uint)sensor_data.press_h<<8|(uint)sensor_data.press_l;
	uint max=0,min=0;
	
	press = press*100/(max-min);
	
//	if(FREE==(g_sysflag&STATUS)) GUI_CaseData_Dis(press,1);
//	else GUI_CaseData_Dis(press,0);
	
	SendString("ADC data:\r\n");			
	SendByteASCII(sensor_data.press_h);
	SendByteASCII(sensor_data.press_l);
	SendString("\r\n");	
//	SendString("POS data:\r\n");			
//	SendByteASCII(sensor_data.possw);
//	SendString("\r\n");
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
	SendString("possw data:\r\n");			
	SendByteASCII(sensor_data.possw);
	SendString("\r\n");			
	SendString("flow data:\r\n");			
	SendByteASCII(sensor_data.flow);
	SendString("\r\n");	
}

/********************************************************************************
 * 函数名：SPI_ISR()
 * 描述  ：SPI中断服务函数
 * 输入  ：-
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/
