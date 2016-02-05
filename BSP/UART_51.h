
/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：UART_51.h
 * 描	述	：
 *                    
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：
 * 库版本	：
 * 创建时间	：2016.1.22
 * 最后编辑	：2016.1.24
 **-------------------------------------------------------------------------------
 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#ifndef __UART_51_H
#define __UART_51_H

void UART_Init(void);
void SendOneByte(unsigned char dat);
void SendString(unsigned char *str);
void SendByteASCII(unsigned char dat);
void SendByteNum(unsigned char dat);

void UART1_SendStr(unsigned char len,unsigned char *str);


#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


