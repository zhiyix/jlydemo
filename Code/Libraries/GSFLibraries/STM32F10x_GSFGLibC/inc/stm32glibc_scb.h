/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_scb.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : SCB(ϵͳ����ģ��) firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_SCB_H
#define __STM32GLIBC_SCB_H

#include "stm32glibc_type.h"



/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
namespace	mcucore
{
    /** @addtogroup CMSIS_CM3_SCB CMSIS CM3 SCB
    memory mapped structure for System Control Block (SCB)
    @{
    */
    enum PriorityGroupingStyle
    {
        GRP16_SUB1	= BByte(011),		// 16 priority groups, 1(none) sub priority
        GRP8_SUB2	= BByte(100),		// 8 priority groups, 2 sub priority (none)
        GRP4_SUB4	= BByte(101),		// 4 priority groups, 4 sub priority (none)
        GRP2_SUB8	= BByte(110),		// 2 priority groups, 8 sub priority (none)
        GRP1_SUB16	= BByte(111),		// 1 (none priority groups, 16 sub priority (none)
    };

    enum AIRCR_KEY
    {
        CONST_REGISTER_KEY = 0x05FA,
        CONST_READ_KEY = 0xFA05,
    };
    // ע��ÿ��д���AIRCR�Ĵ�����Ҫ��VECTKEYSTAT��bit field��д��0x05fa
    enum SCB_AIRCR
    {
        VECTKEYSTAT		= BitFromTo(31, 16),// Register key
        ENDIANESS		= Bit(15),			//
        PRIGROUP		= BitFromTo(10, 8),	// Interrupt priority grouping field
        SYSRESETREQ		= Bit(2),			// System reset request
        VECTCLRACTIVE 	= Bit(1),			// Reserved for Debug use.
        VECTRESET		= Bit(0),			// Reserved for Debug use.
    };

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! CPU ID ��ַ�Ĵ��� �Ĵ���(SCB_CPUID)	@0xE000ED00
    struct SCB_CPUID_Struct
    {
        //! �豸�Ѷ���İ汾��
        __I uint32_t REVISION                      : 4;	//Bit0 ~ Bit3
        //! ϵ�д������ı��
        __I uint32_t PARTNO                        : 12;	//Bit4 ~ Bit15
        __I uint32_t Resv1                         : 4;	//Bit16 ~ Bit19
        //! �豸�Ѷ���ı�����
        __I uint32_t VARIANT                       : 4;	//Bit20 ~ Bit23
        //! ʵ���ߴ��롣ARMΪ0x41
        __I uint32_t IMPLEMENTER                   : 8;	//Bit24 ~ Bit31
    };
    typedef struct SCB_CPUID_Struct SCB_CPUID_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! �жϿ���״̬�Ĵ��� (SCB_ICSR)		@0xE000ED04
    struct SCB_ICSR_Struct
    {
        //! ����ISR�ĺ�����
        __I uint32_t VECTACIVE                     : 10;	//Bit0 ~ Bit9
        //! ����Ļ�����
        __I uint32_t RETTOBASE                     : 1;	//Bit10
        __I uint32_t Resv1                         : 1;	//Bit11
        //! ����ISR�ĺ�����
        __I uint32_t VECTPENDING                   : 10;	//Bit12 ~ Bit21
        //! �жϹ����־
        __I uint32_t ISRPENDING                    : 1;	//Bit22
        //!
        __I uint32_t ISRPREEMPT                    : 1;	//Bit23
        __I uint32_t Resv2                         : 1;	//Bit24
        //! �������SysTickλ
        __IO uint32_t  PENDSTCLR                     : 1;	//Bit25
        __I uint32_t Resv3                         : 7;	//Bit26 ~ Bit31
    };
    typedef struct SCB_ICSR_Struct SCB_ICSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ������ƫ�ƼĴ��� (SCB_VTOR)			@0xE000ED08
    struct SCB_VTOR_Struct
    {
        __IO uint32_t Resv1							: 7;	//Bit0 ~ Bit6
        //! ������Ļ�ַƫ����
        __IO uint32_t TBLOFF							: 22;	//Bit7 ~ Bit28
        //! �������ַλ��Code��0����RAM��1����
        __IO uint32_t TBLBASE						: 1;	//Bit29
        __IO uint32_t Resv2							: 2;	//Bit30 ~ Bit31
    };
    typedef struct SCB_VTOR_Struct SCB_VTOR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! Ӧ���ж��븴λ���ƼĴ��� (SCB_AIRCR)	@0xE000ED0C
    struct SCB_AIRCR_Struct
    {
        //!	ϵͳ��λλ
        __IO uint32_t VECTRESET						: 1;	//Bit0
        //! �����Ч����λ
        __IO uint32_t VECTCLRACTIVE					: 1;	//Bit1
        //! ���ź����ⲿϵͳ��Ч����ʾ����λ
        __IO uint32_t SYSRESETREQ					: 1;	//Bit2
        __IO uint32_t Resv1							: 5;	//Bit3 ~ Bit7
        //! �ж����ȼ�������
        __IO uint32_t PRIGROUP						: 3;	//Bit8 ~ Bit10
        __IO uint32_t Resv2							: 4;	//Bit11 ~ Bit14
        //! ���ݵ��ֽ�˳��λ
        __IO uint32_t ENDIANESS						: 1;	//Bit15
        //! ��ȡʱΪ0xFA05
        //! ע���루register key��:
        //! �ԼĴ�������д����ʱҪ���� VECTKEY����д��0x5FA������д��ֵ�����ԡ�
        __IO uint32_t VECTKEY						: 16;	//Bit16 ~ Bit31
    };
    typedef struct SCB_AIRCR_Struct SCB_AIRCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ϵͳ���ƼĴ��� (SCB_SCR)			@0xE000ED10
    struct SCB_SCR_Struct
    {
        //!
        __IO uint32_t Resv1							: 1;	//Bit0
        //! ���Ӵ�����ģʽ���ص��߳�ģʽʱ����ʼ���˳�ʱ˯�ߡ�
        __IO uint32_t SLEEPONEXIT					: 1;	//Bit1
        //! ���˯��λ��
        __IO uint32_t SLEEPDEEP						: 1;	//Bit2
        __IO uint32_t Resv2							: 1;	//Bit3
        //! ʹ��ʱ�����жϴ�û�й��𵽹���ʱ SEVONPEND ���Ի��� WFE��
        __IO uint32_t SEVONPEND						: 1;	//Bit4
        __IO uint32_t Resv3							: 27;	//Bit5 ~ Bit31
    };
    typedef struct SCB_SCR_Struct SCB_SCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ���ÿ��ƼĴ��� (SCB_CCR)			@0xE000ED14
    struct SCB_CCR_Struct
    {
        //!	Ϊ0ʱ��Ĭ�����ڴ��ϴ��쳣����ʱֻ�ܽ����߳�ģʽ
        __IO uint32_t NONEBASETHRDENA				: 1;	//Bit0
        //! ���д�� 1����ô�û��������д��������жϼĴ����Դ���������
        __IO uint32_t USERSETMPEND					: 1;	//Bit1
        __IO uint32_t Resv1							: 1;	//Bit2
        //! ������������塣
        __IO uint32_t UNALIGN_TRP					: 1;	//Bit3
        //! ��0���塣
        __IO uint32_t DIV_0_TRP 						: 1;	//Bit4
        __IO uint32_t Resv2							: 3;	//Bit5 ~ Bit7
        //! һ�㽫������̽��ϵͳ�豸���Ž����Լ�Ⲣ���������ŵ����⡣
        __IO uint32_t BFHFNMIGN						: 1;	//Bit8
        __IO uint32_t Resv3							: 23;	//Bit9 ~ Bit31
    };
    typedef struct SCB_CCR_Struct SCB_CCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ϵͳ���������ȼ��Ĵ��� (SCB_SHP)	@0xE000ED18
    struct SCB_SHP_Struct
    {
        volatile uint32_t PRI_4	                        : 8;	//Bit0 ~ Bit7
        volatile uint32_t PRI_5	                        : 8;	//Bit8 ~ Bit15
        volatile uint32_t PRI_6	                        : 8;	//Bit16 ~ Bit23
        volatile uint32_t PRI_7	                        : 8;	//Bit24 ~ Bit31
        volatile uint32_t PRI_8	                        : 8;	//Bit0 ~ Bit7
        volatile uint32_t PRI_9	                        : 8;	//Bit8 ~ Bit15
        volatile uint32_t PRI_10                        : 8;	//Bit16 ~ Bit23
        volatile uint32_t PRI_11                        : 8;	//Bit24 ~ Bit31
        volatile uint32_t PRI_12                        : 8;	//Bit0 ~ Bit7
        volatile uint32_t PRI_13                        : 8;	//Bit8 ~ Bit15
        volatile uint32_t PRI_14                        : 8;	//Bit16 ~ Bit23
        volatile uint32_t PRI_15                        : 8;	//Bit24 ~ Bit31
    };
    typedef struct SCB_SHP_Struct SCB_SHP_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ϵͳ������������״̬�Ĵ��� (SCB_SHCSR)	@0xE000ED24
    struct SCB_SHCSR_Struct
    {
        //!	���MemManage�Ѽ����ôΪ1
        __IO uint32_t MEMFAULTACT					: 1;	//Bit0
        //! ���BusFault �Ѽ����ôΪ1
        __IO uint32_t BUSFAULTACT					: 1;	//Bit1
        __IO uint32_t Resv1							: 1;	//Bit2
        //! ���UsageFault�Ѽ����ôΪ1
        __IO uint32_t USGFAULTACT					: 1;	//Bit3
        __IO uint32_t Resv2							: 3;	//Bit4 ~ Bit6
        //! ���SVCall�Ѽ����ôΪ1
        __IO uint32_t SVCALLACT 						: 1;	//Bit7
        //! ���������Ѽ����ôΪ1
        __IO uint32_t MONITORACT						: 1;	//Bit8
        __IO uint32_t Resv3							: 1;	//Bit9
        //! ���PendSV�Ѽ����ôΪ1
        __IO uint32_t PENDSVACT 						: 1;	//Bit10
        //! ���SysTick�Ѽ����ôΪ1
        __IO uint32_t SYSTICKACT						: 1;	//Bit11
        __IO uint32_t Resv4							: 1;	//Bit12
        //! ����ڿ�ʼ���� MemManage ʱ��һ���������ȼ����ж�ȡ��������
        __IO uint32_t MEMFAULTPENDED					: 1;	//Bit13
        //! ����ڿ�ʼ���� BusFault ʱ��һ���������ȼ����ж�ȡ��������
        __IO uint32_t BUSFAULTPENDED					: 1;	//Bit14
        //! ����ڿ�ʼ���� SVCall ʱ��һ���������ȼ����ж�ȡ�������� SVCall
        __IO uint32_t SVCALLPENDED					: 1;	//Bit15
        //! ��Ϊ0ʱ���ܣ���Ϊ1ʱʹ��
        __IO uint32_t MEMFAULTENA					: 1;	//Bit16
        //! ��Ϊ0ʱ���ܣ���Ϊ1ʱʹ��
        __IO uint32_t BUSFAULTENA					: 1;	//Bit17
        //! ��Ϊ0ʱ���ܣ���Ϊ1ʱʹ��
        __IO uint32_t USGFAULTENA					: 1;	//Bit18
        __IO uint32_t Resv5							: 13;	//Bit19 ~ Bit31
    };
    typedef struct SCB_SHCSR_Struct SCB_SHCSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! �����ù���״̬�Ĵ��� (SCB_CFSR)		0xE000ED28
    struct SCB_CFSR_Struct
    {
        //!
        //! �洢���������״̬�Ĵ���
        //!
        //!	ָ�����Υ����־
        __IO uint32_t IACCVIOL						: 1;	//Bit0
        //! ���ݷ���Υ����־
        __IO uint32_t DACCVIOL						: 1;	//Bit1
        __IO uint32_t Resv1							: 1;	//Bit2
        //! �쳣����ʱ�ĳ�ջ���������� 1 ���� 1 �����ϵķ���Υ��
        __IO uint32_t MUNSTKERR						: 1;	//Bit3
        //! �����쳣ʱ��ѹջ����������1���� 1�����ϵķ���Υ��
        __IO uint32_t MSTKERR						: 1;	//Bit4
        __IO uint32_t Resv2							: 2;	//Bit5 ~ Bit6
        //! �洢�������ַ�Ĵ�����MMAR����ַ��Ч��־
        __IO uint32_t MMARVALID 						: 1;	//Bit7
        //!
        //! ���߹���״̬�Ĵ���
        //!
        //!	ָ�����ߴ����־
        __IO uint32_t IBUSERR						: 1;	//Bit0
        //! ��ȷ���������ߴ��󷵻�
        __IO uint32_t PRECISERR						: 1;	//Bit1
        //! ����ȷ���������ߴ���
        __IO uint32_t IMPRECISERR					: 1;	//Bit2
        //! �쳣����ʱ�ĳ�ջ���������� 1 ���� 1 �����ϵ����߹���
        __IO uint32_t UNSTKERR						: 1;	//Bit3
        //! �����쳣ʱ��ѹջ����������1����1�����ϵ����߹���
        __IO uint32_t STKERR							: 1;	//Bit4
        __IO uint32_t Resv3							: 2;	//Bit5 ~ Bit6
        //! ������߹��ϵ�ַ�Ĵ�����BFAR��������Ч�ĵ�ַ����ô��λ��λ
        __IO uint32_t BFARVALID 						: 1;	//Bit7
        //!
        //! ʹ�ù���״̬�Ĵ���
        //!
        //!	�ڴ�������ͼִ��һ��δ�����ָ��ʱ��λUNDEFINSTR��־
        __IO uint32_t UNDEFINSTR						: 1;	//Bit0
        //! EPSR ��ָ��ķǷ���ϣ���δ����ָ�������ĳ���
        __IO uint32_t INVSTATE						: 1;	//Bit1
        //! ��ͼ�� EXC_RETURN �Ƿ����� PC
        __IO uint32_t INVPC							: 1;	//Bit2
        //! ��ͼʹ��Э������ָ���������֧��Э������ָ��
        __IO uint32_t NOCP							: 1;	//Bit3
        __IO uint32_t Resv4							: 4;	//Bit4 ~ Bit7
        //! ��UNALIGN_TRPʹ���Ҵ�����ͼ���в�����Ĵ洢������ʱ���ù��ϲ���
        __IO uint32_t UNALIGNED 						: 1;	//Bit8
        //! ��DIV_0_TRPʹ����SDIV�� UDIVָ��ĳ���Ϊ0ʱ���ù��ϲ���
        __IO uint32_t DIVBYZERO 						: 1;	//Bit9
        __IO uint32_t Resv5							: 6;	//Bit10 ~ Bit15
    };
    typedef struct SCB_CFSR_Struct SCB_CFSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! Ӳ����״̬�Ĵ���(SCB_HFSR)			@0xE000ED2C
    struct SCB_HFSR_Struct
    {
        //!
        __IO uint32_t Resv1							: 1;	//Bit0
        //! ��Ϊ���쳣�������߹��ϣ������ж�����������·������ϣ���ʱ��λ��λ
        __IO uint32_t VECTTBL						: 1;	//Bit1
        __IO uint32_t Resv2							: 28;	//Bit2 ~ Bit29
        //! Ӳ�����ڽ��յ������ù���ʱ����������ȼ�����Ϊ�����ù��ϱ����ܶ���
        __IO uint32_t FORCED 						: 1;	//Bit30
        //! �������������йصĹ��ϣ���ô��λ��λ��
        __IO uint32_t DEBUGEVT						: 1;	//Bit31
    };
    typedef struct SCB_HFSR_Struct SCB_HFSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ���Թ���״̬�Ĵ��� (SCB_DFSR)		@0xE000ED30
    struct SCB_DFSR_Struct
    {
        //!	��ֹ�����־
        __IO uint32_t HALTED							: 1;	//Bit0
        //! BKPT��־��flash�޲������е�BKPT ָ�����ã�Ҳ������������������
        __IO uint32_t BKPT							: 1;	//Bit1
        //! ���ݹ۲������٣�DWT����־
        __IO uint32_t DWTIRAP 						: 1;	//Bit2
        //! ���������־
        __IO uint32_t VCATCH							: 1;	//Bit3
        //! �ⲿ���������־
        __IO uint32_t EXTERNAL						: 1;	//Bit4
        __IO uint32_t Resv1							: 27;	//Bit5 ~ Bit31
    };
    typedef struct SCB_DFSR_Struct SCB_DFSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! �洢��������ϵ�ַ�Ĵ���(SCB_MMFAR)	@0xE000ED34
    struct SCB_MMFAR_Struct
    {
        //!	Mem Manage ���ϵĵ�ַ��
        __IO uint32_t ADDRESS						: 32;	//Bit0 ~ Bit31
    };
    typedef struct SCB_MMFAR_Struct SCB_MMFAR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ���߹��ϵ�ַ�Ĵ��� (SCB_BFAR)	@0xE000ED38
    struct SCB_BFAR_Struct
    {
        //!	���߹��ϵĵ�ַ��
        __IO uint32_t ADDRESS						: 32;	//Bit0 ~ Bit31
    };
    typedef struct SCB_BFAR_Struct SCB_BFAR_Typedef;

    typedef struct
    {
        __I uint32_t CPUID;	/*!< Offset: 0x00  CPU ID Base Register                                  */
        __IO uint32_t  ICSR;		/*!< Offset: 0x04  Interrupt Control State Register                      */
        __IO uint32_t  VTOR;		/*!< Offset: 0x08  Vector Table Offset Register                          */
        __IO uint32_t  AIRCR;	/*!< Offset: 0x0C  Application Interrupt / Reset Control Register        */
        __IO uint32_t  SCR;		/*!< Offset: 0x10  System Control Register                               */
        __IO uint32_t  CCR;		/*!< Offset: 0x14  Configuration Control Register                        */
        __IO uint8_t   SHP[12];	/*!< Offset: 0x18  System Handlers Priority Registers (4-7, 8-11, 12-15) */
        __IO uint32_t  SHCSR;	/*!< Offset: 0x24  System Handler Control and State Register             */
        __IO uint32_t  CFSR;		/*!< Offset: 0x28  Configurable Fault Status Register                    */
        __IO uint32_t  HFSR;		/*!< Offset: 0x2C  Hard Fault Status Register                            */
        __IO uint32_t  DFSR;		/*!< Offset: 0x30  Debug Fault Status Register                           */
        __IO uint32_t  MMFAR;	/*!< Offset: 0x34  Mem Manage Address Register                           */
        __IO uint32_t  BFAR;		/*!< Offset: 0x38  Bus Fault Address Register                            */
        __IO uint32_t  AFSR;		/*!< Offset: 0x3C  Auxiliary Fault Status Register                       */
        __I uint32_t PFR[2];	/*!< Offset: 0x40  Processor Feature Register                            */
        __I uint32_t DFR;		/*!< Offset: 0x48  Debug Feature Register                                */
        __I uint32_t ADR;		/*!< Offset: 0x4C  Auxiliary Feature Register                            */
        __I uint32_t MMFR[4];	/*!< Offset: 0x50  Memory Model Feature Register                         */
        __I uint32_t ISAR[5];	/*!< Offset: 0x60  ISA Feature Register                                  */

        PriorityGroupingStyle GetPriorityGrouping();
        void SetPriorityGrouping(PriorityGroupingStyle s);

        /**
          * @brief  Sets the vector table location and Offset.
          * @param  NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
          *   This parameter can be one of the following values:
          *     @arg NVIC_VectTab_RAM
          *     @arg NVIC_VectTab_FLASH
          * @param  Offset: Vector Table base offset field. This value must be a multiple
          *         of 0x200.
          * @retval None
          */
        finline void SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
        {
            /* Check the parameters */
            //(IS_NVIC_VECTTAB(NVIC_VectTab))	NVIC_VectTab_RAM,NVIC_VectTab_FLASH
            //(IS_NVIC_OFFSET(Offset))

            VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
        }
        void SystemPriorityConfig(IRQn_Type IRQn, uint8_t priority = 0xF)
        {
            if(IRQn < 0)
            {
                /* set Priority for Cortex-M3 System Interrupts */
                SHP[((uint32_t)(IRQn) & 0xF) - 4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
            }
        }
        void SystemReset(void);
    } SCB_Typedef;
}
//!
//! @{
using mcucore::SCB_Typedef;
#define stSCB	((SCB_Typedef *)           SCB_BASE)
#define MCUSCB	(*stSCB)
//! @}
//!
//! @{
/*
#define ITM_BASE            (0xE0000000)			//!< ITM Base Address
#define SCS_BASE            (0xE000E000)			//!< System Control Space Base Address
#define SysTick_BASE        (SCS_BASE +  0x0010)	//!< SysTick Base Address
#define NVIC_BASE           (SCS_BASE +  0x0100)	//!< NVIC Base Address
#define SCB_BASE            (SCS_BASE +  0x0D00)	//!< System Control Block Base Address
#define CoreDebug_BASE      (0xE000EDF0)			//!< Core Debug Base Address
*/
//! @}

#endif /* __STM32GLIBC_SCB_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
