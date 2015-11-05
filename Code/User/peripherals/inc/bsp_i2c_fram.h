#ifndef __I2CFRAM_H
#define	__I2CFRAM_H

#include "stm32l1xx.h"

/* 
 * FM24CL16B 16kb = 16384bit = 16384/8 Byte = 2048 Byte
 *  pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */


void Fram_Test(void);
void ChannelDataDeal(uint8_t channelnum,uint8_t clockchoose,uint8_t Gpschoose);
void SaveHisDataToFram(void);
void ReadFramHisDataToRam(void);
void Down_HisData(void);

void SaveHisDataToFlash(void);
void DownFlash_HisData(void);


uint16_t ReadU16Pointer(const uint16_t PointerAddr);
void WriteU16Pointer(const uint16_t PointerAddr,uint16_t Pointer);

uint32_t ReadU32Pointer(const uint32_t PointerAddr);
void WriteU32Pointer(const uint32_t PointerAddr,uint32_t Pointer);
#endif /* __I2C_EE_H */
