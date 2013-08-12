/*******************************************************************
 File  : dac.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "dac.h"


#define DAC_V_MIN           0       // 0.00V
#define DAC_V_REF           100     // 1.00V
#define DAC_V_MAX           (DAC_V_REF * 1023 / 512)

static void SetDacData( uint unVolIn10bits );

/*
 voltage range: from 1.00V to 2.00V step 0.01V.
 input parameter: 100 times voltage.
*/
void SetVoltage( uint unVol100times )
{
    if ( DAC_V_MAX < unVol100times ){
        unVol100times = DAC_VOL_MAX;
    }
    
    SetDacData( (uint)( (ulong)unVol100times * 512ul / DAC_V_REF ) );
}

static void SetDacData( uint unVolIn10bits )
{
    
}


