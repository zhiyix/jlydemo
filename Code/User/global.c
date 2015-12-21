/**
  ******************************************************************************
  * @file    global.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   全局变量
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  *
  ******************************************************************************
  */

#include "main.h"

struct PowerManagementStr	PManage;
struct FLAGStr			  	Flag;
struct RTCRX8025Str      	Rtc;
struct CircularQueueStr  	Queue;
struct JLYPARAMETERStr   	JlyParam;
struct KEYStr			    Key;
struct JlySensorStr			JlySensor;

union ConfDataTable 	    Conf;

uint8_t        rtc_pt;          //秒计数
uint8_t        display_ct = 5;  //显示
uint8_t 	   uAddress ;
uint8_t 	   Key1ChangAnCount;	//机械按键长按检测计数
uint8_t        ChannelForDisplay;   //显示计数
uint8_t        StartedChannelForDisplay;    //显示启动的通道
uint8_t        Started_Channel;
uint8_t  	   DataBuf[HIS_ONE_MAX_BYTES]={0}; //32通道

//uint16_t	   MsCount; //系统滴答计数

uint16_t       adc[32];
uint16_t       adcCopy[32];

__IO uint16_t  ADC_ConvertedValue[32];
__IO uint8_t   FlagSeniorErr[32];
float          ChannelDataFloat[32];
uint32_t       SYS_CLK;

uint32_t 	   SysTickTestCount;
uint32_t       temptest;
//测试表
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
/*
const unsigned char RESET_ADJUST_CURVE1[100] =
 {0x0F,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,    	//温度(wendu )-40~100
  0x00,0xB4,0x00,0xF8,0x01,0x9B,0x02,0x85,
  0x03,0xCA,0x05,0x55,0x07,0x12,0x08,0xCF,
  0x0A,0x71,0x0B,0xCD,0x0C,0xD5,0x0D,0xA7,
  0x0E,0x40,0x0E,0xB2,0x0E,0xFB,              	//AD
  10,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,                                  	//DEITA
  0x0A,0x0A,0x0A,0x0A,
  0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,};

const unsigned char RESET_ADJUST_CURVE2[100] =
 {0x0B,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,	//湿度
    
  0x00,0x00,0x00,0x64,0x00,0xC8,0x01,0x2C,
  0x01,0x90,0x01,0xF4,0x02,0x58,0x02,0xBC,
  0x03,0x20,0x03,0x84,0x03,0xE8,            //AD

  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,                                //DEITA
						   		

  0x0A,0x0A,0x0A,0x0A,
  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,};

const unsigned char RESET_ADJUST_CURVE3[100] =
 {0x0F,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,    	//温度(wendu )-40~100
  0x00,0xB4,0x00,0xF8,0x01,0x9B,0x02,0x85,
  0x03,0xCA,0x05,0x55,0x07,0x12,0x08,0xCF,
  0x0A,0x71,0x0B,0xCD,0x0C,0xD5,0x0D,0xA7,
  0x0E,0x40,0x0E,0xB2,0x0E,0xFB,              	//AD
  10,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,                                  	//DEITA
  0x0A,0x0A,0x0A,0x0A,
  0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,};
 
const unsigned char RESET_ADJUST_CURVE4[100] =
 {0x0F,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,    	//温度(wendu )-40~100
  0x00,0xB4,0x00,0xF8,0x01,0x9B,0x02,0x85,
  0x03,0xCA,0x05,0x55,0x07,0x12,0x08,0xCF,
  0x0A,0x71,0x0B,0xCD,0x0C,0xD5,0x0D,0xA7,
  0x0E,0x40,0x0E,0xB2,0x0E,0xFB,              	//AD
  10,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,                                  	//DEITA
  0x0A,0x0A,0x0A,0x0A,
  0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,};

const char RESET_CHANNEL_SETUP_TABLE[104] =
 {0x01,0x02,0x01,0x01,0x00,0x00,0x00,0x00,  //???????????????0-7??????
                                            //A0=0x01??  A3=0x02??
 	
  0x01,0x02,0x01,0x01,0x00,0x00,0x00,0x00,  //????,?????????

  0x02,0x01,0x02,0x02,0x00,0x00,0x00,0x00,  //??????
                                            //??2?? ??1??

  0xa8,0x00,0x00,0x00,0xa8,0x00,0xa8,0x00,  //?????????????? ?????value=low*(10?high??)
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //?????100??? 0x64 0x00 300???0x1e 0x01
                                            //??-40  ??0


  0x64,0x00,0x64,0x00,0x64,0x00,0x64,0x00,  //????
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //????
                                            //??100 =0x64 0x00  ??100=0x64  0x00

  0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,  //??????8
  0x00,0x32,0x04,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //boot
  0x03,0x0E,0x55,0x55,0xFC,C_VER_YEAR,C_VER_MONTH,C_VER_DAY,  //No.1??????,???0-7???????0-??7,
  };                                        

const char NewLCDCtrlWord[32] =
{
  0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,  // ???? LCD?????????
                                            // 0x01??  0x08??

  0x81,0x81,0x81,0x81,0x00,0x00,0x00,0x00,  //?????,?????????
                                            //??0.1 ??0.1

  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //?????,??????????0-??7?????????
                                            //0x00????AVCC,0x01????1.5??????,0x02??2.5
                                            //??????
                                            // ??AVCC ??2.5??????
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
};


#define MSP430_Curver1_ADDRESS (unsigned char *)0x0001
#define MSP430_Curver2_ADDRESS (unsigned char *)0x0002
#define MSP430_Curver3_ADDRESS (unsigned char *)0x0003
#define MSP430_Curver4_ADDRESS (unsigned char *)0x0004
const unsigned char *AdjustCurveFirAddress[]=
{
	MSP430_Curver1_ADDRESS,MSP430_Curver2_ADDRESS,
    MSP430_Curver3_ADDRESS,MSP430_Curver4_ADDRESS,
    RESET_ADJUST_CURVE1,RESET_ADJUST_CURVE2,
    RESET_ADJUST_CURVE3,RESET_ADJUST_CURVE4,
};
*/

/* 温度(wendu )-40~100 */
const unsigned char RESET_ADJUST_CURVE1[] =
{    		
	0x00,0xB4,0x00,0xF8,0x01,0x9B,0x02,0x85,
	0x03,0xCA,0x05,0x55,0x07,0x12,0x08,0xCF,
	0x0A,0x71,0x0B,0xCD,0x0C,0xD5,0x0D,0xA7,
	0x0E,0x40,0x0E,0xB2,0x0E,0xFB,          //AD

};
/* 湿度 0-100*/
const unsigned char RESET_ADJUST_CURVE2[] =
{
	0x00,0x00,0x00,0x64,0x00,0xC8,0x01,0x2C,
	0x01,0x90,0x01,0xF4,0x02,0x58,0x02,0xBC,
	0x03,0x20,0x03,0x84,0x03,0xE8,          //AD
						   		
};

/*量程*/
const char RESET_CHANNEL_SETUP_TABLE[] =
{
  0xa8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //量程下限对应两个字节，计算公式value=low*(10的high次方)
											//量程下限如100表示为 0x64 0x00  300表示为0x1e 0x01
											//温度-40  湿度0
  0x64,0x00,0x64,0x00,0x00,0x00,0x00,0x00,  //量程上限
										    //温度100 =0x64 0x00  湿度100=0x64  0x00

};                                        
const unsigned char *AdjustCurveFirAddress[]=
{
    RESET_ADJUST_CURVE1,	//原始温度曲线
	RESET_ADJUST_CURVE2,	//原始湿度曲线
};

//! \brief 配置信息表地址映射
const uint32_t ConfMap_Address[6][2]=
{
	{VirtBasicConfAddr, 		 FRAM_BasicConfAddr},
	{VirtJlyConfAddr, 			 FRAM_JlyConfAddr},
	{VirJlyTimeConfAddr,		 FRAM_JlyTimeConfAddr},
	{VirtAlarmConfAddr, 		 FRAM_AlarmConfAddr},
	{VirtSensorChanelConfAddr, 	 FRAM_SensorChanelConfAddr},
	{VirtTempHumiAdjustConfAddr, FRAM_TempHumiAdjustConfAddr},
};
