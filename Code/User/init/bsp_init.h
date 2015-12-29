#ifndef __INIT_H
#define __INIT_H

#include "stm32l1xx.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  * 0 - on
  */
#define ON  1
#define OFF 0
//-----------------------------------------------
//LED控制
#define Led_CLK			  RCC_AHBPeriph_GPIOH
#define Led_PORT   		  GPIOH	
#define Led_PIN           GPIO_Pin_2
//-----------------------------------------------
//BEEP
#define Beep_CLK		  RCC_AHBPeriph_GPIOE
#define Beep_PORT		  GPIOE
#define Beep_PIN		  GPIO_Pin_7

//机械按键 key
#define Key_CLK		  	  RCC_AHBPeriph_GPIOE
#define Key_PORT		  GPIOE
#define Key1_PIN		  GPIO_Pin_4
//-----------------------------------------------
//ADC电源控制
#define CntlAvcc_CLK	  RCC_AHBPeriph_GPIOC
#define CntlAvcc_PORT	  GPIOC
#define CntlAvcc_PIN	  GPIO_Pin_14
//-----------------------------------------------
//电池电压检测控制
#define CntlBatTest_CLK	  RCC_AHBPeriph_GPIOB
#define CntlBatTest_PORT  GPIOB
#define CntlBatTest_PIN	  GPIO_Pin_2
//-----------------------------------------------
//外接电
#define Power_Deal_CLK	  RCC_AHBPeriph_GPIOD
#define Power_Deal_PORT   GPIOD
//指示 有效输入电源 ACOK
#define Power_ACtest_PIN 	GPIO_Pin_0
//指示充电完成 CHGOK
#define Power_CHGtest_PIN 	GPIO_Pin_1

//液晶背光控制电源端口
#define LcdVccCtrl_CLK	  RCC_AHBPeriph_GPIOD
#define LcdVccCtrl_PORT   GPIOD
#define LcdVccCtrl_PIN    GPIO_Pin_7
//-----------------------------------------------
//看门狗 SYS_WDI
#define SYS_WDI_CLK	  	  RCC_AHBPeriph_GPIOE
#define SYS_WDI_PORT      GPIOE
#define SYS_WDI_PIN       GPIO_Pin_5

////触摸按键控制电源端口
//#define TouchVccCtrl_PORT 
//#define TouchVccCtrl_PIN  

////触摸按键
//#define TouchKey_CLK	  
//#define TouchKey_PORT	  
//#define TouchKey1_PIN	  
//#define TouchKey2_PIN	  
//#define TouchKey3_PIN	  
////HAC电源控制
//#define HacVccCtrl_CLK	  
//#define HacVccCtrl_PORT	  
//#define HacVccCtrl_PIN	  

/***********************************************/
/* 带参宏，可以像内联函数一样使用 */
#define LED(a)	if (a)	\
					GPIO_ResetBits(Led_PORT,Led_PIN);\
					else		\
					GPIO_SetBits(Led_PORT,Led_PIN)
// 蜂鸣器开关 
#define BEEP(a)	if (a)	\
					GPIO_SetBits(Beep_PORT,Beep_PIN);\
					else		\
					GPIO_ResetBits(Beep_PORT,Beep_PIN)
//传感器电源开关
#define AVCC1_POWER(a)	if (a)	\
					GPIO_ResetBits(CntlAvcc_PORT,CntlAvcc_PIN);\
					else		\
					GPIO_SetBits(CntlAvcc_PORT,CntlAvcc_PIN)					
//电池电压检测开关
#define BATTEST_POWER(a)	if (a)	\
					GPIO_SetBits(CntlBatTest_PORT,CntlBatTest_PIN);\
					else		\
					GPIO_ResetBits(CntlBatTest_PORT,CntlBatTest_PIN)
//液晶背光控制开关
#define LcdBackLight(a)	if (a)	\
					GPIO_ResetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN);\
					else		\
					GPIO_SetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN)	
					
					
					
					
//触摸按键电源开关 0打开 1关闭
#define TOUCHKEY_POWER(a)	if (a)	\
					GPIO_ResetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN);\
					else		\
					GPIO_SetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN)	
//HAC电源开关 0打开 1关闭
#define HAC_POWER(a)	if (a)	\
					GPIO_ResetBits(HacVccCtrl_PORT,HacVccCtrl_PIN);\
					else		\
					GPIO_SetBits(HacVccCtrl_PORT,HacVccCtrl_PIN)						
//model 对外接口
#define MODEL_PWRCTRL(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_0)					
			
					
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BSRRH=i;}			//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED_TOGGLE		digitalToggle(Led_PORT,Led_PIN)
#define LED_ON			digitalHi(Led_PORT,Led_PIN)
#define LED_OFF			digitalLo(Led_PORT,Led_PIN)


/* 蜂鸣器 */
#define BELL            BEEP(ON);Delay_ms(20);BEEP(OFF)
#define BELL_longer     BEEP(ON);Delay_ms(100);BEEP(OFF)

/* 读取延时启动时间 */
#define ReadDelayStartTime   Change3BytesTimeToLong(Conf.Jly.MixBoot_Hour ,Conf.Jly.MixBoot_Min,0)

/*读取正常记录间隔时间*/
#define ReadNormalRecIntervalTime  GetRecIntervalTime(Conf.Jly.NormalRec_Day,Conf.Jly.NormalRec_Hour,Conf.Jly.NormalRec_Min,Conf.Jly.NormalRec_Sec)

void SysClock_ReConfig(void);
void SysInit(void);
void PeripheralInit(void);
void OffPowerSupply(void);
void WriteSetFramFlag(void);
int16_t ReadSetFramFlag(void);

#endif
