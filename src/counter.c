/*******************************************************************
 File  : counter.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "setting.h"
#include "counter.h"
#include "uart.h"

#define CTABLE_LEN          STAGE_NUM
#define STATE_MAX           CTABLE_LEN - 1

static uchar ucOvfCntRemain = 0;
static uchar ucLastCnt = 0;

static uchar ucCntState = 0;
static uint unCTable[CTABLE_LEN] = { 0 };

static void Tc0Init( void );
static void Tc0Start( void );
static void Tc0Stop( void );
static void SetTc0Top( uint unCntTop );


void CounterInit( void )
{
    Tc0Init();
}


static void Tc0Init( void )
{
    TCCR0   = /*_BV(FOC0) |*/       // unused
                _BV(WGM01) |        // WGM = 2: CTC Mode(Clear Timer on Compare Match Mode).
              /*_BV(WGM00) |*/      // 
              /*_BV(COM01) |*/      // In CTC Mode, COM = 0: TOP = OCR0
              /*_BV(COM00) |*/      // 
              /*_BV(CS02) |*/       // no clock: counter off
              /*_BV(CS01) |*/       // 
              /*_BV(CS00) |*/       // 
              0;
    
    TIMSK   |=  _BV(OCIE0);
}


void CounterStart( void )
{
    uchar i;
    bool bIsCntValid;
    
    ucCntState = 0;
    bIsCntValid =TRUE;
    
    for ( i = ucCntState; i < CTABLE_LEN; i++ ){
        if ( 0 == unCTable[i] ){
            bIsCntValid = FALSE;
        }
    }
    
    if ( TRUE == bIsCntValid ){
        SetTc0Top( unCTable[ucCntState] );
        Tc0Start();
    } else {
        pgmputs( "I won't start.\n" );
        pgmputs( "Make sure that each Ci in {C1...Cn} is not zero value.\n" );
    }
}


static void Tc0Start( void )
{
    TCNT0   = 0x00;
    
    TCCR0   |=  _BV(CS02) |         // CS = 7, External clock source on T0 pin, clock on rising edge.
                _BV(CS01) |
                _BV(CS00);
}


static void Tc0Stop( void )
{
    TCCR0  &=   ~(_BV(CS02) |         // CS = 0, No clock source.
                  _BV(CS01) |
                  _BV(CS00) );
}


static void SetTc0Top( uint unCntTop )
{
    if ( 256u < unCntTop ){
        ucOvfCntRemain = ( ( unCntTop - 1 ) / 256u ) + 1;
        ucLastCnt = ( unCntTop - 1 ) % 256u;
        OCR0 = 0xff;
    } else {
        OCR0 = ( unCntTop - 1 ) & 0xff;
    }
}

void SetCTable( uchar ucIndex, uint unData )
{
    if ( ucIndex <= CTABLE_LEN ){
        unCTable[ucIndex - 1] = unData;
    }
}


uint GetCTable( uchar ucIndex )
{
    if ( CTABLE_LEN < ucIndex ){
        pgmputs( "IllegalAccess at GetCTable() [counter.c]\n" );
        return -1;
    }
    
    return unCTable[ucIndex - 1];
}


/*----------------------------------------------- T/C0 Compare Match Event --*/
ISR( TIMER0_COMP_vect )
{
    if ( 1 < ucOvfCntRemain ){
        ucOvfCntRemain--;
    } else if ( 1 == ucOvfCntRemain ){
        ucOvfCntRemain--;
        OCR0 = ucLastCnt;
    } else {
        // this state is complete.
        if ( ucCntState < STATE_MAX ){
            SetTc0Top( unCTable[++ucCntState] );
        } else {
            Tc0Stop();
        }
    }
}

