/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "setting.h"
#include "counter.h"
#include "uart.h"

#define CTABLE_LEN          STAGE_NUM
#define STAGE_MAX           CTABLE_LEN - 1

#define CNT_VALUE_MIN       1

static uchar ucOvfCntRemain = 0;
static uchar ucLastCnt = 0;

static uchar ucCntStage = 0;
static uint unCTable[CTABLE_LEN] = { 0 };

static void Tc0Init( void );
static void Tc0Start( void );
static void Tc0Stop( void );
static void SetTc0Top( uint unCntTop );


void CounterInit( void )
{
    uchar i;
    
    for ( i = 0; i < CTABLE_LEN; i++ ){
        unCTable[i] = CNT_VALUE_MIN;
    }
    
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
        SetVoltageByVTable( 1 );
        break;
    case STAGE_2:
        SetVoltageByVTable( 2 );
        break;
    case STAGE_3:
        SetVoltageByVTable( 3 );
        break;
    case STAGE_4:
        SetVoltageByVTable( 4 );
        break;
    case STAGE_5:
        SetVoltageByVTable( 5 );
        break;
    case STAGE_6:
        SetVoltageByVTable( 6 );
        break;
    case STAGE_NONE:
    default:
        SetVoltageByValue( 0 );
        break;
    }
    
}


void CounterStart( void )
{
    uchar i;
    bool bIsCntValid;
    
    SetStageAndVolt( STAGE_1 );
    
    SetTc0Top( unCTable[STAGE_2] );
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


void SetCTable( uchar ucIndex, uint unData )
{
    if ( ucIndex <= CTABLE_LEN ){
        unCTable[ucIndex - 1] = MAX( unData, CNT_VALUE_MIN );
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
        // this stage is complete.
        if ( ucCntStage < STAGE_MAX ){
            SetTc0Top( unCTable[++ucCntStage] );
            SetStageAndVolt( ucCntStage );
        } else {
            Tc0Stop();
        }
    }
}

