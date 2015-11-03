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
#define  C_VER_YEAR      15
#define  C_VER_MONTH     10
#define  C_VER_DAY       23
//! \brief
#define  Gps_choose          0
#define  Clock_choose        1

//��Ӧͨ��
#define  CH_1                0
#define  CH_2                1
#define  CH_3                2
#define  CH_4                3

#define  Headend_BYTES       2 //���ݰ�ͷβ��һ���ֽ�
#define  ID_BYTES            2 //SN��2���ֽ�
//ͨ����Ϊ32ʱһ�������ֽ���(һ��ͨ��2���ֽ�)
#define  HIS_ONE_MAX_BYTES   (uint16_t)(32*2+8*Gps_choose+5+Clock_choose)
//! \brief ��ӵ�غ궨��   
#define  VOLTAGE 
#define  vtest_cnt           7
#define  voltagetesttimenum   30//300 5���Ӽ��һ��        
#define  voltagetesttimenum1  60//600 10���Ӽ��һ��

#define  ExternalPowerchecktime 60 //��ӵ�Դ���ʱ��
//! \brief LCD��ʾ��ʱ��
#define  channeldisplaytime  2       //display_ct%channeldisplaytime 

//��ʪ�� �ݶ�
#define  TempHumi_Offset	 10
//! \brief AD��С�����ֵ
#define  ADC_ERR_L           10
#define  ADC_ERR_H           4090

/****************************************************************************************/
//! \brief Э���ж�Ӧ�������ַ
#define VirtBasicConfAddr			0x0000
#define VirtJlyConfAddr				0x0100
#define VirJlyTimeConfAddr			0x01E0
#define VirtAlarmConfAddr			0x0200
#define VirtSensorChanelConfAddr	0x1000
#define VirtTempHumiAdjustConfAddr	0x2000
#define VirtMax						0x4000

#define VirtOffset					0x0080	/* ������ͨ����У׼��ÿһͨ��֮�������ƫ���� */
/****************************************************************************************/
/**Framʹ��˵��:0-CA0(3232),������Ϣ����ʹ��
  *			    CA0-1000(4096),��������ʹ��
  * 			1000-2000(8192),��Ϊ��ʷ���ݻ�����ʹ��
  */
//! \brief ������Ϣ����Fram�еĵ�ַ
#define FRAM_BasicConfAddr           0x0000	/* ��ʼ��ַ0x0000��������ַ(0x0040-1) ��С   64byte */
#define FRAM_JlyConfAddr             0x0040	/* ��ʼ��ַ0x0040��������ַ(0x0078-1) ��С   56byte */
#define FRAM_JlyTimeConfAddr		 0x0078	/* ��ʼ��ַ0x0078��������ַ(0x0080-1) ��С    8byte */
#define FRAM_AlarmConfAddr           0x0080	/* ��ʼ��ַ0x0080��������ַ(0x00A0-1) ��С   32byte */
#define FRAM_SensorChanelConfAddr    0x00A0	/* ��ʼ��ַ0x00A0��������ַ(0x04A0-1) ��С 1024byte */
#define FRAM_TempHumiAdjustConfAddr  0x04A0 /* ��ʼ��ַ0x04A0��������ַ(0x0CA0-1) ��С 2048byte */

#define FRAM_ConfSize			     3232	/* ������Ϣ���С */
#define FRAM_SensorChanelOffset      0x0010	/* ������ͨ����32byte/2 offset*2,ÿһͨ��֮�������(Fram)ƫ���� */
#define FRAM_TempHumiAdjustOffset    0x0020	/* ������У׼��64byte/2 offset*2,ÿһͨ��֮�������(Fram)ƫ���� */

//! \brief ������Ϣ���г�Ա  ��Fram�еĵ�ַ 
#define FRAM_BatVoltageAddr			 FRAM_BasicConfAddr+20	//��ص�ѹ��ַ
#define FRAM_WorkStatueIsStopAddr	 FRAM_JlyConfAddr+34	//����״̬��ַ

//! \brief FRAM�д��fram��¼ָ���ַ
#define FRAM_RecWriteAddr_Lchar          0x0CA0      
#define FRAM_RecWriteAddr_Hchar          0x0CA1

#define FLASH_SectorWriteAddr_Lchar		 0x0CA2		//Flash����дָ��
#define FLASH_SectorWriteAddr_Hchar		 0x0CA3

#define FLASH_RecWriteAddr_Lchar         0x0CA4     //���flash��¼ָ��
#define FLASH_RecWriteAddr_MidLchar      0x0CA5
#define FLASH_RecWriteAddr_MidHchar      0x0CA6
#define FLASH_RecWriteAddr_Hchar         0x0CA7

#define FLASH_ReadDataAddr_Lchar		 0x0CA8		//flash������ָ��
#define FLASH_ReadDataAddr_MidLchar	     0x0CA9
#define FLASH_ReadDataAddr_MidHchar		 0x0CA10
#define FLASH_ReadDataAddr_Hchar		 0x0CA11

#define FLASH_NoReadingDataNumAddr_Lchar 	0x0CA12	//flashδ�����ݵ�ַ
#define FLASH_NoReadingDataNumAddr_MidLchar 0x0CA12
#define FLASH_NoReadingDataNumAddr_MidHchar 0x0CA12
#define FLASH_NoReadingDataNumAddr_Hchar 	0x0CA12
//! \brief FRAM�е�ַ����
#define FRAM_RecFirstAddr           0x1000      //Fram�д����ʷ���ݵ��׵�ַ
#define FRAM_RecMaxSize				4096		//Fram�д洢���ݵ��ֽ����� (1000-2000)4096

//----------------------------------------------------------
//! \brief
#define FLASH_PAGE_NUM              32768   //flash�ܵ�ҳ��
//���� ����������ģ�����ݴ洢
#define FLASH_RecMaxSize            8192  //flash�д洢���ݵ��ֽ����� (800000) 8M  8388608

#define FLASH_RecFirstAddr           0x000000   //Flash�д�����ݵ��׵�ַ
#define FLASH_SectorFirstAddr		 0x000000
//���� 2������
#define FLASH_SectorNum				 2		//8M��flash��2048 sector 2048
#define FLASH_SectorPerSize          4096		//Flashÿ�������Ĵ�С

//! @}

/*============================ TYPES =========================================*/
//! \brief �Զ����u16���ݽṹ
union MyU16Data
{
	uint16_t Variable;
	uint8_t  Byte[2];
};
//! \brief �Զ����u32���ݽṹ
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
//! \brief rtc�ṹ
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
//! \brief ȫ�ֱ�־
struct FLAG
{
    __IO uint8_t Sec:1;             	//TIM2��ʱ1sʱ��
    __IO uint8_t SysTickSec:1;      	//ϵͳ�δ�ʱ��
	__IO uint8_t Key1DuanAn:1;       	//��е����key1 �̰�
	__IO uint8_t TouchKey1DuanAn:1;     //��������key1 �̰�
	__IO uint8_t TouchKey2DuanAn:1;     //��������key2 �̰�
         uint8_t RecordFramOverFlow:1; 	//Fram�м�¼���������־�������1
         uint8_t RecordFlashOverFlow:1; //Flash�м�¼���������־�������1
	
		 uint8_t Powerdowncountflag:1;	//������ӵ��־
         uint8_t Low_Voltage:1;     	//��ص͵�ѹ��־
		 uint8_t BatLowShan:1;     	    //��ص͵�ѹ��˸
	
         uint8_t BatteryFull:1;     	//��������־
	     uint8_t BatChargeFull:1;       //��س������־
		 uint8_t BatCharging:1;			//������ڳ���б�־��1���ڳ�� 0δ���
         uint8_t ExPwOn:1;				//��ӵ�Դ��־
		 uint8_t ExPwShan:1; 			//��ط�����˸��־
	     uint8_t ExPwFirstDown:1;		//��ӵ��״ζϵ�
	
         uint8_t IsDisplayRightNow:1;   //LCD��ʾ
         uint8_t StartSample:1;         //��ʼ����
         uint8_t EndSample:1;           //��������
         uint8_t MucReset:1;            //�ϵ縴λ����
         uint8_t RtcShan:1;             //rtc
	
		 uint8_t RecTimeDingShiBoot:1;	//��¼��ʱ��㶨ʱ����
		 uint8_t RecTimeDingDianBoot:1; //��¼��ʱ��㶨������
		 uint8_t RecTimeDingDianStop:1; //��¼��ʱ��㶨��ֹͣ
};
//! \brief ��Դ����
struct PowerManagement
{
		 uint8_t  BatChargeFullCount;	   //��س����������
	     uint8_t  HaveExternalPower;	   //������ӵ������
	     uint8_t  JinDuCounts;			   //��ʾ���һ��һ����ǰ
	
	__IO uint16_t BatVoltage_TestTime;     //�೤ʱ����,��ص�ѹ���ʱ����	
	__IO uint16_t BatADC_ConvertedValue;   //��ŵ��ADֵ
	
         uint16_t BatVoltage;              //���ʵ�ʵ���
};

//! \brief ��¼�ǹ���״̬�Ȳ���
struct JLYPARAMETER
{
    uint8_t  WorkStatueIsStop:1; 	//��¼�ǹ���״̬��0ֹͣ����
    uint8_t  LowMode:1;          	//����ģʽ��1�͹��ģ�0�������� 
    uint8_t  LastErrorCode:1;		//������ 
	uint8_t  ShowOffCode;			//������ʽ ,ֹͣ��ʽ ����������ʾ 
	
	
	uint16_t NormalRecIntervalMin;	//������¼��� ��λ��min 
	
	uint32_t SampleInterval;    	//�ɼ�ʱ���� ��λ��s
    uint32_t SampleTime;			//�ɼ�ʱ�� ��λ��s
	
    uint32_t NormalRecInterval;  	//������¼��� ��λ��s
    //uint32_t SaveHisDataTime;       //�洢���
	
	int32_t delay_start_time;		// ��ʱ����ʱ�� 
	
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

