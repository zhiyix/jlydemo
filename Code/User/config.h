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
//Lcd�������ʱ�� 10 s
#define  LcdBackLightTime	 10
//! \brief LCD��ʾ��ʱ��
#define  channeldisplaytime  2       //display_ct%channeldisplaytime 

//��ʪ�� �ݶ�
#define  TempHumi_Offset	 10
//! \brief AD��С�����ֵ
#define  ADC_ERR_L           10
#define  ADC_ERR_H           4090
//�ϵ�20s����������͹���
#define FirstEnterStopModeTime 20
//����ʱ��
#define WakeUpTime             10
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
  *			    CA0-E00(3584),��������ʹ��
  *             E00(3584)-FFE(4094),��Ϊ����fram�Ƿ�������ʹ��
  *             FFE(4094)-1000(4096),��Ϊ����Fram��־ʹ�� 2 Byte
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
#define FRAM_BatVoltageAddr			 FRAM_BasicConfAddr+20	//��ص�ѹ��ַ ��8λ
#define FRAM_WorkStatueIsStopAddr	 FRAM_JlyConfAddr+34	//����״̬��ַ

//flash��¼���������־
#define FLASH_FlashRecOverFlowAddr	 	 FRAM_BasicConfAddr+22 //��8λ
//һ֡���ݴ�С
#define HisOneBytesAddr					 FRAM_BasicConfAddr+23
//! \brief FRAM�д��fram��¼ָ���ַ
#define FRAM_RecWriteAddr_Lchar          FRAM_BasicConfAddr+24      
#define FRAM_RecWriteAddr_Hchar          
//Flash����дָ��
#define FLASH_SectorWriteAddr_Lchar		 FRAM_BasicConfAddr+26 		
#define FLASH_SectorWriteAddr_Hchar		 
//flash�洢�������
#define FLASH_RecMaxSizeAddr			 FRAM_BasicConfAddr+28
//flashδ�����ݵ�ַ
#define FLASH_NoReadingDataNumAddr_Lchar 	FRAM_BasicConfAddr+32	
#define FLASH_NoReadingDataNumAddr_MidLchar 
#define FLASH_NoReadingDataNumAddr_MidHchar 
#define FLASH_NoReadingDataNumAddr_Hchar 	
//flash��������ʼָ��
#define FLASH_ReadDataBeginAddr_Lchar		 FRAM_BasicConfAddr+36		
#define FLASH_ReadDataBeginAddr_MidLchar	     
#define FLASH_ReadDataBeginAddr_MidHchar		 
#define FLASH_ReadDataBeginAddr_Hchar		 
//���flashд����ָ��
#define FLASH_WriteDataAddr_Lchar            FRAM_BasicConfAddr+40     
#define FLASH_WriteDataAddr_MidLchar      
#define FLASH_WriteDataAddr_MidHchar      
#define FLASH_WriteDataAddr_Hchar         
//flash������ָ��
#define FLASH_ReadDataAddr_Lchar             FRAM_BasicConfAddr+44     
#define FLASH_ReadDataAddr_MidLchar      
#define FLASH_ReadDataAddr_MidHchar      
#define FLASH_ReadDataAddr_Hchar         

//���fram�Ƿ񺸽Ӻ�
#define FRAM_TestIsOkAddr					 0xE00
//���ù�Fram��־ 2 Byte
#define FRAM_AlreadySetFlagAddr				 0xFFE
//! \brief FRAM�е�ַ����
#define FRAM_RecFirstAddr           0x1000      //Fram�д����ʷ���ݵ��׵�ַ
#define FRAM_RecMaxSize				4096		//Fram�д洢���ݵ��ֽ����� (1000-2000)4096

//----------------------------------------------------------
//8m flashȫ�������洢��ʷ����
//! \brief
#define FLASH_PAGE_NUM              32768   //flash�ܵ�ҳ��
//���� ��3������ģ�����ݴ洢
#define FLASH_RecMaxSize            12288  //flash�д洢���ݵ��ֽ����� (800000) 8M  8388608

#define FLASH_RecFirstAddr           0x000000   //Flash�д�����ݵ��׵�ַ
#define FLASH_SectorFirstAddr		 0x000000
//���� 3������
#define FLASH_SectorNum				 3		//8M��flash��2048 sector 2048
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
    uint8_t Year;  	 //--
};
//! \brief ȫ�ֱ�־
struct FLAGStr
{
    __IO uint8_t Sec:1;             	//TIM2��ʱ1sʱ��
//    __IO uint8_t SysTickSec:1;      	//ϵͳ�δ�ʱ��
	__IO uint8_t FirstNotEnterStopMode:1;//�״��ϵ粻����͹���
	__IO uint8_t FirstEnterStopMode:1;  //�״��ϵ�ʱ�䵽����͹���
	__IO uint8_t Key1AnXia:1;       	//��е����key1 ����
	__IO uint8_t WakeUpStopModeOnTime:1;	//ʱ�䵽���� StopMode�͹���ģʽ
	
	__IO uint8_t AlarmXiaoYin:1;        //����������־
	__IO uint8_t LcdBackLightOn:1;		//Lcd���������־
	__IO uint8_t TouchKey1DuanAn:1;     //��������key1 �̰�
	__IO uint8_t TouchKey2DuanAn:1;     //��������key2 �̰�
	//__IO uint8_t ChannelOnceSwitch:1;	//ͨ���������ͻ����ع�
	
		 uint8_t Keyflag:1;				//��е�������̰���־
		 uint8_t Key1DuanAn:1;			//��е����Key1�̰�
		 uint8_t Key1ChangAn:1;			//��е����Key1����
		 uint8_t Powerdowncountflag:1;	//������ӵ��־
         uint8_t Low_Voltage:1;     	//��ص͵�ѹ��־
		 uint8_t BatLowShan:1;     	    //��ص͵�ѹ��˸
	
         uint8_t BatteryFull:1;     	//��������־
	     uint8_t BatChargeFull:1;       //��س������־
		 uint8_t BatCharging:1;			//������ڳ���б�־��1���ڳ�� 0δ���
         uint8_t ExPwOn:1;				//��ӵ�Դ��־
		 uint8_t ExPwShan:1; 			//��ط�����˸��־
	     uint8_t ExPwFirstDown:1;		//��ӵ��״ζϵ�
	
         uint8_t IsDisplayRightNow:1;   //LCD��ʾ,��һ�β�����ɺ���ʾ����������
         uint8_t StartSample:1;         //��ʼ����
         uint8_t EndSample:1;           //��������
         uint8_t MucReset:1;            //�ϵ縴λ����
         uint8_t RtcShan:1;             //ʱ�� : ��˸
	
		 uint8_t RecTimeDingShiBoot:1;	//��¼��ʱ��㶨ʱ����
		 uint8_t RecTimeDingDianBoot:1; //��¼��ʱ��㶨������
		 uint8_t RecTimeDingDianStop:1; //��¼��ʱ��㶨��ֹͣ
		 
		 uint8_t SensorTypeIsChange:1;  //ͨ��������δ�ı�
		 //uint8_t FirstSampleOkAlarm:1;	//��һ�β�����ɺ�
		 
		 uint8_t AlarmHuiFu[32];		//���������ָ���־-------�����Ż�Ϊ4���ֽڣ�ÿ����־ռһ��bit
		 		 
};
//! \brief ��Դ����
struct PowerManagementStr
{
		 uint8_t  BatChargeFullCount;	   //��س����������
	     uint8_t  HaveExternalPower;	   //������ӵ������
	     uint8_t  JinDuCounts;			   //��ʾ���һ��һ����ǰ
	
	__IO uint16_t BatVoltage_TestTime;     //�೤ʱ����,��ص�ѹ���ʱ����	
	__IO uint16_t BatADC_ConvertedValue;   //��ŵ��ADֵ
	
         uint16_t BatVoltage;              //���ʵ�ʵ���
};

//! \brief ��¼�ǹ���״̬�Ȳ���
struct JLYPARAMETERStr
{
    uint8_t  WorkStatueIsStop:1; 	//��¼�ǹ���״̬��0ֹͣ����
    uint8_t  LowMode:1;          	//����ģʽ��1�͹��ģ�0�������� 
    uint8_t  LastErrorCode:1;		//������ 
	uint8_t  ShowOffCode;			//������ʽ ,ֹͣ��ʽ ����������ʾ 
	
    __IO uint8_t  WakeUpSource;		//StopMode�»���Դ
	__IO uint8_t  WakeUpCount;			//StopMode�»���ʱ��
	
	uint8_t  FirstEnterStopModeCount;//�ϵ��һ�ν���͹���ʱ��
	uint8_t  LcdBackLightCount;		//Lcd���೤ʱ�����
	uint8_t  ChannelNumActual;		//ʵ��ͨ��������32��ͨ����ʹ���벻ʹ��֮�������,
	uint8_t  ChannelNumActualOld;   //ʵ��ͨ����������,Conf.Jly.ChannelNum���䣬��ͨ������
	uint8_t  ChannelNumOld;			//δ��������֮ǰ��ͨ����
	uint8_t  SensorTypeOld[32];		//δ��������֮ǰ��ͨ������
	uint8_t  ChannelSwitchOld[32];  //δ��������֮ǰ��ͨ��ʹ��λ
	
	uint16_t NormalRecIntervalMin;	//������¼��� ��λ��min 
	
	
	uint32_t SampleInterval;    	//�ɼ�ʱ���� ��λ��s
    uint32_t SampleTime;			//�ɼ�ʱ�� ��λ��s
    uint32_t NormalRecInterval;  	//������¼��� ��λ��s
    //uint32_t SaveHisDataTime;       //�洢���
	int32_t delay_start_time;		// ��ʱ����ʱ�� 
	
};
//����
struct KEYStr
{
	uint8_t DuanAnCount;	//��е����key1�̰�����
	uint8_t ChangAnCount;	//��е����key1��������
	uint8_t KeyNum;			//����ʶ����
};
//----------------------------
extern uint8_t      rtc_pt;
extern uint8_t      display_ct;
extern uint8_t 	    Key1ChangAnCount;
extern uint8_t  	DataBuf[HIS_ONE_MAX_BYTES+Headend_BYTES+ID_BYTES];
extern uint16_t     adc[32];
extern uint16_t     adcCopy[32];
extern uint16_t	    MsCount;
extern uint32_t SYS_CLK;

extern struct 		CircularQueueStr   Queue;
extern struct 		FLAGStr			   Flag;
extern struct       RTCRX8025Str       Rtc;
extern struct       PowerManagementStr PManage;
extern struct       JLYPARAMETERStr    JlyParam;
extern struct 		KEYStr			   Key;
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

