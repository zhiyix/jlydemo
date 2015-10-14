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
//! \brief FRAM中地址定义
#define FRAM_RecFirstAddr           0x0600      //Fram中存放数据的首地址

#define FRAM_RecAddr_Hchar          0x0000      //存放fram记录指针
#define FRAM_RecAddr_Lchar          0x0001

#define FLASH_RecAddr_Lchar          0x0002     //存放flash记录指针
#define FLASH_RecAddr_MidLchar       0x0003
#define FLASH_RecAddr_MidHchar       0x0004
#define FLASH_RecAddr_Hchar          0x0005
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
//! \brief 
struct CircularQueue
{
    uint16_t RecorderPoint;     //fram中记录数据指针
    uint32_t RecorderFlashPoint;//flash中记录数据指针
    uint32_t FlashSectorPoint;	//flash中扇区指针
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
    
    uint32_t Save_Time;
    uint32_t SaveDataTimeOnlyRead;  //采样时间
    uint32_t SaveHisDataTime;       //存储间隔
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
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/


//#ifdef __cplusplus
//}
//#endif

#endif /* __CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

