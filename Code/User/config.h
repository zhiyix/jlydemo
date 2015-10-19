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
#define C_VER_YEAR      11
#define C_VER_MONTH     11
#define C_VER_DAY       11
//! \brief
#define  Gps_choose          0
#define  Clock_choose        1

#define  Channel_count       2
//对应通道
#define  CH_1                0
#define  CH_2                1
#define  CH_3                2
#define  CH_4                3

//! \brief 外接电池宏定义   
#define  VOLTAGE 
#define  vtest_cnt           7
#define  voltagetesttimenum  120        
#define  voltagetesttimenum1 10 //60 测试时修改为10
//! \brief LCD显示的时间
#define  channeldisplaytime  2       //display_ct%channeldisplaytime 
//! \brief AD最小、最大值
#define  ADC_ERR_L           10
#define  ADC_ERR_H           4090

#define  Headend_BYTES       2
#define  ID_BYTES            2
//----------------------------------------------------------
#define HIS_MAX_NUM              ((uint16_t)512/HIS_ONE_BYTES)//1540 ((u16)360/HIS_ONE_BYTES)
//#define HIS_ONE_BYTES            (uint16_t)(Channel_count*8+(5+Clock_choose)+2)//24  72
#define HIS_ONE_BYTES            (uint16_t)(Channel_count*2+8*Gps_choose+5+Clock_choose) 

//! \brief
#define ADJUST_TABLE_HEAD_LENGTH    0x08

//! \brief 协议中对应的虚拟地址
#define VirtBasicConfAddr			0x0000
#define VirtJlyConfAddr				0x0100
#define VirtAlarmConfAddr			0x0200
#define VirtSensorChanelConfAddr	0x1000
#define VirtTempHumiAdjustConfAddr	0x2000
#define VirtMax						0x4000
#define VirtOffset					0x0080	/*!< 传感器通道和校准，每一通道之间的虚拟偏移量 */
//! \brief 配置信息表在Fram中的地址
#define FRAM_BasicConfAddr           0x0000	/*!< 起始地址0x0000，结束地址(0x0040-1) 大小 64byte */
#define FRAM_JlyConfAddr             0x0040	/*!< 起始地址0x0040，结束地址(0x0080-1) 大小 64byte */
#define FRAM_AlarmConfAddr           0x0080	/*!< 起始地址0x0080，结束地址(0x00A0-1) 大小 32byte */
#define FRAM_SensorChanelConfAddr    0x00A0	/*!< 起始地址0x00A0，结束地址(0x04A0-1) 大小 1024byte */
#define FRAM_TempHumiAdjustConfAddr  0x04A0 /*!< 起始地址0x04A0，结束地址(0x0CA0-1) 大小 2048byte */

#define FRAM_ConfSize			     3232	/*!< 配置信息表大小 */
#define FRAM_SensorChanelOffset      0x0020	/*!< 传感器通道，32byte,每一通道之间的物理(Fram)偏移量 */
#define FRAM_TempHumiAdjustOffset    0x0040	/*!< 传感器校准，64byte,每一通道之间的物理(Fram)偏移量 */

//! \brief 配置信息表中成员  在Fram中的地址
#define FRAM_WorkStatueIsStopAddr	 FRAM_JlyConfAddr+42
//! \brief FRAM中存放fram记录指针地址
#define FRAM_RecAddr_Hchar          0x0CA0      
#define FRAM_RecAddr_Lchar          0x0CA1

#define FLASH_RecAddr_Lchar          0x0CA2     //存放flash记录指针
#define FLASH_RecAddr_MidLchar       0x0CA3
#define FLASH_RecAddr_MidHchar       0x0CA4
#define FLASH_RecAddr_Hchar          0x0CA5

//! \brief FRAM中地址定义
#define FRAM_RecFirstAddr           0x1000      //Fram中存放历史数据的首地址


//----------------------------------------------------------
//! \brief
#define FLASH_PAGE_NUM              32768   //flash总的页数
#define Flash_MAX_NUM               ((uint32_t)8388608/HIS_ONE_BYTES) //flash总的字节数

#define FLASH_RecFirstAddr           0x000000   //Flash中存放数据的首地址
#define FLASH_SectorFirstAddr		 0x000000
#define FLASH_SectorPerSize          4096		//Flash每个扇区的大小
//! @}

/*============================ TYPES =========================================*/
//! \brief 
union MYU32
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
    uint32_t TMPS; //--
    uint32_t TCPS; //--
    //u32 SMSS;
    //u32 SS;
    //u32 SB;
//    uint8_t MK;  //--
    uint8_t Second;//--
    uint8_t Minute;//--
    uint8_t Hour;  //--
    uint8_t Day;   //
    uint8_t Week;  //--
    uint8_t Month; //--
    uint8_t Year;  //--
};
//! \brief 全局标志
struct FLAG
{
    __IO uint8_t Sec:1;             //TIM2定时1s时间
    __IO uint8_t SysTickSec:1;      //系统滴答时钟
         uint8_t RecordFramOverFlow:1; //Fram中记录数据溢出标志，溢出置1
         uint8_t RecordFlashOverFlow:1; //Flash中记录数据溢出标志，溢出置1
         
    __IO uint8_t KeyDuanAn:1;       //key 短按
    //! \brief 电池标志
         uint8_t Low_Voltage:1;     //电池低电压标志
         uint8_t BatteryFull:1;     //电池满电标志
    //! \brief 外接电源标志
         uint8_t ExPwOn:1;
    
         uint8_t IsDisplayRightNow:1;   //LCD显示
         uint8_t StartSample:1;         //开始采样
         uint8_t EndSample:1;           //结束采样
         uint8_t MucReset:1;            //上电复位？？
         uint8_t RtcShan:1;             //rtc
};
//! \brief 电池结构
struct BATTERY
{
    __IO uint8_t   Voltage_TestTime;        //多长时间检测
    __IO uint16_t  ADC_BatConvertedValue;   //存放电池AD值
    float          BatDataFloat;            //实际的电量
};
//! \brief 记录仪工作状态等参数
struct JLYPARAMETER
{
    uint8_t WorkStatueIsStop:1; //记录仪工作状态，0停止工作
    uint8_t LowMode:1;          //功耗模式，1低功耗，0正常功耗
    uint8_t LastErrorCode:1;	/*!< 错误码 */
	
    uint32_t Save_Time;
    uint32_t SaveDataTimeOnlyRead;  //采样时间
    uint32_t SaveHisDataTime;       //存储间隔
	
	uint32_t delay_start_time;	/*!< 延时启动时间 */
	
};

//----------------------------
extern uint8_t        rtc_pt;
extern uint8_t      display_ct;
extern uint16_t     testcount;
extern uint16_t     adc[32];
extern uint16_t     adcCopy[32];
extern uint8_t  	DataBuf[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
extern uint16_t	    MsCount;
extern struct 		CircularQueue   Queue;
extern struct 		FLAG			Flag;
extern struct       RTCRX8025       Rtc;
extern struct       BATTERY         Bat;
extern struct       JLYPARAMETER    JlyParam;
extern const char RESET_CHANNEL_SETUP_TABLE[104];
extern const unsigned char *AdjustCurveFirAddress[];
extern const uint32_t ConfMap_Address[5][2];
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/


//#ifdef __cplusplus
//}
//#endif

#endif /* __CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

