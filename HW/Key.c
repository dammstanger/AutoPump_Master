/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��Key.c
 * ��	��	��STC 60S2 ��������
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.1.160216
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.2.16
 * ���༭	��2016.2.16
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "Key.h"
#include "UART_51.h"
#include "Time.h"
/****************************�궨��***********************************************/

/****************************��������***********************************************/
sbit k_home = P2^0;
sbit k_enter = P2^2;
sbit k_left = P2^1;
sbit k_right = P2^4;
sbit k_up = P5^0;
sbit k_down = P2^3;




/********************************************************************************
 * ��������Key_Scan()
 * ����  ������ɨ�裬һ��ֻ��ȡһ������ֵ
 * ����  ��-
 * ����  ������ֵ
 * ����  ��-
 ********************************************************************************/
char Key_Scan()
{
	if(!(k_home&k_enter&k_right&k_left&k_down&k_up))
	{
		delay100ms();
		if(!(k_home&k_enter&k_right&k_left&k_down&k_up))
		{
			if(!k_home) return KEY_HOME;
			else if(!k_enter) return KEY_ENTER;
			else if(!k_right) return KEY_R;
			else if(!k_left) return KEY_L;
			else if(!k_up) return KEY_UP;
			else if(!k_down) return KEY_DOWN;
		}
		return KEY_NONE;
	}
	return KEY_NONE;
}  
 

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/