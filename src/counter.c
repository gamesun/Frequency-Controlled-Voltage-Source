/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "setting.h"
#include "counter.h"
#include "uart.h"
#include "dac.h"
#include "parameter.h"

static uchar ucOvfCntRemain = 0;
static uchar ucLastCnt = 0;

static uchar ucCntStage = 0;


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


uchar GetStage( void )
{
    return ucCntStage;
}


void SetStageAndVolt( uchar ucStage )
{
    ucCntStage = ucStage;
    
    switch ( ucCntStage ){ 
    case STAGE_1:
        SetVoltageByValue( GetVTable( 1 ) );
        break;
    case STAGE_2:
        SetVoltageByValue( GetVTable( 2 ) );
        break;
    case STAGE_3:
        SetVoltageByValue( GetVTable( 3 ) );
        break;
    case STAGE_4:
        SetVoltageByValue( GetVTable( 4 ) );
        break;
    case STAGE_5:
        SetVoltageByValue( GetVTable( 5 ) );
        break;
    case STAGE_6:
        SetVoltageByValue( GetVTable( 6 ) );
        break;
    case STAGE_NONE:
    default:
        SetVoltageByValue( 0 );
        break;
    }
}


void CounterStart( void )
{
    SetTc0Top( GetCTable( 2 ) );
    Tc0Start();
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


/*----------------------------------------------- T/C0 Compare Match Event --*/
ISR( TIMER0_COMP_vect )
{
    if ( 1 < ucOvfCntRemain ){
        ucOvfCntRemain--;
    } else if ( 1 == ucOvfCntRemain ){
        ucOvfCntRemain--;
        OCR0 = ucLastCnt;
    } else {
        // this stage is complete.
        if ( ucCntStage <= STAGE_4 ){
            SetTc0Top( GetCTable( ++ucCntStage ) );
            SetStageAndVolt( ucCntStage );
        } else {
            Tc0Stop();
            SetStageAndVolt( STAGE_6 );
        }
    }
}

