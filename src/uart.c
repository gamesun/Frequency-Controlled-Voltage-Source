/*******************************************************************
 File  : uart.c
 Coder : sunyt
*******************************************************************/

#include "common.h"
#include "uart.h"

#define RECV_BUF_SIZE       32


const char szRxCmdCR  PROGMEM = 13;     // Carriage Return
const char szRxCmdBS  PROGMEM = 8;      // BackSpace


static uchar st_ucRxBuff[RECV_BUF_SIZE];
static uchar st_ucRxBuffIdx;
static bool  st_bRxCmdEnd;

inline bool myIsDigit( const char* str, int nLen );
static void myputc( char );
static void putx( ulong, ulong );
static void myputf( float );


void CmdHandle( void )
{
    uchar   ucCmdBuff[RECV_BUF_SIZE];
    uchar   ucLen;
//    uchar   ucCnt;
    uchar   i;



    if ( st_bRxCmdEnd ){
        st_bRxCmdEnd = FALSE;

        UCSRB &= ~_BV( RXCIE );       // RX INT Off
        ucLen = st_ucRxBuffIdx;
        for ( i = 0; i < ucLen; i++ ){
            ucCmdBuff[i] = st_ucRxBuff[i];
        }
        st_ucRxBuffIdx = 0;
        UCSRB |= _BV( RXCIE );        // RX INT On
        ucCmdBuff[i] = 0;


        putstr( (char*)ucCmdBuff );

        if ( 's' == ucCmdBuff[0] &&            // sin
             'i' == ucCmdBuff[1] &&
             'n' == ucCmdBuff[2] ){
            if ( myIsDigit( (char*)&ucCmdBuff[3], ucLen - 3 ) ){

            } else {
                DBG(putstr( (char*)&ucCmdBuff[3] ););
                pgmputs( ": Invalid Number.\n" );
            }            
        } else if ( 't' == ucCmdBuff[0] &&    // tri
                    'r' == ucCmdBuff[1] &&
                    'i' == ucCmdBuff[2] ){
            pgmputs( "Exporting Triangle wave!" );
        } else if ( 'r' == ucCmdBuff[0] &&    // rec
                    'e' == ucCmdBuff[1] &&
                    'c' == ucCmdBuff[2] ){
            pgmputs( "Exporting Rectangular wave!" );
        } else if ( 's' == ucCmdBuff[0] &&    // saw
                    'a' == ucCmdBuff[1] &&
                    'w' == ucCmdBuff[2] ){
            pgmputs( "Exporting Sawtooth wave!" );
        } else if ( 'h' == ucCmdBuff[0] &&    // help
                    'e' == ucCmdBuff[1] &&
                    'l' == ucCmdBuff[2] &&
                    'p' == ucCmdBuff[3] ){
            pgmputs( " sin[F]  Sine wave\n" );
            pgmputs( " rec[F]  Rectangular wave\n" );
            pgmputs( " tri[F]  Triangle wave\n" );
            pgmputs( " saw[F]  Sawtooth wave\n" );
            pgmputs( " F: the Frequency(Hz)£¬Range 1 - 10000\n" );
            pgmputs( " e.g. \'sin1\' OR \'sin12345\'\n" );
        } else if ( 0 < ucLen ){
//            DBG(putstr( ucCmdBuff ););
            pgmputs( ": command not found.\n" );
        }
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
