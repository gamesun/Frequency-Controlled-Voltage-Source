/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"


static bool bIsTimerLocking = FALSE;
static ulong ulTimerOvfCnt = 0;
void (*TimerIsUp)( void );

void SetTimerByMillisecond( ulong ulPreiodByMillisecond, void (*callback)(void) )
{
    if ( bIsTimerLocking == FALSE ){
        bIsTimerLocking = TRUE;
        TimerIsUp = callback;
        ulTimerOvfCnt = ulPreiodByMillisecond - 1;
        TCNT1 = 0;
        TCCR1B |= _BV(CS10);         // CLKio(No prescaling)
    }
}


bool IsTimerLocking( void )
{
    return bIsTimerLocking;
}

/*---------------------------------------------------- T/C2 Overflow Event --*/
ISR( TIMER2_OVF_vect )
{
    if ( 0 < ulTimerOvfCnt ){
        ulTimerOvfCnt--;
    } else {
        bIsTimerLocking = FALSE;
        TCCR1B &= ~( _BV(CS12) | _BV(CS11) | _BV(CS10) );
        TimerIsUp();
    }
}


void TimerInit( void )
{
    TCCR1B  =   0x00;                       // timer stop.
    
    TCCR1A  =   /*_BV(WGM10)|*/             // WGM = 12: CTC Mode(Clear Timer on Compare Match Mode).
                /*_BV(WGM11)|*/             // WGM
                /*_BV(FOC1A)|*/             // 
                /*_BV(FOC1B)|*/             // 
                /*_BV(COM1A1)|*/            // COM1A = 0: unused
                /*_BV(COM1A0)|*/            // (IO : PD5)
                /*_BV(COM1B1)|*/            // COM1B = 0: unused
                /*_BV(COM1B0)|*/            // (IO : PD4)
                0;
    
    ICR1    =   16000;                      // 16MHz / 16000 -> 1ms
    
    TIMSK   |=    _BV(TOIE1)|               // T/C1, Overflow Interrupt Enable
                /*_BV(OCIE1B)|*/            // T/C1, Output Compare B Match Interrupt Enable
                /*_BV(OCIE1A)|*/            // T/C1, Output Compare A Match Interrupt Enable
                /*_BV(TICIE1)|*/            // T/C1, Input Capture Interrupt Enable
                0;
    
    TCCR1B  =   /*_BV(CS10)|*/              // Clock Select:0, No clock source.
                /*_BV(CS11)|*/              // Clock Select
                /*_BV(CS12)|*/              // Clock Select
                  _BV(WGM12)|               // WGM
                  _BV(WGM13)|               // WGM
                /*_BV(ICES1)|*/             // Input Capture Edge Select
                /*_BV(ICNC1)|*/             // Input Capture Noise Canceler
                0;

}


