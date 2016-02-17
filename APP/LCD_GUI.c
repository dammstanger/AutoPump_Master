/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：LCD_GUI.c
 * 描	述	：GUI设计
 *                    
 * 实验平台	：51开发板
 * 硬件连接	：
 * 版 	本	：V0.0.160217
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2015.7.14
 * 最后编辑	：2016.2.17
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
/****************************变量定义*********************************************/
char g_guilevel = 0;			//用于标记界面从层次，在几级菜单上
bit g_homepage = 0;				//主界面标记，0界面1 =1界面2
long keyword = 123456;
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

void GUI_PasswordMenu(char num)
{
	if(num==0) 	LCD_Dis_ASCIIStr(4,2,"0",TRUE);
	else 		LCD_Dis_ASCIIStr(4,2,"0",FALSE);
	if(num==1) 	LCD_Dis_ASCIIStr(4,4,"1",TRUE);
	else 		LCD_Dis_ASCIIStr(4,4,"1",FALSE);
	if(num==2) 	LCD_Dis_ASCIIStr(4,6,"2",TRUE);
	else 		LCD_Dis_ASCIIStr(4,6,"2",FALSE);
	if(num==3) 	LCD_Dis_ASCIIStr(4,8,"3",TRUE);
	else 		LCD_Dis_ASCIIStr(4,8,"3",FALSE);
	if(num==4) 	LCD_Dis_ASCIIStr(4,10,"4",TRUE);
	else 		LCD_Dis_ASCIIStr(4,10,"4",FALSE);
	if(num==5) 	LCD_Dis_ASCIIStr(4,12,"5",TRUE);
	else 		LCD_Dis_ASCIIStr(4,12,"5",FALSE);
	if(num==6) 	LCD_Dis_ASCIIStr(4,14,"6",TRUE);
	else 		LCD_Dis_ASCIIStr(4,14,"6",FALSE);
	if(num==7) 	LCD_Dis_ASCIIStr(4,16,"7",TRUE);
	else 		LCD_Dis_ASCIIStr(4,16,"7",FALSE);
	if(num==8) 	LCD_Dis_ASCIIStr(4,18,"8",TRUE);
	else 		LCD_Dis_ASCIIStr(4,18,"8",FALSE);
	if(num==9) 	LCD_Dis_ASCIIStr(4,20,"9",TRUE);
	else 		LCD_Dis_ASCIIStr(4,20,"9",FALSE);
}
	

char GUI_DisplayPassword()
{
	char keyval,selitem=0,wei=6,i=5;
	long input=0;
	
	LCD_Clear();
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[45][0],FALSE);		//请输入密码
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[46][0],FALSE);	
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[47][0],FALSE);		//
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[48][0],FALSE);	
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[49][0],FALSE);		
	LCD_Dis_ASCIIStr(1,17,":",FALSE);	
	LCD_Dis_ASCIIStr(4,2,"0",TRUE);
	LCD_Dis_ASCIIStr(4,4,"1",FALSE);
	LCD_Dis_ASCIIStr(4,6,"2",FALSE);
	LCD_Dis_ASCIIStr(4,8,"3",FALSE);
	LCD_Dis_ASCIIStr(4,10,"4",FALSE);
	LCD_Dis_ASCIIStr(4,12,"5",FALSE);
	LCD_Dis_ASCIIStr(4,14,"6",FALSE);
	LCD_Dis_ASCIIStr(4,16,"7",FALSE);
	LCD_Dis_ASCIIStr(4,18,"8",FALSE);
	LCD_Dis_ASCIIStr(4,20,"9",FALSE);
	while(1)
	{
		keyval = Key_Scan();	
		if(keyval!=KEY_NONE)
		{	
			if(keyval==KEY_HOME) 
			{
				g_guilevel = 0;
				return 0;
			}
			else if(keyval==KEY_UP) 
			{
				if(wei++<=6)
					LCD_Dis_ASCIIStr(2,19-wei*2," ",FALSE);
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
						g_guilevel = 2;
						return 1;
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
						if(--i==0) return 0;
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
	LED2 = 0;
	delay200ms();
	LED2 = 1;
	g_guilevel = 0;
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
 * 函数名：GUI_DisplayWaterTemp()
 * 描述  ：显示水温
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void GUI_DisplayWaterTemp()
{	
	//显示温度
	LCD_Dis_Digital_float(1,22,tmp_data.tmp2);
}


/********************************************************************************
 * 函数名：GUI_DisplayAirTemp()
 * 描述  ：显示水温
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：内外部调用
 ********************************************************************************/
void GUI_DisplayAirTemp()
{	
	//显示温度
	LCD_Dis_Digital_float(4,14,tmp_data.tmp1);						//从右往左的顺序增长
}



void GUI_HomeMenu(char selitem)
{
	switch(selitem)
	{
		case 1 :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//历史
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],TRUE);		//设置
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],TRUE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",FALSE);					//HOME
		}break;
		case 2 :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],TRUE);		//历史
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],TRUE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//设置
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",FALSE);					//HOME
		}break;
		default :{	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//历史
					LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		
					
					LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//设置
					LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		
					
					LCD_Dis_ASCIIStr(4,19,"HOME",TRUE);						//HOME
		}break;
	}
}

/********************************************************************************
 * 函数名：GUI_Operation()
 * 描述  ：显示操作界面
 * 输入  ：-		    	
 * 返回  ：- 进入的界面
 * 调用  ：外部调用
 ********************************************************************************/
char GUI_Operation(char keyval)
{
	char selitem = 0;					//选择的值
	if(g_homepage)						//如果在主界面2则先切回1
	{
		LCD_Clear();
		GUI_HomePage();					//默认就为0 HOME项
	}
	else 
	{
		if(keyval==KEY_L) selitem++;
		else if(keyval==KEY_R) selitem--;
		if(selitem>2) selitem = 0;
		else if(selitem<0) selitem = 2;
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
				g_guilevel=1;
				return selitem;
			}
			else if(keyval==KEY_HOME) return 0;
			else if(keyval==KEY_L) selitem++;
			else if(keyval==KEY_R) selitem--;
			if(selitem>2) selitem = 0;
			else if(selitem<0) selitem = 2;
			GUI_HomeMenu(selitem);				//在主菜单下则可以变动	
		}

		if(ReadSoftTimer(TIMER_1)) return 0;	//超时返回
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
	LCD_Dis_Pict(1,1,45,62,Pic_Case);
	
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[0][0],FALSE);		//实时水温
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[1][0],FALSE);		//
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[4][0],FALSE);		//
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[2][0],FALSE);		//	
	LCD_Dis_ASCIIStr(1,17,":",FALSE);
	GUI_DisplayWaterTemp();
	LCD_Dis_Char_8_16(1,23,&CharLib_SplLabel[0][0],FALSE);
	LCD_Dis_ASCIIStr(1,24,"C",FALSE);
	
	LCD_Dis_Char_16_16(2,5,&WordLib_CN[19][0],FALSE);		//状态
	LCD_Dis_Char_16_16(2,6,&WordLib_CN[20][0],FALSE);
	LCD_Dis_ASCIIStr(2,13,":",FALSE);
	LCD_Dis_Char_16_16(2,10,&WordLib_CN[21][0],FALSE);		//空闲
	LCD_Dis_Char_16_16(2,11,&WordLib_CN[22][0],FALSE);	
	
	LCD_Dis_Char_16_16(3,5,&WordLib_CN[34][0],FALSE);		//模式
	LCD_Dis_Char_16_16(3,6,&WordLib_CN[35][0],FALSE);
	LCD_Dis_ASCIIStr(3,13,":",FALSE);
	LCD_Dis_Char_16_16(3,10,&WordLib_CN[13][0],FALSE);		//自动
	LCD_Dis_Char_16_16(3,11,&WordLib_CN[14][0],FALSE);	

	LCD_Dis_Char_16_16(4,2,&WordLib_CN[23][0],FALSE);		//历史
	LCD_Dis_Char_16_16(4,3,&WordLib_CN[24][0],FALSE);		

	LCD_Dis_Char_16_16(4,6,&WordLib_CN[27][0],FALSE);		//设置
	LCD_Dis_Char_16_16(4,7,&WordLib_CN[28][0],FALSE);		

	LCD_Dis_ASCIIStr(4,19,"HOME",TRUE);						//HOME
	
	GUI_CaseData_Dis(55,1);
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
	LCD_Dis_Char_16_16(1,3,&WordLib_CN[4][0],FALSE);		//水位自动控制系统
	LCD_Dis_Char_16_16(1,4,&WordLib_CN[5][0],FALSE);		//
	LCD_Dis_Char_16_16(1,5,&WordLib_CN[13][0],FALSE);		//
	LCD_Dis_Char_16_16(1,6,&WordLib_CN[14][0],FALSE);		//
	LCD_Dis_Char_16_16(1,7,&WordLib_CN[15][0],FALSE);		//
	LCD_Dis_Char_16_16(1,8,&WordLib_CN[16][0],FALSE);		//
	LCD_Dis_Char_16_16(1,9,&WordLib_CN[17][0],FALSE);		//
	LCD_Dis_Char_16_16(1,10,&WordLib_CN[18][0],FALSE);		//
	
	LCD_Dis_Digital_int(2,10,4,RTCtime.year);
	LCD_Dis_Char_16_16(2,6,&WordLib_CN[10][0],FALSE);		//年月日
	LCD_Dis_Digital_int(2,14,2,RTCtime.month);
	LCD_Dis_Char_16_16(2,8,&WordLib_CN[11][0],FALSE);		
	LCD_Dis_Digital_int(2,18,2,RTCtime.date);
	LCD_Dis_Char_16_16(2,10,&WordLib_CN[12][0],FALSE);
	LCD_Dis_Digital_int(3,6,2,RTCtime.hour);					//时分秒
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
	
//	LCD_Dis_Char_16_16(4,1,&WordLib_CN[31][0],FALSE);		//气温
//	LCD_Dis_Char_16_16(4,2,&WordLib_CN[2][0],FALSE);		//	
//	LCD_Dis_ASCIIStr(4,5,":",FALSE);
	GUI_DisplayAirTemp();
	LCD_Dis_Char_8_16(4,15,&CharLib_SplLabel[0][0],FALSE);
	LCD_Dis_ASCIIStr(4,16,"C",FALSE);
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
		//filevel_last = (fallflag?(filevel_last-i):(fallflag+i));			//为什么不行？
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
	LCD_Dis_Digital_int(2,4,0,percent);
	LCD_Dis_ASCIIStr(2,5,"%",FALSE);
	if(percent==99&&percent - percent_last<0)	//从100%到99%，前面一位被水填充
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
