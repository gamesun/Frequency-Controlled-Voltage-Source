/*******************************************************************
 File  : dac.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "setting.h"
#include "dac.h"
#include "spi.h"
#include "uart.h"

#define DAC_V_MIN           0       // 0.00V
#define DAC_V_REF           250     // 2.50V
#define DAC_V_MAX           (DAC_V_REF * 1023ul / 512ul)

#define VTABLE_LEN          STAGE_NUM

static uint unVTable[VTABLE_LEN] = { 0 };

static void SetDacCode( uint unDacCode );
static uint CnvDacCode( uint unVol100times );


// ucIndex: 1 - VTABLE_LEN
void SetVoltageByVTable( uchar ucIndex )
{
    if ( ucIndex < VTABLE_LEN ){
        SetVoltageByValue( unVTable[ucIndex - 1] );
    }
}


// ucIndex: 1 - VTABLE_LEN
void SetVTable( uchar ucIndex, uint unVol100times )
{
    if ( ucIndex < VTABLE_LEN ){
        unVTable[ucIndex - 1] = unVol100times;
    }
}


// ucIndex: 1 - VTABLE_LEN
uint GetVTable( uchar ucIndex )
{
    if ( VTABLE_LEN <= ucIndex ){
        pgmputs( "Exception in GetVTable()\n" );
        return -1;
    }
    
    return unVTable[ucIndex - 1];
}


double CnvToRealVoltage( uint unVol100times )
{
    return ( CnvDacCode( unVol100times ) * (double)DAC_V_REF / 512.0f );
}


/*
 voltage range: from 1.00V to 2.00V step 0.01V.
 input parameter: 100 times voltage.
*/
void SetVoltageByValue( uint unVol100times )
{
    if ( DAC_V_MAX < unVol100times ){
        unVol100times = DAC_V_MAX;
    }
    
    SetDacCode( CnvDacCode( unVol100times ) );
}


static uint CnvDacCode( uint unVol100times )
{
    return (uint)( ( (double)unVol100times * 512.0f / (double)DAC_V_REF ) + 0.5f );
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


