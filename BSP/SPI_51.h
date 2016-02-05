
/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：SPI_51.h
 * 描	述	：
 *                    
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：
 * 库版本		：
 * 创建时间	：2016.1.21
 * 最后编辑	：2016.1.21
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/
#ifndef __SPI_51_H
#define __SPI_51_H
/****************************包含头文件*******************************************/
#include "globaldefine.h"

//SPI模式
#define MASTER			1
#define SLAVER			0

//SPSTAT状态寄存器
#define SPIF				0x80
#define WCOL				0x40

//SPCTL寄存器
#define SSIG				0x80
#define SPEN				0x40
#define DORD				0x20
#define MSTR				0x10
#define CPOL				0x08
#define CPHA				0x04
//EI2
#define ESPI				0x02

// SPI总线速度设置 也就是设置预分频寄存器
#define SPI_SPEED_4   		0				//2.1MHz		11.0592MHz 
#define SPI_SPEED_16 		0x01			//700kHz
#define SPI_SPEED_64   		0x02			//175kHz
#define SPI_SPEED_128  		0x03			//86.5kHz

void SPI_Init(bit mode);			 	//初始化SPI口
void SPI_SetSpeed(bit SpeedSet); 		//设置SPI速度   
char SPI_ReadWriteByte(char dat);		//SPI总线读写一个字节
		 
#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


