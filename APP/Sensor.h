/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��Sensor.h
 * ��	��	�����������ݴ���
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.1.160128
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.1.28
 * ���༭	��2016.1.28
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/
#ifndef __SENSOR_H_
#define __SENSOR_H_

/****************************����ͷ�ļ�*******************************************/

#include "globaldefine.h"

/****************************�궨��***********************************************/
//possw
#define PEC100		0x10u
#define PEC50		0x08u
#define PEC25		0x04u
#define PEC0		0x01u
#define FULL		0x20u		//����
//flow
#define HAS_FLOW	0x00u		//��ˮ��
#define NO_FLOW		0x01u		//ûˮ��
//switch
#define SW_ON			0u					//�Ե�ƽ���忪������ͨ��
#define SW_OFF			1u

/****************************���Ͷ���***********************************************/
typedef struct{
	uint tmp1;
	uint tmp2;
}TMPDAT;

extern TMPDAT tmp_data;
extern uchar g_level_per;

void TemperDatHandle(void);
uint LevelADCHandle(void);
void LevelDatHandle(void);

#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


