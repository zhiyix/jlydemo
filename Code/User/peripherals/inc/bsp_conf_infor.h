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
//! \brief 传感器类型
#define NULL        0   /*!< 未接入传感器 */
#define SENSOR_TEMP 1   /*!< 温度传感器 */
#define SENSOR_HUMI 2   /*!< 湿度传感器 */
//PT
//SHT
//其他传感器类型
//! @}

/*============================ TYPES =========================================*/
//! \brief单精度浮点型
union FISI2CH4
{
    float     ft;
    int       wd;
    short int hwd[2];
    char      byte[4];
};
//! \brief基本配置数据地址表 1 (Display="Hex",ADDRESS_OFFSET=0x0000):
/*!< 总共64个byte */
struct BasicConfDataStr
{
	uint8_t  Sn[6];	/*!< 序列号 SN号 */
	uint16_t :16;	/*!< 保留 */
	
	uint16_t SoftVerml;		/*!< 软件版本 MID LOW */
	uint8_t  SoftVermain;	/*!< 软件版本 Main */
	uint8_t	 :8;		/*!< 保留 */
	
	uint16_t HardVerml;		/*!< 硬件版本 MID LOW */
	uint8_t  HardVermain;	/*!< 硬件版本 Main */
	uint8_t	 :8;		/*!< 保留 */
	
	uint16_t CommuMode;		/*!< 通讯方式 */
	/*后备电池状态说明
	 *@1Bit[1:0]:充电状态
	 *@2Bit[6:4]:电池电量状态
	 */
	uint16_t BatteryState;	/*!< 后备电池状态 */
	
	uint8_t Reserv[44];		/*!< Reserv */
};
//! \brief记录仪配置数据地址表 2 (Display="Hex" ,ADDRESS_OFFSET=0x0100):
/*!< 总共64个byte */
struct JlyConfDataStr
{
	uint8_t RecBootMode;   /*!< 启动方式说明
								@0x00:延时启动(Default)
								@0x01:时间点定时启动
								@0x02:时间点定点启停
								@0x10:异常条件启动 */
	uint8_t RecStopMode;	/*!< 记录方式说明
							 @0x00:先进先出(Default)
							 @0x01:计满停止
							 @0x02:立即停止 */  
	
	/*!< yyyy-MM-dd  hh-mm[BCD]
		 1 定时启动的启动时间点
		 2 延时启动的延时时间(默认0，即立即启动)
		 3 时间点定点启停的启动时间点 */
	uint8_t MixMoot_Min;      /*!< 启动时间-分 */ 
	uint8_t MixMoot_Hour;     /*!< 启动时间-时 */ 
	uint8_t MixMoot_Day;      /*!< 启动时间-日 */ 
	uint8_t MixMoot_Month;    /*!< 启动时间-月 */ 
	uint8_t MixMoot_Year;     /*!< 启动时间-年 */ 
	uint8_t MixMoot_Cen;      /*!< 启动时间- */ 
							  /*!< 8个byte */
	/*!< 定点停止时间 yyyy-MM-dd hh-mm[BCD] */ 
	uint8_t FixedStop_Min;	
	uint8_t FixedStop_Hour;
	uint8_t FixedStop_Day;
	uint8_t FixedStop_Month;
	uint8_t FixedStop_Year;
	uint8_t FixedStop_Cen; /*!< 14个byte */
	
	uint16_t :16;	/*!< Reserv */
	/*!< 正常记录间隔 dd-hh-mm[BCD]:ms */
	uint16_t NormalRecInterval;
	uint8_t  NormalRec_Sec;
	uint8_t  NormalRec_Min;
	uint8_t  NormalRec_Hour;
	uint8_t  NormalRec_Day;
	
	/*!< 异常记录间隔 dd-hh-mm[BCD]:ms */
	uint16_t ExceptionRecInterval;
	uint8_t  ExceptionRec_Sec;
	uint8_t  ExceptionRec_Min;
	uint8_t  ExceptionRec_Hour;
	uint8_t  ExceptionRec_Day;
	
	/*!< 采集间隔 ms */
	uint16_t SampleInterval;
	uint16_t :16;			/*!< Reserv */
	uint16_t :16;			/*!< 34个byte */
	/*!< 仪器时钟的实时时间 */
	uint8_t Time_Sec;
	uint8_t Time_Min;
	uint8_t Time_Week;
	uint8_t Time_Hour;
	uint8_t Time_Day;
	uint8_t Time_Month;
	uint8_t Time_Year;
	uint8_t Time_Cen;
	 
	uint8_t WorkStatueIsStop;  /*!< 记录仪工作状态
									0x00：停止记录 
									0x01：正在记录 */
	uint8_t PowerMode;         /*!< 记录仪功耗模式 
									0x00：正常模式 
									0x01：低功耗模式(省电模式) */
    /*!< 44个byte */								
	uint32_t StorageCapacity;  /*!< flash存储容量 */										 
	uint32_t StorageGroup;	   /*!< flash存储数据组数(条数) */
	
	uint8_t	 ChannelNum;	   /*!< 通道数 */
	uint8_t	 Reserv[11];
};
//! \brief报警配置数据地址表 3 (Display="Hex" ,ADDRESS_OFFSET=0x0200):
/*!< 总共32个byte */	
struct AlarmConfDataStr
{
	uint16_t AlarmSwitch;	/*!< 报警开关
								 Bit0：声音开关
								 Bit1：显示开关
								 Bit2：短信开关 */
	uint8_t  AlarmTime_Mode;	/*!< 报警时间模式
									 Bit0：上班时间
									 Bit1：下班时间 */
	uint8_t  AlarmTime_Conf;	/*!< 报警时间配置
									 Bit0：Monday
									 Bit1：Tuesday
									 Bit2：Wednesday
									 Bit3：Thursday
									 Bit4：Friday
									 Bit5：Weekend */
	uint8_t  OnWork_Min;	/*!< hh-mm[BCD] 上班时间点 */
	uint8_t  OnWork_Hour;
	
	uint8_t  OffWork_Min;	/*!< hh-mm[BCD] 下班时间点 */
	uint8_t  OffWork_Hour;
	uint16_t SoundLightAlarmDelay;	/*!< 声光报警延时 单位秒钟*/
	uint16_t SmsAlarmDelay;			/*!< 短信报警延时 范围从1s到18小时可选*/
	
	uint8_t	 Reserv[20];	/*!< Reserv */	
};
//! \brief传感器通道配置数据地址表 4 (Display="Hex",ADDRESS_OFFSET=0x1000 + Sensor_Channel * 0x80,其中Sensor_Channel取值为0~31):
/*!< 总共32个byte */	
struct SensorChanelConfDataStr
{
	uint8_t  SensorType;	/*!< 传感器类型
								 0x00：NULL
								 0x01：温度
								 0x02：湿度 */
	uint8_t  SensorInterfaceType;	/*!< 传感器接口类型 
								 0x00：模拟
								 0x01：数字*/
	uint8_t  :8;			/*!< Reserv */
	uint8_t  AlarmSwitch;	/*!< 报警开关 */
    uint16_t AlarmStatus;	/*!< 报警状态
								 Bit0：上限报警
								 Bit1：下限报警 
								 Bit15：故障报警 */
	uint16_t :16;	/*!< Reserv */
	uint32_t SensorAddCode;	/*!< 传感器地址码 */
	
	union FISI2CH4 SensorAlarm_High;  /*!< 报警上限 [IEEE-754_1二进制浮点操作数] 0x0000 0000*/ 
    union FISI2CH4 SensorAlarm_Low;   /*!< 报警下限 [IEEE-754_1二进制浮点操作数]0x0000 0000*/
	uint8_t Reserv[12];	/*!< Reserv */
};
//! \brief温湿度传感器校准配置数据地址表 5 (Display="Hex",ADDRESS_OFFSET=0x2000 + Sensor_Channel * 0x80,其中Sensor_Channel取值为0~31):

/*!< 总共64个byte */
struct TempHumiAdjustConfDataStr
{
	uint16_t DataPoint0;	/*!< 默认显示值：-80℃ / 0%RH*/
	uint16_t DataPoint1;	/*!< 默认显示值：-70℃ /10%RH*/
	uint16_t DataPoint2;	/*!< 默认显示值：-60℃ /20%RH*/
	uint16_t DataPoint3;	/*!< 默认显示值：-50℃ /30%RH*/
	uint16_t DataPoint4;	/*!< 默认显示值：-40℃ /40%RH*/
	uint16_t DataPoint5;	/*!< 默认显示值：-30℃ /50%RH*/
	uint16_t DataPoint6;	/*!< 默认显示值：-20℃ /60%RH*/
	uint16_t DataPoint7;	/*!< 默认显示值：-10℃ /70%RH*/
	uint16_t DataPoint8;	/*!< 默认显示值：  0℃ /80%RH*/
	uint16_t DataPoint9;	/*!< 默认显示值： 10℃ /90%RH*/
	uint16_t DataPoint10;	/*!< 默认显示值： 20℃ /100%RH*/
	uint16_t DataPoint11;	/*!< 默认显示值： 30℃ */
	uint16_t DataPoint12;	/*!< 默认显示值： 40℃ */
	uint16_t DataPoint13;	/*!< 默认显示值： 50℃ */
	uint16_t DataPoint14;	/*!< 默认显示值： 60℃ */
	uint16_t DataPoint15;	/*!< 默认显示值： 70℃ */
	uint16_t DataPoint16;	/*!< 默认显示值： 80℃ */
	uint16_t DataPoint17;	/*!< 默认显示值： 90℃ */
	uint16_t DataPoint18;	/*!< 默认显示值：100℃ */
	uint16_t DataPoint19;	/*!< 默认显示值：110℃ */
	uint16_t DataPoint20;	/*!< 默认显示值：120℃ */
	uint16_t DataPoint21;	/*!< 默认显示值：130℃ */
	uint16_t DataPoint22;	/*!< 默认显示值：140℃ */
	uint16_t DataPoint23;	/*!< 默认显示值：150℃ */
	uint8_t Reserv[16];	/*!< Reserv */
};
//! \brief fram flash存储指针结构
struct CircularQueue
{
    uint16_t RecorderPoint;     //fram中记录数据指针
    uint32_t RecorderFlashPoint;//flash中记录数据指针
    uint32_t FlashSectorPoint;	//flash中扇区指针
};
//! \brief 配置表
/*!< 基本配置数据地址表   1 (Display="Hex",ADDRESS_OFFSET=0x0000):
	 记录仪配置数据地址表 2 (Display="Hex" ,ADDRESS_OFFSET=0x0100):
	 报警配置数据地址表   3 (Display="Hex" ,ADDRESS_OFFSET=0x0200):
	 传感器通道配置数据地址表       4 (Display="Hex",ADDRESS_OFFSET=0x1000 + Sensor_Channel * 0x80,其中Sensor_Channel取值为0~31):
	 温湿度传感器校准配置数据地址表 5 (Display="Hex",ADDRESS_OFFSET=0x2000 + Sensor_Channel * 0x80,其中Sensor_Channel取值为0~31):  
	 表 1 在Fram中 起始地址0x0000，结束地址(0x0040-1) 大小 64byte
     表 2 在Fram中 起始地址0x0040，结束地址(0x0080-1) 大小 64byte
     表 3 在Fram中 起始地址0x0080，结束地址(0x00A0-1) 大小 32byte
	 表 4 在Fram中 起始地址0x00A0，结束地址(0x04A0-1) 大小 1024byte,AADDRESS_OFFSET=0x00A0 + Sensor_Channel * 0x20,其中Sensor_Channel取值为0~31):
	 表 5 在Fram中 起始地址0x04A0，结束地址(0x0CA0-1) 大小 2048byte,AADDRESS_OFFSET=0x04A0 + Sensor_Channel * 0x40,其中Sensor_Channel取值为0~31):*/

union ConfDataTable 
{
	#pragma anon_unions
	struct 
	{
		struct BasicConfDataStr Basic;	/*!< 总共64个byte */
		struct JlyConfDataStr Jly;		/*!< 总共64个byte */
		struct AlarmConfDataStr Alarm;	/*!< 总共32个byte */	
		struct SensorChanelConfDataStr Sensor[32];		/*!< 每一通道总共32个byte 32*32=1024*/	
		struct TempHumiAdjustConfDataStr Adjust[32];	/*!< 每一通道总共64个byte 64*32=2048*/
	};
	uint8_t Buf[3232];
};

//传感器通道信息结构
struct SENSORINFOR
{
    uint8_t sensor_type;        //传感器类型
    uint8_t alarm_switch;       //报警开关
    uint8_t alarm_status;       //报警状态
    uint8_t reserved;           //保留
    uint32_t  idennum;          //传感器识别号
    union FISI2CH4 alarm_high;  //报警上限
    union FISI2CH4 alarm_low;   //报警下限
};
//传感器通道1信息表
union SENSEPEIZHIINFOR
{
    struct  SENSORINFOR sensor1_str;
    uint8_t sensor_infor[16];
};

//adc校准数据点
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
//adc校准数据表
union ADCADJUSTINFOR
{
    struct ADCADJUST adc;
    uint8_t adcadjust_infor[16];
};
//系统信息结构
struct SYSPEIZHI
{

    
    
    uint32_t    startalarm_chanel;    //启动报警通道
    uint8_t     downlatestdata;       //下载最新数据使能位
    uint32_t    startsample_chanel;   //启动总的采样通道 
};

extern union  SENSEPEIZHIINFOR  Sensor1;
extern union  ADCADJUSTINFOR    adcjust1;
extern union  ConfDataTable     Conf;
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/


//#ifdef __cplusplus
//}
//#endif

#endif /* __CONFINFOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
