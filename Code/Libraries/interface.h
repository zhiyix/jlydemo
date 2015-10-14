/**
  ******************************************************************************
  * @file              : interface.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 27/09/2014 17:41:57
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2014 STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdio.h>
#include "stm32glibc_type.h"
//! @}
	 
/*============================ MACROS ========================================*/
#ifdef USE_STM32100B_EVAL
#define MESSAGE1   "STM32 MD Value Line "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "  STM32100B-EVAL    "
#elif defined (USE_STM3210B_EVAL)
#define MESSAGE1   "STM32 Medium Density"
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "   STM3210B-EVAL    "
#elif defined (STM32F10X_XL) && defined (USE_STM3210E_EVAL)
#define MESSAGE1   "  STM32 XL Density  "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "   STM3210E-EVAL    "
#elif defined (USE_STM3210E_EVAL)
#define MESSAGE1   " STM32 High Density "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "   STM3210E-EVAL    "
#elif defined (USE_STM3210C_EVAL)
#define MESSAGE1   " STM32 Connectivity "
#define MESSAGE2   " Line Device running"
#define MESSAGE3   " on STM3210C-EVAL   "
#elif defined (USE_STM32100E_EVAL)
#define MESSAGE1   "STM32 HD Value Line "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "  STM32100E-EVAL    "
#endif



#if !defined(USE_STM3210E_EVAL)
//! Ctrl-Beep-LED
//#define BSP_BEEP_CH0			GPIO_Pin_12
//#define BEEPIO					IOPB
//#define BSP_LED_CH1				GPIO_Pin_6
//#define LEDIO1					IOPF
//#define BSP_LED_CH2				GPIO_Pin_7
//#define LEDIO2					IOPF
//#define BSP_LED_CH3				GPIO_Pin_8
//#define LEDIO3					IOPF
//#define BSP_LED_CH4				GPIO_Pin_9
//#define LEDIO4					IOPF
//#define BSP_LED_CH5				GPIO_Pin_10
//#define LEDIO5					IOPF
//! PWM
//#define BSP_GPIOB_CH3			GPIO_Pin_1
//#define BSP_GPIOB_CH4			GPIO_Pin_0
//#define BSP_GPIOF_PWM_GRP		BSP_GPIOB_CH3 | BSP_GPIOB_CH4
//! DEV-GSM
#define BSP_GSM_PWRCTRL			GPIO_Pin_1
#define GSMPWRCTRL				IOPB
//! COM1
#define BSP_COM1_TX				GPIO_Pin_9
#define BSP_COM1_RX				GPIO_Pin_10
#define COM1IO					IOPA
//!	GPIO_Remap_USART1			PB6 | PB7
#define COM1REMAP				GPIO_Remap_NULL
#define BSP_COM1_GRP			BSP_COM1_TX | BSP_COM1_RX
//! COM2
#define BSP_COM2_TX				GPIO_Pin_2
#define BSP_COM2_RX				GPIO_Pin_3
#define COM2IO					IOPA
//!	GPIO_Remap_USART2			PD5 | PD6
#define COM2REMAP				GPIO_Remap_NULL
#define BSP_COM2_GRP			BSP_COM2_TX | BSP_COM2_RX
//! COM3
#define BSP_COM3_TX				GPIO_Pin_10
#define BSP_COM3_RX				GPIO_Pin_11
#define COM3IO					IOPB
//!	GPIO_PartialRemap_USART3 	PC10 | PC11
//!	GPIO_FullRemap_USART3		PD8 | PD9
#define COM3REMAP				GPIO_Remap_NULL
#define BSP_COM3_GRP			BSP_COM3_TX | BSP_COM3_RX
//!*********************************************************
#else
//!*********************************************************
//! Ctrl-Beep-LED
//#define BSP_BEEP_CH0			GPIO_Pin_4
//#define BEEPIO					IOPC
//#define BSP_LED_CH1				GPIO_Pin_5
//#define LEDIO1					IOPC
//#define BSP_LED_CH2				GPIO_Pin_0
//#define LEDIO2					IOPB
//#define BSP_LED_CH3				GPIO_Pin_1
//#define LEDIO3					IOPB
//#define BSP_LED_CH4				GPIO_Pin_11
//#define LEDIO4					IOPE
//#define BSP_LED_CH5				GPIO_Pin_12
//#define LEDIO5					IOPE
////! PWM
//#define BSP_GPIOB_CH3			GPIO_Pin_1
//#define BSP_GPIOB_CH4			GPIO_Pin_0
//#define BSP_GPIOF_PWM_GRP		BSP_GPIOB_CH3 | BSP_GPIOB_CH4
//! I2C
#define BSP_I2C_SCL				GPIO_Pin_8
#define BSP_I2C_SDA				GPIO_Pin_9
#define I2CIO					IOPB
#define BSP_I2C_GRP				BSP_I2C_SCL | BSP_I2C_SDA
//! DEV-GSM
#define BSP_GSM_PWRCTRL			GPIO_Pin_1
#define GSMPWRCTRL				IOPB
#define BSP_GSM_ONOFF			GPIO_Pin_2
#define GSMONOFF				IOPB
#define BSP_GSM_WAKEUP			GPIO_Pin_9
#define GSMWAKEUP				IOPD
#define BSP_GSM_RING			GPIO_Pin_8
#define GSMRING					IOPD
#define BSP_PORTSRC_RING		GPIO_PortSourceGPIOD
#define BSP_PPINSRC_RING		GPIO_PinSource8
#define BSP_EXTILINE_RING		EXTI_Line8
//! COM1
#define BSP_COM1_TX				GPIO_Pin_9
#define BSP_COM1_RX				GPIO_Pin_10
#define COM1IO					IOPA
//!	GPIO_Remap_USART1			PB6 | PB7
#define COM1REMAP				GPIO_Remap_NULL
#define BSP_COM1_GRP			BSP_COM1_TX | BSP_COM1_RX
//! COM2
#define BSP_COM2_TX				GPIO_Pin_2
#define BSP_COM2_RX				GPIO_Pin_3
#define COM2IO					IOPA
//!	GPIO_Remap_USART2			PD5 | PD6
#define COM2REMAP				GPIO_Remap_NULL
#define BSP_COM2_GRP			BSP_COM2_TX | BSP_COM2_RX
//! COM3
#define BSP_COM3_TX				GPIO_Pin_10
#define BSP_COM3_RX				GPIO_Pin_11
#define COM3IO					IOPB
//!	GPIO_PartialRemap_USART3 	PC10 | PC11
//!	GPIO_FullRemap_USART3		PD8 | PD9
#define COM3REMAP				GPIO_Remap_NULL
#define BSP_COM3_GRP			BSP_COM3_TX | BSP_COM3_RX

// GPRS ID
// ϵͳ���õ�gprs��������ַ(GSMģ��֧�����3��GPRS��������)
#define MAX_NUM_TCP_CONN					4

// ip��ַ����󳤶ȣ���"128.128.128.128"
#define MAX_LEN_GPRS_IP						20
// ��������󳤶ȣ���"www.google.com"
#define MAX_LEN_GPRS_DN						32
// �˿ڵ���󳤶ȣ���"8080"
#define MAX_LEN_GPRS_PORT					8

#endif

/*============================ TYPES =========================================*/

/**
  * @brief  Description "�ṩ��ʱ����"
  * @param  time		��λms
  * @retval None
  */
typedef void (*Func_Delay)(int time);

/**
  * @brief  Description "�ṩ��ʱ���ĺ�����"
  * @param  None
  * @retval ��ʱ���ĺ�����
  */
typedef int (*Func_Systick)(void);

void Delay(int nTime);
/**
  * @brief  Description "RTC8025����ʱ�ӼĴ���"
  * @param  None
  * @retval 
  */
#define SYS_RTC_8025_BUF_SIZE	16
typedef union __rt8025_t
{
	struct __rt8025_regs
	{
		uint8_t 	Seconds:7;
		uint8_t		Resv1:1;
		uint8_t 	Minutes:7;
		uint8_t		Resv2:1;
		uint8_t 	Hours:6;
		uint8_t		Resv3:2;
		uint8_t 	Weekdays:3;
		uint8_t		Resv4:5;
		uint8_t 	Days:6;
		uint8_t		Resv5:2;
		uint8_t 	Months:5;
		uint8_t		Resv6:2;
		uint8_t		YearCarry:1;
		uint8_t 	Years;
		uint8_t 	DigitalOffset;
		uint8_t 	Alarm_W_Minute:7;
		uint8_t		Resv7:1;
		uint8_t 	Alarm_W_Hour:6;
		uint8_t		Resv8:2;
		uint8_t 	Alarm_W_day:7;
		uint8_t		Resv9:1;
		uint8_t 	Alarm_D_Minute:7;
		uint8_t		Resv10:1;
		uint8_t 	Alarm_D_Hour:6;
		uint8_t		Resv11:2;
		uint8_t		Resv12;
		uint16_t	Control;
	} STREG;
	char REGS[SYS_RTC_8025_BUF_SIZE];
} RTC8025_Typedef;

typedef union __rt8025_ctrl_t
{
	struct __rt8025_ctrl_regs
	{
		uint16_t 	CT:3;		// INTA interrupt periodic
		uint16_t	Resv1:1;	// TEST for manufacturer
		uint16_t 	CLEN2:1;	// FOUT en RST_VAL:0
		uint16_t	H12_24:1;	// 1:24-Hour Clock
		uint16_t 	DALE:1;		// Alarm D en
		uint16_t	WALE:1;		// Alarm W en
		uint16_t 	DAFG:1;		// Alarm D match occurs, INTA = "L"
		uint16_t	WAFG:1;		// Alarm W match occurs, INTB = "L"
		uint16_t 	CTFG:1;		// Priodic interrupt output, INTA
		uint16_t	CLEN1:1;	// FOUT en RST_VAL:0
		uint16_t 	PON:1;		// Power-On Reset Detection
		uint16_t	XST:1;		// Oscillation Stop Detection
		uint16_t	VDET:1;		// Power Drop Detection
		uint16_t 	VDSL:1;		// Set Power Drop Threshold Voltage
	} STREG;
	uint16_t Control;
} RTC8025_Ctrl_Typedef;

//* ʱ�����ݽṹ��->���� */
typedef struct 
{
  unsigned int  Year;
  unsigned char Month;
  unsigned char Day;
  unsigned char Hour;
  unsigned char Min;
  unsigned char Sec;
  unsigned char Week;
  char			Zone;
} RTC_Times; 

typedef struct
{
//	char	dn[MAX_LEN_GPRS_DN + 1];			// Domain Name
//	char	ip[MAX_LEN_GPRS_IP + 1];			// IP Address
//	char	port[MAX_LEN_GPRS_PORT + 1];		// Port
}GPRS_Connection;

/*============================ INTERFACE =====================================*/

/**
  * @brief  Description "������Ϣ���--�ַ��������"
  * @param  ch			����ַ�
  * @retval None
  */
void DebugOutChar(int ch);

/**
  * @brief  Description "GSM �⺯��������ʼ��"
  * @param  None
  * @retval bool		��ʼ���ɹ����
  */
bool gsm_env_init(GPRS_Connection* ip_com);

/**
  * @brief  Description "���GSM���ź�ǿ��"
  * @param  None
  * @retval int			GSM���ź�ǿ��(5~32Ϊ������ֵ)
  */
int gsm_get_signal(void);

/**
  * @brief  Description "���GSM��GPRS�ĸ���״̬"
  * @param  None
  * @retval bool		GPRSͨѶ�Ƿ�����
  */
int gsm_get_gprs_state(void);

/**
  * @brief  Description "GPRSͨѶ"
  * @param  wbuf		���ͻ�����
  * @param  wlen		���ͻ������ֽ�����С
  * @param  rbuf		���ջ�����
  * @param  rlen		���ջ������ֽ�����С
  * @retval bool		GPRSͨѶ�Ƿ�ɹ�
  */
bool gsm_gprs_comm(char* wbuf, int wlen, char* rbuf, int* rlen);

/**
  * @brief  Description "GPRSͨѶ"
  * @param  wbuf		���ͻ�����
  * @param  wlen		���ͻ������ֽ�����С
  * @param  rbuf		���ջ�����
  * @param  rlen		���ջ������ֽ�����С
  * @retval bool		GPRSͨѶ�Ƿ�ɹ�
  */
bool gsm_gprs_config(char *wbuf, int wlen, char *rbuf, int* rlen);

/**
  * @brief  Description "GPRSͨѶ"
  * @param  package_no	�������������(1..MAX)
  * @param  rbuf		���ջ�����
  * @param  rlen		���ջ������ֽ�����С
  * @retval bool		GPRSͨѶ�Ƿ�ɹ�
  */
bool gsm_gprs_update(int package_no, char *rbuf, int* rlen);

/**
  * @brief  Description "GSM���Ͷ���"
  * @param  pn			�����ֻ�����
  * @param  ascii_utf8	���ͻ�����
  * @retval bool		GSM���Ͷ����Ƿ�ɹ�
  */
bool gsm_send_sms(char* pn, char* ascii_utf8);

/**
  * @brief  Description "GSM���ն���"
  * @param  pn			�����ֻ�����
  * @param  ascii_utf8	���ջ�����
  * @retval bool		GSM���ն����Ƿ�ɹ�
  */
bool gsm_recv_sms(char* pn, RTC_Times* time, char* ascii_utf8, int* len);

/**
  * @brief  Description "ģ��I2Cд����"
  * @param  pdata  		д������ݻ�����ָ��
  * @param  addr		д��������ڴ��ַ
  * @param  count		д����������ݴ�С
  * @retval bool		I2Cд�����Ƿ�ɹ�
  */
bool AI2C_Write(char *pdata, int addr, int count);

/**
  * @brief  Description "ģ��I2C������"
  * @param  pdata  		��ȡ�����ݻ�����ָ��
  * @param  addr		��ȡ�������ڴ��ַ
  * @param  count		��ȡ���������ݴ�С
  * @retval bool		I2C�������Ƿ�ɹ�
  */
bool AI2C_Read(char *pdata, int addr, int count);

/**
  * @brief  Description "ģ��I2CдRTC8025����"
  * @param  pdata  		д������ݻ�����ָ��
  * @param  addr		д��������ڴ��ַ
  * @param  count		д����������ݴ�С
  * @retval bool		RTC8025д�����Ƿ�ɹ�
  */
bool RTC8025_Write(char *pdata, int addr, int count);

/**
  * @brief  Description "ģ��I2C��RTC8025����"
  * @param  pdata  		��ȡ�����ݻ�����ָ��
  * @param  addr		��ȡ�������ڴ��ַ
  * @param  count		��ȡ���������ݴ�С
  * @retval bool		RTC8025�������Ƿ�ɹ�
  */
bool RTC8025_Read(char *pdata, int addr, int count);

/**
  * @brief  Description "RTC8025״̬λ��λ����"
  * @param  None		������λ��־��0 ��������λ��1��������λ��
  * @retval bool		RTC8025�������Ƿ�ɹ�
  */
bool RTC8025_Reset(bool need_reset);




/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx
/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

