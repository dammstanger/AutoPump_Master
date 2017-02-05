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
 * 最后编辑	：2017.1.30
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

/****************************变量声明*********************************************/

/****************************变量定义*********************************************/
//系统标志位
//                    		7			6		5		4		3		2		1		0		Reset Value
char g_sysflag = 0;		//	工作指示	保留	保留	保留	|		状态			|		0000 0000
bit g_timeoverflg=0;	//抽水超时标志位
bit g_offlineflag=0;	//离线标志位
bit g_1stimer_1 = 0;
bit g_1stimer_2 = 0;
bit g_timer_flow_start = 0;
//---------------------------------------------
sbit START 	= P2^7;				//启动
sbit STOP	= P2^5;				//停止
sbit ALARM	= P4^0;				//蜂鸣器
sbit TMP	= P4^4;				//温度数据口
sbit CAPWAI	= P1^3;				//外部检测
sbit CAPNEI	= P1^4;				//内部检测


//关水时延时保证断电检测可靠
#define PUMP_ON 	START = 0;delay200ms();START = 1;delay1s();delay1s()
#define PUMP_OFF 	STOP = 0;delay200ms();STOP = 1;delay1s()

/****************************函数声明*********************************************/
void DataAqurie();
char GUIMenu();
void SystemStatus(char key);

void EXTI0_Init()
{
		IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
    EX0 = 1;                        //enable INT0 interrupt
}

void T0_EXT_Init()
{
		WAKE_CLKO = 0x10;               //将Timer0的中断引脚P3.5用于外部下降沿中断
    ET0 = 1;                        //enable T0 interrupt
}

void main()
{
	uchar key=0;
	uchar i=0;
	uchar alarm_cnt = 0;
	delay1s();

	AUXR = AUXR|0x40;  									// T1, 1T Mode

	UART_Init();
//	IAP_EraseSector(EEPROM_SECT1);		//擦除EEPROM
//	IAP_EraseSector(EEPROM_SECT2);
	if(IAP_EEPROMCheck(SECTOR_ALL))			//有数据，不为空
		DAT_InitDat(FALSE);
	else
		DAT_InitDat(TRUE);								//无数据，设为默认值
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
	EA = 1;								//注意：外设初始化完再开中断！
	SetSoftTimer(TIMER_2,5);			//设置主界面滚动延时
	SoftUpTimer_Reset(&UpTimer1);
	GUI_HomePage();
	
	while(1)
	{
		key = GUIMenu();				//菜单设置
		SystemStatus(key);				//系统状态确认
		
		//执行器更新---------------------------------------------------------------
		if((g_savedat.mode==AUTO)&&(g_sysflag&OFFLINE)==0)
		{	//
			if(g_sysflag&WORK&&(g_sysflag&STATUS||								//在工作过程中遇到问题,
			(sensor_data.possw&FULL)==0||g_level_per>=g_savedat.s_stopl||UpTimer1.min>=OVERTIME_MIN))//水满时
			{	
				PUMP_OFF;
			}									
			else if(g_sysflag==FREE&&(g_sysflag&SLEEP)==0&&g_level_per<=g_savedat.s_startl) 		//没水时,暂不自动抽水
			{
				PUMP_ON;
			}
		}
		
		//界面，指示更新------------------------------------------------------
		if(ReadSoftTimer(TIMER_2))
		{	
			//--------------工作或异常状态----------------------------------------
			if(g_sysflag&WORK||g_sysflag&STATUS)
			{
				SoftUpTimer_Start(&UpTimer1);			//启动抽水计时
				DataAqurie();
				
				//工作时，状态异常需要报警，1分钟响铃一次
				if(g_sysflag&OFFLINE||g_sysflag&OUTSIDE)
				{
					if(alarm_cnt>=12)							
					{
						ALARM = ON;
						delay100ms();
						ALARM = OFF;
						alarm_cnt = 0;
					}
					else alarm_cnt++;
				}
				else if(g_sysflag&SWOFF)
				{
					if(alarm_cnt>=12)							
					{
						alarm_cnt = 0;
						ALARM = ON;
						delay200ms();
						ALARM = OFF;
						delay200ms();
						ALARM = ON;
						delay200ms();
						ALARM = OFF;
					}
					else alarm_cnt++;
				}
				else if(g_level_per>=95)
				{
					if(alarm_cnt>=6)					//水要满时，每30s响铃一次						
					{
						alarm_cnt = 0;
						ALARM = ON;
						delay200ms();
						ALARM = OFF;
						delay200ms();
						ALARM = ON;
						delay200ms();
						ALARM = OFF;
					}
					else alarm_cnt++;					
				}
				else ;
				
				//
				if(g_menumark==0)
				{
					LCD_BL_OFF;
					if(g_homepage)
					{
						LCD_Clear();							//如果在别的页面则返回主页1
						GUI_HomePage();
					}
					else
					{
						GUI_CaseData_Dis(g_level_per,0);		//更新液位，不用太频繁
					}
					SetSoftTimer(TIMER_2,5);					//设置延时
				}
			}
			else{//--------------空闲状态----------------------------------------
				SoftUpTimer_Reset(&UpTimer1);					//停止抽水计时
				if(g_menumark==0)
				{
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
					SetSoftTimer(TIMER_2,10);				//设置主界面滚动延时
				}
			}
		}	
		//-----------------------主界面数据更新---------------------------------		
//		if(g_1stimer_1&&g_menumark==0)
//		{
//			g_1stimer_1 = 0;
//			if(g_homepage==0)
//				GUI_HomePageUpdate();
//			else
//				GUI_HomePage2Update();
//		}
//		if(g_1stimer_2)
//		{
//			g_1stimer_2 = 0;
//			SendByteASCII(Timer_reg);
//			SendOneByte('-');
//			SendByteASCII(Timer2_cnt);
//			SendOneByte('-');
//			SendByteASCII(g_sysflag);
//			SendString("\r\n");	
////			SendByteASCII(sensor_data.possw&PEC0);
//		}
	}//end of while
}


char GUIMenu()
{
	char key;
	if(g_menumark==0)
	{
		//按键扫描----------------------------------------------
		key = Key_Scan();
		//处理键值-----------------------------------------------	
		if(key!=KEY_NONE)
		{
			if(g_sysflag&SWOFF) return key;
			
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
	return key;
}


//系统状态检测
void SystemStatus(char key)
{
	//确定系统状态------------------------------------------
	if(CAPNEI==1)
		g_sysflag &= ~WORK;						//空闲
	else
		g_sysflag |= WORK;						//工作
	//工作与空闲都需要判断的状态-------------------------
	if(g_offlineflag)								//首先判断是否离线
		g_sysflag |= OFFLINE;
	else g_sysflag &= ~OFFLINE;					//
	if(CAPWAI==0&&CAPNEI==1)						//别家抽水
		g_sysflag |= OUTSIDE;
	else g_sysflag &= ~OUTSIDE;					//
	

	if(g_sysflag&WORK)//-------------工作-------------------------------
	{
		//先清除旧状态
		g_sysflag &= ~SWOFF;
		g_sysflag &= ~SLEEP;
//		if(sensor_data.flow==NO_FLOW&&g_timeoverflg==0)	//如果没水出则开始计时
//		{
//			if(g_timer_flow_start==0)
//			{
//				g_timer_flow_start = 1;
//				SetSoftTimer(TIMER_4,30);				//设置上水延时30s
//			}
//			else
//			{
//				if(ReadSoftTimer(TIMER_4))
//				{
//					g_timer_flow_start = 0;
//					g_timeoverflg = 1;
//				}
//			}
//		}
//		else if(sensor_data.flow==HAS_FLOW)
//		{
//			g_timer_flow_start = 0;						//复位计时
//			g_timeoverflg = 0;							//复位超时标记
//		}
//		
//		//超时切换状态
//		if(g_timeoverflg==1)
//		{
//			g_sysflag |= SWOFF;							
//		}
	}//-----------------------------空闲-------------------------------------
	else {
		g_timer_flow_start = 0;						//复位计时
		g_timeoverflg = 0;							//复位超时标记
		
		//s空闲时手动情况下清除SWOFF错误
		if((g_sysflag&SWOFF)&&(g_savedat.mode==MANAUL||key==KEY_HOME))	
			g_sysflag &=~SWOFF;
		
		if(RTCtime.hour>=22||RTCtime.hour<=6)		//晚间进入睡眠状态
			g_sysflag |= SLEEP;
		else g_sysflag &= ~SLEEP;
	}
	
}




void DataAqurie()
{
	static uchar offlinecnt = 0;
	//发送采集数据指令
	DATA_Cmd_Pkg_Send();
	SetSoftTimer(TIMER_3,5);			//设置接收超时
	do{
		if(Trans_RevPakFin||ReadSoftTimer(TIMER_3)) break;
	}
	while(1);
	if(Trans_RevPakFin)					//收到数据
	{
		Trans_RevPakFin = 0;
		g_offlineflag = 0;				//离线标记清零
		offlinecnt = 0;					//离线计数清零
		if(2==Pak_Handle())
		{
			SendString("valid data received.\r\n");		
			LED2=0;
			delay100ms();
			LED2= 1;		
			TemperDatHandle();
			LevelDatHandle();
		}
	}
	else{								//接收超时
		offlinecnt++;
		if(++offlinecnt==5)				//5次超时判断为离线
		{
			g_offlineflag = 1;
			offlinecnt = 0;
		}
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
	g_1stimer_2 = 1;
	SoftTimer();
	DS3231_ReadTime();
	SoftUpTimer(&UpTimer1);
	if(g_menumark==MENU_HOME||g_menumark==MENU_OPERATE){
		if(g_homepage==0)
			GUI_HomePageUpdate();
		else
			GUI_HomePage2Update();
	}
}


/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

