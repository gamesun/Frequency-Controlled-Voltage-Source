/*******************************************************************
 File  : dac.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "dac.h"

#define DAC_PORT_H          PORTB
#define DAC_PORT_L          PORTC

#define DAC_PORT_CLK        PORTB
#define DAC_PORT_CLK_BIT    2

#define DAC_PORT_CLK_H      do{ DAC_PORT_CLK |= _BV( DAC_PORT_CLK_BIT ); }while(0)        // clk H
#define DAC_PORT_CLK_L      do{ DAC_PORT_CLK &= ~_BV( DAC_PORT_CLK_BIT ); }while(0)       // clk L

#define DAC_VOL_MIN         0       // 0.00V
#define DAC_VOL_MAX         500u    // 5.00V

static void SetDacData( uint unVolIn10bits );
static void SetDacClock( void );

/*
             --- DAC900 Timing diagram ---

      -----            ----------            -------
Clock      \          /          \          /
            ----------            ----------
           |    t2    |     t1    |
      -----------  ----------  --------  --------
Data  /\/\/\/\/\/\/  stable  \/\/\/\/\/\/
      \/\/\/\/\/\/\   data   /\/\/\/\/\/\
      -----------  ----------  --------  --------
                 | ts |  th   |

     _________________________________
                                      \
Iout                                   \     _
                                        \   / \___
                                         \_/
                                  |tpd|  tset  |


 | Symbol |         Description         | TYP | Units |
 +--------+-----------------------------+-----+-------+
 |  t1    | Clock Pulse HIGH Time       |  3  |  ns   |
 |  t2    | Clock Pulse LOW Time        |  3  |  ns   |
 |  ts    | Data Setup Time             | 1.5 |  ns   |
 |  th    | Data Hold Time              | 1.0 |  ns   |
 |  tpd   | Propagation Delay Time      |  1  |  ns   |
 |  tset  | Output Setting Time to 0.1% | 30  |  ns   |
*/

/*
 voltage range: from 1.00V to 2.00V step 0.01V.
 input parameter: 100 times voltage.
*/
void SetVoltage( uint unVol100times )
{
    if ( DAC_VOL_MAX < unVol100times ){
        unVol100times = DAC_VOL_MAX;
    }

    SetDacData( (uint)( (ulong)unVol100times * 1024ul / DAC_VOL_MAX ) );

    SetDacClock();
}

static void SetDacData( uint unVolIn10bits )
{
    DAC_PORT_H = ( DAC_PORT_H & 0xFC ) | ( ( unVolIn10bits >> 8 ) & 0x03 );
    DAC_PORT_L = unVolIn10bits & 0xFF;
}

/*
 the highest CPU clock of ATmega16 is 16MHz, that one CPU cycle is 62.5ns.
 so there is no need to delay when control the DAC900.
*/
static void SetDacClock( void )
{
    DAC_PORT_CLK_H;

    DAC_PORT_CLK_L;
}
