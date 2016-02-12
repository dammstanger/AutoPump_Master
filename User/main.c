/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：main.c
 * 描	述	：液位自动控制项目程序
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.0.150711
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2015.7.11
 * 最后编辑	：2016.2.12
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "globaldefine.h"
#include "stdio.h"
#include "UART_51.h"
#include "SI4432.h"
#include "Transmit.h"
#include "DS3231.h"
#include "IIC_soft_51.h"
#include "YXD19264D_51.h"
#include "LCD_GUI.h"
#include "Sensor.h"
/****************************宏定义***********************************************/
#define P2_5	0x20
#define P2_7	0x80

/****************************变量声明*********************************************/

/****************************变量定义*********************************************/
//系统标志位
//                    		7		6		5		4		3		2		1		0		Reset Value
char g_sysflag = 0;		//	保留	保留	保留	保留	|		状态	|		模式	0000 0000
bit g_timeoverflg=0;
//---------------------------------------------
sbit START 	= P2^7;
sbit STOP	= P2^5;
sbit ALARM	= P4^0;
sbit TMP	= P4^4;
sbit CAPWAI	= P1^3;
sbit CAPNEI	= P1^4;

char temp;
char dat[] = "abc";
/****************************函数声明*********************************************/
void SendTemp(uint temp);





void delay200ms(void)   //误差 -0.000000000028us
{
    unsigned char a,b,c;
    for(c=178;c>0;c--)
        for(b=230;b>0;b--)
            for(a=12;a>0;a--);
}

void delay1s(void)   ////11.0592 STC1T 误差 -0.000000000099us
{
    unsigned char a,b,c;
    for(c=217;c>0;c--)
        for(b=171;b>0;b--)
            for(a=73;a>0;a--);
}

void EXTI0_Init()
{
	IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
    EX0 = 1;                        //enable INT0 interrupt
}


void main()
{
	delay1s();
	AUXR = AUXR|0x40;  	// T1, 1T Mode

//	IE2 |= ESPI;
	EXTI0_Init();                         
	UART_Init();
	SPI_Init(MASTER);
	IIC_GPIO_Config();
	LCD_GPIOInit();
	LCD_Init();
	delay1s();
	DS3231_Init();
	SI4432_Init();
	SI4432_SetRxMode();	//接收模式
	//-----------------------------------------------------
	EA = 1;								//注意：外设初始化完再开中断！
	while(1)
	{	
		//按键扫描----------------------------------------------
		
		//确定系统状态------------------------------------------
		g_sysflag &= ~STATUS; 
		if(CAPWAI==1)
			g_sysflag |= FREE;						//空闲
		else if(CAPNEI==1&&g_timeoverflg==0)		//自家抽水
			g_sysflag |= WORK_SELF;
		else if(CAPNEI==1&&g_timeoverflg==1)		//水通在别家
			g_sysflag |= SWOFF;
		else if(CAPNEI==0)							//别家抽水
			g_sysflag |= WORK_OUT;
		
		//-----------------------------------------------	
		switch(g_sysflag&STATUS)
		{
			case FREE :{
				GUI_HomePage();
				while(1)
				{
					//发送采集数据指令
//					DATA_Cmd_Pkg_Send();
					if(Trans_RevPakFin)
					{
						Trans_RevPakFin = 0;
						if(2==Pak_Handle())
						{
							SendString("valid data received.\r\n");		//调试信息时候用
							PressDatHandle();				
							TemperDatHandle();
						}
					}
					LCD_Clear();		//清屏

					DS3231_Init();
					LED1 = 0;
					delay1s();
					delay1s();
					delay1s();
					LCD_Clear();		//清屏
					GUI_HomePage2();
					LED1 = 1;
					delay1s();
					delay1s();
					delay1s();
				}//end of while
			}break;
			case WORK_SELF :{
				
			}break;
			case WORK_OUT :{
				
			}break;
			case SWOFF :{
				
			}break;		}

	}
}

void SendTemp(uint temp)
{		
		SendByteNum(temp/10000);			// 百位显示值
		SendByteNum(temp/1000%10);			// 十位显示值
		SendByteNum(temp/100%10);			// 个位显示值
		SendOneByte('.');
		SendByteNum(temp/10%10);			//十分位显示值
		SendByteNum(temp%10);				//百分位显示值
}


void EXTI0_ISR() interrupt 0 
{	//响应后IE0自动清除
	SI4432_ISR();
//	SendString("ISR completed.\r\n");					//调试信息时候用	
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

