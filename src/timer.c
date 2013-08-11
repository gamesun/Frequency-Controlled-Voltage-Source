/*******************************************************************
 File  : timer.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "timer.h"

#define T_OVF_MAX   1

typedef enum {
    FMS_READY = 0,
    FMS_RUNNING,
    FMS_COMPLETE,
} FREQ_METER_STATE;

static uchar ucTimerOvfCnt;
static FREQ_METER_STATE enFreqMeterState = READY;

static uint unFreqMeterCntStart = 0;
static uint unFreqMeterCntEnd   = 0;
static ulong ulFreqMeterCntRlt  = 0;

static ulong ulFrequency = 0;

static void TC1Init( void );

void TimerInit( void )
{
    TC1Init();
}


void FreqMeterHandle( void )
{
//    ulong ulQuotient;
//    ulong ulRemainder;
    
    switch ( enFreqMeterState ){
    case FMS_READY:
        
        break;
    case FMS_RUNNING:
        
        break;
    case FMS_COMPLETE:
        enFreqMeterState = FMS_READY;
/*
        ulQuotient  = F_CPU / ulFreqMeterCntRlt;
        ulRemainder = F_CPU % ulFreqMeterCntRlt;
        ulFrequency = ulQuotient + ulRemainder / ulFreqMeterCntRlt;
*/
        ulFrequency = F_CPU / ulFreqMeterCntRlt;
        break;
    default:
        break;
    }
    
}


// calc the freq
ulong GetFreq( void )
{
    ulong ulfreq;



    return ulfreq;
}

static void TC1Init( void )
{
    TCCR1B  =   0x00;                       // timer stop.
    
    TCCR1A  =   /*_BV(WGM10)|*/             // WGM = 0: Normal mode, Top is 0xFFFF.
                /*_BV(WGM11)|*/             // WGM
                /*_BV(FOC1A)|*/             // 
                /*_BV(FOC1B)|*/             // 
                /*_BV(COM1A1)|*/            // COM1A = 0: unused
                /*_BV(COM1A0)|*/            // (IO : PD5)
                /*_BV(COM1B1)|*/            // COM1B = 0: unused
                /*_BV(COM1B0)|*/            // (IO : PD4)
                0;
    
    TIMSK   |=  _BV(TOIE1)|                 // T/C1, Overflow Interrupt Enable
                /*_BV(OCIE1B)|*/            // T/C1, Output Compare B Match Interrupt Enable
                /*_BV(OCIE1A)|*/            // T/C1, Output Compare A Match Interrupt Enable
                _BV(TICIE1)|                // T/C1, Input Capture Interrupt Enable
                0;
    
    TCCR1B  =   _BV(CS10)|                  // Clock Select:1, = CLKio / 1.
                /*_BV(CS11)|*/              // Clock Select
                /*_BV(CS12)|*/              // Clock Select
                /*_BV(WGM12)|*/             // WGM
                /*_BV(WGM13)|*/             // WGM
                _BV(ICES1)|                 // Input Capture Edge Select = 1, rising edge.
                /*_BV(ICNC1)|*/             // Input Capture Noise Canceler
                0;

}

/*------------------------------------------- Timer/Counter1 Capture Event --*/
ISR( TIMER1_CAPT_vect )
{
    uchar sreg;
    uint unICRBuff;
    
    sreg = SREG;        // Save global interrupt flag
    cli()               // Disable interrupts
    
    unICRBuff = ICR1;   // read ICR1
    
    SREG = sreg;        // Restore global interrupt flag
    
    switch ( enFreqMeterState ){
    case FMS_READY:
        unFreqMeterCntStart = unICRBuff;
        ucTimerOvfCnt       = 0;
        enFreqMeterState    = FMS_RUNNING;
        break;
    case FMS_RUNNING:
        unFreqMeterCntEnd   = unICRBuff;
        
        // if CntStart <= CntEnd : Rlt = CntEnd - CntStart + 0xffff * OvfCnt
        // if CntEnd < CntStart  : Rlt = 0xffff - CntStart + CntEnd + 0xffff * ( OvfCnt - 1 )
        // -->> Rlt = ( CntEnd + 0xffff * OvfCnt ) - CntStart
        ulFreqMeterCntRlt   = ( (ulong)unFreqMeterCntEnd + 0xfffful * (ulong)ucTimerOvfCnt )
                              - (ulong)unFreqMeterCntStart;
        enFreqMeterState    = FMS_COMPLETE;
        break;
    case FMS_COMPLETE:
        break;
    default:
        break;
    }
}
/*------------------------------------------ Timer/Counter1 Overflow Event --*/
ISR( TIMER1_OVF_vect )
{
    if ( ucTimerOvfCnt < T_OVF_MAX ){
        ucTimerOvfCnt++;
    } else {
        // overflow too many times, means the signal has been taken off.
        // running = FALSE;
    }
}
