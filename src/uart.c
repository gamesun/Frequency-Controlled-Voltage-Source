/*******************************************************************
 File  : uart.c
 Coder : sunyt
*******************************************************************/

#include "common.h"
#include "uart.h"


static void myputc( char );
static void putx( ulong, ulong );

static uchar st_ucRxBuff[RECV_BUF_SIZE];
static uchar st_ucRxBuffIdx;
static bool  st_bRxCmdEnd;

void CmdHandle( void )
{
    uchar   ucCmdBuff[RECV_BUF_SIZE];
//    uchar   ucLen;
//    uchar   ucCnt;
    D2C     UTmp;
    schar   a;
    double  b;

    if ( st_bRxCmdEnd ){
//      ucLen = st_ucRxBuff[1] - 3;//head,len,end are not needed
//      for ( ucCnt = 0; ucCnt < ucLen; ucCnt++ )
//      {
//          ucCmdBuff[ucCnt] = st_ucRxBuff[ucCnt + 2];
//      }
//      st_bRxCmdEnd = FALSE;



        putstr( (char*)ucCmdBuff );

        switch( ucCmdBuff[0] ){
        case RX_CMD_A:
            a = *(schar*)&ucCmdBuff[1];
            putstr( "\na:" );
            putsc( a );
            break;
        case RX_CMD_B:
            UTmp.c[0] = ucCmdBuff[1];
            UTmp.c[1] = ucCmdBuff[2];
            UTmp.c[2] = ucCmdBuff[3];
            UTmp.c[3] = ucCmdBuff[4];
            b = UTmp.d;
            putstr( "\nb:" );
            putf( b );
            break;
        case RX_CMD_C:
            break;
        default:
            break;
        }
    }
}

ISR( USART_RXC_vect )
{
    uchar ucRxBuff;
/*
    if ( st_bRxCmdEnd  )
    {
        ASSERT(0);

    }
*/
    ucRxBuff = UDR;



//  if ( RX_BYTE_HEAD == ucRxBuff )
//  {
//      st_ucRxBuffIdx = 0;
//  }
//  else if ( RX_BYTE_END == ucRxBuff )
//  {
//      st_bRxCmdEnd = TRUE;
//  }
//  st_ucRxBuff[st_ucRxBuffIdx++] = ucRxBuff ;

    myputc( ucRxBuff );
}

void Uart0Init( void )
{
    st_bRxCmdEnd = FALSE;
    st_ucRxBuffIdx = 0;
    memset( st_ucRxBuff,  0, RECV_BUF_SIZE  );

    UCSRB = 0x00;                   // close uart
    UCSRC = _BV(URSEL) | 0x06;      // 8 databits

//    UCSRA = 0x02;                   // freq x 2
//    UBRRH = 0x00;
//    UBRRL = 207;                    // BaudRate:9615 @ 16Mhz Crystal Freq

#undef BAUD  // avoid compiler warning
#define BAUD 9600
#include <util/setbaud.h>
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
#if USE_2X
    UCSRA |= (1 << U2X);
#else
    UCSRA &= ~(1 << U2X);
#endif

                                    // open uart
    UCSRB = _BV(RXCIE)|             // enable receive complete interrupt
            _BV(RXEN)|              // enable receive ,RxD pin will be replace by USART function.
            _BV(TXEN);              // enable transmit ,TxD pin will be replace by USART function.
}

static void myputc( char c )
{
    if (c == '\n')
        myputc('\r');
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
}

void putstr( char *pStr )
{
    while(*pStr != '\0'){
        myputc(*pStr++);
    }
}

static void putx( ulong ulSrc, ulong ulMaxDiv )
{
    ulong ulDiv;

    // find the first significant figures.
    for ( ulDiv = ulMaxDiv; ulDiv; ulDiv /= 10 ){
        if ( ulSrc / ulDiv ){
            break;
        }
    }

    // output from the first significant figures.
    for ( ; ulDiv; ulDiv /= 10 ){
        myputc( ulSrc / ulDiv % 10 + 0x30 );
    }
}

void putuc( uchar ucSrc )
{
    putx( (ulong)ucSrc, 100 );
}

void putsc( schar scSrc )
{
    if ( 0 == scSrc ){
        putstr( "0" );
    } else if ( 0 < scSrc ){
        putuc( (uchar)scSrc );
    } else {
        putstr( "-" );
        putuc( (uchar)(-scSrc) );
    }
}

void putn( uint unSrc )
{
    putx( (ulong)unSrc, 10000 );
}

void putsn( sint snSrc )
{
    if ( 0 == snSrc ){
        putstr( "0" );
    } else if ( 0 < snSrc ){
        putn( (uint)snSrc );
    } else {
        putstr( "-" );
        putn( (uint)(-snSrc) );
    }
}

void putl( ulong ulSrc )
{
    putx( (ulong)ulSrc, 1000000000UL );
}

void putsl( slong slSrc )
{
    if ( 0 == slSrc ){
        putstr( "0" );
    } else if ( 0 < slSrc ){
        putl( (ulong)slSrc );
    } else {
        putstr( "-" );
        putl( (ulong)(-slSrc) );
    }

}

void putf( double fSrc ){
    D2C     UTmp;

    UTmp.d = fSrc;

//  NAN 7FC00000L: Not A Number
//  send cmd
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = 0x7F;
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = 0xC0;
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = 0x00;
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = 0x00;

    loop_until_bit_is_set(UCSRA, UDRE);     // 1st char
    UDR = UTmp.c[0];
    loop_until_bit_is_set(UCSRA, UDRE);     // 2rd char
    UDR = UTmp.c[1];
    loop_until_bit_is_set(UCSRA, UDRE);     // 3th char
    UDR = UTmp.c[2];
    loop_until_bit_is_set(UCSRA, UDRE);     // 4th char
    UDR = UTmp.c[3];
}

