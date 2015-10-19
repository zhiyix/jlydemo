#ifndef __INIT_H
#define __INIT_H

#include "stm32l1xx.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  1
#define OFF 0

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
//model ����ӿ�
#define MODEL_PWRCTRL(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_0)					
/*!< ���������� */
#define BEEP(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_15);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_15)
					
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRH=i;}			//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_12)
#define LED1_ON				digitalHi(GPIOF,GPIO_Pin_12)
#define LED1_OFF			digitalLo(GPIOF,GPIO_Pin_12)

#define LED2_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_11)
#define LED2_ON				digitalHi(GPIOF,GPIO_Pin_11)
#define LED2_OFF			digitalLo(GPIOF,GPIO_Pin_11)

/*!< ������ */
#define BELL            BEEP(ON);Delay_ms(50);BEEP(OFF)
#define BELL_longer     BEEP(ON);Delay_ms(500);BEEP(OFF)

/*!< ��ȡ��ʱ����ʱ�� */
#define ReadDelayStartTime 	Change3BytesTimeToLong(Conf.Jly.MixMoot_Hour ,Conf.Jly.MixMoot_Min,0)
//

void SysInit(void);
void PeripheralInit(void);

#endif
