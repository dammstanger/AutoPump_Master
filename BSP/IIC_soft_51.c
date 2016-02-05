/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：IIC_soft_51.c
 * 描	述	：模拟IIC
 *            
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.0.160129
 * 从属关系	：AutoPumpV2 
 * 库版本	：无
 * 创建时间	：2016.1.29
 * 最后编辑	：2016.1.29
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "IIC_soft_51.h"
#include "intrins.h"
/****************************宏定义***********************************************/

/****************************变量声明*********************************************/

/****************************变量定义*********************************************/
sbit SDA = P3^6;
sbit SCL = P3^7;
/****************************函数声明*********************************************/



/********************************************************************************
 * 函数名：delay_us()
 * 描述  ：1us延时， 
 * 输入  ：
 * 返回  ：-
 * 调用  ：内部
 ********************************************************************************/
void delay_us(uchar n)   
{ 
	n=0; 
}


/********************************************************************************
 * 函数名：IIC_delay()
 * 描述  ：
 * 输入  ：
 * 返回  ：-
 * 调用  ：内部
 ********************************************************************************/
void IIC_delay(void)	//i=14 约3us
{
 	_nop_();
	_nop_();
	_nop_();
 	_nop_();
 	_nop_();
}


/********************************************************************************
 * 函数名：IIC_GPIO_Config()
 * 描述  ：IIC I/O配置
 * 输入  ：
 * 返回  ：-
 * 调用  ：内部
 ********************************************************************************/
void IIC_GPIO_Config(void)
{
	//通信接口默认为准双向
	SCL = 1;	//空闲时总线为高
	SDA = 1;	//空闲时总线为高
	
}


/********************************************************************************
 * 函数名：IIC_Start()
 * 描述  ：产生IIC起始信号
 * 输入  ：
 * 返回  ：-
 * 调用  ：内部
 ********************************************************************************/
 void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	IIC_delay();
 	SDA = 0;					//START:when CLK is high,DATA change form high to low 
	IIC_delay();
	SCL = 0;					//钳住IIC总线，准备发送或接收数据 
}	  
/********************************************************************************
 * 函数名：IIC_Stop()
 * 描述  ：产生IIC停止信号
 * 输入  ：
 * 返回  ：-
 * 调用  ：内部
 ********************************************************************************/
void IIC_Stop(void)
{
	SDA = 0;			//STOP:when CLK is high DATA change form low to high
	SCL = 1;
	IIC_delay();
	SDA = 1;			//发送IIC总线结束信号
	IIC_delay();
}


/********************************************************************************
 * 函数名：IIC_Wait_Ack()
 * 描述  ：等待应答信号到来
 * 输入  ：
 * 返回  ：FALSE，接收应答失败，TRUE，接收应答成功
 * 调用  ：内部
 ********************************************************************************/
bool IIC_Wait_Ack(void)
{
	//SDA_IN();      //SDA设置为输入  
	SCL = 1;
	IIC_delay();
	if(SDA)
	{
		SCL = 0;
		IIC_delay();
        return FALSE;
	}
	SCL = 0;
	IIC_delay();	
	return TRUE;  
} 

/********************************************************************************
 * 函数名：IIC_Ack()
 * 描述  ：产生ACK应答
 * 输入  ：
 * 返回  ：
 * 调用  ：内部
 ********************************************************************************/
void IIC_Ack(void)
{
	SDA = 0;
	SCL = 1;
	IIC_delay();
	SCL = 0;
	IIC_delay();
}
	    
/********************************************************************************
 * 函数名：IIC_Ack()
 * 描述  ：不产生ACK应答	
 * 输入  ：
 * 返回  ：
 * 调用  ：内部
 ********************************************************************************/
void IIC_NAck(void)
{
	SDA = 1;
	SCL = 1;
    IIC_delay();
	SCL = 0;
	IIC_delay();
}					 	

	  
/********************************************************************************
 * 函数名：IIC_Send_Byte()
 * 描述  ：IIC发送一个字节
 * 输入  ：
 * 返回  ：
 * 调用  ：内部
 ********************************************************************************/
void IIC_Send_Byte(uchar dat)
{                        
    uchar t;   
    for(t=0;t<8;t++)
    {   
        SDA = (1&&(dat&0x80));
        dat<<=1; 
		SCL = 1;
		IIC_delay();
		SCL = 0;
		IIC_delay();
	}	 
} 	

/********************************************************************************
 * 函数名：IIC_Read_Byte()
 * 描述  ：读1个字节，ack=1时，发送ACK，ack=0，发送nACK  
 * 输入  ：
 * 返回  ：读取的字节
 * 调用  ：内部
 ********************************************************************************/
uchar IIC_Read_Byte(uchar ack)
{
	unsigned char i,receive=0;
	SDA = 1;
	for(i=0;i<8;i++ )
	{
        receive<<=1;
		SCL = 1;
		IIC_delay();
		if(SDA)receive |= 0x01;
		SCL = 0;
		IIC_delay();      
	}
    if (ack)
		IIC_Ack(); //发送ACK   
    else
		IIC_NAck();//发送nACK 
    return receive;
}

/********************************************************************************
 * 函数名：IIC_WriteReg()
 * 描述  ：对从机写寄存器
 * 输入  ：
 * 返回  ：
 * 调用  ：内部
 ********************************************************************************/
bool IIC_WriteReg(uchar slaveAddr,uchar REG_Address,uchar REG_data)
{	bool retval;
	IIC_Start();                   	//起始信号
	IIC_Send_Byte(slaveAddr);   	//发送设备地址+写信号
	retval=IIC_Wait_Ack();	   
	IIC_Send_Byte(REG_Address);    	//内部寄存器地址
		IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(REG_data);       	//内部寄存器数据
		IIC_Wait_Ack(); 	 											  		   
	IIC_Stop();                    	//发送停止信号
	return retval;
}


/********************************************************************************
 * 函数名：IIC_ReadReg()
 * 描述  ：对从机读寄存器
 * 输入  ：
 * 返回  ：
 * 调用  ：内部
 ********************************************************************************/
uchar IIC_ReadReg(uchar slaveAddr, uchar readAddr)
{
	uchar REG_data;
	IIC_Start();                   	//起始信号
	IIC_Send_Byte(slaveAddr);    	//发送设备地址+写信号
	IIC_Wait_Ack();
	IIC_Send_Byte(readAddr);     	//发送存储单元地址，从0开始
	IIC_Wait_Ack();					//必须要有
	IIC_Start();                   	//起始信号
	IIC_Send_Byte(slaveAddr+1);  	//发送设备地址+读信号
	IIC_Wait_Ack();
	REG_data=IIC_Read_Byte(0);    	//读出寄存器数据不继续再读,发送NACK  
	return REG_data;	
}

/********************************************************************************
 * 函数名：IIC_MultRead()
 * 描述  ：对从机寄存器读取多个字节
 * 输入  ：
 * 返回  ：
 * 调用  ：内部
 ********************************************************************************/
uchar IIC_MultRead(uchar slaveAddr, uchar* pBuffer, uchar readAddr, uchar NumByteToRead)
{
	IIC_Start();                   			//起始信号 				 
	IIC_Send_Byte(slaveAddr);				//发送写器件指令	 
	if(FALSE==IIC_Wait_Ack())
	{
		IIC_Stop();                    		//发送停止信号
		return 1;
	}	IIC_Send_Byte(readAddr);   			//发送寄存器地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Start();  	 	   					//重新启动
	IIC_Send_Byte(slaveAddr+1);				//发送读器件指令
	IIC_Wait_Ack();
	/* While there is data to be read */
	while(NumByteToRead)
	{
		if(NumByteToRead == 1)
		{
			*pBuffer=IIC_Read_Byte(0);    	//读出寄存器数据不继续再读,发送NACK  
		}
		else 
		{
			*pBuffer=IIC_Read_Byte(1);    	//读出寄存器数据继续读,发送ACK  
			pBuffer++;						/* Point to the next location where the byte read will be saved */
		}
		/* Decrement the read bytes counter */
		NumByteToRead--;
	}
	IIC_Stop();                    			//停止信号
	return 0;
}

/********************************************************************************
 * 函数名：IIC_MultWrite()
 * 描述  ：对从机寄存器写多个字节
 * 输入  ：
 * 返回  ：
 * 调用  ：内部
 ********************************************************************************/
uchar IIC_MultWrite(uchar slaveAddr, uchar* pBuffer, uchar wrtieAddr, uchar NumByteToWrite)
{
	IIC_Start();					  		//起始信号

	IIC_Send_Byte(slaveAddr);   			//发送设备地址+写信号
		if(FALSE==IIC_Wait_Ack())
		{
			IIC_Stop();                    	//发送停止信号
			return 1;
		}
	IIC_Send_Byte(wrtieAddr);				//内部寄存器地址
		IIC_Wait_Ack();
	while(NumByteToWrite)
	{
		IIC_Send_Byte(*pBuffer);       		//内部寄存器数据		
		if(FALSE==IIC_Wait_Ack())
		{
			IIC_Stop();						//发送停止信号
			return 1;
		}
		pBuffer++;
		NumByteToWrite--;
	} 										  		   
	IIC_Stop();                    			//发送停止信号
	return 0;
}



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/




