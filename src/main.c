/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "main.h"
#include "uart.h"
#include "counter.h"
#include "spi.h"
#include "cmd.h"
#include "port.h"

static void InitDevices( void );
static void SayWelcome( void );

int main( void )
{
    
    InitDevices();
    
    SayWelcome();

    while ( 1 ){
        UartHandle();
        CmdHandle();
        PortHandle();
    }
    
    return 0;
}


static void InitDevices( void )
{
    cli();

    _delay_ms( 10 );

    MCUCR = 0x00;                           // sleep mode off
    PortInit();
    CounterInit();
    SpiInit();
    UartInit();


    sei();
}


static void SayWelcome( void )
{
    pgmputs( "\nWelcome to Frequency-Controlled Voltage Source.\n" );
    pgmputs( "         Copywrong All Lefts Unreserved.\n" );
    pgmputs( "URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source\n\n" );
    pgmputs( "If you want a list of all supported commands type 'help'.\n" );

    pgmputs( ">" );
}
