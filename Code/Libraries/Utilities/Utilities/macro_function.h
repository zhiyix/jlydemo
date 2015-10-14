/**
  ******************************************************************************
  * @file              : macro_function.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2014-11-21
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
#ifndef MACRO_FUNCTION
#define MACRO_FUNCTION


/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdio.h>
//! @}

/*============================ MACROS ========================================*/
#undef  BIT
#define BIT(x)					((uint32_t)0x01L<<(x))

#undef  Bit
#define Bit(x)					((uint32_t)0x01L<<(x))

/**
  * @brief  Description "�õ�һ��ָ����Χ��BIT����λ��ֵ"
  * @param  None
  * @retval BitFromTo(3, 0)	��õ� 0x0f
  * @note 	1. �������Ϊ��߿ɶ��Բ�д��, ���ڳ��������Ǳ��ʽ��, ���������Զ��Ż�.
  */
#define BitFromTo(h, l)															\
		((h > l)? ((BIT(h) - BIT(l) + BIT(h))): (BIT(l) - BIT(h) + BIT(l)) )
#define BitF2	BitFromTo
#define BitFT	BitFromTo


/**
  * @brief  Description "ʵ��д8λ���������ĺ�"
  * @param  b	�Զ����Ƹ�ʽд�ĳ���, �����Գ���һ���ֽ�(8λ)
  * @retval ��BByte(01011010) == 0x5a
  */
#define BByte(b)	(						\
	(((( 0x##b##L )			) & 1 )<<0)	|	\
	(((( 0x##b##L )	>>4		) & 1 )<<1)	|	\
	(((( 0x##b##L )	>>8		) & 1 )<<2)	|	\
	(((( 0x##b##L )	>>12	) & 1 )<<3)	|	\
	(((( 0x##b##L )	>>16	) & 1 )<<4)	|	\
	(((( 0x##b##L )	>>20	) & 1 )<<5)	|	\
	(((( 0x##b##L )	>>24	) & 1 )<<6)	|	\
	(((( 0x##b##L )	>>28	) & 1 )<<7)		\
	)

/**
  * @brief  Description "��һ����������8λ������ÿһλ��ָ���ı���չ��"
  * @param  var		����
  * @param  n		ָ���ı���, ע��չ���󲻿��Գ���32λ, ����n�ĺϷ���ֵӦ���� 1 ~ 4 (����1��4)֮��
  * @retval None
  */
#define ExpandU8Bit(num, n)	(													\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(0)))) << (0*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(1)))) << (1*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(2)))) << (2*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(3)))) << (3*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(4)))) << (4*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(5)))) << (5*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(6)))) << (6*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(7)))) << (7*(n-1)))		)



/**
  * @brief  Description "ʵ��д16λ�����Ƶĺ�"
  * @param  h	��8λ
  * @param  l	��8λ
  * @retval BWord(01011010, 10100101) == 0x5aa5
  */
#define BWord(h,l)				( (((uint16_t)(BByte(h)))<<8) + BByte(l))

/*============================ TYPES =========================================*/
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERN FUNCTIONS ==============================*/

/**
  * @brief  Description "����������õ�һ��������ĳ����ĳ����λ����ֵ���ֵ"
  * @param  type		����flag������
  * @param  flag		Ҫ��ֵ�ı���
  * @param  which_bit	����flag���ļ���λҪ����ֵ
  * @param  val			��ʾҪ����flag���Ǽ���λ��ֵ
  * @note   1. ���ں�, �������Ҫ��++i֮��ı��ʽ! Ҳ��Ҫֱ��ʹ�õ��ú����õ��ķ���ֵ!
  * @note   2. ���type�ǼĴ���, ��ô�������ǿɶ���
  * @note   3. Ч����, �������(��flag��)���ǳ���, ���������Զ��Ż�, ��������Ǳ���, �����ֶ�д����Ҳ��������ӵ�
  * @note   4. (which_bit) ������Ϊ�˱�֤ AssignBit(flag, BIT0 | BIT1, 0)֮����÷��������
  * @examp 	u8 a = 0xf0;
  *			AssignBit(u8, a, BIT4 | BIT0, BIT0);
  *			ִ�����a == 0xe1;
  */
#define AssignBit(type ,flag , which_bit, val)									\
		((((type)which_bit)&((type)val)) | ((flag)&(~((type)which_bit))))
#define AssignFlag	AssignBit


/**
  * @brief  Description "�õ�һ������ĳ��λλ����λ���ֵ"
  * @param  None
  * @note 	1. ���flag�ǼĴ���, �����ǿɶ���
  */
#define SetBit(type, flag, which_bit)			((flag) | ((type)which_bit))
#define SetB	SetBit


/**
  * @brief  Description "����λ, flag������which_bit������λ����0"
  * @param  None
  * @note 	1. flag�����ǿ���������д��
  */
#define ClearBit(type, flag, which_bit)			((flag) & (~((type)which_bit)))
#define ClearB	ClearBit


/**
  * @brief  Description "���flag�е�ĳһλ�Ƿ���λ, �������λ, "
  * @brief			    "����ֵΪ��0(��һ����1), ���û�б���λ, ����ֵΪ0"
  * @param  None
  * @note 	1. flag�����ǿ��Զ���
  */
#define CheckBit(type, flag, which_bit)			( (flag) & ((type)which_bit) )


/**
  * @brief  Description "�õ�������һ��λ�α���ֵ���ֵ, ����걾����ı������ֵ"
  * @param  type	����var������
  * @param  var		����
  * @param  LSB		��λ�ε����λ
  * @param  len		λ�εĳ��ȣ����ֵΪ8
  * @param  val		λ�ε�ֵ
  * @note 	���var��һ���Ĵ���, ��ô�������������ɶ���
  */
#define AssignBitSection(type, var, LSB, length, value)							\
		AssignBit(type, var, ((1<<(length)) - 1)*LSB, (value)*LSB)
#define AssignBS	AssignBitSection


#endif /* MACRO_FUNCTION */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


