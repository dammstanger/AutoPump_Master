
/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：SI4432.h
 * 描	述	：
 *                    
 * 实验平台	：自控水泵V2.0
 * 硬件连接	：
 * 版 	本	：
 * 库版本		：
 * 创建时间	：2016.1.21
 * 最后编辑	：2016.1.21
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#ifndef __SI4432_H__
#define __SI4432_H__

#include "SPI_51.h"
#include "globaldefine.h"
//*******************************************************************************//
//						   RF4432 接收包长度
//
//******************************************************************************//
//	
//#define LENGTH_REVBUF		10			

//*******************************************************************************//
//						   RF4432 引脚配置
//
//******************************************************************************//
//	
//RF4432模块开关
#define RF4432_SDN_PORT              P4
#define RF4432_SDN_CLK                 
#define RF4432_SDN_PIN               SI4432_SDN
#define RF4432_SDN_1  				{SI4432_SDN = 1;}
#define RF4432_SDN_0  				{SI4432_SDN = 0;}
//RF4432片选信号	 
#define RF4432_SEL_PORT              P3
#define RF4432_SEL_CLK                 
#define RF4432_SEL_PIN               SI4432_SEL
#define RF4432_SEL_1  				{SI4432_SEL = 1;}
#define RF4432_SEL_0  				{SI4432_SEL = 0;}
//IRQ主机数据输入
#define RF4432_IRQ_PORT              P3
#define RF4432_IRQ_CLK                 
#define RF4432_IRQ_PIN               SI4432_IRQ
#define RF4432_IRQ_1  				{SI4432_IRQ = 1;}
#define RF4432_IRQ_0  				{SI4432_IRQ = 0;}
#define RF4432_IRQ  				 RF4432_IRQ_PIN

#define SI4432_INT_ON				EX0 = 1
#define SI4432_INT_OFF				EX0 = 0

//*******************************************************************************//
//						   RF4432 配置寄存器地址
//
//******************************************************************************//
#define DEVICE_TYPE 										 0x00
#define DEVICE_VERSION 										 0x01   //版本号
#define DEVICE_STATUS 										 0x02	//设备状态
#define INTERRUPT_STATUS_1							       	 0x03
#define INTERRUPT_STATUS_2									 0x04
#define INTERRUPT_ENABLE_1 									 0x05
#define INTERRUPT_ENABLE_2 							 	  	 0x06
#define OPERATING_FUNCTION_CONTROL_1					 	 0x07	//工作模式和功能控制1
#define OPERATING_FUNCTION_CONTROL_2 						 0x08	//工作模式和功能控制2
#define CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE					 0x09	//晶振负载电容设置
#define MICROCONTROLLER_OUTPUT_CLOCK 						 0x0A
#define GPIO0_CONFIGURATION 							 	 0x0B	//GPIO0功能设置寄存器见英文文档第105页
#define GPIO1_CONFIGURATION 							 	 0x0C
#define GPIO2_CONFIGURATION 							 	 0x0D
#define IO_PORT_CONFIGURATION 								 0x0E
#define ADC_CONFIGURATION								 	 0x0F
#define ADC_SENSOR_AMPLIFIER_OFFSET					 		 0x10
#define ADC_VALUE 											 0x11
#define TEMPERATURE_SENSOR_CONTROL							 0x12	//温度传感器校准
#define TEMPERATURE_VALUE_OFFSET 							 0x13
#define WAKE_UP_TIMER_PERIOD_1								 0x14
#define WAKE_UP_TIMER_PERIOD_2								 0x15
#define WAKE_UP_TIMER_PERIOD_3						 		 0x16
#define WAKE_UP_TIMER_VALUE_1 								 0x17
#define WAKE_UP_TIMER_VALUE_2 								 0x18
#define LOW_DUTY_CYCLE_MODE_DURATION 						 0x19	
#define LOW_BATTERY_DETECTOR_THRESHOLD						 0x1A	//低压检测阈值寄存器
#define BATTERY_VOLTAGE_LEVEL								 0x1B
#define IF_FILTER_BANDWIDTH 								 0x1C	//中频滤波器带宽寄存器
#define AFC_LOOP_GEARSHIFT_OVERRIDE 						 0x1D
#define AFC_TIMING_CONTROL									 0x1E
#define CLOCK_RECOVERY_GEARSHIFT_OVERRIDE 					 0x1F
#define CLOCK_RECOVERY_OVERSAMPLING_RATIO 			 		 0x20
#define CLOCK_RECOVERY_OFFSET_2 							 0x21
#define CLOCK_RECOVERY_OFFSET_1								 0x22
#define CLOCK_RECOVERY_OFFSET_0 							 0x23
#define CLOCK_RECOVERY_TIMING_LOOP_GAIN_1					 0x24
#define CLOCK_RECOVERY_TIMING_LOOP_GAIN_0 			 		 0x25
#define RECEIVED_SIGNAL_STRENGTH_INDICATOR 			 		 0x26
#define RSSI_THRESHOLD_FOR_CLEAR_CHANNEL_INDICATOR 			 0x27
#define ANTENNA_DIVERSITY_REGISTER_1 						 0x28
#define ANTENNA_DIVERSITY_REGISTER_2 						 0x29
#define DATA_ACCESS_CONTROL 							 	 0x30
#define EZMAC_STATUS 										 0x31
#define HEADER_CONTROL_1 								 	 0x32	//Header 起始码设置
#define HEADER_CONTROL_2 								 	 0x33
#define PREAMBLE_LENGTH 									 0x34	//前导码长度
#define PREAMBLE_DETECTION_CONTROL							 0x35	//前导码检测设置
#define SYNC_WORD_3 									 	 0x36	//同步字节
#define SYNC_WORD_2 								  	 	 0x37
#define SYNC_WORD_1									 		 0x38
#define SYNC_WORD_0 									 	 0x39
#define TRANSMIT_HEADER_3 								 	 0x3A
#define TRANSMIT_HEADER_2									 0x3B
#define TRANSMIT_HEADER_1									 0x3C
#define TRANSMIT_HEADER_0								 	 0x3D
#define TRANSMIT_PACKET_LENGTH								 0x3E	//发送数据包长度，详细请看P125页
#define CHECK_HEADER_3										 0x3F
#define CHECK_HEADER_2										 0x40
#define CHECK_HEADER_1 										 0x41
#define CHECK_HEADER_0									 	 0x42
#define HEADER_ENABLE_3                                      0x43
#define HEADER_ENABLE_2                                      0x44
#define HEADER_ENABLE_1                                      0x45
#define HEADER_ENABLE_0                                      0x46
#define RECEIVED_HEADER_3                                    0x47
#define RECEIVED_HEADER_2                                    0x48
#define RECEIVED_HEADER_1                                    0x49
#define RECEIVED_HEADER_0                                    0x4A
#define RECEIVED_PACKET_LENGTH                               0x4B	//接收数据包长度，详细请看P134页
#define ANALOG_TEST_BUS                                      0x50
#define DIGITAL_TEST_BUS			                         0x51
#define TX_RAMP_CONTROL                                      0x52
#define PLL_TUNE_TIME                                        0x53	//锁相环切换时间
#define CALIBRATION_CONTROL                                  0x55
#define MODEM_TEST                                           0x56
#define CHARGEPUMP_TEST                                      0x57
#define CHARGEPUMP_CURRENT_TRIMMING_OVERRIDE                 0x58
#define DIVIDER_CURRENT_TRIMMING             				 0x59
#define VCO_CURRENT_TRIMMING               					 0x5A
#define VCO_CALIBRATION_OVERRIDE     				         0x5B
#define SYNTHESIZER_TEST            					     0x5C
#define BLOCK_ENABLE_OVERRIDE_1					    	     0x5D
#define BLOCK_ENABLE_OVERRIDE_2					    	     0x5E
#define BLOCK_ENABLE_OVERRIDE_3					     	     0x5F
#define CHANNEL_FILTER_COEFFICIENT_ADDRESS	     	         0x60
#define CHANNEL_FILTER_COEFFICIENT_VALUE	     	         0x61
#define CRYSTAL_OSCILLATOR_CONTROL_TEST	     	    		 0x62
#define RC_OSCILLATOR_COARSE_CALIBRATION_OVERRIDE	         0x63
#define RC_OSCILLATOR_FINE_CALIBRATION_OVERRIDE	     	     0x64
#define LDO_CONTROL_OVERRIDE_ENSPOR	     	     		     0x65
#define LDO_LEVEL_SETTING	     	   						 0x66
#define DELTASIGMA_ADC_TUNING_1	     	    				 0x67
#define DELTASIGMA_ADC_TUNING_2	     	   				     0x68
#define AGC_OVERRIDE_1	     	     						 0x69
#define AGC_OVERRIDE_2	     	   							 0x6A
#define GFSK_FIR_FILTER_COEFFICIENT_ADDRESS	     	         0x6B
#define GFSK_FIR_FILTER_COEFFICIENT_VALUE	     	   	     0x6C
#define TX_POWER	     	  								 0x6D   //发射功率设置,详细请见P153
#define TX_DATA_RATE_1	     								 0x6E	//数据发送波特率设置寄存器1
#define TX_DATA_RATE_0	     	 							 0x6F	//数据发送波特率设置寄存器0
#define MODULATION_MODE_CONTROL_1  	 				   		 0x70	//调制方式控制,详细请见P155
#define MODULATION_MODE_CONTROL_2	 				     	 0x71
#define FREQUENCY_DEVIATION	 				 		    	 0x72
#define FREQUENCY_OFFSET_1	 				    		     0x73
#define FREQUENCY_OFFSET_2	 				   		     	 0x74
#define FREQUENCY_BAND_SELECT	 				   		     0x75	//频段选择,详细请见P157
#define NOMINAL_CARRIER_FREQUENCY_1	 				    	 0x76	//基准载波频率
#define NOMINAL_CARRIER_FREQUENCY_0	 				    	 0x77
#define FREQUENCY_HOPPING_CHANNEL_SELECT	 		    	 0x79	//跳频频道选择,详细请见P158
#define FREQUENCY_HOPPING_STEP_SIZE	 				    	 0x7A	//跳频频道间隔
#define TX_FIFO_CONTROL_1	 				    		     0x7C
#define TX_FIFO_CONTROL_2	 				  			     0x7D
#define RX_FIFO_CONTROL	 				   			   	     0x7E
#define FIFO_ACCESS	 				    					 0x7F  //FIFO读写方式设置，
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//This value should be set according to the experiment value, different batches has different value.
#define CRYSTAL_CAPACITANCE     0xB4
////////////////////////////////////////////////////////////////

/******************************变量声明****************************************/


char SI4432_Check(void);
void SI4432_Init(void);
void SI4432_GPIOInit(void);
void SI4432_RF_Init(void);
void SI4432_SetRxMode(void);
void SI4432_SetTxMode(void);
void SI4432_SetIdleMode(void);
void SI4432_FIFO_SendData(char length, char *payload);
void SI4432_ISR(void);
void debug();


#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

