/*******************************************************************
 File  : dac.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "dac.h"

#define DAC_PORT_H      PORTB
#define DAC_PORT_L      PORTC

#define DAC_VOL_MIN     0       // 0.00V
#define DAC_VOL_MAX     500u    // 5.00V

static void SetDAC( uint unVolIn10bits );

/*
 voltage range: from 1.00V to 2.00V step 0.01V.
 input parameter: 100 times voltage.
*/
void SetVoltage( uint unVol100times )
{
    if ( DAC_VOL_MAX < unVol100times ){
        unVol100times = DAC_VOL_MAX;
    }

    SetDAC( (uint)( (ulong)unVol100times * 1024ul / DAC_VOL_MAX ) );

    // TODO: set clk
}

static void SetDAC( uint unVolIn10bits )
{
    DAC_PORT_H = ( DAC_PORT_H & 0xFC ) | ( ( unVolIn10bits >> 8 ) & 0x03 );
    DAC_PORT_L = unVolIn10bits & 0xFF;
}
