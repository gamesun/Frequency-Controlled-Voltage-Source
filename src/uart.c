/*******************************************************************
 File  : uart.c
 Coder : sunyt
*******************************************************************/

#include "common.h"
#include "uart.h"


const char szRxCmdCR  PROGMEM = 13;     // Carriage Return
const char szRxCmdBS  PROGMEM = 8;      // BackSpace


static uchar st_ucRxBuff[RECV_BUF_SIZE];
static uchar st_ucRxBuffIdx;
static bool  st_bRxCmdEnd;

uchar ucCmdBuff[RECV_BUF_SIZE];
bool bIsCmdExist;

inline bool myIsDigit( const char* str, int nLen );
static void myputc( char );
static void putx( ulong, ulong );
static void myputf( float );


void UartHandle( void )
{
    uchar   ucLen;
    uchar   i;

    if ( st_bRxCmdEnd ){
        st_bRxCmdEnd = FALSE;

        UCSRB &= ~_BV( RXCIE );       // RX INT Off
        ucLen = MIN( st_ucRxBuffIdx, RECV_BUF_SIZE );
        for ( i = 0; i < ucLen; i++ ){
            ucCmdBuff[i] = st_ucRxBuff[i];
        }
        st_ucRxBuffIdx = 0;
        UCSRB |= _BV( RXCIE );        // RX INT On
        ucCmdBuff[i] = 0;
        bIsCmdExist = TRUE;
    }
}

ISR( USART_RXC_vect )
{
    uchar ucRxBuff;
    
    ucRxBuff = UDR;
    myputc( ucRxBuff );
    
    if ( szRxCmdCR == ucRxBuff ){           // CR
        st_bRxCmdEnd = TRUE;
        return;
    }else if ( szRxCmdBS == ucRxBuff ){     // BackSpace
        if ( 0 < st_ucRxBuffIdx ){
            st_ucRxBuffIdx--;
            pgmputs( "\x20\b" );
        }
        return;
    }
    
    st_ucRxBuff[st_ucRxBuffIdx++] = ucRxBuff ;

}

void UartInit( void )
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
        pgmputs( "0" );
    } else if ( 0 < scSrc ){
        putuc( (uchar)scSrc );
    } else {
        pgmputs( "-" );
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
        pgmputs( "0" );
    } else if ( 0 < snSrc ){
        putn( (uint)snSrc );
    } else {
        pgmputs( "-" );
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
        pgmputs( "0" );
    } else if ( 0 < slSrc ){
        putl( (ulong)slSrc );
    } else {
        pgmputs( "-" );
        putl( (ulong)(-slSrc) );
    }

}


void putf( float fSrc )
{
    if ( fSrc < 0 ){
        pgmputs( "-" );
        myputf( -fSrc );
    } else {
        myputf( fSrc );
    }
}

// the input float number should be low than 9.99999E9
static void myputf( float f )
{
    float fSrc;
    long nIntDiv;
    char szDecCnt;
    char szIntCnt;
    char szCntTmp;
    char szBuff[10] = {0};
    uchar ucBuffIdx;
    char szLastNotZero;
    
    fSrc          = f;
    ucBuffIdx     = 0;
    szDecCnt      = 0;
    szIntCnt      = 0;
    szLastNotZero = 0;
    
    for ( nIntDiv = 1000000000L; nIntDiv; nIntDiv /= 10 ){
        if ( (long)fSrc / nIntDiv ){
            break;
        }
    }
    
    do{
        myputc( (long)fSrc / nIntDiv % 10 + 0x30 );
        nIntDiv /= 10;
        szIntCnt++;
    } while ( nIntDiv );
    
    szCntTmp = 7 - szIntCnt;
    
    
    
    if ( szIntCnt ){
        
    } else {
        myputc( '0' );
        
        do {
            fSrc *= 10;
            myputc( (long)fSrc % 10 + '0' );
        } while ( 0 != (long)fSrc % 10 );
    }
    
    do {
        fSrc *= 10;
        szBuff[ucBuffIdx] = (long)fSrc % 10 + 0x30;
        szDecCnt++;
        if ( '0' != szBuff[ucBuffIdx] ){
            szLastNotZero = szDecCnt;
        }
        ucBuffIdx++;
    } while ( szDecCnt < szCntTmp );
    
    if ( szLastNotZero ){
        myputc( '.' );
    }
    
    for ( ucBuffIdx = 0; ucBuffIdx < szLastNotZero; ucBuffIdx++ ){
        myputc( szBuff[ucBuffIdx] );
    }
}

inline bool myIsDigit( const char* str, int nLen )
{
	while ( nLen-- ){
		if (!isdigit(*str++)){
			return FALSE;
		}
	}
	return TRUE;
}
