/*******************************************************************
 File  : counter.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "counter.h"

#define CNT_NUM        10

static uchar ucOvfCntRemain = 0;
static uchar ucLastCnt = 0;

static uchar ucCntState = 0;
static uchar ucCntStateMax = 0;
static uint unCnt[CNT_NUM] = { 0 };

static void Tc0Init( void );
static void Tc0Start( void );
static void Tc0Stop( void );
static void SetTc0Top( uint unCntTop );


void CounterInit( void )
{
    TC0Init();
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
    ucCntStateMax = 5;      // 0 ~ 5: 6 times
    bIsCntValid =TRUE;
    
    for ( i = ucCntState; i < ucCntStateMax; i++ ){
        if ( 0 == unCnt[i] ){
            bIsCntValid = FALSE;
        }
    }
    
    if ( TRUE == bIsCntValid ){
        SetTc0Top();
        Tc0Start();
    } else {
        putstr( "Make sure that each Ci in {C1...Cn} is not zero value." );
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


/* T/C0 Compare Match Event --*/
ISR()
{
    if ( 1 < ucOvfCntRemain ){
        ucOvfCntRemain--;
    } else if ( 1 == ucOvfCntRemain ){
        ucOvfCntRemain--;
        OCR0 = ucLastCnt;
    } else {
        // this stage is complete.
        if ( ucCntState < ucCntStateMax ){
            SetTc0Top( unCnt[++ucCntState] );
        } else {
            Tc0Stop();
        }
    }
}

