/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��EEPROM.h
 * ��	��	��STC 60S2 Ƭ��EEPROM IAP��Ӧ�ñ�̣����� ��flash��
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.1.160218
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.2.18
 * ���༭	��2016.2.18
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "EEPROM_51.h"
#include "UART_51.h"
#include "intrins.h"
/****************************�궨��***********************************************/
//CMD�Ĵ���
#define IAP_CMD_IDLE		0
#define IAP_CMD_READ		1
#define IAP_CMD_WRITE		2
#define IAP_CMD_ERASE		3

//����EEPROM�����ȴ�ʱ��
#define IAP_CONTR_EN		0x83	//ϵͳʱ��<=12MHz


/****************************��������***********************************************/

/********************************************************************************
 * ��������IAP_Idle()
 * ����  ��
 * ����  ��-
 * ����  ��
 * ����  ��-�ڲ�ʹ��
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
 * ��������IAP_ReadByte()
 * ����  ��
 * ����  ��-addr
 * ����  ��
 * ����  ��-
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
 * ��������IAP_WriteByte()
 * ����  ��
 * ����  ��-addr
 * ����  ��
 * ����  ��-
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
 * ��������IAP_EraseSector()
 * ����  ��
 * ����  ��-addr
 * ����  ��
 * ����  ��-
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
 * ��������IAP_EEPROMCheck()
 * ����  ��
 * ����  ��-
 * ����  ��
 * ����  ��-
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
