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
 * ���༭	��2016.1.26
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
/****************************�궨��***********************************************/
#define P2_5	0x20
#define P2_7	0x80

/****************************��������*********************************************/

/****************************��������*********************************************/
sbit START 	= P2^7;
sbit STOP	= P2^5;
sbit ALARM	= P4^0;

sbit TMP	= P4^4;

char temp;
char dat[] = "abc";
/****************************��������*********************************************/
void SendTemp(uint temp);





void delay200ms(void)   //��� -0.000000000028us
{
    unsigned char a,b,c;
    for(c=178;c>0;c--)
        for(b=230;b>0;b--)
            for(a=12;a>0;a--);
}

void delay1s(void)   ////11.0592 STC1T ��� -0.000000000099us
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
	uint tmp=0;
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
	SI4432_SetRxMode();	//����ģʽ
	//-----------------------------------------------------
	EA = 1;								//ע�⣺�����ʼ�����ٿ��жϣ�
	GUI_HomePage();
	while(1)
	{	
		
		DATA_Cmd_Pkg_Send();
		if(Trans_RevPakFin)
		{
			Trans_RevPakFin = 0;
			if(2==Pak_Handle())
			{
				SendString("valid data received.\r\n");		//������Ϣʱ����
				SendString("ADC data:\r\n");			
				SendByteASCII(sensor_data.press_h);
				SendByteASCII(sensor_data.press_l);
				SendString("\r\n");	
				SendString("POS data:\r\n");			
				SendByteASCII(sensor_data.possw);
				SendString("\r\n");				
				tmp = (uint)sensor_data.temp_h<<8|(uint)sensor_data.temp_l;
				SendTemp(tmp);
			}
		}
		
		DS3231_Init();
		LED1 = 0;
//		ALARM = 0;
//		START = 0;
//		STOP = 0;
		delay1s();
//		ALARM = 1;
//		START = 1;
//		STOP = 1;
		LED1 = 1;
	}
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

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

