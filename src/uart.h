/*******************************************************************
 File  : uart.h
 Coder : sunyt
*******************************************************************/

#pragma once

#ifndef __UART_H__
#define __UART_H__

#define RECV_BUF_SIZE       32

//#define DBG(CODE)
#define DBG(CODE) CODE

#define RX_BYTE_HEAD        0x30    // '0'
#define RX_BYTE_END         0x31    // '1'
#define RX_CMD_A            0x41    // 'A'
#define RX_CMD_B            0x42    // 'B'
#define RX_CMD_C            0x43    // 'C'


#define MY_NAN 7FC00000L        // Not A Number

extern uchar g_ucRxBuff[];

extern void UartHandle( void );

extern void UartInit( void );

/* in the file <avr/pgmspace.h>:
 # define PSTR(s) (__extension__({static char __c[] PROGMEM = (s); &__c[0];}))
 */
#define pgmputs(str) putstr_p( PSTR( str ))
extern void putstr_p( const char* );
extern void putstr( char* );
extern void putch( char );
extern uchar putuc( uchar );
extern uchar putsc( schar );
extern uchar putun( uint );
extern uchar putsn( sint );
extern uchar putul( ulong );
extern uchar putsl( slong );
extern void putf( float );

uchar putunAppendSpace( uint unSrc, uchar ucAllLen );


extern uchar ucCmdBuff[RECV_BUF_SIZE];
extern bool bIsCmdExist;

#endif /* __UART_H__ */
