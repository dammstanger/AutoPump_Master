/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：IIC_soft_51.c
 * 描	述	：模拟IIC
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.0.160129
 * 从属关系	：AutoPumpV2 
 * 库版本	：无
 * 创建时间	：2016.1.29
 * 最后编辑	：2016.1.29
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

#ifndef _IIC_SOFT_51_H__
#define _IIC_SOFT_51_H__
/****************************包含头文件*******************************************/
#include "globaldefine.h"
/****************************宏定义***********************************************/

/****************************类型定义*********************************************/

/****************************变量声明*********************************************/

/****************************函数声明*********************************************/
void IIC_GPIO_Config(void);
bool IIC_WriteReg(uchar slaveAddr,uchar REG_Address,uchar REG_data);
uchar IIC_ReadReg(uchar slaveAddr, uchar readAddr);
uchar IIC_MultRead(uchar slaveAddr, uchar* pBuffer, uchar readAddr, uchar NumByteToRead);
uchar IIC_MultWrite(uchar slaveAddr, uchar* pBuffer, uchar wrtieAddr, uchar NumByteToWrite);





#endif

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

