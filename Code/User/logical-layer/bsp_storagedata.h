#ifndef __STORAGEDATA_H
#define	__STORAGEDATA_H

#include "stm32l1xx.h"

/* 
 * FM24CL64B 64kb = 65536bit = 65536/8 Byte = 8192 Byte
 *  pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

void SetFlashOverFlow(uint8_t flowvalue);

void ReadFramHisDataToRam(void);
void Down_HisData(void);

void DownFlash_HisData(void);


uint16_t ReadU16Pointer(const uint16_t PointerAddr);
void WriteU16Pointer(const uint16_t PointerAddr,uint16_t Pointer);

uint32_t ReadU32Pointer(const uint32_t PointerAddr);
void WriteU32Pointer(const uint32_t PointerAddr,uint32_t Pointer);

void SaveDataOnTimeDeal(void);
//void StorageHistoryData(void);
#endif /* __STORAGEDATA_H */
