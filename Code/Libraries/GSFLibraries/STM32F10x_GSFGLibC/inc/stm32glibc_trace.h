/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32glibc_trace.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_TRACE_H
#define __STM32GLIBC_TRACE_H

#include "stm32glibc_type.h"



/* Configuration Mode enumeration --------------------------------------------*/
/*!< DBGMCU_BASE */
#define stDBGMCU	((MCUInfo_Typedef *)		(uint32_t)DBGMCU_BASE)
#define MCUDBG		(*stDBGMCU)
/*!< ITM_BASE */
#define stITM		((ITM_Typedef *)           	(uint32_t)0xE0000000)
#define MCUITM		(*stITM)
/*!< CORE_DBG_BASE */
#define stTRACE		((TRACE_Typedef *)			(uint32_t)0xE000EDF0)
#define MCUTRACE	(*stTRACE)
/*!< Device Electronic Signature */
#define stDEVINFO	((DevInfo_Typedef *)		(uint32_t)0x1FFFF7E0)
#define MCUINFO		(*stDEVINFO)

/* Exported constants --------------------------------------------------------*/
namespace mcucore
{
    enum Trace_Ctrl
    {
        // ITM (���������ָ����ٺ굥Ԫ) //!< ITM ʹ��λ(DEMCR.TRCENA)
        TRACE_ITM_TRCENA              = ((uint32_t)0x01 << 24),
        TRACE_ITM_ITMENA              = ((uint32_t)0x01 << 0),
        // Timestamp prescaler
        TRACE_ITM_TSPRE               = ((uint32_t)0x03 << 8),
    };

    enum DBGMCU_CtrlFlag
    {
        /******************  Bit definition for DBGMCU_CR register  *******************/
        DBGMCU_DBG_SLEEP                 = ((uint32_t)0x00000001),       /*!<Debug Sleep Mode */
        DBGMCU_DBG_STOP                  = ((uint32_t)0x00000002),       /*!<Debug Stop Mode */
        DBGMCU_DBG_STANDBY               = ((uint32_t)0x00000004),       /*!<Debug Standby mode */
        DBGMCU_TRACE_IOEN                = ((uint32_t)0x00000020),       /*!<Trace Pin Assignment Control */

        DBGMCU_TRACE_MODE                = ((uint32_t)0x000000C0),       /*!<TRACE_MODE[1:0] bits (Trace Pin Assignment Control) */
        DBGMCU_TRACE_MODE_0              = ((uint32_t)0x00000040),       /*!<Bit 0 */
        DBGMCU_TRACE_MODE_1              = ((uint32_t)0x00000080),       /*!<Bit 1 */

        DBGMCU_DBG_IWDG_STOP             = ((uint32_t)0x00000100),       /*!<Debug Independent Watchdog stopped when Core is halted */
        DBGMCU_DBG_WWDG_STOP             = ((uint32_t)0x00000200),       /*!<Debug Window Watchdog stopped when Core is halted */
        DBGMCU_DBG_TIM1_STOP             = ((uint32_t)0x00000400),       /*!<TIM1 counter stopped when core is halted */
        DBGMCU_DBG_TIM2_STOP             = ((uint32_t)0x00000800),       /*!<TIM2 counter stopped when core is halted */
        DBGMCU_DBG_TIM3_STOP             = ((uint32_t)0x00001000),       /*!<TIM3 counter stopped when core is halted */
        DBGMCU_DBG_TIM4_STOP             = ((uint32_t)0x00002000),       /*!<TIM4 counter stopped when core is halted */
        DBGMCU_DBG_CAN_STOP              = ((uint32_t)0x00004000),       /*!<Debug CAN stopped when Core is halted */
        DBGMCU_DBG_I2C1_SMBUS_TIMEOUT    = ((uint32_t)0x00008000),       /*!<SMBUS timeout mode stopped when Core is halted */
        DBGMCU_DBG_I2C2_SMBUS_TIMEOUT    = ((uint32_t)0x00010000),       /*!<SMBUS timeout mode stopped when Core is halted */
        DBGMCU_DBG_TIM5_STOP             = ((uint32_t)0x00020000),       /*!<TIM5 counter stopped when core is halted */
        DBGMCU_DBG_TIM6_STOP             = ((uint32_t)0x00040000),       /*!<TIM6 counter stopped when core is halted */
        DBGMCU_DBG_TIM7_STOP             = ((uint32_t)0x00080000),       /*!<TIM7 counter stopped when core is halted */
        DBGMCU_DBG_TIM8_STOP             = ((uint32_t)0x00100000),       /*!<TIM8 counter stopped when core is halted */

    };

    union mcu_info
    {
        struct __st_stmcu_info
        {
            __IO uint16_t DEV_ID;
            __IO uint16_t REV_ID;
        } st;
        __IO uint32_t MCU_INFO;
    };

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ����MCU���üĴ��� (DBGMCU_CR)	@0xE0042000
    struct DBGMCU_CR_Struct
    {
        //!	����˯��ģʽ
        __IO uint32_t DBG_SLEEP						: 1;	//Bit0
        //!	����ֹͣģʽ
        __IO uint32_t DBG_STOP						: 1;	//Bit1
        //!	���Դ���ģʽ
        __IO uint32_t DBG_STANDBY					: 1;	//Bit2
        __IO uint32_t Resv1							: 2;	//Bit3 ~ Bit4
        //!	�����������(Ĭ��״̬)
        __IO uint32_t TRACE_IOEN						: 1;	//Bit5
        //!	�������ŷ������
        __IO uint32_t TRACE_MODE						: 2;	//Bit6 ~ Bit7
        //!	���ں˽������״̬ʱ���Ź�ֹͣ����
        __IO uint32_t DBG_IWDG_STOP					: 1;	//Bit8
        //!	���ں˽������״̬ʱ���Դ��ڿ��Ź�ֹͣ����
        __IO uint32_t DBG_WWDG_STOP					: 1;	//Bit9
        //!	���ں˽������״̬ʱ������ֹͣ���� x=4..1
        __IO uint32_t DBG_TIM1_STOP					: 1;	//Bit10
        //!	���ں˽������״̬ʱ������ֹͣ���� x=4..1
        __IO uint32_t DBG_TIM2_STOP					: 1;	//Bit11
        //!	���ں˽������״̬ʱ������ֹͣ���� x=4..1
        __IO uint32_t DBG_TIM3_STOP					: 1;	//Bit12
        //!	���ں˽������״̬ʱ������ֹͣ���� x=4..1
        __IO uint32_t DBG_TIM4_STOP					: 1;	//Bit13
        //!	���ں˽������״̬ʱ��CAN1ֹͣ����
        __IO uint32_t DBG_CAN1_STOP					: 1;	//Bit14
        //!	������ֹͣʱֹͣSMBUS��ʱģʽ
        __IO uint32_t DBG_I2C1_SMBUS_TIMEOUT			: 1;	//Bit15
        //!	������ֹͣʱֹͣSMBUS��ʱģʽ
        __IO uint32_t DBG_I2C2_SMBUS_TIMEOUT			: 1;	//Bit16
        //!	������ֹͣʱֹͣ��ʱ��������(x=8..5)
        __IO uint32_t DBG_TIM5_STOP					: 1;	//Bit17
        //!	������ֹͣʱֹͣ��ʱ��������(x=8..5)
        __IO uint32_t DBG_TIM6_STOP					: 1;	//Bit18
        //!	������ֹͣʱֹͣ��ʱ��������(x=8..5)
        __IO uint32_t DBG_TIM7_STOP					: 1;	//Bit19
        //!	������ֹͣʱֹͣ��ʱ��������(x=8..5)
        __IO uint32_t DBG_TIM8_STOP					: 1;	//Bit20
        //!	���ں˽������״̬ʱ��CAN2ֹͣ����
        __IO uint32_t DBG_CAN2_STOP					: 1;	//Bit20
        __IO uint32_t Resv2							: 10;	//Bit0
    };
    typedef struct DBGMCU_CR_Struct DBGMCU_CR_Typedef;

    //! @{
    // Device Info
    typedef struct
    {
        // Flash Size Register
        __I uint16_t F_SIZE;
        __I uint16_t Resv1;
        __I uint32_t Resv2;
        /**
         * 0x0080 = 128Kbytes
         */
        __I uint32_t U_ID0;
        __I uint32_t U_ID1;
        __I uint32_t U_ID2;
    } DevInfo_Typedef;
    //! @}

    //! @{
    // Debug MCU
    typedef struct
    {
        __IO uint32_t IDCODE;
        /**
          ΢������STM32F10xxx�ں�һ��MCU ID���롣���ID������ST MCU�Ĳ����ź͹�Ƭ�汾��
          ����DBG_MCU��һ����ɲ��֣�����ӳ�䵽�ⲿPPB������(��29.16��)��
          BIT[31:16] REV_ID[15:0] �汾ʶ��
        	�����ʶ��Ʒ�İ汾
        		С������Ʒ : 0x1000 = �汾A
        		��������Ʒ : 0x0000 = �汾A
        					 0x1000 = �汾A
        					 0x2000 = �汾B
        					 0x2001 = �汾Z
        					 0x2003 = �汾Y
        		��������Ʒ : 0x1000 = �汾A
        					 0x1001 = �汾Z
        		����ϵ�в�Ʒ : 0x1000 = �汾A
        					   0x1001 = �汾Z
        BIT[15:12] ����
        BIT[11:0]  DEV_ID[11:0]���豸ʶ��
        	�������ָʾ���豸���롣����STM32F10x΢��������
        		С������Ʒ���豸����Ϊ0x412��
        		��������Ʒ���豸����Ϊ0x410��
        		��������Ʒ���豸����Ϊ0x414��
        		����ϵ�в�Ʒ���豸����Ϊ0x418��
         */
        __IO uint32_t CR;

        void GetDebugMcuInfo(union mcu_info &chip_info)
        {
            chip_info.MCU_INFO = IDCODE;
        }

        void DebugMcuPeriph(enum DBGMCU_CtrlFlag flg)
        {
            CR = CR | flg;
        }
    } MCUInfo_Typedef;
    //! @}

    typedef struct
    {
        __IO uint32_t SP[32];
        /**0xE0000000
         * StimulusPorts
         */
        __I uint32_t RESERVED1[864];
        __IO uint32_t TENA;
        /**0xE0000E00
         * Trace Enable
         * STIMENA [31:0] Bit Mask Enable
         */
        __I uint32_t RESERVED2[15];
        __IO uint32_t TPRI;
        /**0xE0000E40
         * Trace Privilege
         */
        __I uint32_t RESERVED3[15];
        __IO uint32_t TCR;
        /**0xE0000E80
         * Trace Control Register
         * ITMENA [0] Enable ITM
         */
        __I uint32_t RESERVED4[29];
        __IO uint32_t IWR;
        /**0xE0000EF8
         * Integration Write
         */
        __IO uint32_t IRD;
        /**0xE0000EFC
         * Integration Read
         */
        __IO uint32_t IMC;
        /**0xE0000F00
         * Integration Mode Control
         */
        __I uint32_t RESERVED5[43];
        __IO uint32_t LAR;
        /**0xE0000FB0
         * Lock Access Register
         */
        __IO uint32_t LSR;
        /**0xE0000FB4
         * Lock Status Register
         */
        __I uint32_t RESERVED6[6];
        __IO uint32_t PID4;	// 0x04
        __IO uint32_t PID5;	// 0x00
        __IO uint32_t PID6;	// 0x00
        __IO uint32_t PID7;	// 0x00
        __IO uint32_t PID0;	// 0x01
        __IO uint32_t PID1;	// 0xB0
        __IO uint32_t PID2;	// 0x1B
        __IO uint32_t PID3;	// 0x00
        /**0xE0000FD0
         */
        __IO uint32_t CID0;	// 0x0D
        __IO uint32_t CID1;	// 0xE0
        __IO uint32_t CID2;	// 0x05
        __IO uint32_t CID3;	// 0xB1
        /**0xE0000FF0
         */

        void PutChar(uint32_t wValue)
        {
            while (SP[0] == 0);
            SP[0] = wValue;
        }
    } ITM_Typedef;

    typedef struct
    {
        __IO uint32_t DHCSR;
        /**0xE000EDF0
         * Debug Halting Control and Status Register
         * ResetValue:0x00000000
         */
        __IO uint32_t DCRSR;
        /**0xE000EDF4
         * Debug Core Register Selector Register
         */
        __IO uint32_t DCRDR;
        /**0xE000EDF8
         * Debug Core Register and Data Register
         */
        __IO uint32_t DEMCR;
        /**0xE000EDFC
         * Debug Exception and Monitor Control Register
         * ResetValue:0x00000000
         */

        // ����
        void Init(void)
        {
            DEMCR |= TRACE_ITM_TRCENA;
        }

        void finline PutChar(uint32_t wValue)
        {
            if (DEMCR & TRACE_ITM_TRCENA)
            {
                MCUITM.PutChar(wValue);
            }
        }
    } TRACE_Typedef;
}
//!
//! @{
using mcucore::MCUInfo_Typedef;
using mcucore::ITM_Typedef;
using mcucore::TRACE_Typedef;
using mcucore::DevInfo_Typedef;
//! @}

#endif /* __STM32GLIBC_TRACE_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
