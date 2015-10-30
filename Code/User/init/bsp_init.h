#ifndef __INIT_H
#define __INIT_H

#include "stm32l1xx.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  * 0 - on
  */
#define ON  1
#define OFF 0
/**********************GPIOF*********************/
/*液晶背光控制电源端口*/
#define LcdVccCtrl_CLK	  RCC_AHBPeriph_GPIOF
#define LcdVccCtrl_PORT   GPIOF	
#define LcdVccCtrl_PIN    GPIO_Pin_2
/*触摸按键控制电源端口*/
#define TouchVccCtrl_PORT GPIOF
#define TouchVccCtrl_PIN  GPIO_Pin_3
/************************************************/

/**********************GPIOC*********************/
/*触摸按键*/
#define TouchKey_CLK	  RCC_AHBPeriph_GPIOC
#define TouchKey_PORT	  GPIOC
#define TouchKey1_PIN	  GPIO_Pin_15
#define TouchKey2_PIN	  GPIO_Pin_14
#define TouchKey3_PIN	  GPIO_Pin_13
/***********************************************/
/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_12);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_12)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_11);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_11)

//传感器电源开关
#define AVCC1_POWER(a)	if (a)	\
					GPIO_ResetBits(GPIOE,GPIO_Pin_5);\
					else		\
					GPIO_SetBits(GPIOE,GPIO_Pin_5)					
//电池电压检测开关
#define BATTEST_POWER(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_4)
//触摸按键电源开关 0打开 1关闭
#define TOUCHKEY_POWER(a)	if (a)	\
					GPIO_ResetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN);\
					else		\
					GPIO_SetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN)					
//model 对外接口
#define MODEL_PWRCTRL(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_0)					
// 蜂鸣器开关 
#define BEEP(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_15);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_15)
//液晶背光控制开关
#define LcdBackLight(a)	if (a)	\
					GPIO_SetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN);\
					else		\
					GPIO_ResetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN)				
					
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BSRRH=i;}			//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_12)
#define LED1_ON			digitalHi(GPIOF,GPIO_Pin_12)
#define LED1_OFF		digitalLo(GPIOF,GPIO_Pin_12)

#define AlarmLed2_TOGGLE	digitalToggle(GPIOF,GPIO_Pin_11)
#define AlarmLed2_ON		digitalHi(GPIOF,GPIO_Pin_11)
#define AlarmLed2_OFF		digitalLo(GPIOF,GPIO_Pin_11)

/* 蜂鸣器 */
#define BELL            BEEP(ON);Delay_ms(50);BEEP(OFF)
#define BELL_longer     BEEP(ON);Delay_ms(500);BEEP(OFF)

/* 读取延时启动时间 */
#define ReadDelayStartTime   Change3BytesTimeToLong(Conf.Jly.MixBoot_Hour ,Conf.Jly.MixBoot_Min,0)

/*读取正常记录间隔时间*/
#define ReadNormalRecIntervalTime  GetRecIntervalTime(Conf.Jly.NormalRec_Day,Conf.Jly.NormalRec_Hour,Conf.Jly.NormalRec_Min,Conf.Jly.NormalRec_Sec)


void SysInit(void);
void PeripheralInit(void);

#endif
