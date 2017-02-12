/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：LCD_GUI.c
 * 描	述	：GUI设计
 *                    
 * 实验平台	：AutoPumpV2.0
 * 硬件连接	：
 * 版 	本	：V0.0.160217
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2015.7.14
 * 最后编辑	：2016.2.20
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
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

/****************************宏定义*********************************************/
#define KS_NORMAL			0
#define KS_KEYSET_OK		1
#define KS_KEYSET2			2
#define KS_KEYSET			3
#define KS_KEYSET_ERR		4
/****************************变量定义*********************************************/
bit g_homepage = 0;				//主界面标记，0界面1 =1界面2
uchar g_keycmd = KS_NORMAL;
bit homedispbusy = 0;			//主界面显示忙碌标志，为1时禁止中断内的主界面更新

/****************************函数声明*********************************************/
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
 * 函数名：GUI_DisplayWaterTemp()
 * 描述  ：显示水温
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_DisplayWaterTemp()
{	
	//显示温度
	LCD_Dis_Digital_float(1,22,tmp_data.tmp1);
}


/********************************************************************************
 * 函数名：GUI_DisplayAirTemp()
 * 描述  ：显示气温
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：内外部调用
 ********************************************************************************/
void GUI_DisplayAirTemp()
{	
	//显示温度
	LCD_Dis_Digital_float(4,14,tmp_data.tmp2);						//从右往左的顺序增长
}


/********************************************************************************
 * 函数名：LCD_FlashArrows()
 * 描述  ：箭头闪烁
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
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
 * 函数名：GUI_DisplayPassword()
 * 描述  ：显示密码界面
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
char GUI_DisplayPassword()
{
	char keyval,selitem=0,wei=6,i=5;
	long input=0,input2=0;
	if(g_keycmd==KS_NORMAL)
	{
		if(!g_savedat.s_needkey) {g_menumark = MENU_SET;return 0;}			//没有设置密码
		LCD_Clear();
		LCD_Dis_Char_16_16(1,4,&WordLib_CN[45][0],FALSE);		//请输入密码
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
		LCD_Dis_Char_16_16(1,4,&WordLib_CN[45][0],FALSE);		//请输入新密码
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
			{	//---------------------返回--------------------------------
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
				//---------------输入数字与确认与--------------------------
				if(wei-->0) 			
				{
					input *=10;
					input += selitem;
					LCD_Dis_ASCIIStr(2,17-wei*2,"*",FALSE);
				}
				else{
					//--------------密码设置首次输入-----------------------
					if(g_keycmd==KS_KEYSET)		
					{
						g_keycmd = KS_KEYSET2;
						input2 = input;
						LCD_Clear_Region(1,1,32,192);
						LCD_Dis_Char_16_16(1,3,&WordLib_CN[45][0],FALSE);		//请再次输入新密码
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
					//--------------密码设置二次输入-----------------------
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
							LCD_Dis_Char_16_16(2,5,&WordLib_CN[48][0],FALSE);	//密码不符！
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
					//--------------正常密码输入-----------------------
					else if(g_keycmd==KS_NORMAL)
					{
						if(input==g_savedat.s_keyword)
						{
							g_menumark = MENU_SET;return 1;
						}
						else{
							LCD_Clear_Region(17,1,16,192);
							LCD_Dis_Char_16_16(2,5,&WordLib_CN[48][0],FALSE);	//密码错误！
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
 * 函数名：GUI_Setting()
 * 描述  ：显示
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
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
 * 函数名：GUI_ModeSetting()
 * 描述  ：显示模式设置
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_ModeSetting()
{
	char keyval;
	char selitem;
	
	selitem = g_savedat.mode;
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[34][0],FALSE);			//模式设置
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
					g_savedat.mode = AUTO;			//自动
					g_menumark = MENU_SET;
					DAT_SaveDat(S_MODE,g_savedat);
					return ;				
				}
				else if(selitem==2) 
				{
					g_savedat.mode = MANAUL;		//手动
					g_menumark = MENU_SET;
					DAT_SaveDat(S_MODE,g_savedat);
					return ;					
				}
			}//if(keyval==KEY_ENTER)
		}//if(keyval!=KEY_NONE)
	}//while(1)
	
}



/********************************************************************************
 * 函数名：GUI_SRARTLSetting()
 * 描述  ：自动抽水液位设置
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_SRARTLSetting()
{
	char keyval,wei = 3;
	char input;
	
	input = g_savedat.s_startl;
	LCD_Clear();
	LCD_Dis_Char_16_16(1,2,&WordLib_CN[13][0],FALSE);		//自动抽水液位设置
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
				if(input<0||input>100) input = 100;			//溢出转负啦，赋予最大值
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
						g_savedat.s_startl = input;			//更新抽水液位
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
 * 函数名：GUI_STOPLSetting()
 * 描述  ：自动停水液位设置
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_STOPLSetting()
{
	char keyval,wei = 3;
	char input;
	
	input = g_savedat.s_stopl;
	
	LCD_Clear();
	LCD_Dis_Char_16_16(1,2,&WordLib_CN[13][0],FALSE);		//自动停水液位设置
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
				if(input<0||input>100) input = 100;			//溢出转负啦，赋予最大值
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
						g_savedat.s_stopl = input;			//更新抽水液位
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
 * 函数名：GUI_DATETIMESetting()
 * 描述  ：时间日期设置
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_DATETIMESetting()
{
	char keyval,wei = 1;
	STC_TIME input;
	
	input = RTCtime;
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[12][0],FALSE);		//时间日期
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[38][0],FALSE);		
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[6][0],FALSE);		
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[7][0],FALSE);		
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[27][0],FALSE);		
	LCD_Dis_Char_16_16(1,9,&WordLib_CN[28][0],FALSE);
	LCD_Dis_Digital_int(2,8,4,input.year,FALSE);
	LCD_Dis_Char_16_16(2,5,&WordLib_CN[10][0],FALSE);				//年月日
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
				if(input.hour>23) input.hour = 23;			//溢出	
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
 * 函数名：GUI_KEYSetting()
 * 描述  ：密码设置
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_KEYSetting()
{
	char keyval,wei = 1,input = 0;
	uchar keyon = g_savedat.s_needkey;
	
	if(g_keycmd==KS_KEYSET_OK)
	{
		LCD_Clear();	
		wei = 2;
		LCD_Dis_Char_16_16(2,4,&WordLib_CN[27][0],FALSE);		//设置完成！
		LCD_Dis_Char_16_16(2,5,&WordLib_CN[28][0],FALSE);		
		LCD_Dis_Char_16_16(2,6,&WordLib_CN[62][0],FALSE);		
		LCD_Dis_Char_16_16(2,7,&WordLib_CN[63][0],FALSE);		
		LCD_Dis_ASCIIStr(2,15,"!",FALSE);
		delay1s();
	}
	g_keycmd = KS_NORMAL;	
	LCD_Clear();	
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[48][0],FALSE);		//密码设置
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
					g_savedat.s_needkey = keyon;			//密码开关值保存
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
 * 函数名：GUI_BLSetting()
 * 描述  ：背光设置
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_BLSetting()
{
	char keyval,wei;
	
	wei = g_savedat.s_baklit_on;
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[57][0],FALSE);		//背光设置
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
					g_savedat.s_baklit_on = 0;		//关闭
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
 * 函数名：GUI_DEFAULTSetting()
 * 描述  ：恢复出厂设置
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_DEFAULTSetting()
{
	char keyval,wei = 1,yes = 0;
	
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[72][0],FALSE);		//恢复默认？
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
 * 函数名：GUI_History()
 * 描述  ：显示
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_History()
{
						;
}



/********************************************************************************
 * 函数名：GUI_Operation()
 * 描述  ：显示操作界面		keyval==0xff表示是从别的界面返回的,类型一定是无符号的，
							否则if判断总不成立，有待进一步考究
 * 输入  ：-		    	
 * 返回  ：- 进入的界面
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_Operation(uchar keyval)
{
	char selitem = 1;					//选择的值

	if(keyval==0xff||g_homepage)		//如果在主界面2则先切回1
	{
		keyval = KEY_NONE;
		LCD_Clear();
		GUI_HomePage();					//默认就为0 HOME项
	}
	else 
	{
		if(keyval==KEY_L) selitem++;
		else if(keyval==KEY_R) selitem--;
		if(selitem>3) selitem = 1;
		else if(selitem<1) selitem = 3;
	
		GUI_HomeMenu(selitem);			//在主菜单下则可以变动
	}
	SetSoftTimer(TIMER_1,10);			//设置退出延时定时器 10s
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
			GUI_HomeMenu(selitem);				//在主菜单下则可以变动	
		}

		if(ReadSoftTimer(TIMER_1)) {g_menumark = 0;return ;}	//超时返回
	}
}

/**************************主要是菜单显示****************************************************/
/********************************************************************************
 * 函数名：GUI_SysStatus()
 * 描述  ：显示主界面状态变量
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：内外部调用
 ********************************************************************************/
void GUI_SysStatus(uchar sta)
{
	static uchar sta_last=FREE;
	if(sta!=sta_last)
	{
		LCD_Clear_Region(17,113,16,64);
		sta_last = sta;
	}
	if(sta&OFFLINE)											//优先级最高
	{
			LCD_Dis_Char_16_16(2,10,&WordLib_CN[87][0],FALSE);		//离线
			LCD_Dis_Char_16_16(2,11,&WordLib_CN[85][0],FALSE);			
	}
	else if(sta&OUTSIDE)
	{
			LCD_Dis_Char_16_16(2,10,&WordLib_CN[84][0],FALSE);		//占线
			LCD_Dis_Char_16_16(2,11,&WordLib_CN[85][0],FALSE);			
	}
	else if(sta&SWOFF)
	{
		LCD_Dis_Char_16_16(2,8,&WordLib_CN[8][0],FALSE);		//开关错误
		LCD_Dis_Char_16_16(2,9,&WordLib_CN[9][0],FALSE);			
		LCD_Dis_Char_16_16(2,10,&WordLib_CN[50][0],FALSE);		
		LCD_Dis_Char_16_16(2,11,&WordLib_CN[51][0],FALSE);
	}
	else if(sta&WORK)
	{
		LCD_Dis_Char_16_16(2,9,&WordLib_CN[52][0],FALSE);		//抽水中
		LCD_Dis_Char_16_16(2,10,&WordLib_CN[4][0],FALSE);		//
		LCD_Dis_Char_16_16(2,11,&WordLib_CN[86][0],FALSE);			
	}
	else
	{
		LCD_Dis_Char_16_16(2,10,&WordLib_CN[21][0],FALSE);		//空闲
		LCD_Dis_Char_16_16(2,11,&WordLib_CN[22][0],FALSE);
	}
}


/********************************************************************************
 * 函数名：GUI_SysMode()
 * 描述  ：显示主界面模式变量
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：内外部调用
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
		LCD_Dis_Char_16_16(3,10,&WordLib_CN[36][0],FALSE);		//手动
		LCD_Dis_Char_16_16(3,11,&WordLib_CN[14][0],FALSE);	
	}
	else{
		LCD_Dis_Char_16_16(3,10,&WordLib_CN[13][0],FALSE);		//自动
		LCD_Dis_Char_16_16(3,11,&WordLib_CN[14][0],FALSE);	
	}
}
/********************************************************************************
 * 函数名：GUI_HomePage()
 * 描述  ：显示主界面常量
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：内外部调用
 ********************************************************************************/
void GUI_HomePage()
{
	g_homepage = 0;
	homedispbusy = 1;
	LCD_Dis_Pict(1,1,45,62,Pic_Case);
		
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[6][0],FALSE);		//时间
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[7][0],FALSE);		//
	LCD_Dis_ASCIIStr(1,13,":",FALSE);	
//	GUI_DisplayWaterTemp();
	LCD_Dis_Digital_int(1,16,2,RTCtime.hour,FALSE);			//时分秒
	LCD_Dis_ASCIIStr(1,17,":",FALSE);
	LCD_Dis_Digital_int(1,19,2,RTCtime.min,FALSE);
	LCD_Dis_ASCIIStr(1,20,":",FALSE);
	LCD_Dis_Digital_int(1,22,2,RTCtime.sec,FALSE);
	
//	LCD_Dis_Char_8_16(1,23,&CharLib_SplLabel[0][0],FALSE);	//℃
//	LCD_Dis_ASCIIStr(1,24,"C",FALSE);
	
	LCD_Dis_Char_16_16(2,5,&WordLib_CN[19][0],FALSE);		//状态
	LCD_Dis_Char_16_16(2,6,&WordLib_CN[20][0],FALSE);
	LCD_Dis_ASCIIStr(2,13,":",FALSE);
	GUI_SysStatus(g_sysflag);
	
	LCD_Dis_Char_16_16(3,5,&WordLib_CN[34][0],FALSE);		//模式
	LCD_Dis_Char_16_16(3,6,&WordLib_CN[35][0],FALSE);
	LCD_Dis_ASCIIStr(3,13,":",FALSE);
	GUI_SysMode(g_savedat.mode);

	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//历史
	LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		

	LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//设置
	LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		

	LCD_Dis_ASCIIStr(4,19,"HOME",TRUE);						//HOME
	
	GUI_CaseData_Dis(g_level_per,1);
	homedispbusy = 0;
}


/********************************************************************************
 * 函数名：GUI_HomePage2()
 * 描述  ：显示主界面日历时间，室温
 * 输入  ：-
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_HomePage2()
{
	g_homepage = 1;
	homedispbusy = 1;
	LCD_Dis_Char_16_16(1,3,&WordLib_CN[4][0],FALSE);		//水位自动控制系统
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[5][0],FALSE);		//
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[13][0],FALSE);		//
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[14][0],FALSE);		//
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[15][0],FALSE);		//
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[16][0],FALSE);		//
	LCD_Dis_Char_16_16(1,9,&WordLib_CN[17][0],FALSE);		//
	LCD_Dis_Char_16_16(1,10,&WordLib_CN[18][0],FALSE);		//
	
	LCD_Dis_Digital_int(2,10,4,RTCtime.year,FALSE);
	LCD_Dis_Char_16_16(2,6,&WordLib_CN[10][0],FALSE);		//年月日
	LCD_Dis_Digital_int(2,14,2,RTCtime.month,FALSE);
	LCD_Dis_Char_16_16(2,8,&WordLib_CN[11][0],FALSE);		
	LCD_Dis_Digital_int(2,18,2,RTCtime.date,FALSE);
	LCD_Dis_Char_16_16(2,10,&WordLib_CN[12][0],FALSE);
	LCD_Dis_Digital_int(3,6,2,RTCtime.hour,FALSE);					//时分秒
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
 * 函数名：GUI_HomePageUpdate()
 * 描述  ：主页变量更新
 * 输入  ：-selitem 项
 * 返回  ：- 
 * 调用  ：内部调用
 ********************************************************************************/
void GUI_HomePageUpdate()
{
	if(!homedispbusy){
		if(g_sysflag&WORK)
		{
			LCD_Dis_Digital_int(1,16,2,0,FALSE);					//时分秒
			LCD_Dis_Digital_int(1,19,2,UpTimer1.min,FALSE);
			LCD_Dis_Digital_int(1,22,2,UpTimer1.sec,FALSE);
		}
		else{
			LCD_Dis_Digital_int(1,16,2,RTCtime.hour,FALSE);			//时分秒
			LCD_Dis_Digital_int(1,19,2,RTCtime.min,FALSE);
			LCD_Dis_Digital_int(1,22,2,RTCtime.sec,FALSE);
		}
		GUI_SysStatus(g_sysflag);
		GUI_SysMode(g_savedat.mode);
	}
}

/********************************************************************************
 * 函数名：GUI_HomePage2Update()
 * 描述  ：主页2变量更新
 * 输入  ：-selitem 项
 * 返回  ：- 
 * 调用  ：内部调用
 ********************************************************************************/
void GUI_HomePage2Update()
{
	if(!homedispbusy){
		LCD_Dis_Digital_int(3,6,2,RTCtime.hour,FALSE);					//时分秒
		LCD_Dis_Digital_int(3,9,2,RTCtime.min,FALSE);
		LCD_Dis_Digital_int(3,12,2,RTCtime.sec,FALSE);
	}
}

/********************************************************************************
 * 函数名：GUI_HomeMenu()
 * 描述  ：主页菜单选项
 * 输入  ：-selitem 项
 * 返回  ：- 
 * 调用  ：内部调用
 ********************************************************************************/
void GUI_HomeMenu(char selitem)
{
	homedispbusy = 1;

	switch(selitem)
	{
		case 1 :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//历史
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//设置
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",TRUE);						//HOME
		}break;
		case 2 :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//历史
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],TRUE);		//设置
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],TRUE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",FALSE);					//HOME
		}break;
		case 3 :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],TRUE);		//历史
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],TRUE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//设置
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",FALSE);					//HOME
		}break;
	}
	homedispbusy = 0;

}


/********************************************************************************
 * 函数名：GUI_PasswordMenu()
 * 描述  ：密码菜单选项
 * 输入  ：-num= 项
 * 返回  ：- 
 * 调用  ：内部调用
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
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//返回
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
}


/********************************************************************************
 * 函数名：GUI_ModeSettingMenu()
 * 描述  ：模式设置菜单选项
 * 输入  ：-num= 项
 * 返回  ：- 
 * 调用  ：内部调用
 ********************************************************************************/
void GUI_ModeSettingMenu(uchar num)
{
	if(num==1)
	{
		LCD_Dis_ASCIIStr(2,1,"1",TRUE);	
		LCD_Dis_ASCIIStr(2,2,".",TRUE);
		LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],TRUE);		//自动
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
		LCD_Dis_Char_16_16(3,2,&WordLib_CN[36][0],TRUE);		//手动
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
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//返回
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}	
}


/********************************************************************************
 * 函数名：GUI_BLSettingMenu()
 * 描述  ：模式设置菜单选项
 * 输入  ：-wei= 项
 * 返回  ：- 
 * 调用  ：内部调用
 ********************************************************************************/
void GUI_BLSettingMenu(uchar wei)
{
	if(wei==1)
	{
		LCD_Dis_ASCIIStr(2,1,"1",TRUE);	
		LCD_Dis_ASCIIStr(2,2,".",TRUE);
		LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],TRUE);		//自动
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
		LCD_Dis_Char_16_16(3,2,&WordLib_CN[9][0],TRUE);			//关闭
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
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//返回
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
}

/********************************************************************************
 * 函数名：GUI_PecentSettingMenu()
 * 描述  ：设置数字百分比选项
 * 输入  ：-wei第几位数 ，num百分数
 * 返回  ：- 
 * 调用  ：内部调用
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
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//返回
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}

	LCD_Dis_ASCIIStr(2,13,"%",FALSE);
}

/********************************************************************************
 * 函数名：GUI_DateTimeSettingMenu()
 * 描述  ：设置时间日期选项
 * 输入  ：-
 * 返回  ：- 
 * 调用  ：内部调用
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
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[64][0],TRUE);		//保存
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[65][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[64][0],FALSE);		
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[65][0],FALSE);		
	}
	if(wei==0)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//返回
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
	

}

/********************************************************************************
 * 函数名：GUI_KEYSettingMenu()
 * 描述  ：密码设置选项
 * 输入  ：-
 * 返回  ：- 
 * 调用  ：内部调用
 ********************************************************************************/
void GUI_KEYSettingMenu(char keyon,char wei)
{
	if(wei==1)
		if(keyon)
		{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[8][0],TRUE);		//开关
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
		LCD_Dis_Char_16_16(3,5,&WordLib_CN[59][0],TRUE);		//修改密码
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
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//返回
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
	if(wei==3)		
	{
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[64][0],TRUE);		//保存
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[65][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[64][0],FALSE);		
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[65][0],FALSE);		
	}
	
}

/********************************************************************************
 * 函数名：GUI_DEFAULTSettingMenu()
 * 描述  ：恢复出厂设置选项
 * 输入  ：-
 * 返回  ：- 
 * 调用  ：内部调用
 ********************************************************************************/
void GUI_DEFAULTSettingMenu(char yes,uchar wei)
{
	if(wei==1)
		if(yes)
		{
			LCD_Dis_Char_16_16(2,6,&WordLib_CN[76][0],TRUE);		//是否
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
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[78][0],TRUE);		//确定
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[79][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,2,&WordLib_CN[78][0],FALSE);		
		LCD_Dis_Char_16_16(4,3,&WordLib_CN[79][0],FALSE);		
	}
	if(wei==0)		
	{
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],TRUE);		//返回
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],TRUE);		
	}
	else {
		LCD_Dis_Char_16_16(4,10,&WordLib_CN[29][0],FALSE);		
		LCD_Dis_Char_16_16(4,11,&WordLib_CN[30][0],FALSE);		
	}
	
}

/********************************************************************************
 * 函数名：GUI_SettingMenu()
 * 描述  ：设置菜单选项
 * 输入  ：-num= 项
 * 返回  ：- 
 * 调用  ：内部调用
 ********************************************************************************/
void GUI_SettingMenu(char num, char turn)
{
	if(turn) LCD_Clear();											//翻页
	if(num<5)
	{
		if(num==1)
		{
			LCD_Dis_ASCIIStr(1,1,"1",TRUE);	
			LCD_Dis_ASCIIStr(1,2,".",TRUE);
			LCD_Dis_Char_16_16(1,2,&WordLib_CN[34][0],TRUE);		//模式设置
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
			LCD_Dis_Char_16_16(2,2,&WordLib_CN[13][0],TRUE);		//自动抽水液位设置
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
			LCD_Dis_Char_16_16(3,2,&WordLib_CN[13][0],TRUE);		//自动停水液位设置
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
			LCD_Dis_Char_16_16(4,2,&WordLib_CN[12][0],TRUE);		//时间日期
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
			LCD_Dis_Char_16_16(1,2,&WordLib_CN[48][0],TRUE);		//密码设置
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
			LCD_Dis_Char_16_16(2,2,&WordLib_CN[57][0],TRUE);		//背光设置
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
			LCD_Dis_Char_16_16(3,2,&WordLib_CN[72][0],TRUE);		//恢复默认
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
 * 函数名：GUI_CaseData_Dis()
 * 描述  ：水池数据显示
 * 输入  ：-		    	refill=1 用于界面切换时重新绘制水位
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_CaseData_Dis(char percent,char refill)
{
	char filevel;
	static signed char filevel_smooth=0,filevel_last = 0;
	static char percent_last=0;
	uchar fallflag = 0;
	uchar i;
	
	//设置显示忙标志，避免中断打扰
	homedispbusy = 1;
	
	filevel = (signed char)(percent/2.44+1);
	
	if(refill)	filevel_last = 0;						//需要重新填充，将历史液位置零
	
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
		//filevel_last = (fallflag?(filevel_last-i):(fallflag+i));	//为什么不行？
		switch(filevel_last)
		{
			case 1 : 
			{	if(fallflag)										//往下降
					LCD_Clear_Region(42,5,1,43);					//清理上一条水平线
				else
				{
					LCD_fill_Region(43,6,1,41);
				}
			}break;
			case 2 : 
			{	if(fallflag)										//往下降
					LCD_Clear_Region(41,4,1,45);					//清理上一条水平线
				else
				{
					LCD_fill_Region(42,5,1,43);
				}
			}break;			
			case 3 : 
			{	if(fallflag)										//往下降
					LCD_Clear_Region(40,4,1,45);					//清理上一条水平线
				else
				{
					LCD_fill_Region(41,4,1,45);
				}
			}break;
			case 4 :
			{	if(fallflag)										//往下降
					LCD_Clear_Region(39,3,1,47);					//清理上一条水平线
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
			{	if(fallflag)										//往下降
					LCD_Clear_Region(3,6,1,41);						//清理上一条水平线
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
				if(fallflag)										//往下降
					LCD_Clear_Region(43-filevel_last,3,1,47);		//清理上一条水平线
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
	if(percent - percent_last<0)	//从100%到99%，前面一位被水填充
	{
		if(percent==99)
			LCD_fill_Region(17,9,16,8);
		else if(percent==9)
			LCD_fill_Region(17,9,16,16);
	}
	percent_last = percent;
	
	//复位显示忙标志
	homedispbusy = 0;
}



uchar code Pic_arrow[]={
	0x00,0xF0,0xFF,0xF0,0x00,
	0x1C,0xFF,0xFF,0xFF,0x1C,
	0x00,0xFF,0xFF,0xFF,0x00,
};

/******************** (C) COPYRIGHT 2015 DammStanger ****END OF FILE***************************/
