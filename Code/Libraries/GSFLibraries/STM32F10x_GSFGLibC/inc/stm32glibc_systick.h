/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_systick.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : SCB(ϵͳ����ģ��) firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_SYSTICK_H
#define __STM32GLIBC_SYSTICK_H

#include "stm32glibc_type.h"



/* Configuration Mode enumeration --------------------------------------------*/
#define SYSTICK_LOAD_RELOAD			((uint32_t)0xFFFFFFul)

/* Exported constants --------------------------------------------------------*/
namespace	mcucore
{
    /** @addtogroup CMSIS_CM3_SCB CMSIS CM3 SCB
    memory mapped structure for System Control Block (SCB)
    @{
    */
    enum SYSTICK_CLKSRC
    {        
		SYSTICK_CLKSRC_HCLK_Div8    =((uint32_t)0xFFFFFFFB),
		SYSTICK_CLKSRC_HCLK         =((uint32_t)0x00000004),
    };

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ϵͳʱ�ӽ��Ŀ�����״̬�Ĵ���   (SYSTICK_CTRL)	@0xE000E010
    struct SYSTICK_CTRL_Struct
    {
        //!	1= ����������������ģʽ��multi-shot��
        __IO uint32_t ENABLE							: 1;	//Bit0
        //! 1= ���¼�����0�ᵼ�¹���SysTick������ 
        __IO uint32_t TICKINT						: 1;	//Bit1
        //! 0= �ⲿ�ο�ʱ�� 1= �ں�ʱ�� 
        __IO uint32_t CLKSOURCE						: 1;	//Bit2
        __IO uint32_t Resv1							: 13;	//Bit3 ~ Bit15
        //! ���ϴζ�ȡ��ʱ������ʼ�������ʱ��������0���򷵻�1����ȡʱ���㡣
        __IO uint32_t COUNTFLAG						: 1;	//Bit16
        __IO uint32_t Resv2							: 15;	//Bit17 ~ Bit31
    };
    typedef struct SYSTICK_CTRL_Struct SYSTICK_CTRL_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ϵͳʱ�ӽ���У׼ֵ�Ĵ���(SYSTICK_CALIB)			@0xE000E01C
    struct SYSTICK_CALIB_Struct
    {
		//! ��ֵ������ 10ms ��ʱ����װֵ
        __IO uint32_t TENMS							: 24;	//Bit0 ~ Bit23
        __IO uint32_t Resv1							: 6;	//Bit24 ~ Bit29
        //! 1=  ����ʱ��Ƶ�ʵ�ԭ��У��ֵ���Ǿ�ȷ�� 10ms
        __IO uint32_t SKEW	 						: 1;	//Bit30
        //! 1= û���ṩ�ο�ʱ�� 
        __IO uint32_t NOREF							: 1;	//Bit31
    };
    typedef struct SYSTICK_CALIB_Struct SYSTICK_CALIB_Typedef;

    typedef struct
    {
        __IO uint32_t CTRL;			/*!< Offset: 0x00  SysTick Control and Status Register */
        __IO uint32_t LOAD;			/*!< Offset: 0x04  SysTick Reload Value Register       */
        __IO uint32_t VAL;			/*!< Offset: 0x08  SysTick Current Value Register      */
        __I uint32_t CALIB;		/*!< Offset: 0x0C  SysTick Calibration Register        */   

        // Systick ����
        bool Config(uint32_t ticks, uint32_t CLKSource = SYSTICK_CLKSRC_HCLK);
		// <59s
        void DelayUS(uint32_t times);
		// <59s
        void DelayMS(uint32_t times);
    } Systick_Typedef;
}
//!
//! @{
using mcucore::Systick_Typedef;
#define stSystick	((Systick_Typedef *)           SysTick_BASE)
#define SYSTICK		(*stSystick)
//! @}
//!
//! @{
/*
#define ITM_BASE            (0xE0000000)			//!< ITM Base Address
#define SCS_BASE            (0xE000E000)			//!< System Control Space Base Address
#define SYSTICK_BASE        (SCS_BASE +  0x0010)	//!< SysTick Base Address
#define NVIC_BASE           (SCS_BASE +  0x0100)	//!< NVIC Base Address
#define SCB_BASE            (SCS_BASE +  0x0D00)	//!< System Control Block Base Address
#define CoreDebug_BASE      (0xE000EDF0)			//!< Core Debug Base Address
*/
//! @}

#endif /* __STM32GLIBC_SYSTICK_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
