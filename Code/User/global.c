/**
  ******************************************************************************
  * @file    global.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ȫ�ֱ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:
  *
  ******************************************************************************
  */

#include "main.h"

struct BATTERY        Bat;
struct FLAG			  Flag;
struct RTCRX8025      Rtc;
struct CircularQueue  Queue;
struct JLYPARAMETER   JlyParam;

struct BasicConfDataStr Basic;	/*!< �ܹ�64��byte */
struct JlyConfDataStr 	Jly;	/*!< �ܹ�64��byte */
struct AlarmConfDataStr Alarm;	/*!< �ܹ�32��byte */	
struct SensorChanelConfDataStr Sensor[32];		/*!< ÿһͨ���ܹ�32��byte 32*32=1024*/	
struct TempHumiAdjustConfDataStr Adjust[32];	/*!< ÿһͨ���ܹ�64��byte 64*32=2048*/

union  SENSEPEIZHIINFOR  Sensor1;
union  ADCADJUSTINFOR    adcjust1;
union ConfDataTable 	 Conf;

uint8_t        rtc_pt;          //�����
uint8_t        display_ct = 5;  //��ʾ
uint8_t 	   uAddress ;
uint8_t        ChannelForDisplay;   //��ʾ����
uint8_t        StartedChannelForDisplay;    //��ʾ������ͨ��
uint8_t        Started_Channel =3;
uint8_t  	   DataBuf[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES]={0};

uint16_t	   MsCount; //ϵͳ�δ����

uint16_t       testcount;

uint16_t       adc[32];
uint16_t       adcCopy[32];

__IO uint16_t  ADC_ConvertedValue[32];
__IO uint8_t   FlagSeniorErr[32];
float          ChannelDataFloat[32];

//���Ա�
const uint32_t lcd_test[] = 
{
	0x01,
	0x02,
	0x04,
	0x08,
	
	0x10,
	0x20,
	0x40,
	0x80,
	
	0x100,
	0x200,
	0x400,
	0x800,
	
	0x1000,
	0x2000,
	0x4000,
	0x8000,
	
	0x10000,
	0x20000,
	0x40000,
	0x80000,
	
	0x100000,
	0x200000,
	0x400000,
	0x800000,
	
	0x1000000,
	0x2000000,
	0x4000000,
	0x8000000,
	
	0x10000000,
	0x20000000,
	0x40000000,
	0x80000000,
};
//---------------------------------------------
const unsigned char RESET_ADJUST_CURVE1[] =
{    	
	//�¶�(wendu )-40~100
	0x00,0xB4,0x00,0xF8,0x01,0x9B,0x02,0x85,
	0x03,0xCA,0x05,0x55,0x07,0x12,0x08,0xCF,
	0x0A,0x71,0x0B,0xCD,0x0C,0xD5,0x0D,0xA7,
	0x0E,0x40,0x0E,0xB2,0x0E,0xFB,          //AD

};

const unsigned char RESET_ADJUST_CURVE2[] =
{
	//ʪ��
	0x00,0x00,0x00,0x64,0x00,0xC8,0x01,0x2C,
	0x01,0x90,0x01,0xF4,0x02,0x58,0x02,0xBC,
	0x03,0x20,0x03,0x84,0x03,0xE8,          //AD
						   		
};
const char RESET_CHANNEL_SETUP_TABLE[] =
{
  0xa8,0x00,0x00,0x00, //�������޶�Ӧ�����ֽڣ����㹫ʽvalue=low*(10��high�η�)
					   //����������100��ʾΪ 0x64 0x00  300��ʾΪ0x1e 0x01
                       //�¶�-40  ʪ��0
  0x64,0x00,0x64,0x00, //��������
                       //�¶�100 =0x64 0x00  ʪ��100=0x64  0x00

};                                        
const unsigned char *AdjustCurveFirAddress[]=
{
    RESET_ADJUST_CURVE1,
	RESET_ADJUST_CURVE2,
//    adcjust1.adcadjust_infor,
};

//! \brief ������Ϣ���ַӳ��
const uint32_t ConfMap_Address[5][2]=
{
	{VirtBasicConfAddr, 		 FRAM_BasicConfAddr},
	{VirtJlyConfAddr, 			 FRAM_JlyConfAddr},
	{VirtAlarmConfAddr, 		 FRAM_AlarmConfAddr},
	{VirtSensorChanelConfAddr, 	 FRAM_SensorChanelConfAddr},
	{VirtTempHumiAdjustConfAddr, FRAM_TempHumiAdjustConfAddr},
};
