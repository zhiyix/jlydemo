/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_usart.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : USART firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_usart.h"
#include "stm32glibc_rcc.h"
#include ".\template.h"


namespace periph_c
{

    /**
      * @brief  Description "Initializes the USARTx peripheral according to the specified
      *                      parameters in the USART_InitStruct ."
      * @param  None
      * @retval None
      */
    bool USART_Typedef::Config(u32 ulBaudRate,
                               u32 eMode,
                               USART_WordLength eDataBits,
                               USART_Parity eParity,
                               USART_StopBits eStopBits)
    {
        RCC_ClocksTypeDef RCC_ClocksStatus;
        u32 RCC_PeriphID = 0;
		u32 CUSART_BASE = (u32)this;
		
        if (!this->__GetClockID(RCC_PeriphID, CUSART_BASE))
            return false;
        if (CUSART_BASE == USART1_BASE)
        {
            CLOCK.APB2PeriphClockCmd(RCC_PeriphID, ENABLE);
        }
        else
        {
            CLOCK.APB1PeriphClockCmd(RCC_PeriphID, ENABLE);
        }
        CLOCK.GetClocksFreq(RCC_ClocksStatus);

        // ��ʧ��
        this->Disable();
        // ���ò����ʷ�����
        if (CUSART_BASE == USART1_BASE)
        {
            this->__BaudRate(ulBaudRate, RCC_ClocksStatus.PCLK2_Frequency);
        }
        else
        {
            this->__BaudRate(ulBaudRate, RCC_ClocksStatus.PCLK1_Frequency);
        }
        // ��������λ
        if (eParity == USART_PARITY_NO)
        {
            // У��λ
            this->__Parity(USART_PARITY_NO);
            this->__WordLen(USART_WORDLENGTH_8DB);
        }
        else
        {
            // У��λ
            this->__Parity(eParity);
            this->__WordLen(USART_WORDLENGTH_9DB);
        }
        // ֹͣλ
        this->__StopBits(eStopBits);
        // ����ʹ��
        if (eMode & USART_Mode_Rx)
        {
            this->USART_Cmd(RE, ENABLE);
        }
        // ����ʹ��
        if (eMode & USART_Mode_Tx)
        {
            this->USART_Cmd(TE, ENABLE);
        }
        // ʹ�ܴ���
        this->Enable();
        return true;
    }

    /**
      * @brief  Description "���ô��ڵ��ж�"
      * @param  None
      * @retval None
      */
    void USART_Typedef::InterruptConfig(USART_Int IntBits, FunctionalState e)
    {
        CR1 = (e == DISABLE) ? ClearBBit((CR1), IntBits) : SetBBit((CR1), IntBits);
    }
    /**
      * @brief  Description "������ʹ��/ʧ��"
      * @param  None
      * @retval None
      */
    void USART_Typedef::USART_Cmd(u32 flg, FunctionalState e)
    {
        CR1 = (e == DISABLE) ? ClearBBit((CR1), flg) : SetBBit((CR1), flg);
    }
}

/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
