/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：SI4432.c
 * 描	述	：SI4432无线模块的驱动
 * 注	意	：	ItStatus1 = SI4432_ReadReg(0x03);													//read the Interrupt Status1 register
				ItStatus2 = SI4432_ReadReg(0x04);	读中断状态有3个功能，1：释放nIRQ引脚；
				2：读取各个中断位（包括未使能的中断）；3：清除被使能的中断位（未使能的无法清除）
				一般成对使用。
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：V0.1.160121
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.1.21
 * 最后编辑	：2016.1.22
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "SI4432.h"
#include "stdio.h"
#include "UART_51.h"
#include "globaldefine.h"
#include "Transmit.h"

/****************************宏定义***********************************************/
#define TxGPIOSetting()   SI4432_WriteReg(0x0E, 0x02)
#define RxGPIOSetting()   SI4432_WriteReg(0x0E, 0x01)
#define IdleGPIOSetting() SI4432_WriteReg(0x0E, 0x01)
#define RR		0x00		//读命令
#define WR		0x80		//写命令



/****************************变量定义*********************************************/
//----------引脚变量定义-----------------
sbit SI4432_SDN	= 	P4^3;
sbit SI4432_IRQ	= 	P3^2;
sbit SI4432_SEL = 	P3^3;
//---------------------------------------
char 	ItStatus1,ItStatus2;
/****************************函数声明*********************************************/


void SI4432_Delayms(int n)
{
	uchar a,b;
#ifdef DELAYFUNC
	Delay_ms(n);			//From Systick
#else
	for(;n>0;n--)			//11.0592MHz 1T 1一个单位1.0007ms
		for(b=129;b>0;b--)
			for(a=20;a>0;a--);
#endif
}

void SI4432_GPIOInit()
{
	RF4432_SDN_0;		//开启模块
	RF4432_SEL_1;		//片选拉高，低电平有效
	RF4432_IRQ_1;		//中断引脚置高，低电平触发
}

void SI4432_WriteReg(char addr,char val)
{
	RF4432_SEL_0;                  
	SPI_ReadWriteByte(addr|WR);      
	SPI_ReadWriteByte(val);          
	RF4432_SEL_1;      
}

char SI4432_ReadReg(char addr)
{
	char retval;
	RF4432_SEL_0;                  
	SPI_ReadWriteByte(addr|RR);      
	retval = SPI_ReadWriteByte(0xff);          
	RF4432_SEL_1;    
	return retval;	
}

char SI4432_Check()
{
	char retval;
	SendString("Check SI4432...\r\n");
	retval = SI4432_ReadReg(0x00);	
	SendString("DEVICE_TYPE: 0x%x.\r\n");
	SendByteASCII(retval);
	SendString("\r\n");
	if(retval!=0x08)
		return 1;
	retval = SI4432_ReadReg(0x01);
	SendString("DEVICE_VERSION: 0x%x.\r\n");	
	SendByteASCII(retval);
	SendString("\r\n");
	retval = SI4432_ReadReg(0x02);	
	if((retval&0x0f)==0)
		SendString("DEVICE_STATUS: at idle mode.\r\n");
	else if((retval&0x0f)==1)
		SendString("DEVICE_STATUS: at RX mode.\r\n");	
	else if((retval&0x0f)==2)
		SendString("DEVICE_STATUS: at TX mode.\r\n");	
	retval = SI4432_ReadReg(0x09);	
	SendString("Crystal Oscillator Load Capacitance: %x\r\n");
	SendByteASCII(retval);
	SendString("\r\n");
	return 0;
}

void SI4432_RF_Init(void)
{
	//Set the physical parameters
	//Frequency:       433MHz
	//Modulation type: GFSK
	//CrytalTolerance: Rx,10ppm; Tx, 10ppm
	//Data rate:       9.6Kbps/1.2Kbps
	//AFC:             Enable
	//Frequency deviation: 20KHz

	//set the center frequency to 433 MHz
	SI4432_WriteReg(0x75, 0x53);									//write data to the Frequency Band Select register             
	SI4432_WriteReg(0x76, 0x4B);									//write data to the Nominal Carrier Frequency1 register
	SI4432_WriteReg(0x77, 0x00);  									//write data to the Nominal Carrier Frequency0 register

	//set the desired TX data rate (1.2kbps)
	SI4432_WriteReg(0x6E, 0x09);									//write data to the TXDataRate 1 register
	SI4432_WriteReg(0x6F, 0xD5);									//write data to the TXDataRate 0 register
	SI4432_WriteReg(0x70, 0x2C);									//write data to the Modulation Mode Control 1 register
	SI4432_WriteReg(0x58, 0x80);

	//set the TX power to MAX. dBm
	SI4432_WriteReg(0x6D, 0x1F);									//write data to the TX Power register 

	//set the Tx deviation register (+-20kHz)
	SI4432_WriteReg(0x72, 0x20);									//write data to the Frequency Deviation register 

								/*set the modem parameters according to the exel calculator(parameters: 1.2 kbps, deviation: 20 kHz*/
	SI4432_WriteReg(0x1C, 0x2C);															//write data to the IF Filter Bandwidth register		
	SI4432_WriteReg(0x20, 0x41);															//write data to the Clock Recovery Oversampling Ratio register		
	SI4432_WriteReg(0x21, 0x60);															//write data to the Clock Recovery Offset 2 register		
	SI4432_WriteReg(0x22, 0x27);															//write data to the Clock Recovery Offset 1 register		
	SI4432_WriteReg(0x23, 0x52);															//write data to the Clock Recovery Offset 0 register		
	SI4432_WriteReg(0x24, 0x00);															//write data to the Clock Recovery Timing Loop Gain 1 register		
	SI4432_WriteReg(0x25, 0x04);															//write data to the Clock Recovery Timing Loop Gain 0 register		
	SI4432_WriteReg(0x1D, 0x40);															//write data to the AFC Loop Gearshift Override register		
	SI4432_WriteReg(0x1E, 0x0A);
	SI4432_WriteReg(0x2A, 0x0F);															//write data to the AFC Limiter register		
	SI4432_WriteReg(0x1F, 0x03);
	SI4432_WriteReg(0x69, 0x60);
	
							/*set the packet structure and the modulation type*/
	//set the preamble length to 10bytes if the antenna diversity is used and set to 5bytes if not 
	SI4432_WriteReg(0x34, 0x0C);															//write data to the Preamble Length register
	//set preamble detection threshold to 20bits
	SI4432_WriteReg(0x35, 0x2A); 															//write data to the Preamble Detection Control  register

	//Disable header bytes; set variable packet length (the length of the payload is defined by the
	//received packet length field of the packet); set the synch word to two bytes long
	SI4432_WriteReg(0x33, 0x02);															//write data to the Header Control2 register    
	
	//Set the sync word pattern to 0x2DD4
	SI4432_WriteReg(0x36, 0x2D);															//write data to the Sync Word 3 register
	SI4432_WriteReg(0x37, 0xD4);															//write data to the Sync Word 2 register

	//enable the TX & RX packet handler and CRC-16 (IBM) check
	SI4432_WriteReg(0x30, 0x8D);															//write data to the Data Access Control register
	//Disable the receive header filters
   	SI4432_WriteReg(0x32, 0x00 );															//write data to the Header Control1 register            

	//enable FIFO mode and GFSK modulation
	SI4432_WriteReg(0x71, 0x63);															//write data to the Modulation Mode Control 2 register

	//set  Crystal Oscillator Load Capacitance register
	SI4432_WriteReg(0x09, CRYSTAL_CAPACITANCE);												//write data to the Crystal Oscillator Load Capacitance register
}

void SI4432_Init()
{
	SendString("SI4432 IO Init......\r\n");
	SI4432_GPIOInit();
	SendString("IO Init completed.\r\n");
	SI4432_Delayms(20);
	while(SI4432_Check())
	{
		SendString("SI4432 is not found......\r\n");
		SI4432_Delayms(500);
	}
	/*set the GPIO's according the testcard type*/
	SI4432_WriteReg(0x0B, 0xCA);						//GPIO0 Configuration	Direct Digital Output
   	SI4432_WriteReg(0x0C, 0xCA);						//GPIO1 Configuration	Direct Digital Output
   	SI4432_WriteReg(0x0D, 0xCA);						//GPIO2 Configuration	Direct Digital Output
	SI4432_Delayms(10);
	SendString("SI4432 GPIOInit complete,config device......\r\n");
	
	//读取中断状态寄存器清除中断标志以便释放NIRQ引脚
	ItStatus1 = SI4432_ReadReg(0x03);			//读取中断标志寄存器1
	ItStatus2 = SI4432_ReadReg(0x04);			//读取中断标志寄存器2
	
	//SW reset，软件复位这个模块主控芯片   
    SI4432_WriteReg(0x07, 0x80);				//write 0x80 to the Operating & Function Control1 register
	SI4432_Delayms(20);
	
	//等待软复位完成，当软复位完成后有中断发生。客户也可以在这里直接延时至少20ms而不用等待中断来；
	//等待至少20ms后复位完成，这时候必须读中断状态寄存器释放中断引脚
	//while ( NIRQ == 1);  
	//read interrupt status registers to clear the interrupt flags and release NIRQ pin
	ItStatus1 = SI4432_ReadReg(0x03);			//读取中断标志寄存器1
	ItStatus2 = SI4432_ReadReg(0x04);			//读取中断标志寄存器2
	
	SendString("Config RF......\r\n");
	//根据不同的射频参数初始化射频模块；
    SI4432_RF_Init();	
	
	//要清除中断，先要打开中断
	SI4432_WriteReg(0x05, 0xff);			
	SI4432_WriteReg(0x06, 0xff);			
	ItStatus1 = SI4432_ReadReg(0x03);			//读取中断标志寄存器1	
	ItStatus2 = SI4432_ReadReg(0x04);			//读取中断标志寄存器2	
	
	SI4432_WriteReg(0x05, 0x07);				//！！！！方便起见，发送和接收中断及CRC校验错误都开启！！！！
	SI4432_WriteReg(0x06, 0x00);				//！！！！这样，在进入中断时，有关的中断标志都能被及时清除，不影响后续中断类型的判断！！！！
	SendString("SI4432Init complete.\r\n");
}


//------------------------------------------------------------------------------------------------
// Function Name :RFSetRxMode
//
// Return Value : None
// Parameters   : None
// Notes        : 设置模块工作在接收状态下，当一切准备好的时候，调用该函数可以让模块工作于接收模式下
//
//-----------------------------------------------------------------------------------------------
void SI4432_SetRxMode(void)
{  
	//read interrupt status registers to release all pending interrupts
	ItStatus1 = SI4432_ReadReg(0x03);			//读取中断标志寄存器1	
	ItStatus2 = SI4432_ReadReg(0x04);			//读取中断标志寄存器2	

	//Set the Rx switcher，利用GPIO口控制模块内部的天线开关。
	RxGPIOSetting();
	
	/*enable receiver chain*/
	SI4432_WriteReg(0x07, 0x05);
	
//	SendString("SI4432_Set RxMode......\r\n");
}

//------------------------------------------------------------------------------------------------
// Function Name :SI4432_SetTxMode
//
// Return Value : None
// Parameters   : None
// Notes        : 设置模块工作在接收状态下，当一切准备好的时候，调用该函数可以让模块工作于发射状态模式下
//
//-----------------------------------------------------------------------------------------------
void SI4432_SetTxMode(void)
{
	
	//Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
	ItStatus1 = SI4432_ReadReg(0x03);			//读取中断标志寄存器1	
	ItStatus2 = SI4432_ReadReg(0x04);			//读取中断标志寄存器2	

	//Set the Rx switcher，利用GPIO口控制模块内部的天线开关。
	TxGPIOSetting();
	
	/*enable transmitter*/
	//The radio forms the packet and send it automatically.
	SI4432_WriteReg(0x07, 0x09);						//0x09

//	SendString("SI4432_Set TxMode......\r\n");
}

//------------------------------------------------------------------------------------------------
// Function Name :SI4432_SetIdleMode
//
// Return Value : None
// Parameters   : None
// Notes        : 设置模块工作在空闲状态下。当模块不需要接收数据也不需要发射数据的时候可以调用该函数以便省电；
//
//-----------------------------------------------------------------------------------------------
void SI4432_SetIdleMode(void)
{

	//Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
	ItStatus1 = SI4432_ReadReg(0x03);			//读取中断标志寄存器1	
	ItStatus2 = SI4432_ReadReg(0x04);			//读取中断标志寄存器2	

	IdleGPIOSetting();																//空闲时为接收打开

	//The radio will work in idle mode
	SI4432_WriteReg(0x07, 0x01);													//配置为ready 模式，使能更快切换到发送和接收

//	SendString("SI4432_Set IdleMode......\r\n");
}

//------------------------------------------------------------------------------------------------
// Function Name :SI4432_FIFO_SendData
//
// Return Value : None
// Parameters   : uint8 length: the length of the content which will be sent out. This value should
//                not more than 64bytes because the FIFO is 64bytes. If it is bigger than 64, the
//                content should be sent out for more times according to the FIFO empty interrupt.
//                uint8 *payload: point to the content.
// Notes        : In FIFO mode, send out one packet data.
//                调用该函数可以把数据写入模块的FIFO中并把数据发送出去；数据长度不能大于64个字节；
//
//-----------------------------------------------------------------------------------------------
void SI4432_FIFO_SendData(char length, char *payload)
{
	char i;
	
//	SI4432_SetIdleMode();
 																			
	//set the length of the payload.	
	SI4432_WriteReg(0x3E, length);

	//fill the payload into the transmit FIFO
	for(i = 0; i < length; i++)
	{
		SI4432_WriteReg(0x7F, payload[i]);	
	}

	SI4432_SetTxMode();
//	SendString("SI4432_Send FIFO data....\r\n");

	/*查询方式*/
//	while(RF4432_IRQ == 1);				//查询中断引脚来判断发送完成，这样造成必须屏蔽其他中断的麻烦
//	SendString("Has been Sended.\r\n");	//调试信息时候用
	//读取状态寄存器0x03和0x04，以便释放中断
//	ItStatus1 = SI4432_ReadReg(0x03);			//读取中断标志寄存器1	
//	ItStatus2 = SI4432_ReadReg(0x04);			//读取中断标志寄存器2	

}

/********************************************************************************
 * 函数名：SI4432_ISR()
 * 描述  ：SPI中断服务函数
 * 输入  ：-
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void SI4432_ISR(void)
{
	uchar i,Length_revdat;
	i = SI4432_ReadReg(0x07);					//读取设备模式
//	SendString("mode in ISR:\r\n");
//	SendByteASCII(i);
//	SendString("\r\n");

	//读标志位，清零
	ItStatus1 = SI4432_ReadReg(0x03);			//读取中断标志寄存器1	
	ItStatus2 = SI4432_ReadReg(0x04);			//读取中断标志寄存器2	
//	SendString("Interrupt occued\r\n");
//	SendByteASCII(ItStatus1);
//	SendByteASCII(ItStatus2);
//	SendString("\r\n");
	if(ItStatus1&0x02)							//接收到有效数据
	{
		SendString(" rev Interrupt\r\n");
		
		Length_revdat = SI4432_ReadReg(0x4B);	//读取有效数据长度
		if(PAG_REVSIZE==Length_revdat)
		{
			for(i=0;i<Length_revdat;i++)
			{
				Revbuf[i] = SI4432_ReadReg(0x7F);
			}
		}
		//reset the RX FIFO
		SI4432_WriteReg(0x08, 0x02);			//write 0x02 to the Operating Function Control 2 register
		SI4432_WriteReg(0x08, 0x00);	
		//接收数据后模块自动进入ready模式，需要切换回接收模式才能持续接收数据，否则接收中断也不会产生
		SI4432_SetRxMode();	
		Trans_RevPakFin = 1;					//标记接收到数据
	}
	if(ItStatus1&0x04)							//数据发送完成
	{
		SendString(" send Interrupt\r\n");
		
		//切换回接收模式
		SI4432_SetRxMode();			
	}
}


void debug()
{
	char i;
	i = SI4432_ReadReg(0x07);												//read the Interrupt Status1 register		
	SendString("mode register:\r\n");
	SendByteASCII(i);
	SendString("\r\n");
}
/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

