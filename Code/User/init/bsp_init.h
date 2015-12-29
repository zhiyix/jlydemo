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
//LED����
#define Led_CLK			  RCC_AHBPeriph_GPIOH
#define Led_PORT   		  GPIOH	
#define Led_PIN           GPIO_Pin_2
//-----------------------------------------------
//BEEP
#define Beep_CLK		  RCC_AHBPeriph_GPIOE
#define Beep_PORT		  GPIOE
#define Beep_PIN		  GPIO_Pin_7

//��е���� key
#define Key_CLK		  	  RCC_AHBPeriph_GPIOE
#define Key_PORT		  GPIOE
#define Key1_PIN		  GPIO_Pin_4
//-----------------------------------------------
//ADC��Դ����
#define CntlAvcc_CLK	  RCC_AHBPeriph_GPIOC
#define CntlAvcc_PORT	  GPIOC
#define CntlAvcc_PIN	  GPIO_Pin_14
//-----------------------------------------------
//��ص�ѹ������
#define CntlBatTest_CLK	  RCC_AHBPeriph_GPIOB
#define CntlBatTest_PORT  GPIOB
#define CntlBatTest_PIN	  GPIO_Pin_2
//-----------------------------------------------
//��ӵ�
#define Power_Deal_CLK	  RCC_AHBPeriph_GPIOD
#define Power_Deal_PORT   GPIOD
//ָʾ ��Ч�����Դ ACOK
#define Power_ACtest_PIN 	GPIO_Pin_0
//ָʾ������ CHGOK
#define Power_CHGtest_PIN 	GPIO_Pin_1

//Һ��������Ƶ�Դ�˿�
#define LcdVccCtrl_CLK	  RCC_AHBPeriph_GPIOD
#define LcdVccCtrl_PORT   GPIOD
#define LcdVccCtrl_PIN    GPIO_Pin_7
//-----------------------------------------------
//���Ź� SYS_WDI
#define SYS_WDI_CLK	  	  RCC_AHBPeriph_GPIOE
#define SYS_WDI_PORT      GPIOE
#define SYS_WDI_PIN       GPIO_Pin_5

////�����������Ƶ�Դ�˿�
//#define TouchVccCtrl_PORT 
//#define TouchVccCtrl_PIN  

////��������
//#define TouchKey_CLK	  
//#define TouchKey_PORT	  
//#define TouchKey1_PIN	  
//#define TouchKey2_PIN	  
//#define TouchKey3_PIN	  
////HAC��Դ����
//#define HacVccCtrl_CLK	  
//#define HacVccCtrl_PORT	  
//#define HacVccCtrl_PIN	  

/***********************************************/
/* ���κ꣬��������������һ��ʹ�� */
#define LED(a)	if (a)	\
					GPIO_ResetBits(Led_PORT,Led_PIN);\
					else		\
					GPIO_SetBits(Led_PORT,Led_PIN)
// ���������� 
#define BEEP(a)	if (a)	\
					GPIO_SetBits(Beep_PORT,Beep_PIN);\
					else		\
					GPIO_ResetBits(Beep_PORT,Beep_PIN)
//��������Դ����
#define AVCC1_POWER(a)	if (a)	\
					GPIO_ResetBits(CntlAvcc_PORT,CntlAvcc_PIN);\
					else		\
					GPIO_SetBits(CntlAvcc_PORT,CntlAvcc_PIN)					
//��ص�ѹ��⿪��
#define BATTEST_POWER(a)	if (a)	\
					GPIO_SetBits(CntlBatTest_PORT,CntlBatTest_PIN);\
					else		\
					GPIO_ResetBits(CntlBatTest_PORT,CntlBatTest_PIN)
//Һ��������ƿ���
#define LcdBackLight(a)	if (a)	\
					GPIO_ResetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN);\
					else		\
					GPIO_SetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN)	
					
					
					
					
//����������Դ���� 0�� 1�ر�
#define TOUCHKEY_POWER(a)	if (a)	\
					GPIO_ResetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN);\
					else		\
					GPIO_SetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN)	
//HAC��Դ���� 0�� 1�ر�
#define HAC_POWER(a)	if (a)	\
					GPIO_ResetBits(HacVccCtrl_PORT,HacVccCtrl_PIN);\
					else		\
					GPIO_SetBits(HacVccCtrl_PORT,HacVccCtrl_PIN)						
//model ����ӿ�
#define MODEL_PWRCTRL(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_0)					
			
					
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRH=i;}			//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define LED_TOGGLE		digitalToggle(Led_PORT,Led_PIN)
#define LED_ON			digitalHi(Led_PORT,Led_PIN)
#define LED_OFF			digitalLo(Led_PORT,Led_PIN)


/* ������ */
#define BELL            BEEP(ON);Delay_ms(20);BEEP(OFF)
#define BELL_longer     BEEP(ON);Delay_ms(100);BEEP(OFF)

/* ��ȡ��ʱ����ʱ�� */
#define ReadDelayStartTime   Change3BytesTimeToLong(Conf.Jly.MixBoot_Hour ,Conf.Jly.MixBoot_Min,0)

/*��ȡ������¼���ʱ��*/
#define ReadNormalRecIntervalTime  GetRecIntervalTime(Conf.Jly.NormalRec_Day,Conf.Jly.NormalRec_Hour,Conf.Jly.NormalRec_Min,Conf.Jly.NormalRec_Sec)

void SysClock_ReConfig(void);
void SysInit(void);
void PeripheralInit(void);
void OffPowerSupply(void);
void WriteSetFramFlag(void);
int16_t ReadSetFramFlag(void);

#endif
