/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：LCD_GUI.h
 * 描	述	：GUI设计
 *                    
 * 实验平台	：AutoPumpV2.0
 * 硬件连接	：
 * 版 	本	：V0.0.160217
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2015.7.14
 * 最后编辑	：2016.2.17
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

#ifndef __LCD_GUI_H_
#define __LCD_GUI_H_

/****************************包含头文件*******************************************/
#include "globaldefine.h"
/****************************宏定义***********************************************/

/****************************类型定义*********************************************/

/****************************变量声明*********************************************/
extern uchar code Pic_arrow[];
extern uchar code Pic_xuejie[];
extern uchar g_keycmd;
/****************************变量定义*********************************************/

/****************************函数声明*********************************************/
void LCD_FlashArrows(bool enable);
void GUI_Operation(char keyval);
char GUI_DisplayPassword(void);
void GUI_Setting(void);
void GUI_ModeSetting(void);
void GUI_SRARTLSetting(void);
void GUI_STOPLSetting(void);
void GUI_DATETIMESetting(void);
void GUI_KEYSetting(void);
void GUI_BLSetting(void);
void GUI_DEFAULTSetting(void);
void GUI_History(void);
void GUI_CaseData_Dis(char percent,char refill);
void GUI_HomePage(void);
void GUI_HomePage2(void);
void GUI_HomePageUpdate(void);
void GUI_HomePage2Update(void);
#endif

/******************** (C) COPYRIGHT 2015 DammStanger *****************************************/
