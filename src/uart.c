/*******************************************************************
 File  : uart.c
 Coder : sunyt
*******************************************************************/

#include "common.h"
#include "uart.h"


const char chCR  PROGMEM = 13;     // CR (Carriage Return)
const char chLF  PROGMEM = 10;     // LF£¨Line Feed)
const char chBS  PROGMEM = 8;      // BackSpace
const char chSP  PROGMEM = 32;     // Space

static uchar st_ucRxBuff[RECV_BUF_SIZE];
static volatile uchar st_ucRxBuffIdx;
static volatile bool  st_bRxCmdEnd;
static volatile uchar st_ucRxCharPrevious;

uchar ucCmdBuff[RECV_BUF_SIZE];
bool bIsCmdExist;

inline bool myIsDigit( const char* str, int nLen );
static void myputc( char );
static uchar putx( ulong, ulong );
static uchar myputf( float );


void UartHandle( void )
{
    uchar   ucLen;
    uchar   i;
    
    if ( st_bRxCmdEnd ){
        st_bRxCmdEnd = FALSE;

        UCSRB &= ~_BV( RXCIE );       // RX INT Off
        
        if ( ( 1 == st_ucRxBuffIdx ) && 
                ( chLF == st_ucRxBuff[0] ) && ( chCR == st_ucRxCharPrevious ) ){
            ;           // the second char 'LF' of 'CR+LF'.
        } else if ( ( ( 1 == st_ucRxBuffIdx ) && 
                ( ( chCR == st_ucRxBuff[0] ) ||     // CR
                ( chLF == st_ucRxBuff[0] ) ) ) ||   // LF
            ( ( 2 == st_ucRxBuffIdx ) && 
                ( chCR == st_ucRxBuff[0] ) &&       // CR+LF
                ( chLF == st_ucRxBuff[1] ) ) ){
            pgmputs( "\n>" );           // input is only a CR(or LF or CR+LF).
        } else {
            ucLen = MIN( st_ucRxBuffIdx, RECV_BUF_SIZE );
            for ( i = 0; i < ucLen && chCR != st_ucRxBuff[i] && chLF != st_ucRxBuff[i]; i++ ){
                ucCmdBuff[i] = st_ucRxBuff[i];
            }
            pgmputs( "\n" );
            ucCmdBuff[i] = 0;
            bIsCmdExist = TRUE;
        }
        
        st_ucRxBuffIdx = 0;
        
        UCSRB |= _BV( RXCIE );        // RX INT On
    }
}

ISR( USART_RXC_vect )
{
    static volatile uchar ucRxBuff = 0;
    
    st_ucRxCharPrevious = ucRxBuff;
    
    ucRxBuff = UDR;
    
    if ( chBS == ucRxBuff ){      // BackSpace
        if ( 0 < st_ucRxBuffIdx ){
            st_ucRxBuffIdx--;
            myputc( chBS );
            myputc( chSP );     // clear the current char on TeraTerm
            myputc( chBS );
        }
    } else {
        if ( st_ucRxBuffIdx < RECV_BUF_SIZE ){
            st_ucRxBuff[st_ucRxBuffIdx++] = ucRxBuff;
            if ( ( chCR == ucRxBuff ) || ( chLF == ucRxBuff )){            // CR
                st_bRxCmdEnd = TRUE;
            } else {
                myputc( ucRxBuff );
            }
        } else {
            st_bRxCmdEnd = TRUE;
        }
    }
}

void UartInit( void )
{
    st_bRxCmdEnd = FALSE;
    st_ucRxBuffIdx = 0;
    st_ucRxCharPrevious = 0;
    memset( st_ucRxBuff,  0, RECV_BUF_SIZE  );

    UCSRB = 0x00;                   // close uart
    UCSRC = _BV(URSEL) | 0x06;      // 8 databits

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

void putstr_p( const char *pStr )
{
    while( pgm_read_byte(pStr) != 0 )
        myputc(pgm_read_byte(pStr++));
}

void putch( char c )
{
    myputc( c );
}

static uchar putx( ulong ulSrc, ulong ulMaxDiv )
{
    ulong ulDiv;
    uchar ucLen;
    
    // find the first significant figures.
    for ( ulDiv = ulMaxDiv; 1 < ulDiv; ulDiv /= 10 ){
        if ( ulSrc / ulDiv ){
            break;
        }
    }

    ucLen = 0;
    // output from the first significant figures.
    for ( ; ulDiv; ulDiv /= 10 ){
        myputc( ulSrc / ulDiv % 10 + 0x30 );
        ucLen++;
    }
    
    return ucLen;
}


uchar putuc( uchar ucSrc )
{
    return putx( (ulong)ucSrc, 100 );
}


uchar putsc( schar scSrc )
{
    if ( 0 == scSrc ){
        pgmputs( "0" );
        return 1;
    } else if ( 0 < scSrc ){
        return putuc( (uchar)scSrc );
    } else {
        pgmputs( "-" );
        return putuc( (uchar)(-scSrc) ) + 1;
    }
}


uchar putunAppendSpace( uint unSrc, uchar ucAllLen )
{
    uchar ucPrintedLen;
    
    ucPrintedLen = putun( unSrc );
    
    for ( ; ucPrintedLen < ucAllLen; ucPrintedLen++ ){
        putch( ' ' );
    }
    
    return ucPrintedLen;
}


uchar putun( uint unSrc )
{
    return putx( (ulong)unSrc, 10000 );
}


uchar putsn( sint snSrc )
{
    if ( 0 == snSrc ){
        pgmputs( "0" );
        return 1;
    } else if ( 0 < snSrc ){
        return putun( (uint)snSrc );
    } else {
        pgmputs( "-" );
        return putun( (uint)(-snSrc) ) + 1;
    }
}


uchar putul( ulong ulSrc )
{
    return putx( (ulong)ulSrc, 1000000000UL );
}


uchar putsl( slong slSrc )
{
    if ( 0 == slSrc ){
        pgmputs( "0" );
        return 1;
    } else if ( 0 < slSrc ){
        return putul( (ulong)slSrc );
    } else {
        pgmputs( "-" );
        return putul( (ulong)(-slSrc) ) + 1;
    }

}


uchar putf( float fSrc )
{
    if ( fSrc < 0 ){
        pgmputs( "-" );
        return myputf( -fSrc ) + 1;
    } else {
        return myputf( fSrc );
    }
}


uchar putfAppendSpace( float fSrc, uchar ucAllLen )
{
    uchar ucPrintedLen;
    
    ucPrintedLen = putf( fSrc );
    
    for ( ; ucPrintedLen < ucAllLen; ucPrintedLen++ ){
        putch( ' ' );
    }
    
    return ucPrintedLen;
}


// the input float number should be smaller than 9.99999E9
static uchar myputf( float f )
{
    float fSrc;
    long nIntDiv;
    char szDecCnt;
    char szIntCnt;
    char szCntTmp;
    char szBuff[10] = {0};
    uchar ucBuffIdx;
    char szLastNotZero;
    uchar ucPrintedLen;
    
    fSrc          = f;
    ucBuffIdx     = 0;
    szDecCnt      = 0;
    szIntCnt      = 0;
    szLastNotZero = 0;
    ucPrintedLen  = 0;
    
    // Integer Number
    for ( nIntDiv = 1000000000L; 1 < nIntDiv; nIntDiv /= 10 ){
        if ( (long)fSrc / nIntDiv ){
            break;
        }
    }
    
    do{
        myputc( (long)fSrc / nIntDiv % 10 + '0' );
        ucPrintedLen++;
        nIntDiv /= 10;
        szIntCnt++;
    } while ( nIntDiv );
    
    szCntTmp = 7 - szIntCnt;
    
    // Decimal Number
    if ( 0 == szIntCnt ){
        myputc( '0' );
        ucPrintedLen++;
        
        do {
            fSrc *= 10;
            myputc( (long)fSrc % 10 + '0' );
            ucPrintedLen++;
        } while ( 0 != (long)fSrc % 10 );
    }
    
    do {
        fSrc *= 10;
        szBuff[ucBuffIdx] = (long)fSrc % 10 + '0';
        szDecCnt++;
        if ( '0' != szBuff[ucBuffIdx] ){
            szLastNotZero = szDecCnt;
        }
        ucBuffIdx++;
    } while ( szDecCnt < szCntTmp );
    
    if ( szLastNotZero ){
        myputc( '.' );
        ucPrintedLen++;
    }
    
    for ( ucBuffIdx = 0; ucBuffIdx < szLastNotZero; ucBuffIdx++ ){
        myputc( szBuff[ucBuffIdx] );
        ucPrintedLen++;
    }
    
    return ucPrintedLen;
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
