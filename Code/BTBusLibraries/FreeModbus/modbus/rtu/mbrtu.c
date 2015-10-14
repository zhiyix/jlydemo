/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbrtu.c,v 1.18 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
// #include "port.h"
#include "..\app_cfg.h"

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
/* ----------------------- Modbus includes ----------------------------------*/
// #include "mb.h"
// #include "mb_m.h"
// #include "mbrtu.h"
// #include "mbframe.h"

// #include "mbcrc.h"
// #include "mbport.h"

#include "./mbrtu.h"
#include "./mbcrc.h"

#if MB_SLAVE_RTU_ENABLED > 0
/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */

/*============================ MACROS ========================================*/
/*######--------------------------------######--------------------------------*/
//	<<< Configuration Wizard >>>
//	<<< Use Configuration Wizard in Context Menu >>>

//  <h> AT Stack RTU Config

// 	<o> CRC校验字节大小	
//	   <i> Size of CRC field in PDU.			
//	   <0=> 无校验 
//	   <1=> 1字节校验
//	   <2=> 2字节校验
#define MB_SER_PDU_SIZE_CRC				2

//	<o> PDU正文地址偏移量
//	   <i> Offset of Slave Address in Ser-PDU.
//	   <0-10>
#define MB_SER_PDU_ADDR_OFF    			0x0000
//	<o> PDU正文内容偏移量
//	   <i> Offset of Modbus-PDU in Ser-PDU.
//	   <1-10>
#define MB_SER_PDU_PDU_OFF     			0x0001

//  <q1.4> DRP: Data Read Protocol
// 	  	   <i> EBI_MCR:  Data Read Protocol
//         <0=> Standard Read
//         <1=> Early Read
#define MB_DataReadProtocol 			_WDWORD(0xFFE00024, 0x00000010) 

//  </h>

//	<<< end of configuration section >>>

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_ERROR              /*!< If the frame is invalid. */
} eMBRcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_XMIT               /*!< Transmitter is in transfer state. */
} eMBSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBSndState eSndState;
static volatile eMBRcvState eRcvState;

volatile UCHAR  ucRTUBuf[MB_SER_PDU_SIZE_MAX];

//发送缓冲队列
static volatile UCHAR *pucSndBufferCur;
//发送缓冲队列中 数据
static volatile USHORT usSndBufferCount;
//接收缓冲队列中 位置
static volatile USHORT usRcvBufferPos;

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBRTUInit( UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ULONG           usTimerT35_50us;

    ( void )ucSlaveAddress;
    ENTER_CRITICAL_SECTION(  );

    /* Modbus RTU uses 8 Databits. */
    if( xMBPortSerialInit( ucPort, ulBaudRate, 8, eParity ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        //如果波特率超过19200 使用固定的时间间隔，1750us
        //其他情况，则要进行计算。
        if( ulBaudRate > 19200 )
        {
            usTimerT35_50us = 35;       /* 1750us. */
        }
        else
        {
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
        }
        
        //初始化定时器
        if( xMBPortTimersInit( ( USHORT ) usTimerT35_50us ) != TRUE )
        {
            eStatus = MB_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

void
eMBRTUStart( void )
{
    ENTER_CRITICAL_SECTION(  );
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    //eRcvState 初始化状态
    eRcvState = STATE_RX_INIT;
    //使能接收，禁止发送
    vMBPortSerialEnable( TRUE, FALSE );
    //启动定时器
    vMBPortTimersEnable();

    EXIT_CRITICAL_SECTION( );
}

void
eMBRTUStop( void )
{
    ENTER_CRITICAL_SECTION(  );
    //禁止接收，禁止发送
    vMBPortSerialEnable( FALSE, FALSE );
    vMBPortTimersDisable(  );
    EXIT_CRITICAL_SECTION(  );
}

eMBErrorCode
eMBRTUReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION(  );
    assert( usRcvBufferPos < MB_SER_PDU_SIZE_MAX );

    /* Length and CRC check */
    if( ( usRcvBufferPos >= MB_SER_PDU_SIZE_MIN )
        && ( usMBCRC16( ( UCHAR * ) ucRTUBuf, usRcvBufferPos ) == 0 ) )
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = ucRTUBuf[MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = ( USHORT )( usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC );

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = ( UCHAR * ) & ucRTUBuf[MB_SER_PDU_PDU_OFF];
    }
    else
    {
        eStatus = MB_EIO;
		xMBOutput(1, "MB_EIO:eMBRTUReceive:\r\n");
    }

    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eMBErrorCode
eMBRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;

    ENTER_CRITICAL_SECTION(  );

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( eRcvState == STATE_RX_IDLE )
    {
        /* First byte before the Modbus-PDU is the slave address. */
        pucSndBufferCur = ( UCHAR * ) pucFrame - 1;
        usSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        pucSndBufferCur[MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16( ( UCHAR * ) pucSndBufferCur, usSndBufferCount );
        ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 & 0xFF );
        ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 >> 8 );

        /* Activate the transmitter. */
        //发送状态转换，在中断中不断发送
        eSndState = STATE_TX_XMIT;
        
        //启动第一次发送，这样才可以进入发送完成中断
        xMBPortSerialPutByte( ( CHAR )*pucSndBufferCur );
        pucSndBufferCur++;  /* next byte in sendbuffer. */
        usSndBufferCount--;
        
        //使能发送状态，禁止接收状态
        vMBPortSerialEnable( FALSE, TRUE );
    }
    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
	
	if (eStatus == MB_ENOERR)
	{
		//MB_STATUS_IND(MB_STA_IND_UTX, TRUE);
	}
    return eStatus;
}

BOOL
xMBRTUReceiveFSM( void )
{
    BOOL            xTaskNeedSwitch = FALSE;
    UCHAR           ucByte;

    assert( eSndState == STATE_TX_IDLE );
	
	//MB_STATUS_IND(MB_STA_IND_URXIRQ, TRUE);

    //读串口接收数据，实际上该函数在串口接收中断中被执行
    /* Always read the character. */
    ( void )xMBPortSerialGetByte( ( CHAR * ) & ucByte );
    
    //根据不同的状态转移
    switch ( eRcvState )
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_RX_INIT:
        vMBPortTimersEnable( );
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_RX_ERROR:
        vMBPortTimersEnable( );
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE.
         */
    case STATE_RX_IDLE:
        //接收到一个数据，保存串口数据，重启定时器
        usRcvBufferPos = 0;
        ucRTUBuf[usRcvBufferPos++] = ucByte;
        //状态转移，数据接收中
        eRcvState = STATE_RX_RCV;

        /* Enable t3.5 timers. */
        //开启定时器，相当于重启定时器
        vMBPortTimersEnable();

		//MB_STATUS_IND(MB_STA_IND_URX, TRUE);
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_RX_RCV:
        if( usRcvBufferPos < MB_SER_PDU_SIZE_MAX )
        {
            ucRTUBuf[usRcvBufferPos++] = ucByte;
        }
        else
        {
            eRcvState = STATE_RX_ERROR;
        }
        //开启定时器，相当于重启定时器
        vMBPortTimersEnable();
        break;
    }
	//MB_STATUS_IND(MB_STA_IND_URXIRQ, FALSE);
    return xTaskNeedSwitch;
}

BOOL
xMBRTUTransmitFSM( void )
{
    BOOL            xNeedPoll = FALSE;

    assert( eRcvState == STATE_RX_IDLE );

	//MB_STATUS_IND(MB_STA_IND_UTXIRQ, TRUE);
    switch ( eSndState )
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        //发送处于空闲状态，使能接收，禁止发送
        vMBPortSerialEnable( TRUE, FALSE );
        break;

    case STATE_TX_XMIT:
        /* check if we are finished. */
        //检查发送是否完成
        if( usSndBufferCount != 0 )
        {
            //发送数据
            xMBPortSerialPutByte( ( CHAR )*pucSndBufferCur );
            pucSndBufferCur++;  /* next byte in sendbuffer. */
            usSndBufferCount--;
        }
        else
        {
            //传递任务，发送完成
            xNeedPoll = xMBPortEventPost( EV_FRAME_SENT );
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            //接收使能，发送禁止
            vMBPortSerialEnable( TRUE, FALSE );
            //状态改变，发送空闲
            eSndState = STATE_TX_IDLE;
			//MB_STATUS_IND(MB_STA_IND_UTX, FALSE);
        }
        break;
    }

	//MB_STATUS_IND(MB_STA_IND_UTXIRQ, FALSE);
    return xNeedPoll;
}

BOOL
xMBRTUTimerT35Expired( void )
{
    BOOL            xNeedPoll = FALSE;


	//MB_STATUS_IND(MB_STA_IND_TIMEOUT, TRUE);
    switch ( eRcvState )
    {
        /* Timer t35 expired. Startup phase is finished. */
        //这是一个启动状态，运行到这里说明启动状态完成。
    case STATE_RX_INIT:
        xNeedPoll = xMBPortEventPost( EV_READY );
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
        //当串口处于数据接收状态，此时若进入定时器超时中断，说明接收到完整的modbus数据
    case STATE_RX_RCV:
        //发送事件，接收到完整的modbus数据
        xNeedPoll = xMBPortEventPost( EV_FRAME_RECEIVED );
        break;

        /* An error occured while receiving the frame. */
    case STATE_RX_ERROR:
        break;

        /* Function called in an illegal state. */
    default:
        assert( ( eRcvState == STATE_RX_INIT ) ||
                ( eRcvState == STATE_RX_RCV ) || ( eRcvState == STATE_RX_ERROR ) );
		break;
    }
    //串口接收状态 变为空闲状态。
    eRcvState = STATE_RX_IDLE;
	//MB_STATUS_IND(MB_STA_IND_URX, FALSE);
    

	//MB_STATUS_IND(MB_STA_IND_UTX, FALSE);
    //禁止定时器
    vMBPortTimersDisable(  );

	//MB_STATUS_IND(MB_STA_IND_TIMEOUT, FALSE);
    return xNeedPoll;
}
#endif

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
