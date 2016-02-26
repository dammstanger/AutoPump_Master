/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：Sensor.h
 * 描	述	：传感器数据处理
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
#ifndef __SENSOR_H_
#define __SENSOR_H_

/****************************包含头文件*******************************************/

#include "globaldefine.h"

/****************************宏定义***********************************************/
//possw
#define PEC100		0x10
#define PEC50		0x08
#define PEC25		0x04
#define PEC0		0x01
#define FULL		0x20		//满了
//flow
#define HAS_FLOW	0x00		//有水出
#define NO_FLOW		0x01		//没水出
/****************************类型定义***********************************************/
typedef struct{
	uint tmp1;
	uint tmp2;
}TMPDAT;

extern TMPDAT tmp_data;

void TemperDatHandle(void);
void PressDatHandle(void);
void LevelDatHandle(void);

#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


