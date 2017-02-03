/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：Time.c
 * 描	述	：STC 60S2 延时，定时，等时间函数	11.0592MHz
 *            
 * 实验平台	：自控水泵V2.0	
 * 硬件连接	：
 * 版 	本	：V0.1.160217
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.2.17
 * 最后编辑	：2016.2.17
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "Time.h"

/****************************宏定义***********************************************/

/****************************变量定义***********************************************/
STR_TIM_M_S UpTimer1={0,0,0};


uchar Timer_reg=0;
uchar Timer1_cnt = 0;			//最小单位1s,最大定时255秒
uchar Timer2_cnt = 0;
uchar Timer3_cnt = 0;
uchar Timer4_cnt = 0;
/********************************************************************************
 * 函数名：delay100ms()
 * 描述  ：
 * 输入  ：-
 * 返回  ：
 * 调用  ：-
 ********************************************************************************/
void delay100ms(void)   //
{
    unsigned char a,b,c;
    for(c=178;c>0;c--)
        for(b=120;b>0;b--)
            for(a=12;a>0;a--);
}

/********************************************************************************
 * 函数名：delay200ms()
 * 描述  ：
 * 输入  ：-
 * 返回  ：
 * 调用  ：-
 ********************************************************************************/
void delay200ms(void)   //误差 -0.000000000028us
{
    unsigned char a,b,c;
    for(c=178;c>0;c--)
        for(b=230;b>0;b--)
            for(a=12;a>0;a--);
}

/********************************************************************************
 * 函数名：delay1s()
 * 描述  ：
 * 输入  ：-
 * 返回  ：
 * 调用  ：-
 ********************************************************************************/
void delay1s(void)   ////11.0592 STC1T 误差 -0.000000000099us
{
    unsigned char a,b,c;
    for(c=217;c>0;c--)
        for(b=171;b>0;b--)
            for(a=73;a>0;a--);
}

	

/********************************************************************************
 * 函数名：ReadSoftTimer
 * 描述  ：读取软件定时器寄存器
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
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
 * 函数名：
 * 描述  ：
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void SetSoftTimer(uchar timerx,long val)
{
	switch(timerx)
	{
		case TIMER_1	:{
			Timer_reg &=~0x01;Timer1_cnt = val;		//定时标记清零，计时器赋值
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

//读分钟，舍去秒值
uchar SoftTimer_ReadMin(STR_TIM_M_S timer)
{
	return timer.min;
}
	
uint SoftTimer_ReadSec(STR_TIM_M_S timer)
{
	return timer.min*60+timer.sec;
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

