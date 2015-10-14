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
#define NULL        0   //未接入传感器
#define SENSOR_TEMP 1   //温度传感器
#define SENSOR_HUMI 2   //湿度传感器
//PT
//SHT
//其他传感器类型
//! @}

/*============================ TYPES =========================================*/
//! \brief基本配置数据地址表 (Display="Hex",ADDRESS_OFFSET=0x0000):
struct BasicConfDataStr
{
	uint16_t sn_low;	//序列号 SN号
	uint16_t sn_mid;
	uint16_t sn_high;
	uint16_t :16;
	
	uint16_t soft_verml;	//软件版本 MID LOW
	uint8_t	 :8;			//保留
	uint16_t soft_vermain;	//软件版本 Main
	
	uint16_t hard_verml;	//硬件版本 MID LOW
	uint8_t	 :8;			//保留
	uint16_t hard_vermain;	//硬件版本 Main
	
	uint16_t commu_mode;	//通讯方式
	/*后备电池状态说明
	 *@1Bit[1:0]:充电状态
     *@2Bit[6:4]:电池电量状态
     */
	uint16_t battery_state;	//后备电池状态
	
	uint16_t :16;			//Reserv
	uint16_t :16;
	uint16_t :16;
	uint16_t :16;
	uint16_t :16;
	uint16_t :16;
};
//! \brief记录仪配置数据地址表(Display="Hex" ,ADDRESS_OFFSET=0x0100):
struct JlyConfDataStr
{
	/*启动方式说明
	 *@0x00:延时启动(Default)
	 *@0x01:时间点定时启动
	 *@0x02:时间点定点启停
	 *@0x10:异常条件启动
     */
    uint8_t recbootmode;  //启动方式
	/*记录方式说明
	 *@0x00:先进先出(Default)
	 *@0x01:计满停止
	 *@0x02:立即停止
     */
    uint8_t recmode;  //记录方式
	
	
};
//单精度浮点型
union FISI2CH4
{
    float     ft;
    int       wd;
    short int hwd[2];
    char      byte[4];
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
    uint8_t     :8;                 //工作状态
    uint16_t    rec_fram_memory;    //记录存储Fram大小
    uint32_t    rec_flash_memory;   //记录存储flash大小
    uint8_t     :8;                 //保留
    uint32_t    password:24;        //密码口令
    uint32_t    soft_ver;           //软件版本号
    uint32_t    hard_ver;           //硬件版本号
    uint8_t     :8;                 //保留
    
    uint8_t     username[16];       //用户名
    uint8_t     *title[11];         //标题
    uint8_t     *use_fun[5];        //记录仪用途
    
    uint16_t    recboot_year;       //记录启动时间-年
    uint8_t     recboot_month;      //记录启动时间-月
    uint8_t     recboot_day;        //记录启动时间-日
    uint8_t     recboot_hour;       //记录启动时间-时
    uint8_t     recboot_min;        //记录启动时间-分
    uint8_t     recboot_sec;        //记录启动时间-秒
    uint8_t     recgap_hour;        //记录间隔时间-时
    uint8_t     recgap_min;         //记录间隔时间-分
    uint8_t     recgap_sec;         //记录间隔时间-秒    
    
    
    uint8_t     delayboot_hour;     //延时启动时间-时
    uint8_t     delayboot_min;      //延时启动时间-分
    uint8_t     delayboot_sec;      //延时启动时间-秒
    
    
    uint16_t    recframaddr_low:8;     //fram中记录数据指针低位(2字节)
    uint16_t    recframaddr_high:8;    //fram中记录数据指针高位
    uint32_t    recflashaddr_low:8;    //flash中记录数据指针bit0(4字节)
    uint32_t    recflashaddr_mid0:8;   //flash中记录数据指针bit1
    uint32_t    recflashaddr_mid1:8;   //flash中记录数据指针bit2
    uint32_t    recflashaddr_high:8;   //flash中记录数据指针bit3
    
    /*记录仪工作状态说明
     *正在记录 01
     *停止记录 00
     */
    uint8_t     workstatueisstop;  //记录仪工作状态
    
    uint8_t     powermode;         //记录仪功耗模式 00正常模式 01低功耗模式(省电模式)
    
    uint8_t     remark1;           //备注
    uint32_t    remark2;           //备注
    uint32_t    :32;               //备用
    uint32_t    startalarm_chanel;    //启动报警通道
    uint8_t     downlatestdata;       //下载最新数据使能位
    uint32_t    startsample_chanel;   //启动总的采样通道
    
    uint8_t     ctrlword;             //控制字
    
    uint32_t    :32;                  //备用4字节
    uint32_t    :32;                  //备用4字节
    
    
};
//用到的全局变量
struct JLY
{
    uint32_t    save_time;           //保存数据时间
    uint32_t    delayboot_time;      //延时启动时间
    
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
