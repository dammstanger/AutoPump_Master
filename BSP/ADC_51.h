/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：ADC_51.h
 * 描	述	：STC 60S2 ADC外设驱动
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.1.160128
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.1.28
 * 最后编辑	：2016.1.28
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/
#ifndef __ADC_51_H
#define __ADC_51_H
	
/****************************包含头文件*******************************************/
	  	    	
/****************************宏定义***********************************************/
//P1ASF
#define AFPORT_P1_0		0x01		//通道0所在的P1.0口复用
//ADC_CONTR
#define ADC_PWR			0x80
#define ADC_SPEED_90T	0x60		//90个时钟周期转换一次
#define ADC_SPEED_180T	0x40		//180个时钟周期转换一次
#define ADC_SPEED_360T	0x20		//360个时钟周期转换一次
#define ADC_SPEED_540T	0x00		//540个时钟周期转换一次
#define ADC_SPEED		ADC_SPEED_540T
#define ADC_FLAG		0x10		//完成标志位，有软件清零
#define ADC_SRART		0x08		//启动转换，结束后为0
#define ADC_CH0			0x00		//通道0	P1.0
#define ADC_CH1			0x01		//通道1 P1.1
#define ADC_CH2			0x02		//通道2 P1.2
#define ADC_CH3			0x03		//通道3 P1.3
#define ADC_CH4			0x04		//通道4 P1.4
#define ADC_CH5			0x05		//通道5 P1.5
#define ADC_CH6			0x06		//通道6 P1.6
#define ADC_CH7			0x07		//通道7 P1.7

					
void ADC_Init(void);
void ADC_STARTCOV(char ch,char speed);

#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


