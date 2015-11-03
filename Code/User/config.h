/**
  ******************************************************************************
  * @file              : 
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 201x STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_H
#define __CONFIG_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32l1xx.h"

//! @}

/*============================ MACROS ========================================*/
//! @{

//! \brief 测试定义
#define  C_VER_YEAR      15
#define  C_VER_MONTH     10
#define  C_VER_DAY       23
//! \brief
#define  Gps_choose          0
#define  Clock_choose        1

//对应通道
#define  CH_1                0
#define  CH_2                1
#define  CH_3                2
#define  CH_4                3

#define  Headend_BYTES       2 //数据包头尾各一个字节
#define  ID_BYTES            2 //SN号2个字节
//通道数为32时一包数据字节数(一个通道2个字节)
#define  HIS_ONE_MAX_BYTES   (uint16_t)(32*2+8*Gps_choose+5+Clock_choose)
//! \brief 外接电池宏定义   
#define  VOLTAGE 
#define  vtest_cnt           7
#define  voltagetesttimenum   30//300 5分钟检测一次        
#define  voltagetesttimenum1  60//600 10分钟检测一次

#define  ExternalPowerchecktime 60 //外接电源检测时间
//! \brief LCD显示的时间
#define  channeldisplaytime  2       //display_ct%channeldisplaytime 

//温湿度 梯度
#define  TempHumi_Offset	 10
//! \brief AD最小、最大值
#define  ADC_ERR_L           10
#define  ADC_ERR_H           4090

/****************************************************************************************/
//! \brief 协议中对应的虚拟地址
#define VirtBasicConfAddr			0x0000
#define VirtJlyConfAddr				0x0100
#define VirJlyTimeConfAddr			0x01E0
#define VirtAlarmConfAddr			0x0200
#define VirtSensorChanelConfAddr	0x1000
#define VirtTempHumiAdjustConfAddr	0x2000
#define VirtMax						0x4000

#define VirtOffset					0x0080	/* 传感器通道和校准，每一通道之间的虚拟偏移量 */
/****************************************************************************************/
/**Fram使用说明:0-CA0(3232),配置信息表已使用
  *			    CA0-1000(4096),供开发者使用
  * 			1000-2000(8192),作为历史数据缓存已使用
  */
//! \brief 配置信息表在Fram中的地址
#define FRAM_BasicConfAddr           0x0000	/* 起始地址0x0000，结束地址(0x0040-1) 大小   64byte */
#define FRAM_JlyConfAddr             0x0040	/* 起始地址0x0040，结束地址(0x0078-1) 大小   56byte */
#define FRAM_JlyTimeConfAddr		 0x0078	/* 起始地址0x0078，结束地址(0x0080-1) 大小    8byte */
#define FRAM_AlarmConfAddr           0x0080	/* 起始地址0x0080，结束地址(0x00A0-1) 大小   32byte */
#define FRAM_SensorChanelConfAddr    0x00A0	/* 起始地址0x00A0，结束地址(0x04A0-1) 大小 1024byte */
#define FRAM_TempHumiAdjustConfAddr  0x04A0 /* 起始地址0x04A0，结束地址(0x0CA0-1) 大小 2048byte */

#define FRAM_ConfSize			     3232	/* 配置信息表大小 */
#define FRAM_SensorChanelOffset      0x0010	/* 传感器通道，32byte/2 offset*2,每一通道之间的物理(Fram)偏移量 */
#define FRAM_TempHumiAdjustOffset    0x0020	/* 传感器校准，64byte/2 offset*2,每一通道之间的物理(Fram)偏移量 */

//! \brief 配置信息表中成员  在Fram中的地址 
#define FRAM_BatVoltageAddr			 FRAM_BasicConfAddr+20	//电池电压地址
#define FRAM_WorkStatueIsStopAddr	 FRAM_JlyConfAddr+34	//工作状态地址

//! \brief FRAM中存放fram记录指针地址
#define FRAM_RecWriteAddr_Lchar          0x0CA0      
#define FRAM_RecWriteAddr_Hchar          0x0CA1

#define FLASH_SectorWriteAddr_Lchar		 0x0CA2		//Flash扇区写指针
#define FLASH_SectorWriteAddr_Hchar		 0x0CA3

#define FLASH_RecWriteAddr_Lchar         0x0CA4     //存放flash记录指针
#define FLASH_RecWriteAddr_MidLchar      0x0CA5
#define FLASH_RecWriteAddr_MidHchar      0x0CA6
#define FLASH_RecWriteAddr_Hchar         0x0CA7

#define FLASH_ReadDataAddr_Lchar		 0x0CA8		//flash读数据指针
#define FLASH_ReadDataAddr_MidLchar	     0x0CA9
#define FLASH_ReadDataAddr_MidHchar		 0x0CA10
#define FLASH_ReadDataAddr_Hchar		 0x0CA11

#define FLASH_NoReadingDataNumAddr_Lchar 	0x0CA12	//flash未读数据地址
#define FLASH_NoReadingDataNumAddr_MidLchar 0x0CA12
#define FLASH_NoReadingDataNumAddr_MidHchar 0x0CA12
#define FLASH_NoReadingDataNumAddr_Hchar 	0x0CA12
//! \brief FRAM中地址定义
#define FRAM_RecFirstAddr           0x1000      //Fram中存放历史数据的首地址
#define FRAM_RecMaxSize				4096		//Fram中存储数据的字节总数 (1000-2000)4096

//----------------------------------------------------------
//! \brief
#define FLASH_PAGE_NUM              32768   //flash总的页数
//测试 用两个扇区模拟数据存储
#define FLASH_RecMaxSize            8192  //flash中存储数据的字节总数 (800000) 8M  8388608

#define FLASH_RecFirstAddr           0x000000   //Flash中存放数据的首地址
#define FLASH_SectorFirstAddr		 0x000000
//测试 2个扇区
#define FLASH_SectorNum				 2		//8M的flash有2048 sector 2048
#define FLASH_SectorPerSize          4096		//Flash每个扇区的大小

//! @}

/*============================ TYPES =========================================*/
//! \brief 自定义的u16数据结构
union MyU16Data
{
	uint16_t Variable;
	uint8_t  Byte[2];
};
//! \brief 自定义的u32数据结构
union MyU32Data
{
    uint32_t Variable;
    struct BSS
    {
        uint8_t Low;
        uint8_t MidLow;
        uint8_t MidHigh;
        uint8_t High;
    } Bss;
    uint8_t Byte[4];
};
//! \brief rtc结构
struct RTCRX8025
{
//    #define RTC_MK_SEC          5 
//    #define RTC_MK_MIN          11
    uint32_t TMPS; 	  //--
    uint32_t TCPS; 	 //--
    //u32 SMSS;
    //u32 SS;
    //u32 SB;
    uint8_t SCount;  //--
    uint8_t Second;	 //--
    uint8_t Minute;	 //--
    uint8_t Hour;  	 //--
    uint8_t Day;   	 //
    uint8_t Week;  	 //--
    uint8_t Month; 	 //--
    uint8_t Year;  	 //--
};
//! \brief 全局标志
struct FLAG
{
    __IO uint8_t Sec:1;             	//TIM2定时1s时间
    __IO uint8_t SysTickSec:1;      	//系统滴答时钟
	__IO uint8_t Key1DuanAn:1;       	//机械按键key1 短按
	__IO uint8_t TouchKey1DuanAn:1;     //触摸按键key1 短按
	__IO uint8_t TouchKey2DuanAn:1;     //触摸按键key2 短按
         uint8_t RecordFramOverFlow:1; 	//Fram中记录数据溢出标志，溢出置1
         uint8_t RecordFlashOverFlow:1; //Flash中记录数据溢出标志，溢出置1
	
		 uint8_t Powerdowncountflag:1;	//接入外接电标志
         uint8_t Low_Voltage:1;     	//电池低电压标志
		 uint8_t BatLowShan:1;     	    //电池低电压闪烁
	
         uint8_t BatteryFull:1;     	//电池满电标志
	     uint8_t BatChargeFull:1;       //电池充满电标志
		 uint8_t BatCharging:1;			//电池正在充电中标志，1正在充电 0未充电
         uint8_t ExPwOn:1;				//外接电源标志
		 uint8_t ExPwShan:1; 			//电池符号闪烁标志
	     uint8_t ExPwFirstDown:1;		//外接电首次断掉
	
         uint8_t IsDisplayRightNow:1;   //LCD显示
         uint8_t StartSample:1;         //开始采样
         uint8_t EndSample:1;           //结束采样
         uint8_t MucReset:1;            //上电复位？？
         uint8_t RtcShan:1;             //rtc
	
		 uint8_t RecTimeDingShiBoot:1;	//记录仪时间点定时启动
		 uint8_t RecTimeDingDianBoot:1; //记录仪时间点定点启动
		 uint8_t RecTimeDingDianStop:1; //记录仪时间点定点停止
};
//! \brief 电源管理
struct PowerManagement
{
		 uint8_t  BatChargeFullCount;	   //电池充满电检测计数
	     uint8_t  HaveExternalPower;	   //接入外接电检测计数
	     uint8_t  JinDuCounts;			   //表示电池一格一格往前
	
	__IO uint16_t BatVoltage_TestTime;     //多长时间检测,电池电压检测时间间隔	
	__IO uint16_t BatADC_ConvertedValue;   //存放电池AD值
	
         uint16_t BatVoltage;              //电池实际电量
};

//! \brief 记录仪工作状态等参数
struct JLYPARAMETER
{
    uint8_t  WorkStatueIsStop:1; 	//记录仪工作状态，0停止工作
    uint8_t  LowMode:1;          	//功耗模式，1低功耗，0正常功耗 
    uint8_t  LastErrorCode:1;		//错误码 
	uint8_t  ShowOffCode;			//启动方式 ,停止方式 ，故障码显示 
	
	
	uint16_t NormalRecIntervalMin;	//正常记录间隔 单位：min 
	
	uint32_t SampleInterval;    	//采集时间间隔 单位：s
    uint32_t SampleTime;			//采集时间 单位：s
	
    uint32_t NormalRecInterval;  	//正常记录间隔 单位：s
    //uint32_t SaveHisDataTime;       //存储间隔
	
	int32_t delay_start_time;		// 延时启动时间 
	
};

//----------------------------
extern uint8_t      rtc_pt;
extern uint8_t      display_ct;
extern uint8_t 	    Key1ChangAnCount;
extern uint8_t  	DataBuf[HIS_ONE_MAX_BYTES+Headend_BYTES+ID_BYTES];
extern uint16_t     adc[32];
extern uint16_t     adcCopy[32];
extern uint16_t	    MsCount;

extern struct 		CircularQueue   Queue;
extern struct 		FLAG			Flag;
extern struct       RTCRX8025       Rtc;
extern struct       PowerManagement PManage;
extern struct       JLYPARAMETER    JlyParam;

extern const char RESET_CHANNEL_SETUP_TABLE[104];
extern const unsigned char *AdjustCurveFirAddress[];
extern const uint32_t ConfMap_Address[6][2];
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/


//#ifdef __cplusplus
//}
//#endif

#endif /* __CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

