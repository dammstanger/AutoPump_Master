/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��LCD_GUI.c
 * ��	��	��GUI���
 *                    
 * ʵ��ƽ̨	��51������
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
/****************************��������*********************************************/
bit g_homepage = 0;				//�������ǣ�0����1 =1����2

char needkey = 0;
long keyword = 123456;
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

void GUI_PasswordMenu(char num)
{
	if(num==0) 	LCD_Dis_ASCIIStr(4,3,"0",TRUE);
	else 		LCD_Dis_ASCIIStr(4,3,"0",FALSE);
	if(num==1) 	LCD_Dis_ASCIIStr(4,5,"1",TRUE);
	else 		LCD_Dis_ASCIIStr(4,5,"1",FALSE);
	if(num==2) 	LCD_Dis_ASCIIStr(4,7,"2",TRUE);
	else 		LCD_Dis_ASCIIStr(4,7,"2",FALSE);
	if(num==3) 	LCD_Dis_ASCIIStr(4,9,"3",TRUE);
	else 		LCD_Dis_ASCIIStr(4,9,"3",FALSE);
	if(num==4) 	LCD_Dis_ASCIIStr(4,11,"4",TRUE);
	else 		LCD_Dis_ASCIIStr(4,11,"4",FALSE);
	if(num==5) 	LCD_Dis_ASCIIStr(4,13,"5",TRUE);
	else 		LCD_Dis_ASCIIStr(4,13,"5",FALSE);
	if(num==6) 	LCD_Dis_ASCIIStr(4,15,"6",TRUE);
	else 		LCD_Dis_ASCIIStr(4,15,"6",FALSE);
	if(num==7) 	LCD_Dis_ASCIIStr(4,17,"7",TRUE);
	else 		LCD_Dis_ASCIIStr(4,17,"7",FALSE);
	if(num==8) 	LCD_Dis_ASCIIStr(4,19,"8",TRUE);
	else 		LCD_Dis_ASCIIStr(4,19,"8",FALSE);
	if(num==9) 	LCD_Dis_ASCIIStr(4,21,"9",TRUE);
	else 		LCD_Dis_ASCIIStr(4,21,"9",FALSE);
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
	long input=0;
	
	if(!needkey) {g_menumark = MENU_SET;return 0;}			//û����������
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[45][0],FALSE);		//����������
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[46][0],FALSE);	
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[47][0],FALSE);		//
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[48][0],FALSE);	
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[49][0],FALSE);		
	LCD_Dis_ASCIIStr(1,17,":",FALSE);
	GUI_PasswordMenu(selitem);	
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
			if(selitem>9) selitem = 0;
			else if(selitem<0) selitem = 9;
			GUI_PasswordMenu(selitem);
			
			if(keyval==KEY_ENTER)
			{
				if(wei-->0) 
				{
					input *=10;
					input += selitem;
					LCD_Dis_ASCIIStr(2,17-wei*2,"*",FALSE);
				}
				else{
					if(input==keyword)
					{
						g_menumark = MENU_SET;return 1;
					}
					else{
						LCD_Clear_Region(17,1,16,192);
						LCD_Dis_Char_16_16(2,5,&WordLib_CN[48][0],FALSE);	
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
			}//if(keyval==KEY_ENTER)
		}//if(keyval!=KEY_NONE)
	}//while(1)
		
}

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
			LCD_Dis_Char_16_16(1,2,&WordLib_CN[34][0],FALSE);		//ģʽ����
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
			LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],FALSE);		//�Զ���ˮҺλ����
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
			LCD_Dis_Char_16_16(3,2,&WordLib_CN[13][0],FALSE);		//�Զ�ͣˮҺλ����
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
			LCD_Dis_Char_16_16(4,2,&WordLib_CN[12][0],FALSE);		//ʱ������
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
			LCD_Dis_Char_16_16(1,2,&WordLib_CN[48][0],FALSE);		//��������
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
			LCD_Dis_Char_16_16(2,2,&WordLib_CN[57][0],FALSE);		//��������
			LCD_Dis_Char_16_16(2,3,&WordLib_CN[58][0],FALSE);		
			LCD_Dis_Char_16_16(2,4,&WordLib_CN[27][0],FALSE);		
			LCD_Dis_Char_16_16(2,5,&WordLib_CN[28][0],FALSE);		
		}
	}
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
				if(selitem<1) {selitem = 6;turn = 1;}
				else if(selitem==4){turn = 1;}
				else turn = 0;
			}
			else if(keyval==KEY_DOWN)
			{
				selitem++;
				if(selitem>6) {selitem = 1;turn = 1;}
				else if(selitem==5){turn = 1;}
				else turn = 0;
			}
			GUI_SettingMenu(selitem,turn);
			
			if(keyval==KEY_ENTER||keyval==KEY_R)
			{
				g_menumark = MENU_SET|(selitem<<4);
				return ;
			}//if(keyval==KEY_ENTER)
		}//if(keyval!=KEY_NONE)
	}//while(1)
}


void GUI_ModeSettingMenu(char num)
{
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[34][0],FALSE);			//ģʽ����
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[35][0],FALSE);		
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[27][0],FALSE);		
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[28][0],FALSE);	
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
	char keyval,selitem=1;
	LCD_Clear();
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
			else if(keyval==KEY_UP)
			{
				selitem--;
				if(selitem<1) selitem = 2;
			}
			else if(keyval==KEY_DOWN)
			{
				selitem++;
				if(selitem>2) selitem = 1;
			}
			GUI_ModeSettingMenu(selitem);
			
			if(keyval==KEY_ENTER)
			{
				g_menumark = MENU_OPERATE;
				return ;
			}//if(keyval==KEY_ENTER)
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
 * ��������GUI_DisplayWaterTemp()
 * ����  ����ʾˮ��
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void GUI_DisplayWaterTemp()
{	
	//��ʾ�¶�
	LCD_Dis_Digital_float(1,22,tmp_data.tmp2);
}


/********************************************************************************
 * ��������GUI_DisplayAirTemp()
 * ����  ����ʾˮ��
 * ����  ��-		    	
 * ����  ��- 
 * ����  �����ⲿ����
 ********************************************************************************/
void GUI_DisplayAirTemp()
{	
	//��ʾ�¶�
	LCD_Dis_Digital_float(4,14,tmp_data.tmp1);						//���������˳������
}



void GUI_HomeMenu(char selitem)
{
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
		SendByteASCII(keyval);
		SendString("\n\r");
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
	LCD_Dis_Pict(1,1,45,62,Pic_Case);
	
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[0][0],FALSE);		//ʵʱˮ��
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[1][0],FALSE);		//
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[4][0],FALSE);		//
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[2][0],FALSE);		//	
	LCD_Dis_ASCIIStr(1,17,":",FALSE);
	GUI_DisplayWaterTemp();
	LCD_Dis_Char_8_16(1,23,&CharLib_SplLabel[0][0],FALSE);
	LCD_Dis_ASCIIStr(1,24,"C",FALSE);
	
	LCD_Dis_Char_16_16(2,5,&WordLib_CN[19][0],FALSE);		//״̬
	LCD_Dis_Char_16_16(2,6,&WordLib_CN[20][0],FALSE);
	LCD_Dis_ASCIIStr(2,13,":",FALSE);
	LCD_Dis_Char_16_16(2,10,&WordLib_CN[21][0],FALSE);		//����
	LCD_Dis_Char_16_16(2,11,&WordLib_CN[22][0],FALSE);	
	
	LCD_Dis_Char_16_16(3,5,&WordLib_CN[34][0],FALSE);		//ģʽ
	LCD_Dis_Char_16_16(3,6,&WordLib_CN[35][0],FALSE);
	LCD_Dis_ASCIIStr(3,13,":",FALSE);
	LCD_Dis_Char_16_16(3,10,&WordLib_CN[13][0],FALSE);		//�Զ�
	LCD_Dis_Char_16_16(3,11,&WordLib_CN[14][0],FALSE);	

	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//��ʷ
	LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		

	LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//����
	LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		

	LCD_Dis_ASCIIStr(4,19,"HOME",TRUE);						//HOME
	
	GUI_CaseData_Dis(55,1);
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
	LCD_Dis_Char_16_16(1,3,&WordLib_CN[4][0],FALSE);		//ˮλ�Զ�����ϵͳ
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[5][0],FALSE);		//
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[13][0],FALSE);		//
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[14][0],FALSE);		//
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[15][0],FALSE);		//
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[16][0],FALSE);		//
	LCD_Dis_Char_16_16(1,9,&WordLib_CN[17][0],FALSE);		//
	LCD_Dis_Char_16_16(1,10,&WordLib_CN[18][0],FALSE);		//
	
	LCD_Dis_Digital_int(2,10,4,RTCtime.year);
	LCD_Dis_Char_16_16(2,6,&WordLib_CN[10][0],FALSE);		//������
	LCD_Dis_Digital_int(2,14,2,RTCtime.month);
	LCD_Dis_Char_16_16(2,8,&WordLib_CN[11][0],FALSE);		
	LCD_Dis_Digital_int(2,18,2,RTCtime.date);
	LCD_Dis_Char_16_16(2,10,&WordLib_CN[12][0],FALSE);
	LCD_Dis_Digital_int(3,6,2,RTCtime.hour);					//ʱ����
	LCD_Dis_ASCIIStr(3,7,":",FALSE);
	LCD_Dis_Digital_int(3,9,2,RTCtime.min);
	LCD_Dis_ASCIIStr(3,10,":",FALSE);
	LCD_Dis_Digital_int(3,12,2,RTCtime.sec);
	
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
	
//	LCD_Dis_Char_16_16(4,1,&WordLib_CN[31][0],FALSE);		//����
//	LCD_Dis_Char_16_16(4,2,&WordLib_CN[2][0],FALSE);		//	
//	LCD_Dis_ASCIIStr(4,5,":",FALSE);
	GUI_DisplayAirTemp();
	LCD_Dis_Char_8_16(4,15,&CharLib_SplLabel[0][0],FALSE);
	LCD_Dis_ASCIIStr(4,16,"C",FALSE);
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
		//filevel_last = (fallflag?(filevel_last-i):(fallflag+i));			//Ϊʲô���У�
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
	LCD_Dis_Digital_int(2,4,0,percent);
	LCD_Dis_ASCIIStr(2,5,"%",FALSE);
	if(percent==99&&percent - percent_last<0)	//��100%��99%��ǰ��һλ��ˮ���
	{
		LCD_fill_Region(17,9,16,8);
	}
	percent_last = percent;
}


uchar code Pic_arrow[]={
	0x00,0xF0,0xFF,0xF0,0x00,
	0x1C,0xFF,0xFF,0xFF,0x1C,
	0x00,0xFF,0xFF,0xFF,0x00,
};

/******************** (C) COPYRIGHT 2015 DammStanger ****END OF FILE***************************/
