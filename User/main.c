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
#include "Key.h"
#include "Time.h"
#include "EEPROM_51.h"
#include "Menu.h"
#include "DS18B20.h"
#include "Dataopt.h"
/****************************宏定义***********************************************/
#define P2_5	0x20
#define P2_7	0x80

/****************************变量声明*********************************************/

/****************************变量定义*********************************************/
//系统标志位
//                    		7			6		5		4		3		2		1		0		Reset Value
char g_sysflag = 0;		//	工作指示	保留	保留	保留	|		状态			|		0000 0000
bit g_timeoverflg=0;	//抽水超时标志位
bit g_offlineflag=0;	//离线标志位
bit g_1stimer_1 = 0;
bit g_timer_flow_start = 0;
//---------------------------------------------
sbit START 	= P2^7;
sbit STOP	= P2^5;
sbit ALARM	= P4^0;
sbit TMP	= P4^4;
sbit CAPWAI	= P1^3;				//外部检测
sbit CAPNEI	= P1^4;				//内部检测

char temp;
char dat[] = "abc";

#define PUMP_ON 	START = 0;delay200ms();START = 1
#define PUMP_OFF 	STOP = 0;delay200ms();STOP = 1

/****************************函数声明*********************************************/
void DataAqurie();


void EXTI0_Init()
{
	IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
    EX0 = 1;                        //enable INT0 interrupt
}

void T0_EXT_Init()
{
	WAKE_CLKO = 0x10;               //将Timer0的中断用于外部下降沿中断
    ET0 = 1;                        //enable T0 interrupt
}

void main()
{
	char key;
	uchar i=0;
	delay1s();

	AUXR = AUXR|0x40;  	// T1, 1T Mode

	UART_Init();
//	IAP_EraseSector(EEPROM_SECT1);		//擦除EEPROM
//	IAP_EraseSector(EEPROM_SECT2);
	if(IAP_EEPROMCheck(SECTOR_ALL))		//有数据，不为空
		DAT_InitDat(FALSE);
	else
		DAT_InitDat(TRUE);				//无数据，设为默认值
	EXTI0_Init();     
	T0_EXT_Init();	
	SPI_Init(MASTER);
	IIC_GPIO_Config();

	P4SW |= 0x10;			//18B20
	SendString("ROMID:\r\n");
	DS18B20_Read_RomID(*RomID);
	SendROMID(1);
	SendString("\r\n");
	DS18B20_ReadTemperature(1);
	
	LCD_GPIOInit();	
	LCD_Init();
	delay1s();
	DS3231_Init();
//	DS3231_SetTime(RTCtime);
	SI4432_Init();
	SI4432_SetRxMode();	//接收模式
	//-----------------------------------------------------
	IE0 = 0;
	IE1 = 0;
	TI = 0;
	EA = 1;				//注意：外设初始化完再开中断！
	SetSoftTimer(TIMER_2,5);		//设置主界面滚动延时
	GUI_HomePage();
	
	while(1)
	{
		if(g_menumark==0)
		{
			//按键扫描----------------------------------------------
			key = Key_Scan();
			//处理键值-----------------------------------------------	
			if(key!=KEY_NONE)
			{
				if(g_savedat.s_baklit_on)
					LCD_BL_ON;
				g_menumark = MENU_OPERATE;
			}	
		}
		else
		{
			key = 0xff;
		}
		//查询并进入界面				
		switch(g_menumark)
		{
			case MENU_OPERATE 	:{
				GUI_Operation(key);
				SetSoftTimer(TIMER_2,5);		//设置主界面滚动延时
				}break;
			case MENU_KEYINPUT 	:{GUI_DisplayPassword();}break;
			case MENU_HISTORY :{
				GUI_History();
			}break;
		}
		//2级查询
		switch(g_menumark)
		{
			case MENU_SET:{GUI_Setting();}break;
		}
		//3级查询
		switch(g_menumark)
		{
			case MENU_MODE :{GUI_ModeSetting();}break;
			case MENU_STARTL :{GUI_SRARTLSetting();}break;
			case MENU_STOPL :{GUI_STOPLSetting();}break;
			case MENU_DATETIME :{ GUI_DATETIMESetting();}break;
			case MENU_KEYSET :{GUI_KEYSetting();}break;
			case MENU_BLSET :{GUI_BLSetting();}break;
			case MENU_DEFSET :{GUI_DEFAULTSetting();}break;
		}
		
		//确定系统状态------------------------------------------
		if(CAPNEI==1)
		{
			g_sysflag |= FREE;						//空闲
			
		}
		else
		{
			g_sysflag |= WORK;						//工作
			g_timeoverflg = 0;
		}
		//-------------------------
		if(g_sysflag&WORK&&sensor_data.flow==NO_FLOW&&g_timeoverflg==0)
		{
			if(g_timer_flow_start==0)
			{
				g_timer_flow_start = 1;
				SetSoftTimer(TIMER_4,10);				//设置上水延时30s
			}
			else
			{
				if(ReadSoftTimer(TIMER_2))
				{
					g_timer_flow_start = 0;
					g_timeoverflg = 1;
				}
			}		
		}
		else{
			g_timer_flow_start = 0;
		}
		//-------------------------
		if(g_offlineflag)							//首先判断是否离线
			g_sysflag |= OFFLINE;
		else if((sensor_data.flow==NO_FLOW)&&g_timeoverflg==1)	//水通在别家
			g_sysflag |= SWOFF;
		else if(CAPWAI==0&&CAPNEI==1)				//别家抽水
			g_sysflag |= OUTSIDE;
		

		
		//执行器更新---------------------------------------------------------------
		if(g_savedat.mode==AUTO)
		{	//
			if(g_sysflag&WORK&&(g_sysflag&STATUS||								//在工作过程中遇到问题
				(sensor_data.possw&FULL)==0||(sensor_data.possw&PEC100)==0)) 	//水满时
			{PUMP_OFF;}									//关水
			else if(g_sysflag==FREE&&(sensor_data.possw&PEC0)==0) 					//没水时
			{
				PUMP_ON;
			}
			
		}
		
		//界面更新-------------------------------------------------------
		if(ReadSoftTimer(TIMER_2))
		{//--------------工作状态----------------------------------------
			if(g_sysflag&WORK)
			{
				DataAqurie();
				if(g_sysflag&OUTSIDE)
				{
					ALARM = ON;
					delay200ms();
					ALARM = OFF;
				}
				else if(g_sysflag&SWOFF)
				{
					ALARM = ON;
					delay200ms();
					ALARM = OFF;
					delay200ms();
					ALARM = ON;
					delay200ms();
					ALARM = OFF;
				}
				else if(g_sysflag&OFFLINE)
				{
					 delay200ms();
				}
				else delay200ms();
				
				if(g_homepage) LCD_Clear();				//如果在别的页面则返回主页1
				
				GUI_HomePage();
				SetSoftTimer(TIMER_2,5);				//设置延时
			}
			else{//--------------空闲状态----------------------------------------
				LCD_BL_OFF;
				if(g_homepage)
				{
					DataAqurie();
					
					LCD_Clear();
					delay100ms();
					delay100ms();
					GUI_HomePage();
				}
				else{
					LCD_Clear();
					GUI_HomePage2();
				}
				SetSoftTimer(TIMER_2,8);				//设置主界面滚动延时
			}
		}	
		//-----------------------主界面数据更新---------------------------------		
		if(g_1stimer_1&&(g_menumark==0||g_menumark==MENU_OPERATE))
		{
			g_1stimer_1 = 0;
			if(g_homepage==0)
				GUI_HomePageUpdate();
			else
				GUI_HomePage2Update();
		}
		
	}//end of while
}




void DataAqurie()
{
	//发送采集数据指令
	DATA_Cmd_Pkg_Send();
	SetSoftTimer(TIMER_3,5);			//设置发送超时
	do{
		if(Trans_RevPakFin||ReadSoftTimer(TIMER_3)) break;
	}
	while(1);
	if(Trans_RevPakFin)					//收到数据
	{
		Trans_RevPakFin = 0;
		g_offlineflag = 0;				
		if(2==Pak_Handle())
		{
			SendString("valid data received.\r\n");		
			LED2=0;
			delay100ms();
			LED2= 1;
			PressDatHandle();				
			TemperDatHandle();
			LevelDatHandle();
		}
	}
	else{								//接收超时
		g_offlineflag = 1;
		LED1 = 0;						//红灯闪
		delay100ms();
		LED1 = 1;
	}
}


void EXTI0_ISR() interrupt 0 
{	//响应后IE0自动清除
	SI4432_ISR();
//	SendString("ISR completed.\r\n");					//调试信息时候用	
}

void T0_EXT_ISR() interrupt 1
{	//响应后IE1自动清除
	g_1stimer_1 = 1;
	SoftTimer();
	DS3231_ReadTime();
//	SendByteASCII(sensor_data.possw);
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

