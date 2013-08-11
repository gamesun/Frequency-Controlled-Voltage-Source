/*******************************************************************
 File  : timer.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "timer.h"

//static uint un;

static void TC1Init( void );

void TimerInit( void )
{
    TC1Init();
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
    // TODO: read ICR1L, then read ICR1H
    
}
/*------------------------------------------ Timer/Counter1 Overflow Event --*/
ISR( TIMER1_OVF_vect )
{
    // TODO: set flag
    
}
