/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32glibc_exti.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_EXTI_H
#define __STM32GLIBC_EXTI_H

#include "stm32glibc_type.h"
#include "template.h"


/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
namespace	periph_c
{
#if (false)
    enum Exti_Line
    {
        EXTI_Line0 = Bit(0), EXTI_Line1 = Bit(1), EXTI_Line2 = Bit(2), EXTI_Line3 = Bit(3),
        EXTI_Line4 = Bit(4), EXTI_Line5 = Bit(5), EXTI_Line6 = Bit(6), EXTI_Line7 = Bit(7),
        EXTI_Line8 = Bit(8), EXTI_Line9 = Bit(9), EXTI_Line10 = Bit(10), EXTI_Line11 = Bit(11),
        EXTI_Line12 = Bit(12), EXTI_Line13 = Bit(13), EXTI_Line14 = Bit(14), EXTI_Line15 = Bit(15),
        EXTI_Line16 = Bit(16), EXTI_Line17 = Bit(17), EXTI_Line18 = Bit(18), EXTI_Line19 = Bit(19),
    };
#endif
    enum Exti_Trigger
    {
        EXTI_Trigger_Rising = 0x08,
        EXTI_Trigger_Falling = 0x0C,
        EXTI_Trigger_Rising_Falling = 0x10,
    };
    typedef struct
    {
        __IO uint32_t IMR;
        __IO uint32_t EMR;
        __IO uint32_t RTSR;
        __IO uint32_t FTSR;
        __IO uint32_t SWIER;
        __IO uint32_t PR;

        // �ⲿ�ж��½��ش���ʹ��/ʧ��
        __forceinline void RisingTrigger(uint16_t lines, FunctionalState e = ENABLE)
        {
            RTSR = (e == DISABLE ? ClearBBit(RTSR, lines) : SetBBit(RTSR, lines));
        }
        // �ⲿ�ж������ش���ʹ��/ʧ��
        __forceinline void FallingTrigger(uint16_t lines, FunctionalState e = ENABLE)
        {
            FTSR = (e == DISABLE ? ClearBBit(FTSR, lines) : SetBBit(FTSR, lines));
        }
        // �����ж�����
        void Trigger(uint16_t lines, FunctionalState fallingTrig = ENABLE, FunctionalState risingTrig = ENABLE)
        {
            RisingTrigger(lines, risingTrig);
            FallingTrigger(lines, fallingTrig);
        }

        /**
          * @brief  Description "ʹ�ܻ�ʧ���ⲿ�ж�"
          * @param  lines	�������ļ����ⲿ�ж�, �����Ǽ�����������
          * @param  en		ENABLEΪʹ�ܣ�DISABLEΪʧ��
          * @retval None
          */
        void Interrupt(uint16_t lines, const FunctionalState e = ENABLE)
        {
            IMR = ((e == DISABLE) ? (ClearBBit(IMR, lines)) : (SetBBit(IMR, lines)));
        }

        // ���жϱ�־
        __forceinline uint16_t ReadPending(uint32_t lines)
        {
            return CheckBBit(PR, lines);   //
        }
        __forceinline void GenerateSWInterrupt(uint32_t lines)
        {
			SWIER = lines;   // д1����
        }
        // ����жϱ�־��ע�����жϴ������˳�ǰһ��Ҫ����жϱ�־
        __forceinline void ClearPending(uint32_t lines)
        {
            PR = lines;   // д1����
        }
    } EXTI_Typedef;
}
//!
//! @{
using periph_c::EXTI_Typedef;
#define EXTICLASS	((EXTI_Typedef *)           EXTI_BASE)
#define IOEXTI		(*EXTICLASS)
//! @}

#endif /* __STM32GLIBC_EXTI_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
