/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "setting.h"
#include "dac.h"
#include "spi.h"
#include "uart.h"

#define DAC_V_MIN           0       // 0.00V
#define DAC_V_REF           2500    // 2.500V
#define DAC_V_MAX           (DAC_V_REF * 1023ul / 512ul)

#define VTABLE_LEN          STAGE_NUM

static uint unVTable[VTABLE_LEN] = { 0 };

static void SetDacCode( uint unDacCode );
static uint CnvDacCode( uint unVol1000times );


void DacInit( void )
{
    SetVoltageByValue( 0 );
}

// ucIndex: 1 - VTABLE_LEN
void SetVoltageByVTable( uchar ucIndex )
{
    if ( ucIndex <= VTABLE_LEN ){
        SetVoltageByValue( unVTable[ucIndex - 1] );
    }
}


// ucIndex: 1 - VTABLE_LEN
void SetVTable( uchar ucIndex, uint unVol1000times )
{
    if ( ucIndex <= VTABLE_LEN ){
        unVTable[ucIndex - 1] = MIN( unVol1000times, DAC_V_MAX );
    }
}


// ucIndex: 1 - VTABLE_LEN
uint GetVTable( uchar ucIndex )
{
    if ( VTABLE_LEN < ucIndex ){
        pgmputs( "IllegalAccess at GetVTable() [dac.c]\n" );
        return -1;
    }
    
    return unVTable[ucIndex - 1];
}


double CnvToRealVoltage( uint unVol1000times )
{
    return ( CnvDacCode( MIN(unVol1000times, DAC_V_MAX) ) * (double)DAC_V_REF / 512.0f );
}


/*
 voltage range: from 1.00V to 2.00V step 0.01V.
 input parameter: 100 times voltage.
*/
void SetVoltageByValue( uint unVol1000times )
{
    SetDacCode( CnvDacCode( unVol1000times ) );
}


static uint CnvDacCode( uint unVol1000times )
{
    return (uint)( ( (double)unVol1000times * 512.0f / (double)DAC_V_REF ) + 0.5f );
}


static void SetDacCode( uint unDacCode )
{
    uchar ucBuff[2];
    
    if ( SpiIsBusy() ){
        
    } else {
        unDacCode <<= 2;
        ucBuff[0] = unDacCode >> 8;
        ucBuff[1] = unDacCode & 0xff;
        
        PORTB &= ~_BV(4);
        SpiTransmit( ucBuff, 2 );
    }
}


