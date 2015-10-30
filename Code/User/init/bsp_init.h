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
/*Һ��������Ƶ�Դ�˿�*/
#define LcdVccCtrl_CLK	  RCC_AHBPeriph_GPIOF
#define LcdVccCtrl_PORT   GPIOF	
#define LcdVccCtrl_PIN    GPIO_Pin_2
/*�����������Ƶ�Դ�˿�*/
#define TouchVccCtrl_PORT GPIOF
#define TouchVccCtrl_PIN  GPIO_Pin_3
/************************************************/

/**********************GPIOC*********************/
/*��������*/
#define TouchKey_CLK	  RCC_AHBPeriph_GPIOC
#define TouchKey_PORT	  GPIOC
#define TouchKey1_PIN	  GPIO_Pin_15
#define TouchKey2_PIN	  GPIO_Pin_14
#define TouchKey3_PIN	  GPIO_Pin_13
/***********************************************/
/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_12);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_12)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_11);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_11)

//��������Դ����
#define AVCC1_POWER(a)	if (a)	\
					GPIO_ResetBits(GPIOE,GPIO_Pin_5);\
					else		\
					GPIO_SetBits(GPIOE,GPIO_Pin_5)					
//��ص�ѹ��⿪��
#define BATTEST_POWER(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_4)
//����������Դ���� 0�� 1�ر�
#define TOUCHKEY_POWER(a)	if (a)	\
					GPIO_ResetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN);\
					else		\
					GPIO_SetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN)					
//model ����ӿ�
#define MODEL_PWRCTRL(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_0)					
// ���������� 
#define BEEP(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_15);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_15)
//Һ��������ƿ���
#define LcdBackLight(a)	if (a)	\
					GPIO_SetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN);\
					else		\
					GPIO_ResetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN)				
					
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRH=i;}			//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_12)
#define LED1_ON			digitalHi(GPIOF,GPIO_Pin_12)
#define LED1_OFF		digitalLo(GPIOF,GPIO_Pin_12)

#define AlarmLed2_TOGGLE	digitalToggle(GPIOF,GPIO_Pin_11)
#define AlarmLed2_ON		digitalHi(GPIOF,GPIO_Pin_11)
#define AlarmLed2_OFF		digitalLo(GPIOF,GPIO_Pin_11)

/* ������ */
#define BELL            BEEP(ON);Delay_ms(50);BEEP(OFF)
#define BELL_longer     BEEP(ON);Delay_ms(500);BEEP(OFF)

/* ��ȡ��ʱ����ʱ�� */
#define ReadDelayStartTime   Change3BytesTimeToLong(Conf.Jly.MixBoot_Hour ,Conf.Jly.MixBoot_Min,0)

/*��ȡ������¼���ʱ��*/
#define ReadNormalRecIntervalTime  GetRecIntervalTime(Conf.Jly.NormalRec_Day,Conf.Jly.NormalRec_Hour,Conf.Jly.NormalRec_Min,Conf.Jly.NormalRec_Sec)


void SysInit(void);
void PeripheralInit(void);

#endif
