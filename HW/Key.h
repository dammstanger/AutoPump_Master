/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：Key.h
 * 描	述	：STC 60S2 按键驱动
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.1.160216
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.2.16
 * 最后编辑	：2016.2.16
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/
#ifndef __KYE_H
#define __KYE_H
/****************************包含头文件*******************************************/
#include "globaldefine.h"

/****************************宏定义***********************************************/
#define KEY_HOME		1
#define KEY_ENTER		2
#define KEY_UP			3
#define KEY_DOWN		4
#define KEY_L			5
#define KEY_R			6
#define KEY_NONE		0


char Key_Scan(void);
		 
#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


