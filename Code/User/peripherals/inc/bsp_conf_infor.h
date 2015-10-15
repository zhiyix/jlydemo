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
#ifndef __CONFINFOR_H
#define __CONFINFOR_H

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
//! \brief ����������
#define NULL        0   /*!< δ���봫���� */
#define SENSOR_TEMP 1   /*!< �¶ȴ����� */
#define SENSOR_HUMI 2   /*!< ʪ�ȴ����� */
//PT
//SHT
//��������������
//! @}

/*============================ TYPES =========================================*/
//! \brief�����ȸ�����
union FISI2CH4
{
    float     ft;
    int       wd;
    short int hwd[2];
    char      byte[4];
};
//! \brief�����������ݵ�ַ�� (Display="Hex",ADDRESS_OFFSET=0x0000):
union BasicConfDataTable
{
	struct BasicConfDataStr
	{
		uint8_t Sn[6];	/*!< ���к� SN�� */
		uint16_t :16;
		
		uint16_t SoftVerml;		/*!< ����汾 MID LOW */
		uint16_t SoftVermain:8;	/*!< ����汾 Main */
		uint8_t	 :8;			/*!< ���� */
		
		uint16_t HardVerml;		/*!< Ӳ���汾 MID LOW */
		uint16_t HardVermain:8;	/*!< Ӳ���汾 Main */
		uint8_t	 :8;			/*!< ���� */
		
		uint16_t CommuMode;		/*!< ͨѶ��ʽ */
		/*�󱸵��״̬˵��
		 *@1Bit[1:0]:���״̬
		 *@2Bit[6:4]:��ص���״̬
		 */
		uint16_t BatteryState;	/*!< �󱸵��״̬ */
		
		uint16_t :16;			/*!< Reserv */
		uint16_t :16;
		uint16_t :16;
		uint16_t :16;
		uint16_t :16;
		uint16_t :16;
		//...
	} BasicConfData;
	
	uint8_t BasicConfBuf[256];
} Basic;
//! \brief��¼���������ݵ�ַ��(Display="Hex" ,ADDRESS_OFFSET=0x0100):
union JlyConfDataTable
{
	struct JlyConfDataStr
	{
		uint8_t RecBootMode;   /*!< ������ʽ˵��
									@0x00:��ʱ����(Default)
									@0x01:ʱ��㶨ʱ����
									@0x02:ʱ��㶨����ͣ
									@0x10:�쳣�������� */
		uint8_t RecMode;	/*!< ��¼��ʽ˵��
								 @0x00:�Ƚ��ȳ�(Default)
								 @0x01:����ֹͣ
								 @0x02:����ֹͣ */  
		
		/*!< yyyy-MM-dd  hh-mm[BCD]
			 1 ��ʱ����������ʱ���
			 2 ��ʱ��������ʱʱ��(Ĭ��0������������)
			 3 ʱ��㶨����ͣ������ʱ��� */
		uint8_t MixMoot_Min;      /*!< ����ʱ��-�� */ 
		uint8_t MixMoot_Hour;     /*!< ����ʱ��-ʱ */ 
		uint8_t MixMoot_Day;      /*!< ����ʱ��-�� */ 
		uint8_t MixMoot_Month;    /*!< ����ʱ��-�� */ 
		uint8_t MixMoot_Year;     /*!< ����ʱ��-�� */ 
		uint8_t MixMoot_Cen;      /*!< ����ʱ��- */ 
		/*!< ����ֹͣʱ�� yyyy-MM-dd hh-mm[BCD] */ 
		uint8_t FixedStop_Min;	
		uint8_t FixedStop_Hour;
		uint8_t FixedStop_Day;
		uint8_t FixedStop_Month;
		uint8_t FixedStop_Year;
		uint8_t FixedStop_Cen;
		
		/*!< ������¼��� dd-hh-mm[BCD]:ms */
		uint16_t NormalRecInterval;
		uint8_t  NormalRec_Sec;
		uint8_t  NormalRec_Min;
		uint8_t  NormalRec_Hour;
		uint8_t  NormalRec_Day;
		
		/*!< �쳣��¼��� dd-hh-mm[BCD]:ms */
		uint16_t ExceptionRecInterval;
		uint8_t  ExceptionRec_Sec;
		uint8_t  ExceptionRec_Min;
		uint8_t  ExceptionRec_Hour;
		uint8_t  ExceptionRec_Day;
		
		/*!< �ɼ���� ms */
		uint16_t SampleInterval;
		uint16_t :16;	/*!< Reserv */
		uint16_t :16;
		
		/*!< ����ʱ�ӵ�ʵʱʱ�� */
		uint8_t Time_Sec;
		uint8_t Time_Min;
		uint8_t Time_Week;
		uint8_t Time_Hour;
		uint8_t Time_Day;
		uint8_t Time_Month;
		uint8_t Time_Year;
		uint8_t Time_Cen;
		 
		uint8_t WorkStatueIsStop;  /*!< ��¼�ǹ���״̬
										0x00��ֹͣ��¼ 
										0x01�����ڼ�¼ */
		uint8_t PowerMode;         /*!< ��¼�ǹ���ģʽ 
										0x00������ģʽ 
										0x01���͹���ģʽ(ʡ��ģʽ) */								
		uint32_t StorageCapacity;  /*!< flash�洢���� */										 
		uint32_t StorageGroup;	   /*!< flash�洢��������(����) */
		//...
	} JlyConfData;
	
	uint8_t JlyConfBuf[256];
} Jly;
//! \brief�����������ݵ�ַ��(Display="Hex" ,ADDRESS_OFFSET=0x0200):
union AlarmConfDataTable
{
	struct AlarmConfDataStr
	{
		uint16_t AlarmSwitch;	/*!< ��������
									 Bit0����������
									 Bit1����ʾ����
									 Bit2�����ſ��� */
		uint8_t  AlarmTime_Mode;	/*!< ����ʱ��ģʽ
										 Bit0���ϰ�ʱ��
										 Bit1���°�ʱ�� */
		uint8_t  AlarmTime_Conf;	/*!< ����ʱ������
										 Bit0��Monday
										 Bit1��Tuesday
										 Bit2��Wednesday
										 Bit3��Thursday
										 Bit4��Friday
										 Bit5��Weekend */
		uint8_t  OnWork_Min;	/*!< hh-mm[BCD] �ϰ�ʱ��� */
		uint8_t  OnWork_Hour;
		
		uint8_t  OffWork_Min;	/*!< hh-mm[BCD] �°�ʱ��� */
		uint8_t  OffWork_Hour;
		uint16_t SoundLightAlarmDelay;	/*!< ���ⱨ����ʱ ��λ����*/
		uint16_t SmsAlarmDelay;			/*!< ���ű�����ʱ ��Χ��1s��18Сʱ��ѡ*/
		
		uint16_t :16;	/*!< Reserv */
		uint16_t :16;
		//...
	} AlarmConfData;
	
	uint8_t AlarmConfBuf[256];	/*!< ��С���������������������������������� */
} Alarm;
//! \brief������ͨ���������ݵ�ַ��(Display="Hex",ADDRESS_OFFSET=0x1000 + Sensor_Channel * 0x80,����Sensor_ChannelȡֵΪ0~31):
struct SensorChanelConfDataStr
{
	uint8_t  SensorType;	/*!< ����������
								 0x00��NULL
								 0x01���¶�
								 0x02��ʪ�� */
	uint8_t  AlarmSwitch;	/*!< �������� */
    uint16_t AlarmStatus;	/*!< ����״̬
								 Bit0�����ޱ���
								 Bit1�����ޱ��� 
								 Bit15�����ϱ��� */
	uint32_t SensorAddCode;	/*!< ��������ַ�� */
	
	union FISI2CH4 SensorAlarm_High;  /*!< �������� [IEEE-754_1�����Ƹ��������] 0x0000 0000*/ 
    union FISI2CH4 SensorAlarm_Low;   /*!< �������� [IEEE-754_1�����Ƹ��������]0x0000 0000*/
	//...
};
union xxxx 
{
	struct 
	{
	struct BasicConfDataStr basic;
	// ...
	struct AlarmConfDataStr alarm;
	struct SensorChanelConfDataStr sensor[32];
	// ...
	};
	uint8_t buf[3069];
} XXXXX;
//! \brief��ʪ�ȴ�����У׼�������ݵ�ַ��(Display="Hex",ADDRESS_OFFSET=0x2000 + Sensor_Channel * 0x80,����Sensor_ChannelȡֵΪ0~31):
struct TempHumiAdjustConfDataStr
{
	uint16_t DataPoint0;	/*!< Ĭ����ʾֵ��-80�� / 0%RH*/
	uint16_t DataPoint1;	/*!< Ĭ����ʾֵ��-70�� /10%RH*/
	uint16_t DataPoint2;	/*!< Ĭ����ʾֵ��-60�� /20%RH*/
	uint16_t DataPoint3;	/*!< Ĭ����ʾֵ��-50�� /30%RH*/
	uint16_t DataPoint4;	/*!< Ĭ����ʾֵ��-40�� /40%RH*/
	uint16_t DataPoint5;	/*!< Ĭ����ʾֵ��-30�� /50%RH*/
	uint16_t DataPoint6;	/*!< Ĭ����ʾֵ��-20�� /60%RH*/
	uint16_t DataPoint7;	/*!< Ĭ����ʾֵ��-10�� /70%RH*/
	uint16_t DataPoint8;	/*!< Ĭ����ʾֵ��  0�� /80%RH*/
	uint16_t DataPoint9;	/*!< Ĭ����ʾֵ�� 10�� /90%RH*/
	uint16_t DataPoint10;	/*!< Ĭ����ʾֵ�� 20�� /100%RH*/
	uint16_t DataPoint11;	/*!< Ĭ����ʾֵ�� 30�� */
	uint16_t DataPoint12;	/*!< Ĭ����ʾֵ�� 40�� */
	uint16_t DataPoint13;	/*!< Ĭ����ʾֵ�� 50�� */
	uint16_t DataPoint14;	/*!< Ĭ����ʾֵ�� 60�� */
	uint16_t DataPoint15;	/*!< Ĭ����ʾֵ�� 70�� */
	uint16_t DataPoint16;	/*!< Ĭ����ʾֵ�� 80�� */
	uint16_t DataPoint17;	/*!< Ĭ����ʾֵ�� 90�� */
	uint16_t DataPoint18;	/*!< Ĭ����ʾֵ��100�� */
	uint16_t DataPoint19;	/*!< Ĭ����ʾֵ��110�� */
	uint16_t DataPoint20;	/*!< Ĭ����ʾֵ��120�� */
	uint16_t DataPoint21;	/*!< Ĭ����ʾֵ��130�� */
	uint16_t DataPoint22;	/*!< Ĭ����ʾֵ��140�� */
	uint16_t DataPoint23;	/*!< Ĭ����ʾֵ��150�� */
};
//������ͨ����Ϣ�ṹ
struct SENSORINFOR
{
    uint8_t sensor_type;        //����������
    uint8_t alarm_switch;       //��������
    uint8_t alarm_status;       //����״̬
    uint8_t reserved;           //����
    uint32_t  idennum;          //������ʶ���
    union FISI2CH4 alarm_high;  //��������
    union FISI2CH4 alarm_low;   //��������
};
//������ͨ��1��Ϣ��
union SENSEPEIZHIINFOR
{
    struct  SENSORINFOR sensor1_str;
    uint8_t sensor_infor[16];
};

//adcУ׼���ݵ�
struct ADCADJUST
{
    uint16_t adjustf20;
    uint16_t adjustf10;
    uint16_t adjust10;
    uint16_t adjust20;
    uint16_t adjust30;
    uint16_t adjust40;
    uint16_t adjust50;
    uint16_t adjust60;
};
//adcУ׼���ݱ�
union ADCADJUSTINFOR
{
    struct ADCADJUST adc;
    uint8_t adcadjust_infor[16];
};
//ϵͳ��Ϣ�ṹ
struct SYSPEIZHI
{

    
    
    uint32_t    startalarm_chanel;    //��������ͨ��
    uint8_t     downlatestdata;       //������������ʹ��λ
    uint32_t    startsample_chanel;   //�����ܵĲ���ͨ�� 
};
//�õ���ȫ�ֱ���
struct JLY
{
    uint32_t    save_time;           //��������ʱ��
    uint32_t    delayboot_time;      //��ʱ����ʱ��
    
};
extern union  SENSEPEIZHIINFOR  Sensor1;
extern union  ADCADJUSTINFOR    adcjust1;;
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/


//#ifdef __cplusplus
//}
//#endif

#endif /* __CONFINFOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
