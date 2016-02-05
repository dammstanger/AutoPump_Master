/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：Transmit.h
 * 描	述	：RTU通信协议,从机端
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.1.160124
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.1.24
 * 最后编辑	：2016.1.25
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

#ifndef __TRANSMIT_H_
#define __TRANSMIT_H_

/****************************包含头文件*******************************************/

#include "UART_51.h"
#include "globaldefine.h"
/****************************类型定义*********************************************/
//typedef struct{
//	uchar temp_h;
//	uchar temp_l;
//	uchar press_h;
//	uchar press_l;
//	uchar flow;
//	uchar possw;
//}PAG_DATA;						//已放入glogaldefiine.h
extern PAG_DATA *pag_data;

/****************************变量声明*********************************************/

/****************************宏定义***********************************************/

#define TransPkgSend()			\
{								\
	Sendbuf[0] = 0x55;			\
	Sendbuf[1] = 0xaa;			\
	SI4432_FIFO_SendData(12,Sendbuf);	\
}

#define PAG_SEDSIZE				12
#define PAG_REVSIZE				12

//----------包头偏移地址----------------
#define PAG_HEAD_ADDR_H			0x00
#define PAG_HEAD_ADDR_L			0X01
//----协议标准值
#define PAG_HEAD_H				0x55
#define PAG_HEAD_L				0xAA

#define Is_Valid_Cmd_Pkg()		((*(Revbuf+PAG_HEAD_ADDR_H)==PAG_HEAD_H) && (*(Revbuf+PAG_HEAD_ADDR_L)==PAG_HEAD_L))		

//----------指令偏移地址----------------
#define PAG_CMD_ADDR_H			0x02
#define PAG_CMD_ADDR_L			0x03
//----指令类型
//
#define PAG_DATA_CMD			0x1000		//数据
#define PAG_DATA_CMD_H			0x10
#define PAG_DATA_CMD_L			0x00

#define PAG_DATA_ACK			0x1100
#define PAG_DATA_ACK_H			0x11
#define PAG_DATA_ACK_L			0x00

//#define PAG_TEMP_CMD			0x1000		//温度
//#define PAG_TEMP_CMD_H			0x10
//#define PAG_TEMP_CMD_L			0x00

//#define PAG_TEMP_ACK			0x1100
//#define PAG_TEMP_ACK_H			0x11
//#define PAG_TEMP_ACK_L			0x00
//
//#define PAG_PRESS_CMD			0x2000			//压力	
//#define PAG_PRESS_CMD_H			0x20
//#define PAG_PRESS_CMD_L			0x00

//#define PAG_PRESS_ACK			0x2100
//#define PAG_PRESS_ACK_H			0x21
//#define PAG_PRESS_ACK_L			0x00
////
//#define PAG_FLOW_CMD			0x3000			//流量
//#define PAG_FLOW_CMD_H			0x30
//#define PAG_FLOW_CMD_L			0x00

//#define PAG_FLOW_ACK			0x3100
//#define PAG_FLOW_ACK_H			0x31
//#define PAG_FLOW_ACK_L			0x00
////
//#define PAG_POSSW_CMD			0x4000			//位置开关
//#define PAG_POSSW_CMD_H			0x40
//#define PAG_POSSW_CMD_L			0x00

//#define PAG_POSSW_ACK			0x4100
//#define PAG_POSSW_ACK_H			0x41
//#define PAG_POSSW_ACK_L			0x00


//----------数据偏移地址----------------
#define PAG_TEMP_ADDR_H			0X04
#define PAG_TEMP_ADDR_L			0x05
#define PAG_PRESS_ADDR_H		0X06
#define PAG_PRESS_ADDR_L		0x07
#define PAG_FLOW_ADDR			0X08
#define PAG_POSSW_ADDR			0x09

#define Is_DATA_Cmd_Pkg()		((*(Revbuf+PAG_CMD_ADDR_H)==PAG_DATA_CMD_H) && (*(Revbuf+PAG_CMD_ADDR_L)==PAG_DATA_CMD_L))		
#define Is_DATA_Ack_Pkg()		((*(Revbuf+PAG_CMD_ADDR_H)==PAG_DATA_ACK_H) && (*(Revbuf+PAG_CMD_ADDR_L)==PAG_DATA_ACK_L))		

#define DATA_Cmd_Pkg_Send()						\
{												\
	*(Sendbuf+PAG_CMD_ADDR_H) = PAG_DATA_CMD_H;	\
	*(Sendbuf+PAG_CMD_ADDR_L) = PAG_DATA_CMD_L;	\
	TransPkgSend();								\
}

#define DATA_Cmd_Handle()										\
{																\
}

#define DATA_Cmd_ACK()									\
{														\
	*(Sendbuf+PAG_CMD_ADDR_H) = 	PAG_DATA_ACK_H;	\
	*(Sendbuf+PAG_CMD_ADDR_L) = 	PAG_DATA_ACK_L;	\
	*(Sendbuf+PAG_TEMP_ADDR_H) = 	pag_data->temp_h;			\
	*(Sendbuf+PAG_TEMP_ADDR_L) = 	pag_data->temp_l;			\
	*(Sendbuf+PAG_PRESS_ADDR_H) = 	pag_data->press_h;			\
	*(Sendbuf+PAG_PRESS_ADDR_L) = 	pag_data->press_l;			\
	*(Sendbuf+PAG_FLOW_ADDR) = 		pag_data->flow;				\
	*(Sendbuf+PAG_POSSW_ADDR) = 	pag_data->possw;			\
	TransPkgSend();													\
}

/****************************结构体定义*******************************************/

/****************************变量声明*********************************************/
extern uchar Sendbuf[PAG_SEDSIZE];
extern uchar Revbuf[PAG_REVSIZE];
extern uchar Trans_RevPakFin;
extern uchar Trans_RevPakErr;

/****************************函数声明*********************************************/
void PakRev_OverTimeDeal(void);
uchar PakRev_BufHandle(uchar dat);
uchar Pak_Handle(void);

#endif
/******************* (C) COPYRIGHT 2014 DammStanger *****END OF FILE************/

