/**
  ******************************************************************************
  * @file              : 
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 201x STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFINFOR_H
#define __CONFINFOR_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32l1xx.h"

//! @}

/*============================ MACROS ========================================*/
//! @{
//! \brief ����������
#define NULL        0   //δ���봫����
#define SENSOR_TEMP 1   //�¶ȴ�����
#define SENSOR_HUMI 2   //ʪ�ȴ�����
//PT
//SHT
//��������������
//! @}

/*============================ TYPES =========================================*/
//! \brief�����������ݵ�ַ�� (Display="Hex",ADDRESS_OFFSET=0x0000):
struct BasicConfDataStr
{
	uint16_t sn_low;	//���к� SN��
	uint16_t sn_mid;
	uint16_t sn_high;
	uint16_t :16;
	
	uint16_t soft_verml;	//����汾 MID LOW
	uint8_t	 :8;			//����
	uint16_t soft_vermain;	//����汾 Main
	
	uint16_t hard_verml;	//Ӳ���汾 MID LOW
	uint8_t	 :8;			//����
	uint16_t hard_vermain;	//Ӳ���汾 Main
	
	uint16_t commu_mode;	//ͨѶ��ʽ
	/*�󱸵��״̬˵��
	 *@1Bit[1:0]:���״̬
     *@2Bit[6:4]:��ص���״̬
     */
	uint16_t battery_state;	//�󱸵��״̬
	
	uint16_t :16;			//Reserv
	uint16_t :16;
	uint16_t :16;
	uint16_t :16;
	uint16_t :16;
	uint16_t :16;
};
//! \brief��¼���������ݵ�ַ��(Display="Hex" ,ADDRESS_OFFSET=0x0100):
struct JlyConfDataStr
{
	/*������ʽ˵��
	 *@0x00:��ʱ����(Default)
	 *@0x01:ʱ��㶨ʱ����
	 *@0x02:ʱ��㶨����ͣ
	 *@0x10:�쳣��������
     */
    uint8_t recbootmode;  //������ʽ
	/*��¼��ʽ˵��
	 *@0x00:�Ƚ��ȳ�(Default)
	 *@0x01:����ֹͣ
	 *@0x02:����ֹͣ
     */
    uint8_t recmode;  //��¼��ʽ
	
	
};
//�����ȸ�����
union FISI2CH4
{
    float     ft;
    int       wd;
    short int hwd[2];
    char      byte[4];
};
//������ͨ����Ϣ�ṹ
struct SENSORINFOR
{
    uint8_t sensor_type;        //����������
    uint8_t alarm_switch;       //��������
    uint8_t alarm_status;       //����״̬
    uint8_t reserved;           //����
    uint32_t  idennum;          //������ʶ���
    union FISI2CH4 alarm_high;  //��������
    union FISI2CH4 alarm_low;   //��������
};
//������ͨ��1��Ϣ��
union SENSEPEIZHIINFOR
{
    struct  SENSORINFOR sensor1_str;
    uint8_t sensor_infor[16];
};

//adcУ׼���ݵ�
struct ADCADJUST
{
    uint16_t adjustf20;
    uint16_t adjustf10;
    uint16_t adjust10;
    uint16_t adjust20;
    uint16_t adjust30;
    uint16_t adjust40;
    uint16_t adjust50;
    uint16_t adjust60;
};
//adcУ׼���ݱ�
union ADCADJUSTINFOR
{
    struct ADCADJUST adc;
    uint8_t adcadjust_infor[16];
};
//ϵͳ��Ϣ�ṹ
struct SYSPEIZHI
{
    uint8_t     :8;                 //����״̬
    uint16_t    rec_fram_memory;    //��¼�洢Fram��С
    uint32_t    rec_flash_memory;   //��¼�洢flash��С
    uint8_t     :8;                 //����
    uint32_t    password:24;        //�������
    uint32_t    soft_ver;           //����汾��
    uint32_t    hard_ver;           //Ӳ���汾��
    uint8_t     :8;                 //����
    
    uint8_t     username[16];       //�û���
    uint8_t     *title[11];         //����
    uint8_t     *use_fun[5];        //��¼����;
    
    uint16_t    recboot_year;       //��¼����ʱ��-��
    uint8_t     recboot_month;      //��¼����ʱ��-��
    uint8_t     recboot_day;        //��¼����ʱ��-��
    uint8_t     recboot_hour;       //��¼����ʱ��-ʱ
    uint8_t     recboot_min;        //��¼����ʱ��-��
    uint8_t     recboot_sec;        //��¼����ʱ��-��
    uint8_t     recgap_hour;        //��¼���ʱ��-ʱ
    uint8_t     recgap_min;         //��¼���ʱ��-��
    uint8_t     recgap_sec;         //��¼���ʱ��-��    
    
    
    uint8_t     delayboot_hour;     //��ʱ����ʱ��-ʱ
    uint8_t     delayboot_min;      //��ʱ����ʱ��-��
    uint8_t     delayboot_sec;      //��ʱ����ʱ��-��
    
    
    uint16_t    recframaddr_low:8;     //fram�м�¼����ָ���λ(2�ֽ�)
    uint16_t    recframaddr_high:8;    //fram�м�¼����ָ���λ
    uint32_t    recflashaddr_low:8;    //flash�м�¼����ָ��bit0(4�ֽ�)
    uint32_t    recflashaddr_mid0:8;   //flash�м�¼����ָ��bit1
    uint32_t    recflashaddr_mid1:8;   //flash�м�¼����ָ��bit2
    uint32_t    recflashaddr_high:8;   //flash�м�¼����ָ��bit3
    
    /*��¼�ǹ���״̬˵��
     *���ڼ�¼ 01
     *ֹͣ��¼ 00
     */
    uint8_t     workstatueisstop;  //��¼�ǹ���״̬
    
    uint8_t     powermode;         //��¼�ǹ���ģʽ 00����ģʽ 01�͹���ģʽ(ʡ��ģʽ)
    
    uint8_t     remark1;           //��ע
    uint32_t    remark2;           //��ע
    uint32_t    :32;               //����
    uint32_t    startalarm_chanel;    //��������ͨ��
    uint8_t     downlatestdata;       //������������ʹ��λ
    uint32_t    startsample_chanel;   //�����ܵĲ���ͨ��
    
    uint8_t     ctrlword;             //������
    
    uint32_t    :32;                  //����4�ֽ�
    uint32_t    :32;                  //����4�ֽ�
    
    
};
//�õ���ȫ�ֱ���
struct JLY
{
    uint32_t    save_time;           //��������ʱ��
    uint32_t    delayboot_time;      //��ʱ����ʱ��
    
};
extern union  SENSEPEIZHIINFOR  Sensor1;
extern union  ADCADJUSTINFOR    adcjust1;;
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/


//#ifdef __cplusplus
//}
//#endif

#endif /* __CONFINFOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
