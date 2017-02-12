/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��LCD_GUI.c
 * ��	��	��GUI���
 *                    
 * ʵ��ƽ̨	��AutoPumpV2.0
 * Ӳ������	��
 * �� 	��	��V0.0.160217
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2015.7.14
 * ���༭	��2016.2.20
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

#include "LCD_GUI.h"
#include "YXD19264D_51.h"
#include "Sensor.h"
#include "Key.h"
#include "Time.h"
#include "DS3231.h"
#include "Menu.h"
#include "UART_51.h"
#include "Dataopt.h"

/****************************�궨��*********************************************/
#define KS_NORMAL			0
#define KS_KEYSET_OK		1
#define KS_KEYSET2			2
#define KS_KEYSET			3
#define KS_KEYSET_ERR		4
/****************************��������*********************************************/
bit g_homepage = 0;				//�������ǣ�0����1 =1����2
uchar g_keycmd = KS_NORMAL;
bit homedispbusy = 0;			//��������ʾæµ��־��Ϊ1ʱ��ֹ�ж��ڵ����������

/****************************��������*********************************************/
void GUI_SettingMenu(char num, char turn);
void GUI_HomeMenu(char selitem);
void GUI_PasswordMenu(char num);
void GUI_ModeSettingMenu(uchar num);
void GUI_PecentSettingMenu(uchar num,uchar wei);
void GUI_DateTimeSettingMenu(STC_TIME dat,uchar wei);
void GUI_KEYSettingMenu(char keyon,char wei);
void GUI_BLSettingMenu(uchar wei);
void GUI_DEFAULTSettingMenu(char yes,uchar wei);

/********************************************************************************
 * ��������GUI_DisplayWaterTemp()
 * ����  ����ʾˮ��
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_DisplayWaterTemp()
{	
	//��ʾ�¶�
	LCD_Dis_Digital_float(1,22,tmp_data.tmp1);
}


/********************************************************************************
 * ��������GUI_DisplayAirTemp()
 * ����  ����ʾ����
 * ����  ��-		    	
 * ����  ��- 
 * ����  �����ⲿ����
 ********************************************************************************/
void GUI_DisplayAirTemp()
{	
	//��ʾ�¶�
	LCD_Dis_Digital_float(4,14,tmp_data.tmp2);						//���������˳������
}


/********************************************************************************
 * ��������LCD_FlashArrows()
 * ����  ����ͷ��˸
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void LCD_FlashArrows(bool enable)
{
	static bit flash = 0;
	uchar buf[26][5]={0};
	if(enable)
	{
		flash =~flash;
		if(flash) 	LCD_Dis_Pict(17,55,24,5,Pic_arrow);	
		else		LCD_Clear_Region(17,55,24,5);
	}
	else
	{
		if(flash)
		{
			LCD_Clear_Region(17,55,24,5);
			flash = 0;
		}
	}	
}


/********************************************************************************
 * ��������GUI_DisplayPassword()
 * ����  ����ʾ�������
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
char GUI_DisplayPassword()
{
	char keyval,selitem=0,wei=6,i=5;
	long input=0,input2=0;
	if(g_keycmd==KS_NORMAL)
	{
		if(!g_savedat.s_needkey) {g_menumark = MENU_SET;return 0;}			//û����������
		LCD_Clear();
		LCD_Dis_Char_16_16(1,4,&WordLib_CN[45][0],FALSE);		//����������
		LCD_Dis_Char_16_16(1,5,&WordLib_CN[46][0],FALSE);	
		LCD_Dis_Char_16_16(1,6,&WordLib_CN[47][0],FALSE);		//
		LCD_Dis_Char_16_16(1,7,&WordLib_CN[48][0],FALSE);	
		LCD_Dis_Char_16_16(1,8,&WordLib_CN[49][0],FALSE);		
		LCD_Dis_ASCIIStr(1,17,":",FALSE);
		GUI_PasswordMenu(selitem);	
	}
	else if(g_keycmd==KS_KEYSET)
	{
		LCD_Clear();
		LCD_Dis_Char_16_16(1,4,&WordLib_CN[45][0],FALSE);		//������������
		LCD_Dis_Char_16_16(1,5,&WordLib_CN[46][0],FALSE);	
		LCD_Dis_Char_16_16(1,6,&WordLib_CN[47][0],FALSE);		
		LCD_Dis_Char_16_16(1,7,&WordLib_CN[66][0],FALSE);			
		LCD_Dis_Char_16_16(1,8,&WordLib_CN[48][0],FALSE);	
		LCD_Dis_Char_16_16(1,9,&WordLib_CN[49][0],FALSE);		
		LCD_Dis_ASCIIStr(1,19,":",FALSE);
		GUI_PasswordMenu(selitem);			
	}
	while(1)
	{
		keyval = Key_Scan();	
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_menumark = MENU_OPERATE;return 0;
			}
			else if(keyval==KEY_UP) 
			{
				if(wei++<=6)
				{
					LCD_Dis_ASCIIStr(2,19-wei*2," ",FALSE);
					input /= 10; 
					
				}
				else wei = 6;
			}			
			else if(keyval==KEY_L) selitem--;
			else if(keyval==KEY_R) selitem++;
			if(selitem>10) selitem = 0;
			else if(selitem<0) selitem = 10;
			GUI_PasswordMenu(selitem);
			
			if(keyval==KEY_ENTER)
			{	//---------------------����--------------------------------
				if(selitem==10)
				{
					if(g_keycmd==KS_NORMAL)
					{
						g_menumark = MENU_OPERATE;return 0;
					}
					else{
						g_menumark = MENU_KEYSET;return 0;
					}
				}
				//---------------����������ȷ����--------------------------
				if(wei-->0) 			
				{
					input *=10;
					input += selitem;
					LCD_Dis_ASCIIStr(2,17-wei*2,"*",FALSE);
				}
				else{
					//--------------���������״�����-----------------------
					if(g_keycmd==KS_KEYSET)		
					{
						g_keycmd = KS_KEYSET2;
						input2 = input;
						LCD_Clear_Region(1,1,32,192);
						LCD_Dis_Char_16_16(1,3,&WordLib_CN[45][0],FALSE);		//���ٴ�����������
						LCD_Dis_Char_16_16(1,4,&WordLib_CN[67][0],FALSE);
						LCD_Dis_Char_16_16(1,5,&WordLib_CN[68][0],FALSE);					
						LCD_Dis_Char_16_16(1,6,&WordLib_CN[46][0],FALSE);
						LCD_Dis_Char_16_16(1,7,&WordLib_CN[47][0],FALSE);
						LCD_Dis_Char_16_16(1,8,&WordLib_CN[66][0],FALSE);
						LCD_Dis_Char_16_16(1,9,&WordLib_CN[48][0],FALSE);
						LCD_Dis_Char_16_16(1,10,&WordLib_CN[49][0],FALSE);						
						LCD_Dis_ASCIIStr(1,21,":",FALSE);
						wei = 6;
						input = 0;
					}
					//--------------�������ö�������-----------------------
					else if(g_keycmd==KS_KEYSET2)
					{
						if(input==input2)
						{
							g_savedat.s_keyword = input2;
							DAT_SaveDat(S_KEY,g_savedat);
							g_menumark = MENU_KEYSET;
							g_keycmd = KS_KEYSET_OK;
							return 1;
						}
						else{
							LCD_Clear_Region(17,1,16,192);
							LCD_Dis_Char_16_16(2,5,&WordLib_CN[48][0],FALSE);	//���벻����
							LCD_Dis_Char_16_16(2,6,&WordLib_CN[49][0],FALSE);		
							LCD_Dis_Char_16_16(2,7,&WordLib_CN[69][0],FALSE);	
							LCD_Dis_Char_16_16(2,8,&WordLib_CN[70][0],FALSE);		
							LCD_Dis_ASCIIStr(2,17,"!",FALSE);
							delay1s();
							g_menumark = MENU_KEYSET;
							g_keycmd = KS_KEYSET_ERR;
							return 0;
						}
					}
					//--------------������������-----------------------
					else if(g_keycmd==KS_NORMAL)
					{
						if(input==g_savedat.s_keyword)
						{
							g_menumark = MENU_SET;return 1;
						}
						else{
							LCD_Clear_Region(17,1,16,192);
							LCD_Dis_Char_16_16(2,5,&WordLib_CN[48][0],FALSE);	//�������
							LCD_Dis_Char_16_16(2,6,&WordLib_CN[49][0],FALSE);		
							LCD_Dis_Char_16_16(2,7,&WordLib_CN[50][0],FALSE);	
							LCD_Dis_Char_16_16(2,8,&WordLib_CN[51][0],FALSE);		
							LCD_Dis_ASCIIStr(2,17,"!",FALSE);
							delay1s();
							LCD_Clear_Region(17,1,16,192);
							wei = 6;
							input = 0;
							if(--i==0) {g_menumark = MENU_OPERATE;return 0;}
						}
					}
				}
			}//if(keyval==KEY_ENTER)
		}//if(keyval!=KEY_NONE)
	}//while(1)
}

/********************************************************************************
 * ��������GUI_Setting()
 * ����  ����ʾ
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_Setting()
{
	char keyval,selitem=1,turn=0;
	LCD_Clear();
	GUI_SettingMenu(selitem,turn);
	while(1)
	{
		keyval = Key_Scan();	
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME||keyval==KEY_L) 
			{
				g_menumark = MENU_OPERATE;return ;
			}
			else if(keyval==KEY_UP)
			{
				selitem--;
				if(selitem<1) {selitem = 7;turn = 1;}
				else if(selitem==4){turn = 1;}
				else turn = 0;
			}
			else if(keyval==KEY_DOWN)
			{
				selitem++;
				if(selitem>7) {selitem = 1;turn = 1;}
				else if(selitem==5){turn = 1;}
				else turn = 0;
			}	
			else if(keyval==KEY_ENTER||keyval==KEY_R)
			{
				g_menumark = MENU_SET|(selitem<<4);
				return ;
			}//if(keyval==KEY_ENTER)
			GUI_SettingMenu(selitem,turn);
		}//if(keyval!=KEY_NONE)
	}//while(1)
}


/********************************************************************************
 * ��������GUI_ModeSetting()
 * ����  ����ʾģʽ����
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_ModeSetting()
{
	char keyval;
	char selitem;
	
	selitem = g_savedat.mode;
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[34][0],FALSE);			//ģʽ����
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[35][0],FALSE);		
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[27][0],FALSE);		
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[28][0],FALSE);	
	GUI_ModeSettingMenu(selitem);
	while(1)
	{
		keyval = Key_Scan();	
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_menumark = MENU_OPERATE;
				return ;
			}
			else if(keyval==KEY_L) 
			{
				g_menumark = MENU_SET;
				return ;
			}
			else if(keyval==KEY_UP)
			{
				selitem--;
				if(selitem<0) selitem = 2;
			}
			else if(keyval==KEY_DOWN)
			{
				selitem++;
				if(selitem>2) selitem = 0;
			}
			GUI_ModeSettingMenu(selitem);
			
			if(keyval==KEY_ENTER)
			{
				if(selitem==0)
				{
					g_menumark = MENU_SET;
					return ;
				}
				else if(selitem==1) 
				{
					g_savedat.mode = AUTO;			//�Զ�
					g_menumark = MENU_SET;
					DAT_SaveDat(S_MODE,g_savedat);
					return ;				
				}
				else if(selitem==2) 
				{
					g_savedat.mode = MANAUL;		//�ֶ�
					g_menumark = MENU_SET;
					DAT_SaveDat(S_MODE,g_savedat);
					return ;					
				}
			}//if(keyval==KEY_ENTER)
		}//if(keyval!=KEY_NONE)
	}//while(1)
	
}



/********************************************************************************
 * ��������GUI_SRARTLSetting()
 * ����  ���Զ���ˮҺλ����
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_SRARTLSetting()
{
	char keyval,wei = 3;
	char input;
	
	input = g_savedat.s_startl;
	LCD_Clear();
	LCD_Dis_Char_16_16(1,2,&WordLib_CN[13][0],FALSE);		//�Զ���ˮҺλ����
	LCD_Dis_Char_16_16(1,3,&WordLib_CN[14][0],FALSE);		
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[52][0],FALSE);		
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[4][0],FALSE);		
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[4][0],FALSE);		
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[5][0],FALSE);		
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[27][0],FALSE);		
	LCD_Dis_Char_16_16(1,9,&WordLib_CN[28][0],FALSE);		
	GUI_PecentSettingMenu(input,wei);
	while(1)
	{
		keyval = Key_Scan();	
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_menumark = MENU_OPERATE;
				return ;
			}
			else if(keyval==KEY_L)
			{
				wei++;
				if(wei>3) wei = 0;
			}
			else if(keyval==KEY_R)
			{
				wei--;
				if(wei<0) wei = 3;
			}
			else if(keyval==KEY_UP)
			{
				if(wei==1) {input++;}
				else if(wei==2) {input+=10;}
				else if(wei==3){input+=100;}
				if(input<0||input>100) input = 100;			//���ת�������������ֵ
			}
			else if(keyval==KEY_DOWN)
			{
				if(wei==1) {input--;}
				else if(wei==2) {input-=10;}
				else if(wei==3){input-=100;}
				if(input<0) input = 0;
			}
			else if(keyval==KEY_ENTER)
			{
				if(wei==0)
				{
					g_menumark = MENU_SET;
					return ;
				}
				else 
				{
					if(input>=g_savedat.s_stopl) 
					{
						;
					}
					else{
						g_savedat.s_startl = input;			//���³�ˮҺλ
						g_menumark = MENU_SET;
						DAT_SaveDat(S_STARTL,g_savedat);
					return ;
					}
				}
			}//if(keyval==KEY_ENTER)
			GUI_PecentSettingMenu(input,wei);
		}//if(keyval!=KEY_NONE)
	}//while(1)
}

/********************************************************************************
 * ��������GUI_STOPLSetting()
 * ����  ���Զ�ͣˮҺλ����
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_STOPLSetting()
{
	char keyval,wei = 3;
	char input;
	
	input = g_savedat.s_stopl;
	
	LCD_Clear();
	LCD_Dis_Char_16_16(1,2,&WordLib_CN[13][0],FALSE);		//�Զ�ͣˮҺλ����
	LCD_Dis_Char_16_16(1,3,&WordLib_CN[14][0],FALSE);		
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[56][0],FALSE);		
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[4][0],FALSE);		
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[4][0],FALSE);		
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[5][0],FALSE);		
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[27][0],FALSE);		
	LCD_Dis_Char_16_16(1,9,&WordLib_CN[28][0],FALSE);		
	GUI_PecentSettingMenu(input,wei);
	while(1)
	{
		keyval = Key_Scan();	
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_menumark = MENU_OPERATE;
				return ;
			}
			else if(keyval==KEY_L)
			{
				wei++;
				if(wei>3) wei = 0;
			}
			else if(keyval==KEY_R)
			{
				wei--;
				if(wei<0) wei = 3;
			}
			else if(keyval==KEY_UP)
			{
				if(wei==1) {input++;}
				else if(wei==2) {input+=10;}
				else if(wei==3){input+=100;}
				if(input<0||input>100) input = 100;			//���ת�������������ֵ
			}
			else if(keyval==KEY_DOWN)
			{
				if(wei==1) {input--;}
				else if(wei==2) {input-=10;}
				else if(wei==3){input-=100;}
				if(input<0) input = 0;
			}			
			else if(keyval==KEY_ENTER)
			{
				if(wei==0)
				{
					g_menumark = MENU_SET;
					return ;
				}
				else 
				{
					if(input<=g_savedat.s_startl) 
					{
						;
					}
					else{
						g_savedat.s_stopl = input;			//���³�ˮҺλ
						g_menumark = MENU_SET;
						DAT_SaveDat(S_STOPL,g_savedat);
					return ;
					}
				}
			}//if(keyval==KEY_ENTER)
			GUI_PecentSettingMenu(input,wei);
		}//if(keyval!=KEY_NONE)
	}//while(1)
}


/********************************************************************************
 * ��������GUI_DATETIMESetting()
 * ����  ��ʱ����������
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_DATETIMESetting()
{
	char keyval,wei = 1;
	STC_TIME input;
	
	input = RTCtime;
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[12][0],FALSE);		//ʱ������
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[38][0],FALSE);		
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[6][0],FALSE);		
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[7][0],FALSE);		
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[27][0],FALSE);		
	LCD_Dis_Char_16_16(1,9,&WordLib_CN[28][0],FALSE);
	LCD_Dis_Digital_int(2,8,4,input.year,FALSE);
	LCD_Dis_Char_16_16(2,5,&WordLib_CN[10][0],FALSE);				//������
	LCD_Dis_Char_16_16(2,7,&WordLib_CN[11][0],FALSE);		
	LCD_Dis_Char_16_16(2,9,&WordLib_CN[12][0],FALSE);
	LCD_Dis_ASCIIStr(3,7,":",FALSE);
	LCD_Dis_ASCIIStr(3,10,":",FALSE);
	LCD_Dis_Char_16_16(3,8,&WordLib_CN[37][0],FALSE);		
	LCD_Dis_Char_16_16(3,9,&WordLib_CN[38][0],FALSE);

	GUI_DateTimeSettingMenu(input,wei);
	while(1)
	{
		keyval = Key_Scan();	
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_menumark = MENU_OPERATE;
				return ;
			}
			else if(keyval==KEY_R)
			{
				wei++;
				if(wei>9) wei = 0;
			}
			else if(keyval==KEY_L)
			{
				wei--;
				if(wei<0) wei = 9;
			}
			else if(keyval==KEY_UP)
			{
				if(wei==2) {input.year++;}
				else if(wei==1) {input.year+=10;}
				else if(wei==3) {input.month++;}
				else if(wei==4) {input.date++;}
				else if(wei==5) {input.hour++;}
				else if(wei==6) {input.min++;}
				else if(wei==7) {input.sec++;}
				else if(wei==8) {input.week++;}
				if(input.year>2099) input.year = 2000;	
				if(input.month>12) input.month = 1;			
				if(input.date>31) input.date = 1;			
				if(input.hour>23) input.hour = 0;			
				if(input.min>59) input.min = 0;			
				if(input.sec>59) input.sec = 0;	
				if(input.week>7) input.week = 1;
			}
			else if(keyval==KEY_DOWN)
			{
				if(wei==2) {input.year--;}
				else if(wei==1) {input.year-=10;}
				else if(wei==3) {input.month--;}
				else if(wei==4) {input.date--;}
				else if(wei==5) {input.hour--;}
				else if(wei==6) {input.min--;}
				else if(wei==7) {input.sec--;}
				else if(wei==8) {input.week--;}
				if(input.year<2000) input.year = 2099;	
				if(input.month<1) input.month = 1;			
				if(input.date<1) input.date = 1;			
				if(input.hour>23) input.hour = 23;			//���	
				if(input.min>59) input.min = 59;			
				if(input.sec>59) input.sec = 59;	
				if(input.week<1) input.week = 7;
			}
			GUI_DateTimeSettingMenu(input,wei);
			
			if(keyval==KEY_ENTER)
			{
				if(wei==0)
				{
					g_menumark = MENU_SET;
					return ;
				}
				else if(wei==9)
				{
					DS3231_SetTime(input);
					g_menumark = MENU_SET;
					return ;				
				}

			}//if(keyval==KEY_ENTER)
		}//if(keyval!=KEY_NONE)
	}//while(1)
}


/********************************************************************************
 * ��������GUI_KEYSetting()
 * ����  ����������
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_KEYSetting()
{
	char keyval,wei = 1,input = 0;
	uchar keyon = g_savedat.s_needkey;
	
	if(g_keycmd==KS_KEYSET_OK)
	{
		LCD_Clear();	
		wei = 2;
		LCD_Dis_Char_16_16(2,4,&WordLib_CN[27][0],FALSE);		//������ɣ�
		LCD_Dis_Char_16_16(2,5,&WordLib_CN[28][0],FALSE);		
		LCD_Dis_Char_16_16(2,6,&WordLib_CN[62][0],FALSE);		
		LCD_Dis_Char_16_16(2,7,&WordLib_CN[63][0],FALSE);		
		LCD_Dis_ASCIIStr(2,15,"!",FALSE);
		delay1s();
	}
	g_keycmd = KS_NORMAL;	
	LCD_Clear();	
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[48][0],FALSE);		//��������
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[49][0],FALSE);		
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[27][0],FALSE);		
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[28][0],FALSE);		
	GUI_KEYSettingMenu(keyon,wei);	
	while(1)
	{
		keyval = Key_Scan();	
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_menumark = MENU_OPERATE;return ;
			}
			else if(keyval==KEY_UP) 
			{
				if(--wei<0) wei = 3;
			}
			else if(keyval==KEY_DOWN) 
			{
				if(++wei>3) wei = 0;
			}			
			else if(keyval==KEY_L||keyval==KEY_R)
			{
				if(wei==1) keyon = !keyon;
			}	
			else if(keyval==KEY_ENTER)
			{
				if(wei==2) 
				{
					g_menumark = MENU_KEYINPUT;
					g_keycmd = KS_KEYSET;
					return ;
				}
				else if(wei==3)
				{
					g_savedat.s_needkey = keyon;			//���뿪��ֵ����
					g_menumark = MENU_SET;
					DAT_SaveDat(S_KEYSW,g_savedat);
					return ;
				}
				else if(wei==0)
				{
					g_menumark = MENU_SET;
					return ;
				}
			}//if(keyval==KEY_ENTER)
			GUI_KEYSettingMenu(keyon,wei);	
		}//if(keyval!=KEY_NONE)
	}//while(1)
}




/********************************************************************************
 * ��������GUI_BLSetting()
 * ����  ����������
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_BLSetting()
{
	char keyval,wei;
	
	wei = g_savedat.s_baklit_on;
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[57][0],FALSE);		//��������
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[58][0],FALSE);
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[27][0],FALSE);
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[28][0],FALSE);		
	GUI_BLSettingMenu(wei);
	while(1)
	{
		keyval = Key_Scan();
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_menumark = MENU_OPERATE;
				return ;
			}
			else if(keyval==KEY_L)
			{
				g_menumark = MENU_SET;
				return ;
			}
			else if(keyval==KEY_UP)
			{
				wei--;
				if(wei<0) wei = 2;
			}
			else if(keyval==KEY_DOWN)
			{
				wei++;
				if(wei>2) wei = 0;
			}
			else if(keyval==KEY_ENTER)
			{
				if(wei==0)
				{
					g_menumark = MENU_SET;
					return ;
				}
				else if(wei==1) 
				{
					g_savedat.s_baklit_on = 1;
					g_menumark = MENU_SET;
					LCD_BL_ON;
					DAT_SaveDat(S_BL,g_savedat);
					
					return ;
				}
				else if(wei==2) 
				{
					g_savedat.s_baklit_on = 0;		//�ر�
					LCD_BL_OFF;
					DAT_SaveDat(S_BL,g_savedat);
					g_menumark = MENU_SET;
					return ;
				}
			}//if(keyval==KEY_ENTER)
			GUI_BLSettingMenu(wei);
		}//if(keyval!=KEY_NONE)
	}//while(1)
}



/********************************************************************************
 * ��������GUI_DEFAULTSetting()
 * ����  ���ָ���������
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_DEFAULTSetting()
{
	char keyval,wei = 1,yes = 0;
	
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[72][0],FALSE);		//�ָ�Ĭ�ϣ�
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[73][0],FALSE);
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[74][0],FALSE);
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[75][0],FALSE);		
	LCD_Dis_ASCIIStr(1,15,"?",FALSE);
	GUI_DEFAULTSettingMenu(yes,wei);
	while(1)
	{
		keyval = Key_Scan();
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_menumark = MENU_OPERATE;
				return ;
			}
			else if(keyval==KEY_L||keyval==KEY_R)
			{
				if(wei==1) yes = !yes;
			}
			else if(keyval==KEY_UP)
			{
				wei--;
				if(wei<0) wei = 2;
			}
			else if(keyval==KEY_DOWN)
			{
				wei++;
				if(wei>2) wei = 0;
			}
			else if(keyval==KEY_ENTER)
			{
				if(wei==0)
				{
					g_menumark = MENU_SET;
					return ;
				}
				else if(wei==2) 
				{
					DAT_SaveDat(S_ALL,defaultdat);
					g_menumark = MENU_SET;
					return ;
				}
			}//if(keyval==KEY_ENTER)
			GUI_DEFAULTSettingMenu(yes,wei);
		}//if(keyval!=KEY_NONE)
	}//while(1)
}



/********************************************************************************
 * ��������GUI_History()
 * ����  ����ʾ
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_History()
{
						;
}



/********************************************************************************
 * ��������GUI_Operation()
 * ����  ����ʾ��������		keyval==0xff��ʾ�Ǵӱ�Ľ��淵�ص�,����һ�����޷��ŵģ�
							����if�ж��ܲ��������д���һ������
 * ����  ��-		    	
 * ����  ��- ����Ľ���
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_Operation(uchar keyval)
{
	char selitem = 1;					//ѡ���ֵ

	if(keyval==0xff||g_homepage)		//�����������2�����л�1
	{
		keyval = KEY_NONE;
		LCD_Clear();
		GUI_HomePage();					//Ĭ�Ͼ�Ϊ0 HOME��
	}
	else 
	{
		if(keyval==KEY_L) selitem++;
		else if(keyval==KEY_R) selitem--;
		if(selitem>3) selitem = 1;
		else if(selitem<1) selitem = 3;
	
		GUI_HomeMenu(selitem);			//�����˵�������Ա䶯
	}
	SetSoftTimer(TIMER_1,10);			//�����˳���ʱ��ʱ�� 10s
	while(1)
	{
		keyval = Key_Scan();
		if(keyval!=KEY_NONE)
		{	SetSoftTimer(TIMER_1,10);
			if(keyval==KEY_ENTER)
			{
				g_menumark=selitem<<12;
				return ;
			}
			else if(keyval==KEY_HOME) {g_menumark = 0;return ;}
			else if(keyval==KEY_L) selitem++;
			else if(keyval==KEY_R) selitem--;
			if(selitem>3) selitem = 1;
			else if(selitem<1) selitem = 3;
			GUI_HomeMenu(selitem);				//�����˵�������Ա䶯	
		}

		if(ReadSoftTimer(TIMER_1)) {g_menumark = 0;return ;}	//��ʱ����
	}
}

/**************************��Ҫ�ǲ˵���ʾ****************************************************/
/********************************************************************************
 * ��������GUI_SysStatus()
 * ����  ����ʾ������״̬����
 * ����  ��-		    	
 * ����  ��- 
 * ����  �����ⲿ����
 ********************************************************************************/
void GUI_SysStatus(uchar sta)
{
	static uchar sta_last=FREE;
	if(sta!=sta_last)
	{
		LCD_Clear_Region(17,113,16,64);
		sta_last = sta;
	}
	if(sta&OFFLINE)											//���ȼ����
	{
			LCD_Dis_Char_16_16(2,10,&WordLib_CN[87][0],FALSE);		//����
			LCD_Dis_Char_16_16(2,11,&WordLib_CN[85][0],FALSE);			
	}
	else if(sta&OUTSIDE)
	{
			LCD_Dis_Char_16_16(2,10,&WordLib_CN[84][0],FALSE);		//ռ��
			LCD_Dis_Char_16_16(2,11,&WordLib_CN[85][0],FALSE);			
	}
	else if(sta&SWOFF)
	{
		LCD_Dis_Char_16_16(2,8,&WordLib_CN[8][0],FALSE);		//���ش���
		LCD_Dis_Char_16_16(2,9,&WordLib_CN[9][0],FALSE);			
		LCD_Dis_Char_16_16(2,10,&WordLib_CN[50][0],FALSE);		
		LCD_Dis_Char_16_16(2,11,&WordLib_CN[51][0],FALSE);
	}
	else if(sta&WORK)
	{
		LCD_Dis_Char_16_16(2,9,&WordLib_CN[52][0],FALSE);		//��ˮ��
		LCD_Dis_Char_16_16(2,10,&WordLib_CN[4][0],FALSE);		//
		LCD_Dis_Char_16_16(2,11,&WordLib_CN[86][0],FALSE);			
	}
	else
	{
		LCD_Dis_Char_16_16(2,10,&WordLib_CN[21][0],FALSE);		//����
		LCD_Dis_Char_16_16(2,11,&WordLib_CN[22][0],FALSE);
	}
}


/********************************************************************************
 * ��������GUI_SysMode()
 * ����  ����ʾ������ģʽ����
 * ����  ��-		    	
 * ����  ��- 
 * ����  �����ⲿ����
 ********************************************************************************/
void GUI_SysMode(uchar mode)
{
	static uchar mode_last=AUTO;
	if(mode!=mode_last)
	{
		LCD_Clear_Region(33,113,16,64);
		mode_last = mode;
	}
	if(mode==MANAUL)
	{
		LCD_Dis_Char_16_16(3,10,&WordLib_CN[36][0],FALSE);		//�ֶ�
		LCD_Dis_Char_16_16(3,11,&WordLib_CN[14][0],FALSE);	
	}
	else{
		LCD_Dis_Char_16_16(3,10,&WordLib_CN[13][0],FALSE);		//�Զ�
		LCD_Dis_Char_16_16(3,11,&WordLib_CN[14][0],FALSE);	
	}
}
/********************************************************************************
 * ��������GUI_HomePage()
 * ����  ����ʾ�����泣��
 * ����  ��-		    	
 * ����  ��- 
 * ����  �����ⲿ����
 ********************************************************************************/
void GUI_HomePage()
{
	g_homepage = 0;
	homedispbusy = 1;
	LCD_Dis_Pict(1,1,45,62,Pic_Case);
		
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[6][0],FALSE);		//ʱ��
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[7][0],FALSE);		//
	LCD_Dis_ASCIIStr(1,13,":",FALSE);	
//	GUI_DisplayWaterTemp();
	LCD_Dis_Digital_int(1,16,2,RTCtime.hour,FALSE);			//ʱ����
	LCD_Dis_ASCIIStr(1,17,":",FALSE);
	LCD_Dis_Digital_int(1,19,2,RTCtime.min,FALSE);
	LCD_Dis_ASCIIStr(1,20,":",FALSE);
	LCD_Dis_Digital_int(1,22,2,RTCtime.sec,FALSE);
	
//	LCD_Dis_Char_8_16(1,23,&CharLib_SplLabel[0][0],FALSE);	//��
//	LCD_Dis_ASCIIStr(1,24,"C",FALSE);
	
	LCD_Dis_Char_16_16(2,5,&WordLib_CN[19][0],FALSE);		//״̬
	LCD_Dis_Char_16_16(2,6,&WordLib_CN[20][0],FALSE);
	LCD_Dis_ASCIIStr(2,13,":",FALSE);
	GUI_SysStatus(g_sysflag);
	
	LCD_Dis_Char_16_16(3,5,&WordLib_CN[34][0],FALSE);		//ģʽ
	LCD_Dis_Char_16_16(3,6,&WordLib_CN[35][0],FALSE);
	LCD_Dis_ASCIIStr(3,13,":",FALSE);
	GUI_SysMode(g_savedat.mode);

	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//��ʷ
	LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		

	LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//����
	LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		

	LCD_Dis_ASCIIStr(4,19,"HOME",TRUE);						//HOME
	
	GUI_CaseData_Dis(g_level_per,1);
	homedispbusy = 0;
}


/********************************************************************************
 * ��������GUI_HomePage2()
 * ����  ����ʾ����������ʱ�䣬����
 * ����  ��-
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_HomePage2()
{
	g_homepage = 1;
	homedispbusy = 1;
	LCD_Dis_Char_16_16(1,3,&WordLib_CN[4][0],FALSE);		//ˮλ�Զ�����ϵͳ
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[5][0],FALSE);		//
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[13][0],FALSE);		//
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[14][0],FALSE);		//
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[15][0],FALSE);		//
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[16][0],FALSE);		//
	LCD_Dis_Char_16_16(1,9,&WordLib_CN[17][0],FALSE);		//
	LCD_Dis_Char_16_16(1,10,&WordLib_CN[18][0],FALSE);		//
	
	LCD_Dis_Digital_int(2,10,4,RTCtime.year,FALSE);
	LCD_Dis_Char_16_16(2,6,&WordLib_CN[10][0],FALSE);		//������
	LCD_Dis_Digital_int(2,14,2,RTCtime.month,FALSE);
	LCD_Dis_Char_16_16(2,8,&WordLib_CN[11][0],FALSE);		
	LCD_Dis_Digital_int(2,18,2,RTCtime.date,FALSE);
	LCD_Dis_Char_16_16(2,10,&WordLib_CN[12][0],FALSE);
	LCD_Dis_Digital_int(3,6,2,RTCtime.hour,FALSE);					//ʱ����
	LCD_Dis_ASCIIStr(3,7,":",FALSE);
	LCD_Dis_Digital_int(3,9,2,RTCtime.min,FALSE);
	LCD_Dis_ASCIIStr(3,10,":",FALSE);
	LCD_Dis_Digital_int(3,12,2,RTCtime.sec,FALSE);
	
	LCD_Dis_Char_16_16(3,8,&WordLib_CN[37][0],FALSE);		
	LCD_Dis_Char_16_16(3,9,&WordLib_CN[38][0],FALSE);
	switch(RTCtime.week)
	{
		case 1 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[39][0],FALSE);break;		
		case 2 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[40][0],FALSE);break;		
		case 3 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[41][0],FALSE);break;		
		case 4 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[42][0],FALSE);break;		
		case 5 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[43][0],FALSE);break;		
		case 6 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[44][0],FALSE);break;		
		case 7 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[33][0],FALSE);break;		
	}
	
	GUI_DisplayAirTemp();
	LCD_Dis_Char_8_16(4,15,&CharLib_SplLabel[0][0],FALSE);
	LCD_Dis_ASCIIStr(4,16,"C",FALSE);
	homedispbusy = 0;
}


/********************************************************************************
 * ��������GUI_HomePageUpdate()
 * ����  ����ҳ��������
 * ����  ��-selitem ��
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_HomePageUpdate()
{
	if(!homedispbusy){
		if(g_sysflag&WORK)
		{
			LCD_Dis_Digital_int(1,16,2,0,FALSE);					//ʱ����
			LCD_Dis_Digital_int(1,19,2,UpTimer1.min,FALSE);
			LCD_Dis_Digital_int(1,22,2,UpTimer1.sec,FALSE);
		}
		else{
			LCD_Dis_Digital_int(1,16,2,RTCtime.hour,FALSE);			//ʱ����
			LCD_Dis_Digital_int(1,19,2,RTCtime.min,FALSE);
			LCD_Dis_Digital_int(1,22,2,RTCtime.sec,FALSE);
		}
		GUI_SysStatus(g_sysflag);
		GUI_SysMode(g_savedat.mode);
	}
}

/********************************************************************************
 * ��������GUI_HomePage2Update()
 * ����  ����ҳ2��������
 * ����  ��-selitem ��
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_HomePage2Update()
{
	if(!homedispbusy){
		LCD_Dis_Digital_int(3,6,2,RTCtime.hour,FALSE);					//ʱ����
		LCD_Dis_Digital_int(3,9,2,RTCtime.min,FALSE);
		LCD_Dis_Digital_int(3,12,2,RTCtime.sec,FALSE);
	}
}

/********************************************************************************
 * ��������GUI_HomeMenu()
 * ����  ����ҳ�˵�ѡ��
 * ����  ��-selitem ��
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_HomeMenu(char selitem)
{
	homedispbusy = 1;

	switch(selitem)
	{
		case 1 :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//��ʷ
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//����
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",TRUE);						//HOME
		}break;
		case 2 :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//��ʷ
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],TRUE);		//����
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],TRUE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",FALSE);					//HOME
		}break;
		case 3 :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],TRUE);		//��ʷ
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],TRUE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//����
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",FALSE);					//HOME
		}break;
	}
	homedispbusy = 0;

}


/********************************************************************************
 * ��������GUI_PasswordMenu()
 * ����  ������˵�ѡ��
 * ����  ��-num= ��
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_PasswordMenu(char num)
{
	if(num==0) 	LCD_Dis_ASCIIStr(3,3,"0",TRUE);
	else 		LCD_Dis_ASCIIStr(3,3,"0",FALSE);
	if(num==1) 	LCD_Dis_ASCIIStr(3,5,"1",TRUE);
	else 		LCD_Dis_ASCIIStr(3,5,"1",FALSE);
	if(num==2) 	LCD_Dis_ASCIIStr(3,7,"2",TRUE);
	else 		LCD_Dis_ASCIIStr(3,7,"2",FALSE);
	if(num==3) 	LCD_Dis_ASCIIStr(3,9,"3",TRUE);
	else 		LCD_Dis_ASCIIStr(3,9,"3",FALSE);
	if(num==4) 	LCD_Dis_ASCIIStr(3,11,"4",TRUE);
	else 		LCD_Dis_ASCIIStr(3,11,"4",FALSE);
	if(num==5) 	LCD_Dis_ASCIIStr(3,13,"5",TRUE);
	else 		LCD_Dis_ASCIIStr(3,13,"5",FALSE);
	if(num==6) 	LCD_Dis_ASCIIStr(3,15,"6",TRUE);
	else 		LCD_Dis_ASCIIStr(3,15,"6",FALSE);
	if(num==7) 	LCD_Dis_ASCIIStr(3,17,"7",TRUE);
	else 		LCD_Dis_ASCIIStr(3,17,"7",FALSE);
	if(num==8) 	LCD_Dis_ASCIIStr(3,19,"8",TRUE);
	else 		LCD_Dis_ASCIIStr(3,19,"8",FALSE);
	if(num==9) 	LCD_Dis_ASCIIStr(3,21,"9",TRUE);
	else 		LCD_Dis_ASCIIStr(3,21,"9",FALSE);
	if(num==10)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
}


/********************************************************************************
 * ��������GUI_ModeSettingMenu()
 * ����  ��ģʽ���ò˵�ѡ��
 * ����  ��-num= ��
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_ModeSettingMenu(uchar num)
{
	if(num==1)
	{
		LCD_Dis_ASCIIStr(2,1,"1",TRUE);	
		LCD_Dis_ASCIIStr(2,2,".",TRUE);
		LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],TRUE);		//�Զ�
		LCD_Dis_Char_16_16(2,3,&WordLib_CN[14][0],TRUE);		
	}
	else{
		LCD_Dis_ASCIIStr(2,1,"1",FALSE);	
		LCD_Dis_ASCIIStr(2,2,".",FALSE);
		LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],FALSE);		
		LCD_Dis_Char_16_16(2,3,&WordLib_CN[14][0],FALSE);		
	}
	if(num==2)
	{
		LCD_Dis_ASCIIStr(3,1,"2",TRUE);	
		LCD_Dis_ASCIIStr(3,2,".",TRUE);
		LCD_Dis_Char_16_16(3,2,&WordLib_CN[36][0],TRUE);		//�ֶ�
		LCD_Dis_Char_16_16(3,3,&WordLib_CN[14][0],TRUE);		
	}
	else{
		LCD_Dis_ASCIIStr(3,1,"2",FALSE);	
		LCD_Dis_ASCIIStr(3,2,".",FALSE);
		LCD_Dis_Char_16_16(3,2,&WordLib_CN[36][0],FALSE);		
		LCD_Dis_Char_16_16(3,3,&WordLib_CN[14][0],FALSE);		
	}	
	if(num==0)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}	
}


/********************************************************************************
 * ��������GUI_BLSettingMenu()
 * ����  ��ģʽ���ò˵�ѡ��
 * ����  ��-wei= ��
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_BLSettingMenu(uchar wei)
{
	if(wei==1)
	{
		LCD_Dis_ASCIIStr(2,1,"1",TRUE);	
		LCD_Dis_ASCIIStr(2,2,".",TRUE);
		LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],TRUE);		//�Զ�
		LCD_Dis_Char_16_16(2,3,&WordLib_CN[14][0],TRUE);		
	}
	else{
		LCD_Dis_ASCIIStr(2,1,"1",FALSE);	
		LCD_Dis_ASCIIStr(2,2,".",FALSE);
		LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],FALSE);		
		LCD_Dis_Char_16_16(2,3,&WordLib_CN[14][0],FALSE);		
	}
	if(wei==2)
	{
		LCD_Dis_ASCIIStr(3,1,"2",TRUE);	
		LCD_Dis_ASCIIStr(3,2,".",TRUE);
		LCD_Dis_Char_16_16(3,2,&WordLib_CN[9][0],TRUE);			//�ر�
		LCD_Dis_Char_16_16(3,3,&WordLib_CN[71][0],TRUE);		
	}
	else{
		LCD_Dis_ASCIIStr(3,1,"2",FALSE);	
		LCD_Dis_ASCIIStr(3,2,".",FALSE);
		LCD_Dis_Char_16_16(3,2,&WordLib_CN[9][0],FALSE);		
		LCD_Dis_Char_16_16(3,3,&WordLib_CN[71][0],FALSE);		
	}	
	if(wei==0)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
}

/********************************************************************************
 * ��������GUI_PecentSettingMenu()
 * ����  ���������ְٷֱ�ѡ��
 * ����  ��-wei�ڼ�λ�� ��num�ٷ���
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_PecentSettingMenu(uchar num,uchar wei)
{
	uchar bai,shi,ge;
	bai = num/100;
	shi = num/10%10;
	ge = num%10;
	if(wei==3)
		LCD_Dis_Digital_int(2,10,1,bai,TRUE);
	else LCD_Dis_Digital_int(2,10,1,bai,FALSE);
	if(wei==2)
		LCD_Dis_Digital_int(2,11,1,shi,TRUE);
	else LCD_Dis_Digital_int(2,11,1,shi,FALSE);
	if(wei==1)
		LCD_Dis_Digital_int(2,12,1,ge,TRUE);
	else LCD_Dis_Digital_int(2,12,1,ge,FALSE);
	if(wei==0)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}

	LCD_Dis_ASCIIStr(2,13,"%",FALSE);
}

/********************************************************************************
 * ��������GUI_DateTimeSettingMenu()
 * ����  ������ʱ������ѡ��
 * ����  ��-
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_DateTimeSettingMenu(STC_TIME dat,uchar wei)
{
	uchar bit2,bit1;
	
	bit2 = dat.year/10%10;
	bit1 = dat.year%10;
	if(wei==1)
		LCD_Dis_Digital_int(2,7,1,bit2,TRUE);
	else LCD_Dis_Digital_int(2,7,1,bit2,FALSE);
	if(wei==2)
		LCD_Dis_Digital_int(2,8,1,bit1,TRUE);
	else LCD_Dis_Digital_int(2,8,1,bit1,FALSE);	

	if(wei==3)
		LCD_Dis_Digital_int(2,12,2,dat.month,TRUE);
	else LCD_Dis_Digital_int(2,12,2,dat.month,FALSE);
	
	if(wei==4)
		LCD_Dis_Digital_int(2,16,2,dat.date,TRUE);
	else LCD_Dis_Digital_int(2,16,2,dat.date,FALSE);

	if(wei==5)
		LCD_Dis_Digital_int(3,6,2,dat.hour,TRUE);
	else LCD_Dis_Digital_int(3,6,2,dat.hour,FALSE);

	if(wei==6)
		LCD_Dis_Digital_int(3,9,2,dat.min,TRUE);
	else LCD_Dis_Digital_int(3,9,2,dat.min,FALSE);
	
	if(wei==7)
		LCD_Dis_Digital_int(3,12,2,dat.sec,TRUE);
	else LCD_Dis_Digital_int(3,12,2,dat.sec,FALSE);

	if(wei==8)
		switch(dat.week)
		{
			case 1 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[39][0],TRUE);break;		
			case 2 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[40][0],TRUE);break;		
			case 3 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[41][0],TRUE);break;		
			case 4 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[42][0],TRUE);break;		
			case 5 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[43][0],TRUE);break;		
			case 6 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[44][0],TRUE);break;		
			case 7 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[33][0],TRUE);break;		
		}
	else
		switch(dat.week)
		{
			case 1 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[39][0],FALSE);break;		
			case 2 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[40][0],FALSE);break;		
			case 3 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[41][0],FALSE);break;		
			case 4 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[42][0],FALSE);break;		
			case 5 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[43][0],FALSE);break;		
			case 6 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[44][0],FALSE);break;		
			case 7 : 	LCD_Dis_Char_16_16(3,10,&WordLib_CN[33][0],FALSE);break;		
		}
	if(wei==9)		
	{
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[64][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[65][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[64][0],FALSE);		
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[65][0],FALSE);		
	}
	if(wei==0)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
	

}

/********************************************************************************
 * ��������GUI_KEYSettingMenu()
 * ����  ����������ѡ��
 * ����  ��-
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_KEYSettingMenu(char keyon,char wei)
{
	if(wei==1)
		if(keyon)
		{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[8][0],TRUE);		//����
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[9][0],FALSE);	
		}
		else{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[8][0],FALSE);		
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[9][0],TRUE);		
		}	
	else{
		if(keyon)
		{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[8][0],FALSE);		
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[9][0],FALSE);	
			LCD_fill_Region(17,81,16,1);
			LCD_fill_Region(17,81,1,16);
			LCD_fill_Region(17,97,16,1);
			LCD_fill_Region(32,81,1,16);
		}
		else{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[8][0],FALSE);		
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[9][0],FALSE);
			LCD_fill_Region(17,97,16,1);
			LCD_fill_Region(17,97,1,16);
			LCD_fill_Region(17,112,16,1);
			LCD_fill_Region(32,97,1,16);			
		}	
	}
	if(wei==2)
	{
		LCD_Dis_Char_16_16(3,5,&WordLib_CN[59][0],TRUE);		//�޸�����
		LCD_Dis_Char_16_16(3,6,&WordLib_CN[60][0],TRUE);
		LCD_Dis_Char_16_16(3,7,&WordLib_CN[48][0],TRUE);		
		LCD_Dis_Char_16_16(3,8,&WordLib_CN[49][0],TRUE);		
	}
	else{
		LCD_Dis_Char_16_16(3,5,&WordLib_CN[59][0],FALSE);		
		LCD_Dis_Char_16_16(3,6,&WordLib_CN[60][0],FALSE);
		LCD_Dis_Char_16_16(3,7,&WordLib_CN[48][0],FALSE);		
		LCD_Dis_Char_16_16(3,8,&WordLib_CN[49][0],FALSE);		
	}
	if(wei==0)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
	if(wei==3)		
	{
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[64][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[65][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[64][0],FALSE);		
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[65][0],FALSE);		
	}
	
}

/********************************************************************************
 * ��������GUI_DEFAULTSettingMenu()
 * ����  ���ָ���������ѡ��
 * ����  ��-
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_DEFAULTSettingMenu(char yes,uchar wei)
{
	if(wei==1)
		if(yes)
		{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[76][0],TRUE);		//�Ƿ�
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[77][0],FALSE);	
		}
		else{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[76][0],FALSE);		
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[77][0],TRUE);		
		}	
	else{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[76][0],FALSE);		
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[77][0],FALSE);	
		if(yes)
		{

			LCD_fill_Region(17,81,16,1);
			LCD_fill_Region(17,81,1,16);
			LCD_fill_Region(17,97,16,1);
			LCD_fill_Region(32,81,1,16);
		}
		else{
			LCD_fill_Region(17,97,16,1);
			LCD_fill_Region(17,97,1,16);
			LCD_fill_Region(17,112,16,1);
			LCD_fill_Region(32,97,1,16);			
		}	
	}
	if(wei==2)		
	{
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[78][0],TRUE);		//ȷ��
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[79][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[78][0],FALSE);		
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[79][0],FALSE);		
	}
	if(wei==0)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//����
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
	
}

/********************************************************************************
 * ��������GUI_SettingMenu()
 * ����  �����ò˵�ѡ��
 * ����  ��-num= ��
 * ����  ��- 
 * ����  ���ڲ�����
 ********************************************************************************/
void GUI_SettingMenu(char num, char turn)
{
	if(turn) LCD_Clear();											//��ҳ
	if(num<5)
	{
		if(num==1)
		{
			LCD_Dis_ASCIIStr(1,1,"1",TRUE);	
			LCD_Dis_ASCIIStr(1,2,".",TRUE);
			LCD_Dis_Char_16_16(1,2,&WordLib_CN[34][0],TRUE);		//ģʽ����
			LCD_Dis_Char_16_16(1,3,&WordLib_CN[35][0],TRUE);		
			LCD_Dis_Char_16_16(1,4,&WordLib_CN[27][0],TRUE);		
			LCD_Dis_Char_16_16(1,5,&WordLib_CN[28][0],TRUE);		
		}
		else{
			LCD_Dis_ASCIIStr(1,1,"1",FALSE);	
			LCD_Dis_ASCIIStr(1,2,".",FALSE);
			LCD_Dis_Char_16_16(1,2,&WordLib_CN[34][0],FALSE);		
			LCD_Dis_Char_16_16(1,3,&WordLib_CN[35][0],FALSE);		
			LCD_Dis_Char_16_16(1,4,&WordLib_CN[27][0],FALSE);		
			LCD_Dis_Char_16_16(1,5,&WordLib_CN[28][0],FALSE);		
		}
		if(num==2)
		{
			LCD_Dis_ASCIIStr(2,1,"2",TRUE);	
			LCD_Dis_ASCIIStr(2,2,".",TRUE);
			LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],TRUE);		//�Զ���ˮҺλ����
			LCD_Dis_Char_16_16(2,3,&WordLib_CN[14][0],TRUE);		
			LCD_Dis_Char_16_16(2,4,&WordLib_CN[52][0],TRUE);		
			LCD_Dis_Char_16_16(2,5,&WordLib_CN[4][0],TRUE);		
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[4][0],TRUE);		
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[5][0],TRUE);		
			LCD_Dis_Char_16_16(2,8,&WordLib_CN[27][0],TRUE);		
			LCD_Dis_Char_16_16(2,9,&WordLib_CN[28][0],TRUE);			
		}
		else{
			LCD_Dis_ASCIIStr(2,1,"2",FALSE);	
			LCD_Dis_ASCIIStr(2,2,".",FALSE);
			LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],FALSE);		
			LCD_Dis_Char_16_16(2,3,&WordLib_CN[14][0],FALSE);		
			LCD_Dis_Char_16_16(2,4,&WordLib_CN[52][0],FALSE);		
			LCD_Dis_Char_16_16(2,5,&WordLib_CN[4][0],FALSE);		
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[4][0],FALSE);		
			LCD_Dis_Char_16_16(2,7,&WordLib_CN[5][0],FALSE);		
			LCD_Dis_Char_16_16(2,8,&WordLib_CN[27][0],FALSE);		
			LCD_Dis_Char_16_16(2,9,&WordLib_CN[28][0],FALSE);			
		}
		if(num==3)
		{
			LCD_Dis_ASCIIStr(3,1,"3",TRUE);	
			LCD_Dis_ASCIIStr(3,2,".",TRUE);
			LCD_Dis_Char_16_16(3,2,&WordLib_CN[13][0],TRUE);		//�Զ�ͣˮҺλ����
			LCD_Dis_Char_16_16(3,3,&WordLib_CN[14][0],TRUE);		
			LCD_Dis_Char_16_16(3,4,&WordLib_CN[56][0],TRUE);		
			LCD_Dis_Char_16_16(3,5,&WordLib_CN[4][0],TRUE);		
			LCD_Dis_Char_16_16(3,6,&WordLib_CN[4][0],TRUE);		
			LCD_Dis_Char_16_16(3,7,&WordLib_CN[5][0],TRUE);		
			LCD_Dis_Char_16_16(3,8,&WordLib_CN[27][0],TRUE);		
			LCD_Dis_Char_16_16(3,9,&WordLib_CN[28][0],TRUE);			
		}
		else{
			LCD_Dis_ASCIIStr(3,1,"3",FALSE);	
			LCD_Dis_ASCIIStr(3,2,".",FALSE);
			LCD_Dis_Char_16_16(3,2,&WordLib_CN[13][0],FALSE);		
			LCD_Dis_Char_16_16(3,3,&WordLib_CN[14][0],FALSE);		
			LCD_Dis_Char_16_16(3,4,&WordLib_CN[56][0],FALSE);		
			LCD_Dis_Char_16_16(3,5,&WordLib_CN[4][0],FALSE);		
			LCD_Dis_Char_16_16(3,6,&WordLib_CN[4][0],FALSE);		
			LCD_Dis_Char_16_16(3,7,&WordLib_CN[5][0],FALSE);		
			LCD_Dis_Char_16_16(3,8,&WordLib_CN[27][0],FALSE);		
			LCD_Dis_Char_16_16(3,9,&WordLib_CN[28][0],FALSE);				
		}
		if(num==4)
		{
			LCD_Dis_ASCIIStr(4,1,"4",TRUE);	
			LCD_Dis_ASCIIStr(4,2,".",TRUE);
			LCD_Dis_Char_16_16(4,2,&WordLib_CN[12][0],TRUE);		//ʱ������
			LCD_Dis_Char_16_16(4,3,&WordLib_CN[38][0],TRUE);		
			LCD_Dis_Char_16_16(4,4,&WordLib_CN[6][0],TRUE);		
			LCD_Dis_Char_16_16(4,5,&WordLib_CN[7][0],TRUE);		
			LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],TRUE);		
			LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],TRUE);		
		}
		else{
			LCD_Dis_ASCIIStr(4,1,"4",FALSE);	
			LCD_Dis_ASCIIStr(4,2,".",FALSE);
			LCD_Dis_Char_16_16(4,2,&WordLib_CN[12][0],FALSE);		
			LCD_Dis_Char_16_16(4,3,&WordLib_CN[38][0],FALSE);		
			LCD_Dis_Char_16_16(4,4,&WordLib_CN[6][0],FALSE);		
			LCD_Dis_Char_16_16(4,5,&WordLib_CN[7][0],FALSE);		
			LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		
			LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		
		}
	}//if(num<5)
	else{
		if(num==5)
		{
			LCD_Dis_ASCIIStr(1,1,"5",TRUE);	
			LCD_Dis_ASCIIStr(1,2,".",TRUE);
			LCD_Dis_Char_16_16(1,2,&WordLib_CN[48][0],TRUE);		//��������
			LCD_Dis_Char_16_16(1,3,&WordLib_CN[49][0],TRUE);		
			LCD_Dis_Char_16_16(1,4,&WordLib_CN[27][0],TRUE);		
			LCD_Dis_Char_16_16(1,5,&WordLib_CN[28][0],TRUE);		
		}
		else{
			LCD_Dis_ASCIIStr(1,1,"5",FALSE);	
			LCD_Dis_ASCIIStr(1,2,".",FALSE);
			LCD_Dis_Char_16_16(1,2,&WordLib_CN[48][0],FALSE);		
			LCD_Dis_Char_16_16(1,3,&WordLib_CN[49][0],FALSE);		
			LCD_Dis_Char_16_16(1,4,&WordLib_CN[27][0],FALSE);		
			LCD_Dis_Char_16_16(1,5,&WordLib_CN[28][0],FALSE);		
		}
		if(num==6)
		{
			LCD_Dis_ASCIIStr(2,1,"6",TRUE);	
			LCD_Dis_ASCIIStr(2,2,".",TRUE);
			LCD_Dis_Char_16_16(2,2,&WordLib_CN[57][0],TRUE);		//��������
			LCD_Dis_Char_16_16(2,3,&WordLib_CN[58][0],TRUE);		
			LCD_Dis_Char_16_16(2,4,&WordLib_CN[27][0],TRUE);		
			LCD_Dis_Char_16_16(2,5,&WordLib_CN[28][0],TRUE);			
		}
		else{
			LCD_Dis_ASCIIStr(2,1,"6",FALSE);	
			LCD_Dis_ASCIIStr(2,2,".",FALSE);
			LCD_Dis_Char_16_16(2,2,&WordLib_CN[57][0],FALSE);		
			LCD_Dis_Char_16_16(2,3,&WordLib_CN[58][0],FALSE);		
			LCD_Dis_Char_16_16(2,4,&WordLib_CN[27][0],FALSE);		
			LCD_Dis_Char_16_16(2,5,&WordLib_CN[28][0],FALSE);		
		}
		if(num==7)
		{
			LCD_Dis_ASCIIStr(3,1,"7",TRUE);	
			LCD_Dis_ASCIIStr(3,2,".",TRUE);
			LCD_Dis_Char_16_16(3,2,&WordLib_CN[72][0],TRUE);		//�ָ�Ĭ��
			LCD_Dis_Char_16_16(3,3,&WordLib_CN[73][0],TRUE);		
			LCD_Dis_Char_16_16(3,4,&WordLib_CN[74][0],TRUE);		
			LCD_Dis_Char_16_16(3,5,&WordLib_CN[75][0],TRUE);			
		}
		else{
			LCD_Dis_ASCIIStr(3,1,"7",FALSE);	
			LCD_Dis_ASCIIStr(3,2,".",FALSE);
			LCD_Dis_Char_16_16(3,2,&WordLib_CN[72][0],FALSE);		
			LCD_Dis_Char_16_16(3,3,&WordLib_CN[73][0],FALSE);		
			LCD_Dis_Char_16_16(3,4,&WordLib_CN[74][0],FALSE);		
			LCD_Dis_Char_16_16(3,5,&WordLib_CN[75][0],FALSE);		
		}
	}
}


/********************************************************************************
 * ��������GUI_CaseData_Dis()
 * ����  ��ˮ��������ʾ
 * ����  ��-		    	refill=1 ���ڽ����л�ʱ���»���ˮλ
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_CaseData_Dis(char percent,char refill)
{
	char filevel;
	static signed char filevel_smooth=0,filevel_last = 0;
	static char percent_last=0;
	uchar fallflag = 0;
	uchar i;
	
	//������ʾæ��־�������жϴ���
	homedispbusy = 1;
	
	filevel = (signed char)(percent/2.44+1);
	
	if(refill)	filevel_last = 0;						//��Ҫ������䣬����ʷҺλ����
	
	filevel_smooth = filevel - filevel_last;
	if(filevel_smooth<0)
	{
		fallflag = 1;
		filevel_smooth = 0-filevel_smooth;
	}
	for(i=0;i<filevel_smooth;i++)
	{		
		if(fallflag)	filevel_last -= 1;
		else 			filevel_last += 1;
		//filevel_last = (fallflag?(filevel_last-i):(fallflag+i));	//Ϊʲô���У�
		switch(filevel_last)
		{
			case 1 : 
			{	if(fallflag)										//���½�
					LCD_Clear_Region(42,5,1,43);					//������һ��ˮƽ��
				else
				{
					LCD_fill_Region(43,6,1,41);
				}
			}break;
			case 2 : 
			{	if(fallflag)										//���½�
					LCD_Clear_Region(41,4,1,45);					//������һ��ˮƽ��
				else
				{
					LCD_fill_Region(42,5,1,43);
				}
			}break;			
			case 3 : 
			{	if(fallflag)										//���½�
					LCD_Clear_Region(40,4,1,45);					//������һ��ˮƽ��
				else
				{
					LCD_fill_Region(41,4,1,45);
				}
			}break;
			case 4 :
			{	if(fallflag)										//���½�
					LCD_Clear_Region(39,3,1,47);					//������һ��ˮƽ��
				else
				{
					LCD_fill_Region(40,4,1,45);
				}
			}break;
			case 38 :
			{	if(fallflag)							
					LCD_Clear_Region(5,4,1,45);		
				else
				{
					LCD_fill_Region(6,4,1,45);
				}
			}break;
			case 39 :
			{	if(fallflag)							
					LCD_Clear_Region(4,5,1,43);		
				else
				{
					LCD_fill_Region(5,4,1,45);
				}
			}break;
			case 40 :
			{	if(fallflag)										//���½�
					LCD_Clear_Region(3,6,1,41);						//������һ��ˮƽ��
				else
				{
					LCD_fill_Region(4,5,1,43);
				}
			}break;
			case 41 :
			{	
					LCD_fill_Region(3,6,1,41);
			}break;	
			default :
			{	
				if(fallflag)										//���½�
					LCD_Clear_Region(43-filevel_last,3,1,47);		//������һ��ˮƽ��
				else
				{
					LCD_fill_Region(44-filevel_last,3,1,47);
				}				
			}break;
		}
	}
	if(percent!=0)
		LCD_Dis_Digital_int(2,4,0,percent,FALSE);
	else 
		LCD_Dis_Digital_int(2,4,1,percent,FALSE);
	LCD_Dis_ASCIIStr(2,5,"%",FALSE);
	if(percent - percent_last<0)	//��100%��99%��ǰ��һλ��ˮ���
	{
		if(percent==99)
			LCD_fill_Region(17,9,16,8);
		else if(percent==9)
			LCD_fill_Region(17,9,16,16);
	}
	percent_last = percent;
	
	//��λ��ʾæ��־
	homedispbusy = 0;
}



uchar code Pic_arrow[]={
	0x00,0xF0,0xFF,0xF0,0x00,
	0x1C,0xFF,0xFF,0xFF,0x1C,
	0x00,0xFF,0xFF,0xFF,0x00,
};

/******************** (C) COPYRIGHT 2015 DammStanger ****END OF FILE***************************/
