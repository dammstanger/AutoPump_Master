/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��Time.c
 * ��	��	��STC 60S2 ��ʱ����ʱ����ʱ�亯��	11.0592MHz
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0	
 * Ӳ������	��
 * �� 	��	��V0.1.160217
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.2.17
 * ���༭	��2016.2.17
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "Time.h"

/****************************�궨��***********************************************/

/****************************��������***********************************************/
STR_TIM_M_S UpTimer1={0,0,0};


uchar Timer_reg=0;
uchar Timer1_cnt = 0;			//��С��λ1s,���ʱ255��
uchar Timer2_cnt = 0;
uchar Timer3_cnt = 0;
uchar Timer4_cnt = 0;
/********************************************************************************
 * ��������delay100ms()
 * ����  ��
 * ����  ��-
 * ����  ��
 * ����  ��-
 ********************************************************************************/
void delay100ms(void)   //
{
    unsigned char a,b,c;
    for(c=178;c>0;c--)
        for(b=120;b>0;b--)
            for(a=12;a>0;a--);
}

/********************************************************************************
 * ��������delay200ms()
 * ����  ��
 * ����  ��-
 * ����  ��
 * ����  ��-
 ********************************************************************************/
void delay200ms(void)   //��� -0.000000000028us
{
    unsigned char a,b,c;
    for(c=178;c>0;c--)
        for(b=230;b>0;b--)
            for(a=12;a>0;a--);
}

/********************************************************************************
 * ��������delay1s()
 * ����  ��
 * ����  ��-
 * ����  ��
 * ����  ��-
 ********************************************************************************/
void delay1s(void)   ////11.0592 STC1T ��� -0.000000000099us
{
    unsigned char a,b,c;
    for(c=217;c>0;c--)
        for(b=171;b>0;b--)
            for(a=73;a>0;a--);
}

	

/********************************************************************************
 * ��������ReadSoftTimer
 * ����  ����ȡ�����ʱ���Ĵ���
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
uchar ReadSoftTimer(uchar timerx)
{
	switch(timerx)
	{
		case TIMER_1	:{return (Timer_reg&TIMEUP_1);}
		case TIMER_2	:{return (Timer_reg&TIMEUP_2);}
		case TIMER_3	:{return (Timer_reg&TIMEUP_3);}	
		case TIMER_4	:{return (Timer_reg&TIMEUP_4);}	
		default	:break;					
	}
	return 0;
}

/********************************************************************************
 * ��������
 * ����  ��
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void SetSoftTimer(uchar timerx,long val)
{
	switch(timerx)
	{
		case TIMER_1	:{
			Timer_reg &=~0x01;Timer1_cnt = val;		//��ʱ������㣬��ʱ����ֵ
			}break;
		case TIMER_2	:{
			Timer_reg &=~0x02;Timer2_cnt = val;			
			}break;
		case TIMER_3	:{
			Timer_reg &=~0x04;Timer3_cnt = val;			
			}break;
		case TIMER_4	:{
			Timer_reg &=~0x08;Timer4_cnt = val;			
			}break;
		default	:break;			
	}
}

void SoftTimer()
{
	if(Timer1_cnt)	Timer1_cnt--;
	else 		Timer_reg |= 0x01;
	if(Timer2_cnt)	Timer2_cnt--;
	else 		Timer_reg |= 0x02;	
	if(Timer3_cnt)	Timer3_cnt--;
	else 		Timer_reg |= 0x04;
	if(Timer4_cnt)	Timer4_cnt--;
	else 		Timer_reg |= 0x08;
}


void SoftUpTimer_Reset(STR_TIM_M_S *timer)
{
	timer->working = 0;
	timer->sec = 0;
	timer->min = 0;
}

void SoftUpTimer_Start(STR_TIM_M_S *timer)
{
	if(timer->working!=1) timer->working = 1;
}


void SoftUpTimer_Stop(STR_TIM_M_S *timer)
{
	if(timer->working) timer->working = 0;
}

void SoftUpTimer(STR_TIM_M_S *timer)
{
	if(timer->working)
	{
		if(timer->sec==59)
		{
			timer->sec = 0;
			if(timer->min==255)
				timer->min = 0;
			else timer->min++;
		}
		timer->sec++;
	}
}

//�����ӣ���ȥ��ֵ
uchar SoftTimer_ReadMin(STR_TIM_M_S timer)
{
	return timer.min;
}
	
uint SoftTimer_ReadSec(STR_TIM_M_S timer)
{
	return timer.min*60+timer.sec;
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

