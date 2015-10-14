/***************************************************************************
 *   Copyright(C)2009-2012 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __BTBUS_H__
#define __BTBUS_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "main.h"

// #include "mb.h"
// #include "mbport.h"

/*============================ MACROS ========================================*/
/*######--------------------------------######--------------------------------*/
//	<<< Configuration Wizard >>>
//	<<< Use Configuration Wizard in Context Menu >>>

//  <h> Free ModBus Interface (FBI)

//	<e> FreeModBus
#define	BT_SLAVE_MODE_EN				1
//	<o> 设备从机的默认地址
//	   <0-199><#+1>
//	   <i> TIP:从机的默认地址
#define BT_DefaultSlaveAddress			1
// 	<o> 设备通讯速率	 					
//	   <9600=> 9600 波特率    
//	   <38400=> 38400 波特率    
//	   <115200=> 115200 波特率
#define BT_DefaultDeviceBaudRate		115200
//	</e>

//	<e> (01) Coils Register
//	   <i> TIP:线圈寄存器。
#define	COILS_EN						1
//	<o> 线圈起始地址
//	   <0x0-0x10>
#define REG_COILS_START       			0x0000
// 	<o> 线圈数量
#define REG_COILS_SIZE        			16
//	</e>

//	<e> (02) Discrete Register
//	   <i> TIP:开关寄存器。
#define	DISCRET_EN						1
//	<o> 开关寄存器起始地址
//	   <0x0-0x10>
#define REG_DISCRETE_START    			0x0000
// 	<o> 开关寄存器数量
#define REG_DISCRETE_SIZE     			16
//	</e>

//	<e> (03) Holding Register
//	   <i> TIP:输入寄存器。
#define	HOLDING_EN						1
//	<o> 保持寄存器起始地址
//	   <0x00-0x1000:0x10>
#define REG_HOLDING_START				0x0001
// 	<o> 保持寄存器数量
//	   <i> MAXSingleQuantity=BT_PDU_FUNC_READ_REGCNT_MAX
//	   <16-1024:16>
#define REG_HOLDING_NREGS				64
// 	<o> 保持寄存器内存大小(2Byte)
//	   <16-1024:16>
#define REG_HOLDING_LENGTH				1024
//	</e>

//	<e> (04) Input Register
//	   <i> TIP:输入寄存器。
#define	INPUT_EN						1
//	<o> 输入寄存器起始地址
//	   <0x00-0x1000:0x10>
#define REG_INPUT_START					0x0
// 	<o> 输入寄存器数量
//	   <i> MAXSingleQuantity=BT_PDU_FUNC_READ_REGCNT_MAX
//	   <0x0-0x1000:0x100>
#define REG_INPUT_NREGS					0x0100
// 	<o> 输入寄存器内存大小(2Byte)
//	   <16-1024:16>
#define REG_INPUT_LENGTH				2048
//	</e>

//	<e> (99) LEDROW Register
//	   <i> TIP:LED排数。
#define	LED_ROW_EN						1
// 	<o> LED排数寄存器数量 				
//	   <24=> 23 Bytes    
// 	   <28=> 27 Bytes
#define REG_LED_ROW_NREGS				24
//	</e>

//  <q1.4> DRP: Data Read Protocol
// 	  	   <i> EBI_MCR:  Data Read Protocol
//         <0=> Standard Read
//         <1=> Early Read
#define BT_DataReadProtocol 			_WDWORD(0xFFE00024, 0x00000010) 

//  </h>

//	<<< end of configuration section >>>

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

#ifdef __cplusplus
extern "C" {
#endif
//!
int btbus_init(void);
int btbus_main(void);
//!
#ifdef __cplusplus
}
#endif

#endif
/* EOF */
