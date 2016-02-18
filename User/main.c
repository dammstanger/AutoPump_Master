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
 * ���༭	��2016.2.12
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
/****************************�궨��***********************************************/
#define P2_5	0x20
#define P2_7	0x80

/****************************��������*********************************************/

/****************************��������*********************************************/
//ϵͳ��־λ
//                    		7		6		5		4		3		2		1		0		Reset Value
char g_sysflag = 0;		//	����	����	����	����	|		״̬	|		ģʽ	0000 0000
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
/****************************��������*********************************************/
void SendTemp(uint temp);






void EXTI0_Init()
{
	IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
    EX0 = 1;                        //enable INT0 interrupt
}
void T0_EXT_Init()
{
	WAKE_CLKO = 0x10;               //��Timer0���ж������ⲿ�½����ж�
    ET0 = 1;                        //enable T0 interrupt
}

void main()
{
	char key;
	uchar i=0;
	delay1s();
	AUXR = AUXR|0x40;  	// T1, 1T Mode

//	IE2 |= ESPI;
	UART_Init();
	IAP_EEPROMCheck();
	EXTI0_Init();     
	T0_EXT_Init();	
	SPI_Init(MASTER);
	IIC_GPIO_Config();
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
	EA = 1;				//ע�⣺�����ʼ�����ٿ��жϣ�
	SetSoftTimer(TIMER_2,5);		//���������������ʱ
	GUI_HomePage();
	while(1)
	{	
		//����ɨ��----------------------------------------------
		key = Key_Scan();
		//�����ֵ-----------------------------------------------	
		if(key!=KEY_NONE)
		{
			LCD_BL_ON;
			switch(GUI_Operation(key))
			{
				case 1 :{
					GUI_DisplayPassword();
					GUI_Setting();
					LCD_Clear();
					GUI_HomePage();
				}break;
				case 2 :{
					GUI_History();
				}break;
				default :{
					;
				}break;
			}
			SetSoftTimer(TIMER_2,5);		//���������������ʱ
		}			
		//ȷ��ϵͳ״̬------------------------------------------
		g_sysflag &= ~STATUS; 
		if(CAPWAI==1)
			g_sysflag |= FREE;						//����
		else if(CAPNEI==1&&g_timeoverflg==0)		//�Լҳ�ˮ
			g_sysflag |= WORK_SELF;
		else if(CAPNEI==1&&g_timeoverflg==1)		//ˮͨ�ڱ��
			g_sysflag |= SWOFF;
		else if(CAPNEI==0)							//��ҳ�ˮ
			g_sysflag |= WORK_OUT;
		
		switch(g_sysflag&STATUS)
		{
			case FREE :{
			}break;
			case WORK_SELF :{
				
			}break;
			case WORK_OUT :{
				
			}break;
			case SWOFF :{
				
			}break;		
		}//end of switch
		if(ReadSoftTimer(TIMER_2))			//����ʱ�䵽
		{	LCD_BL_OFF;
			if(g_homepage)
			{
				//���Ͳɼ�����ָ��
				DATA_Cmd_Pkg_Send();
				if(Trans_RevPakFin)
				{
					Trans_RevPakFin = 0;
					if(2==Pak_Handle())
					{
						SendString("valid data received.\r\n");		
						PressDatHandle();				
						TemperDatHandle();
					}
				}
				LCD_Clear();
				delay100ms();
				GUI_HomePage();
				for(i=0;i<10;i++)
				{
					IAP_WriteByte(EEPROM_SECT1+i,i);
				}
			}
			else{
				DS3231_ReadTime();
				LCD_Clear();
				delay100ms();
				GUI_HomePage2();
				for(i=0;i<10;i++)
				{
					SendString("EEPROM data:\r\n");
					SendByteASCII(IAP_ReadByte(EEPROM_SECT1+i));
					SendString("\r\n");
				}
			}
			SetSoftTimer(TIMER_2,2);		//���������������ʱ
		}

	}//end of while
}

void SendTemp(uint temp)
{		
		SendByteNum(temp/10000);			// ��λ��ʾֵ
		SendByteNum(temp/1000%10);			// ʮλ��ʾֵ
		SendByteNum(temp/100%10);			// ��λ��ʾֵ
		SendOneByte('.');
		SendByteNum(temp/10%10);			//ʮ��λ��ʾֵ
		SendByteNum(temp%10);				//�ٷ�λ��ʾֵ
}


void EXTI0_ISR() interrupt 0 
{	//��Ӧ��IE0�Զ����
	SI4432_ISR();
//	SendString("ISR completed.\r\n");					//������Ϣʱ����	
}

void T0_EXT_ISR() interrupt 1
{	//��Ӧ��IE1�Զ����
	SoftTimer();
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

