/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：EEPROM.h
 * 描	述	：STC 60S2 片内EEPROM IAP（应用编程）驱动 （flash）
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.1.160218
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.2.18
 * 最后编辑	：2016.2.18
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "EEPROM_51.h"
#include "UART_51.h"
#include "intrins.h"
/****************************宏定义***********************************************/
//CMD寄存器
#define IAP_CMD_IDLE		0
#define IAP_CMD_READ		1
#define IAP_CMD_WRITE		2
#define IAP_CMD_ERASE		3

//定义EEPROM操作等待时间
#define IAP_CONTR_EN		0x83	//系统时钟<=12MHz


/****************************变量定义***********************************************/

/********************************************************************************
 * 函数名：IAP_Idle()
 * 描述  ：
 * 输入  ：-
 * 返回  ：
 * 调用  ：-内部使用
 ********************************************************************************/
void IAP_Idle()
{
	IAP_CONTR = 0;		
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0x80;
	IAP_ADDRL = 0;
}

/********************************************************************************
 * 函数名：IAP_ReadByte()
 * 描述  ：
 * 输入  ：-addr
 * 返回  ：
 * 调用  ：-
 ********************************************************************************/
uchar IAP_ReadByte(uint addr)
{
	uchar dat;
	IAP_CONTR = IAP_CONTR_EN;		
	IAP_CMD = IAP_CMD_READ;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr>>8;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();
 	_nop_();
	dat=IAP_DATA;
	
	IAP_Idle();
	return dat;
	
}


/********************************************************************************
 * 函数名：IAP_WriteByte()
 * 描述  ：
 * 输入  ：-addr
 * 返回  ：
 * 调用  ：-
 ********************************************************************************/
void IAP_WriteByte(uint addr,uchar dat)
{
	IAP_CONTR = IAP_CONTR_EN;		
	IAP_CMD = IAP_CMD_WRITE;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr>>8;
	
	IAP_DATA = dat;
	
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();
 	_nop_();
	
	IAP_Idle();
}


/********************************************************************************
 * 函数名：IAP_EraseSector()
 * 描述  ：
 * 输入  ：-addr
 * 返回  ：
 * 调用  ：-
 ********************************************************************************/
void IAP_EraseSector(uint addr)
{
	IAP_CONTR = IAP_CONTR_EN;		
	IAP_CMD = IAP_CMD_ERASE;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr>>8;
	
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();
 	_nop_();
	
	IAP_Idle();
}



/********************************************************************************
 * 函数名：IAP_EEPROMCheck()
 * 描述  ：
 * 输入  ：-
 * 返回  ：
 * 调用  ：-
 ********************************************************************************/
uchar IAP_EEPROMCheck(uchar sector)
{
	uint i,reval=0;
	if(sector&0x01)
	{
		for(i=0;i<512;i++)
		{
			if(IAP_ReadByte(EEPROM_SECT1+i)!=0xff)
			{
				SendString("EEPROM sector1 has data.\r\n");	
				reval = 1;
				break;
			}
		}	
	}
	if(sector&0x02)
	{
		for(i=0;i<512;i++)
		{
			if(IAP_ReadByte(EEPROM_SECT2+i)!=0xff)
			{
				SendString("EEPROM sector2 has data.\r\n");	
				reval |= 2;
				break;
			}
		}		
	}
	if(reval==0) SendString("room is clean.\r\n");		
	return reval;
}
/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/
