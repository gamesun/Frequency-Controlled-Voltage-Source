/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __UART_H__
#define __UART_H__

#define RECV_BUF_SIZE       32

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
extern uchar putf( float );
extern uchar putunAppendSpace( uint unSrc, uchar ucAllLen );
extern uchar putfAppendSpace( float fSrc, uchar ucAllLen );

extern uchar ucCmdBuff[RECV_BUF_SIZE];
extern bool bIsCmdExist;

#endif /* __UART_H__ */
