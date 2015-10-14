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
//! \brief ���Զ���
#define C_VER_YEAR      11
#define C_VER_MONTH     11
#define C_VER_DAY       11
//! \brief
#define  Gps_choose          0
#define  Clock_choose        1

#define  Channel_count       2
//��Ӧͨ��
#define  CH_1                0
#define  CH_2                1
#define  CH_3                2
#define  CH_4                3

//! \brief ��ӵ�غ궨��   
#define  VOLTAGE 
#define  vtest_cnt           7
#define  voltagetesttimenum  120        
#define  voltagetesttimenum1 10 //60 ����ʱ�޸�Ϊ10
//! \brief LCD��ʾ��ʱ��
#define  channeldisplaytime  2       //display_ct%channeldisplaytime 
//! \brief AD��С�����ֵ
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
//! \brief FRAM�е�ַ����
#define FRAM_RecFirstAddr           0x0600      //Fram�д�����ݵ��׵�ַ

#define FRAM_RecAddr_Hchar          0x0000      //���fram��¼ָ��
#define FRAM_RecAddr_Lchar          0x0001

#define FLASH_RecAddr_Lchar          0x0002     //���flash��¼ָ��
#define FLASH_RecAddr_MidLchar       0x0003
#define FLASH_RecAddr_MidHchar       0x0004
#define FLASH_RecAddr_Hchar          0x0005
//----------------------------------------------------------
//! \brief
#define FLASH_PAGE_NUM              32768   //flash�ܵ�ҳ��
#define Flash_MAX_NUM               ((uint32_t)8388608/HIS_ONE_BYTES) //flash�ܵ��ֽ���

#define FLASH_RecFirstAddr           0x000000   //Flash�д�����ݵ��׵�ַ
#define FLASH_SectorFirstAddr		 0x000000
#define FLASH_SectorPerSize          4096		//Flashÿ�������Ĵ�С
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
    uint16_t RecorderPoint;     //fram�м�¼����ָ��
    uint32_t RecorderFlashPoint;//flash�м�¼����ָ��
    uint32_t FlashSectorPoint;	//flash������ָ��
};
//! \brief rtc�ṹ
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
//! \brief ȫ�ֱ�־
struct FLAG
{
    __IO uint8_t Sec:1;             //TIM2��ʱ1sʱ��
    __IO uint8_t SysTickSec:1;      //ϵͳ�δ�ʱ��
         uint8_t RecordFramOverFlow:1; //Fram�м�¼���������־�������1
         uint8_t RecordFlashOverFlow:1; //Flash�м�¼���������־�������1
         
    __IO uint8_t KeyDuanAn:1;       //key �̰�
    //! \brief ��ر�־
         uint8_t Low_Voltage:1;     //��ص͵�ѹ��־
         uint8_t BatteryFull:1;     //��������־
    //! \brief ��ӵ�Դ��־
         uint8_t ExPwOn:1;
    
         uint8_t IsDisplayRightNow:1;   //LCD��ʾ
         uint8_t StartSample:1;         //��ʼ����
         uint8_t EndSample:1;           //��������
         uint8_t MucReset:1;            //�ϵ縴λ����
         uint8_t RtcShan:1;             //rtc
};
//! \brief ��ؽṹ
struct BATTERY
{
    __IO uint8_t   Voltage_TestTime;        //�೤ʱ����
    __IO uint16_t  ADC_BatConvertedValue;   //��ŵ��ADֵ
    float          BatDataFloat;            //ʵ�ʵĵ���
};
//! \brief ��¼�ǹ���״̬�Ȳ���
struct JLYPARAMETER
{
    uint8_t WorkStatueIsStop:1; //��¼�ǹ���״̬��0ֹͣ����
    uint8_t LowMode:1;          //����ģʽ��1�͹��ģ�0��������
    
    uint32_t Save_Time;
    uint32_t SaveDataTimeOnlyRead;  //����ʱ��
    uint32_t SaveHisDataTime;       //�洢���
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
