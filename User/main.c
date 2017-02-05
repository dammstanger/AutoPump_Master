/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��main.c
 * ��	��	��Һλ�Զ�������Ŀ����
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.0.150711
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2015.7.11
 * ���༭	��2017.1.30
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
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
/****************************�궨��***********************************************/

/****************************��������*********************************************/

/****************************��������*********************************************/
//ϵͳ��־λ
//                    		7			6		5		4		3		2		1		0		Reset Value
char g_sysflag = 0;		//	����ָʾ	����	����	����	|		״̬			|		0000 0000
bit g_timeoverflg=0;	//��ˮ��ʱ��־λ
bit g_offlineflag=0;	//���߱�־λ
bit g_1stimer_1 = 0;
bit g_1stimer_2 = 0;
bit g_timer_flow_start = 0;
//---------------------------------------------
sbit START 	= P2^7;				//����
sbit STOP	= P2^5;				//ֹͣ
sbit ALARM	= P4^0;				//������
sbit TMP	= P4^4;				//�¶����ݿ�
sbit CAPWAI	= P1^3;				//�ⲿ���
sbit CAPNEI	= P1^4;				//�ڲ����


//��ˮʱ��ʱ��֤�ϵ���ɿ�
#define PUMP_ON 	START = 0;delay200ms();START = 1;delay1s();delay1s()
#define PUMP_OFF 	STOP = 0;delay200ms();STOP = 1;delay1s()

/****************************��������*********************************************/
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
		WAKE_CLKO = 0x10;               //��Timer0���ж�����P3.5�����ⲿ�½����ж�
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
//	IAP_EraseSector(EEPROM_SECT1);		//����EEPROM
//	IAP_EraseSector(EEPROM_SECT2);
	if(IAP_EEPROMCheck(SECTOR_ALL))			//�����ݣ���Ϊ��
		DAT_InitDat(FALSE);
	else
		DAT_InitDat(TRUE);								//�����ݣ���ΪĬ��ֵ
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
	SI4432_SetRxMode();	//����ģʽ
	//-----------------------------------------------------
	IE0 = 0;
	IE1 = 0;
	TI = 0;
	EA = 1;								//ע�⣺�����ʼ�����ٿ��жϣ�
	SetSoftTimer(TIMER_2,5);			//���������������ʱ
	SoftUpTimer_Reset(&UpTimer1);
	GUI_HomePage();
	
	while(1)
	{
		key = GUIMenu();				//�˵�����
		SystemStatus(key);				//ϵͳ״̬ȷ��
		
		//ִ��������---------------------------------------------------------------
		if((g_savedat.mode==AUTO)&&(g_sysflag&OFFLINE)==0)
		{	//
			if(g_sysflag&WORK&&(g_sysflag&STATUS||								//�ڹ�����������������,
			(sensor_data.possw&FULL)==0||g_level_per>=g_savedat.s_stopl||UpTimer1.min>=OVERTIME_MIN))//ˮ��ʱ
			{	
				PUMP_OFF;
			}									
			else if(g_sysflag==FREE&&(g_sysflag&SLEEP)==0&&g_level_per<=g_savedat.s_startl) 		//ûˮʱ,�ݲ��Զ���ˮ
			{
				PUMP_ON;
			}
		}
		
		//���棬ָʾ����------------------------------------------------------
		if(ReadSoftTimer(TIMER_2))
		{	
			//--------------�������쳣״̬----------------------------------------
			if(g_sysflag&WORK||g_sysflag&STATUS)
			{
				SoftUpTimer_Start(&UpTimer1);			//������ˮ��ʱ
				DataAqurie();
				
				//����ʱ��״̬�쳣��Ҫ������1��������һ��
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
					if(alarm_cnt>=6)					//ˮҪ��ʱ��ÿ30s����һ��						
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
						LCD_Clear();							//����ڱ��ҳ���򷵻���ҳ1
						GUI_HomePage();
					}
					else
					{
						GUI_CaseData_Dis(g_level_per,0);		//����Һλ������̫Ƶ��
					}
					SetSoftTimer(TIMER_2,5);					//������ʱ
				}
			}
			else{//--------------����״̬----------------------------------------
				SoftUpTimer_Reset(&UpTimer1);					//ֹͣ��ˮ��ʱ
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
					SetSoftTimer(TIMER_2,10);				//���������������ʱ
				}
			}
		}	
		//-----------------------���������ݸ���---------------------------------		
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
		//����ɨ��----------------------------------------------
		key = Key_Scan();
		//�����ֵ-----------------------------------------------	
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
	//��ѯ���������				
	switch(g_menumark)
	{
		case MENU_OPERATE 	:{
			GUI_Operation(key);
			SetSoftTimer(TIMER_2,5);		//���������������ʱ
			}break;
		case MENU_KEYINPUT 	:{GUI_DisplayPassword();}break;
		case MENU_HISTORY :{
			GUI_History();
		}break;
	}
	//2����ѯ
	switch(g_menumark)
	{
		case MENU_SET:{GUI_Setting();}break;
	}
	//3����ѯ
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


//ϵͳ״̬���
void SystemStatus(char key)
{
	//ȷ��ϵͳ״̬------------------------------------------
	if(CAPNEI==1)
		g_sysflag &= ~WORK;						//����
	else
		g_sysflag |= WORK;						//����
	//��������ж���Ҫ�жϵ�״̬-------------------------
	if(g_offlineflag)								//�����ж��Ƿ�����
		g_sysflag |= OFFLINE;
	else g_sysflag &= ~OFFLINE;					//
	if(CAPWAI==0&&CAPNEI==1)						//��ҳ�ˮ
		g_sysflag |= OUTSIDE;
	else g_sysflag &= ~OUTSIDE;					//
	

	if(g_sysflag&WORK)//-------------����-------------------------------
	{
		//�������״̬
		g_sysflag &= ~SWOFF;
		g_sysflag &= ~SLEEP;
//		if(sensor_data.flow==NO_FLOW&&g_timeoverflg==0)	//���ûˮ����ʼ��ʱ
//		{
//			if(g_timer_flow_start==0)
//			{
//				g_timer_flow_start = 1;
//				SetSoftTimer(TIMER_4,30);				//������ˮ��ʱ30s
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
//			g_timer_flow_start = 0;						//��λ��ʱ
//			g_timeoverflg = 0;							//��λ��ʱ���
//		}
//		
//		//��ʱ�л�״̬
//		if(g_timeoverflg==1)
//		{
//			g_sysflag |= SWOFF;							
//		}
	}//-----------------------------����-------------------------------------
	else {
		g_timer_flow_start = 0;						//��λ��ʱ
		g_timeoverflg = 0;							//��λ��ʱ���
		
		//s����ʱ�ֶ���������SWOFF����
		if((g_sysflag&SWOFF)&&(g_savedat.mode==MANAUL||key==KEY_HOME))	
			g_sysflag &=~SWOFF;
		
		if(RTCtime.hour>=22||RTCtime.hour<=6)		//������˯��״̬
			g_sysflag |= SLEEP;
		else g_sysflag &= ~SLEEP;
	}
	
}




void DataAqurie()
{
	static uchar offlinecnt = 0;
	//���Ͳɼ�����ָ��
	DATA_Cmd_Pkg_Send();
	SetSoftTimer(TIMER_3,5);			//���ý��ճ�ʱ
	do{
		if(Trans_RevPakFin||ReadSoftTimer(TIMER_3)) break;
	}
	while(1);
	if(Trans_RevPakFin)					//�յ�����
	{
		Trans_RevPakFin = 0;
		g_offlineflag = 0;				//���߱������
		offlinecnt = 0;					//���߼�������
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
	else{								//���ճ�ʱ
		offlinecnt++;
		if(++offlinecnt==5)				//5�γ�ʱ�ж�Ϊ����
		{
			g_offlineflag = 1;
			offlinecnt = 0;
		}
		LED1 = 0;						//�����
		delay100ms();
		LED1 = 1;
	}
}


void EXTI0_ISR() interrupt 0 
{	//��Ӧ��IE0�Զ����
	SI4432_ISR();
//	SendString("ISR completed.\r\n");					//������Ϣʱ����	
}

void T0_EXT_ISR() interrupt 1
{	//��Ӧ��IE1�Զ����
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

