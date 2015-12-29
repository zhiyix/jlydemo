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


#define  Headend_BYTES       2 //数据包头尾各一个字节
#define  ID_BYTES            2 //SN号2个字节
//通道数为32时一包数据字节数(一个通道2个字节)
#define  HIS_ONE_MAX_BYTES   (uint16_t)(32*2+8*Gps_choose+5+Clock_choose)


//Lcd背光点亮时间 10 s
#define  LcdBackLightTime	 10
//! \brief LCD显示的时间
#define  channeldisplaytime  2       //display_ct%channeldisplaytime 


//唤醒时间
#define WakeUpTime             10
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
  *			    CA0-E00(3584),供开发者使用
  *             E00(3584)-FFE(4094),作为测试fram是否有问题使用
  *             FFE(4094)-1000(4096),作为配置Fram标志使用 2 Byte
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
#define FRAM_SensorChanelOffset32    0x0020	// 传感器通道，32byte,每一通道之间的物理(Fram)偏移量
#define FRAM_TempHumiAdjustOffset    0x0020	/* 传感器校准，64byte/2 offset*2,每一通道之间的物理(Fram)偏移量 */

//! \brief 配置信息表中成员  在Fram中的地址 
#define FRAM_BatVoltageAddr			 FRAM_BasicConfAddr+20	//电池电压地址 低8位
#define FRAM_WorkStatueIsStopAddr	 FRAM_JlyConfAddr+34	//工作状态地址
#define FRAM_AlarmStatusBaseAddr	 FRAM_SensorChanelConfAddr+4	//32个通道报警状态基地址

//flash记录数据溢出标志
#define FLASH_FlashRecOverFlowAddr	 	 FRAM_BasicConfAddr+22 //低8位
//一帧数据大小
#define HisOneBytesAddr					 FRAM_BasicConfAddr+23
//! \brief FRAM中存放fram记录指针地址
#define FRAM_RecWriteAddr_Lchar          FRAM_BasicConfAddr+24      
#define FRAM_RecWriteAddr_Hchar          
//Flash扇区写指针
#define FLASH_SectorWriteAddr_Lchar		 FRAM_BasicConfAddr+26 		
#define FLASH_SectorWriteAddr_Hchar		 
//flash存储最大容量地址
#define FLASH_RecMaxSizeAddr			 FRAM_BasicConfAddr+28
//flash未读数据地址
#define FLASH_NoReadingDataNumAddr_Lchar 	FRAM_BasicConfAddr+32	
#define FLASH_NoReadingDataNumAddr_MidLchar 
#define FLASH_NoReadingDataNumAddr_MidHchar 
#define FLASH_NoReadingDataNumAddr_Hchar 	
//flash读数据起始指针
#define FLASH_ReadDataBeginAddr_Lchar		 FRAM_BasicConfAddr+36		
#define FLASH_ReadDataBeginAddr_MidLchar	     
#define FLASH_ReadDataBeginAddr_MidHchar		 
#define FLASH_ReadDataBeginAddr_Hchar		 
//存放flash写数据指针
#define FLASH_WriteDataAddr_Lchar            FRAM_BasicConfAddr+40     
#define FLASH_WriteDataAddr_MidLchar      
#define FLASH_WriteDataAddr_MidHchar      
#define FLASH_WriteDataAddr_Hchar         
//flash读数据指针
#define FLASH_ReadDataAddr_Lchar             FRAM_BasicConfAddr+44     
#define FLASH_ReadDataAddr_MidLchar      
#define FLASH_ReadDataAddr_MidHchar      
#define FLASH_ReadDataAddr_Hchar         
//flash中历史数据条数地址
#define FLASH_HistoryDataNumAddr			 FRAM_BasicConfAddr+48



//检测fram是否焊接好
#define FRAM_TestIsOkAddr					 0xE00
//配置过Fram标志 2 Byte
#define FRAM_AlreadySetFlagAddr				 0xFFE
//! \brief FRAM中地址定义
#define FRAM_RecFirstAddr           0x1000      //Fram中存放历史数据的首地址
#define FRAM_RecMaxSize				4096		//Fram中存储数据的字节总数 (1000-2000)4096

//----------------------------------------------------------
//8m flash全部用来存储历史数据
//! \brief
#define FLASH_PAGE_NUM              32768   //flash总的页数
//测试 用5个扇区模拟数据存储
#define FLASH_RecMaxSize            20480  //flash中存储数据的字节总数 (800000) 8M  8388608

#define FLASH_RecFirstAddr           0x000000   //Flash中存放数据的首地址
#define FLASH_SectorFirstAddr		 0x000000
//测试 5个扇区
#define FLASH_SectorNum				 5		//8M的flash有2048 sector 2048
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
struct RTCRX8025Str
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
    uint16_t Year;  	 //--
};
//RX8025 控制寄存器
struct STREGStr
{
	uint8_t CT:3;
	uint8_t :1;	//TEST
	uint8_t :1;
	uint8_t H12_24:1;
	uint8_t DALE:1;
	uint8_t WALE:1;
	
	uint8_t DAFG:1;
	uint8_t WAFG:1;
	uint8_t CTFG:1;
	uint8_t :1;
	uint8_t PON:1;
	uint8_t XST:1;
	uint8_t VDET:1;
	uint8_t VDSL:1;
};
typedef union
{
	struct STREGStr STREG;
	uint8_t Control[2];
}RTC8025_Ctrl_Typedef;
//RX8025 闹钟功能寄存器
struct RTCRX8025AlarmStr
{
	uint8_t Alarm_W_Minute;
	uint8_t Alarm_W_Hour;
	uint8_t Alarm_W_Weekday;
	uint8_t Alarm_D_Minute;
	uint8_t Alarm_D_Hour;
};
typedef union
{
	struct RTCRX8025AlarmStr RX8025Alarm;
	uint8_t Alarm[5];
}RTC8025_Alarm_Typedef;
//! \brief 全局标志
struct FLAGStr
{
    __IO uint8_t Sec:1;             	//TIM2定时1s时间
//    __IO uint8_t SysTickSec:1;      	//系统滴答时钟

	__IO uint8_t FirstEnterStopMode:1;  //首次上电时间到进入低功耗
	__IO uint8_t Key1AnXia:1;       	//机械按键key1 按下
	__IO uint8_t WakeUpStopModeOnTime:1;	//时间到唤醒 StopMode低功耗模式
	
	__IO uint8_t AlarmXiaoYin:1;        //报警消音标志
	__IO uint8_t LcdBackLightOn:1;		//Lcd背光点亮标志
	__IO uint8_t TouchKey1DuanAn:1;     //触摸按键key1 短按
	__IO uint8_t TouchKey2DuanAn:1;     //触摸按键key2 短按
	__IO uint8_t KeyEnableOrDisable:1;	//按键是否打开/关闭
	__IO uint8_t AlarmTimeDelayIsOut:1; //声光报警时间到
	__IO uint8_t StartSample:1;         //开始采样
	__IO uint8_t StorageData:1;			//存储数据
	
		 uint8_t Keyflag:1;				//机械按键长短按标志
		 uint8_t Key1DuanAn:1;			//机械按键Key1短按
		 uint8_t Key1ChangAn:1;			//机械按键Key1长按
         uint8_t Low_Voltage:1;     	//电池低电压标志
		 uint8_t BatLowShan:1;     	    //电池低电压闪烁
	
         uint8_t BatteryFull:1;     	//电池满电标志
	     uint8_t BatChargeFull:1;       //电池充满电标志
		 uint8_t BatCharging:1;			//电池正在充电中标志，1正在充电 0未充电
         uint8_t ExPwOn:1;				//外接电源标志
		 uint8_t ExPwShan:1; 			//电池符号闪烁标志
	     uint8_t ExPwFirstDown:1;		//外接电首次断掉
	
         uint8_t IsDisplayRightNow:1;   //LCD显示,第一次采样完成后显示，检索报警
         
		 uint8_t OpenAdcPower:1;		//打开adc电源
         uint8_t EndSample:1;           //结束采样
         uint8_t MucReset:1;            //上电复位？？
         uint8_t RtcShan:1;             //时钟 : 闪烁
		 
		 uint8_t RecTimeDingDianBoot:1; //记录仪时间点定点启动
		 
		 uint8_t SensorTypeIsChange:1;  //通道类型有未改变
		 uint8_t ChannelSwitchIsOn:1;	//通道使能打开
		 uint8_t StopRecording:1;		//停止记录标志
		 
		 uint8_t AlarmHuiFu[32];		//报警消音恢复标志-------可以优化为4个字节，每个标志占一个bit
		 		 
};
//! \brief 电源管理
struct PowerManagementStr
{
//! \brief 外接电池宏定义   
#define  VOLTAGE 
#define  vtest_cnt           5
#define  LowVoltageTestTimeNum     30//300 电池达到低电压时 5分钟检测一次        
#define  NormalVoltageTestTimeNum  10//60//600 电池没达到低电压时 10分钟检测一次

//#define  ExternalPowerchecktime 3 //外接电源检测时间
	
		 //uint8_t  BatChargeFullCount;	   //电池充满电检测计数
	     //uint8_t  HaveExternalPower;	   //接入外接电检测计数
	     uint8_t  JinDuCounts;			   //表示电池一格一格往前
	
	__IO uint16_t BatVoltage_TestTime;     //多长时间检测,电池电压检测时间间隔	
	     uint16_t BatADCValueCopy;   	   
		 uint16_t BatADCValue;			   //存放电池AD值
	
         uint16_t BatVoltage;              //电池实际电量
};

//! \brief 记录仪工作状态等参数
struct JLYPARAMETERStr
{
    uint8_t  WorkStatueIsOrNotStop:1; //记录仪工作状态，0停止工作
    uint8_t  LowMode:1;          	//功耗模式，1低功耗，0正常功耗 
    uint8_t  FramErrorCode:1;		//错误码 
	uint8_t  ShowOffCode;			//启动方式 ,停止方式 ，故障码显示 
	
    __IO uint8_t  WakeUpSource;		//StopMode下唤醒源
	__IO uint8_t  WakeUpCount;		//StopMode下唤醒时间
	
	uint8_t  ContinueExcessiveTimes;//连续超标次数 0-10可设置
	
	uint8_t  RtcSecCount;			//rx8025 秒计数
	uint8_t  NormalRecIntervalMin;	//正常记录间隔 单位：min 
	uint8_t  NormalRecIntervalHour;	//正常记录间隔 单位：hour 
	
	uint8_t  LcdBackLightCount;		//Lcd亮多长时间计数
	uint8_t  ChannelNumActual;		//实际通道数量，32个通道中使能与不使能之后的数量,
	uint8_t  ChannelNumActualOld;   //实际通道数量备份,Conf.Jly.ChannelNum不变，有通道开关
	uint8_t  ChannelNumOld;			//未重新配置之前的通道数
	uint8_t  SensorTypeOld[32];		//未重新配置之前的通道类型
	uint8_t  ChannelSwitchOld[32];  //未重新配置之前的通道使能位
	uint8_t  ContinueExcessiveCount[32];//连续超标计数
	
	uint16_t SoundLightAlarmTimeDelay;	//声光报警延时 单位s 1s到18小时可设置
	uint16_t AlarmTimeDelayCount;	//声光报警延时计数
	
	
	uint32_t SampleInterval;    	//采集时间间隔 单位：s
    uint32_t SampleTimeCount;		//采集时间计数
    uint32_t NormalRecIntervalSec;  //正常记录间隔 单位：s
	int32_t  delay_start_time;		// 延时启动时间 
	uint32_t DataNumInFlash;		//测试flash中数据数量 使用变量---------------
};
//按键
struct KEYStr
{
	uint8_t DuanAnCount;	//机械按键key1短按计数
	uint8_t ChangAnCount;	//机械按键key1长按计数
	uint8_t KeyNum;			//按键识别码
};

//传感器
struct JlySensorStr
{
//! \brief AD最小、最大值
#define  ADC_ERR_L           10
#define  ADC_ERR_H           4090
//温湿度 梯度
#define  TempHumi_Offset	 10
//adc 采样次数
#define  ADCSamplingNum		 10
//传感器稳定计数时间
#define  SensorStableTimes	 6 //湿度响应时间 6s
	
	bool bAdcComplete:1;	//adc数据采集完成
	
	__IO uint8_t SensorStableCount;//传感器稳定计数器
	
};
//----------------------------
extern uint8_t      rtc_pt;
extern uint8_t      display_ct;
extern uint8_t 	    Key1ChangAnCount;
extern uint8_t  	DataBuf[HIS_ONE_MAX_BYTES];
extern uint16_t     adc[32];
extern uint16_t     adcCopy[32];
extern uint16_t	    MsCount;
extern uint32_t     SYS_CLK;

//测试变量
extern uint32_t		SysTickTestCount;
extern uint32_t     temptest;
extern uint32_t	    PowerCount;

extern struct 		CircularQueueStr   Queue;
extern struct 		FLAGStr			   Flag;
extern struct       RTCRX8025Str       Rtc;
extern struct       PowerManagementStr PManage;
extern struct       JLYPARAMETERStr    JlyParam;
extern struct 		KEYStr			   Key;
extern struct 	    JlySensorStr	   JlySensor;

extern const char   RESET_CHANNEL_SETUP_TABLE[104];
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

