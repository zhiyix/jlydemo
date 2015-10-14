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

#ifndef __FREEMODBUS_MODBUS_MODBUS__
#define __FREEMODBUS_MODBUS_MODBUS__

/*============================ INCLUDES ======================================*/
//! \brief import head files
#include "./include/mb.h"
//! \brief configuration
#include "./include/mbconfig.h"
#include "./include/mbframe.h"
#include "./include/mbproto.h"
#include "./include/mbfunc.h"

#include "./include/mbutils.h"

#include "./include/mbport.h"

#if MB_SLAVE_RTU_ENABLED == 1 || MB_MASTER_RTU_ENABLED == 1
#include "./rtu/mbrtu.h"
#endif
#if MB_SLAVE_ASCII_ENABLED == 1 || MB_MASTER_ASCII_ENABLED == 1
#include "./ascii/mbascii.h"
#endif
#if MB_SLAVE_TCP_ENABLED == 1 || MB_MASTER_TCP_ENABLED == 1
#include "./tcp/mbtcp.h"
#endif

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
#include "./include/mb_m.h"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
/*============================ INCLUDES ======================================*/

#endif
