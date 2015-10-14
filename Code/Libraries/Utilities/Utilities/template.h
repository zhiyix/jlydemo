/**
  ******************************************************************************
  * @file              : template.h
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
#ifndef TEMPLATE_H
#define TEMPLATE_H

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
//! @}

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERN FUNCTIONS ==============================*/
/**
  * @brief  Description "�õ�һ������bits��������ļ���λ�����λ"
  * @exampl u16 a = BByte(00111000); ��ôGetLSB(a)�᷵�� BByte(00001000)
  * @retval None
  */
template<class B>
__forceinline B GetLSB(B bits)
{
    return (B)(((bits << 1) | bits) ^ (bits << 1));
}


/**
  * @brief  Description "�õ�һ������ĳ��λλ����λ���ֵ, �����������ı���������ֵ"
  * @note  	����ͨ����, ��ģ��ķ�ʽ�����һ��. ������Ҫ����������optimize for time, ���������һ������
  * @note  	��ɴ����ʱ���ϵ��˷�. (˵�����˷�����Ϊ���ݲ����Ĺ��̱ȼ���Ĺ��̸���...)
  * @note 	1. ���flag�ǼĴ���, �����ǿɶ���
  */
template<class Flag_Type, class W>
__forceinline Flag_Type SetBBit(Flag_Type flag, W whichBit)
{
    return ((flag) | ((Flag_Type)whichBit));
}


/**
  * @brief  Description "����λ, flag������which_bit������λ����0"
  * @note   1. flag�����ǿ���������д��
  */
template<class Flag_Type, class W>
__forceinline Flag_Type ClearBBit(Flag_Type flag, W whichBit)
{
    return ((flag) & (~((Flag_Type)whichBit)));
}


/**
  * @brief  Description "���flag�е�ĳЩ�Ƿ���λ, �����Щλ��Ϊ0, �򷵻�0, ���򷵻�����ֵ"
  * @note   1. flag�����ǿ��Զ���
  */
template<class Flag_Type, class W>
__forceinline Flag_Type CheckBBit(Flag_Type flag, W whichBit)
{
    return 	( (flag) & ((Flag_Type)whichBit) );
}


/**
  * @brief  Description "������������õ�һ��������ĳ����ĳ����λ����ֵ���ֵ"
  * @param  flag	Ҫ��ֵ�ı���
  * @param  which_bit ����flag���ļ���λҪ����ֵ
  * @param  val		��ʾҪ����flag���Ǽ���λ��ֵ
  * @note	1. ���ں�, �������Ҫ��++i֮��ı��ʽ! Ҳ��Ҫֱ��ʹ�õ��ú����õ��ķ���ֵ!
  * @note	2. ���type�ǼĴ���, ��ô�������ǿɶ���
  * @note	3. Ч����, �������(��flag��)���ǳ���, ���������Զ��Ż�, ��������Ǳ���, �����ֶ�д����Ҳ��������ӵ�
  * @examp 	u8 a = 0xf0;
  *			AssignBit(u8, a, BIT4 | BIT0, BIT0);
  *			ִ�����a == 0xe1;
  */
template<class Flag_Type, class W, class V>
__forceinline Flag_Type AssignBBit(Flag_Type flag, W whichBit, V val)
{
    return ((((Flag_Type)whichBit) & ((Flag_Type)val)) | ((flag) & (~((Flag_Type)whichBit))));
}


/**
  * @brief  Description "�õ�������һ��λ�α���ֵ���ֵ, �������������ı������ֵ"
  * @param  var		����
  * @param  LSB		��λ�ε����λ
  * @param  len		λ�εĳ��ȣ����ֵΪ8
  * @param  val		λ�ε�ֵ
  * @note	���var��һ���Ĵ���, ��ô�������������ɶ���
  */
template<class Flag_Type, class LSB, class L, class V>
__forceinline Flag_Type AssignBField(Flag_Type flag, LSB lsb, L len, V val)
{
    return AssignBBit(flag, ((1 << (len)) - 1) * ((Flag_Type)lsb), (Flag_Type)val * ((Flag_Type)lsb));
}


/**
  * @brief  Description "�õ�һ��������һ�� bit field ������Ϊָ����ֵ��ı���ֵ, �������������ı������ֵ"
  * @brief  Description "�õ���������������λ����ֵ���ֵ, �����������ı䴫��ı�����ֵ"
  * @param  flag	����
  * @param  bits	�����ļ���λ
  * @param  val		bits��ʾ�ļ���λҪ������ֵ
  * @note	val ����ҪԤ����λ, ���������assignb��ʹ�ò�ͬ
  * @examp 	u16 a = 0;	a = assignbs(a, BitFromTo(11, 4), 0x5a);
  *			��ôִ�����a = 0x05a0;
  */
template<class Flag_Type, class BITS, class V>
__forceinline Flag_Type AssignBField(Flag_Type flag, BITS bits, V val)
{
    return AssignBBit(flag, bits, (((Flag_Type)val) * ((Flag_Type)( GetLSB(bits) ))));
}

#define	assignf		AssignBField			// assign bit field


/**
  * @brief  Description "��һ��������һ�� bit field ��ֵ"
  * @param  None
  * @retval None
  */
template<class Flag_Type, class BITS>
__forceinline Flag_Type ReadField(Flag_Type flag, BITS bits)
{
    return (Flag_Type)((flag & bits) / (Flag_Type)GetLSB(bits));
}

#define readf		ReadField


/**
  * @brief  Description "�������letter��һ��Сд��ĸ, ʹ������������Եõ���Ӧ�Ĵ����ĸ(����ı������ֵ)"
  * @param  None
  * @retval None
  */
__forceinline uint8_t getupper(uint8_t letter)
{
    return 	((((letter) >= 'a') && ((letter) <= 'z')) ? ((letter) - 'a' + 'A') : (letter) );
}


/*******************************************************************************
2012.3.7
����getupperδ���ԣ�����������ͨ�������Դ��룺��������Ԥ��һ�£�

�����������Ŀ���ǲ��ñ��������Ҳ��Եļ��������Ż�����
void fun(u32 a, u32 b, u32 c, u32 d, u32 e)
{

}
int main(void)
{
	u32 a, b, c, d = 0xffffffff;
	a = assignb(a, Bit(4), 0xff);
	b = assignbs(b, Bit(4), 4, 0xf);
	c = setb(c, Bit(0));
	d = clearb(d, c);
	fun(a, b, c, d, 0);
��

2012.3.10
	��������һ�����ܺ�����toggleb���������ķ�ʽʵ�֡�
*******************************************************************************/

#endif /* TEMPLATE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

