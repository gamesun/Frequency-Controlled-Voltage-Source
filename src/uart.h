/*******************************************************************
 File  : uart.h
 Coder : sunyt
*******************************************************************/

#pragma once

#ifndef __UART_H__
#define __UART_H__



//#define DBG(CODE)
#define DBG(CODE) CODE

#define RX_BYTE_HEAD        0x30    // '0'
#define RX_BYTE_END         0x31    // '1'
#define RX_CMD_A            0x41    // 'A'
#define RX_CMD_B            0x42    // 'B'
#define RX_CMD_C            0x43    // 'C'

#define RECV_BUF_SIZE       32

#define MY_NAN 7FC00000L        // Not A Number

extern uchar g_ucRxBuff[];

extern void CmdHandle( void );

extern void UartInit( void );

extern void putstr( char* );

extern void putuc( uchar );
extern void putsc( schar );
extern void putn( uint );
extern void putsn( sint );
extern void putl( ulong );
extern void putsl( slong );
extern void putf( double );

extern uint g_dbgLCD;


#endif /* __UART_H__ */
