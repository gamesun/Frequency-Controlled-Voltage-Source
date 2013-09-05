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


static double fDacCoefK = 0.0f;     // Coefficient K
static double fDacCoefB = 0.0f;     // Coefficient B


static void SetDacCode( uint unDacCode );


void DacInit( void )
{
//    SetVoltageByValue( 0 );
}


double IdealVoltToRealVolt( uint unVol1000times )
{
    return ( IdealVoltToDacCode( MIN(unVol1000times, DAC_V_MAX) ) * (double)DAC_V_REF / 512.0f );
}


double DacCodeToRealVolt( uint unCode )
{
    return ( MIN(unCode, DAC_CODE_MAX) * (double)DAC_V_REF / 512.0f );
}


/*
 voltage range: from 1.00V to 2.00V step 0.01V.
 input parameter: 100 times voltage.
*/
void SetVoltageByValue( uint unVol1000times )
{
    SetDacCode( IdealVoltToDacCode( unVol1000times ) );
}


void SetVoltageByCode( uint unCode )
{
    SetDacCode( MIN(unCode, DAC_CODE_MAX) );
}


uint IdealVoltToDacCode( uint unVol1000times )
{
    return (uint)( ( (double)MIN(unVol1000times, DAC_V_MAX) * 512.0f / (double)DAC_V_REF ) + 0.5f );
}


static void SetDacCode( uint unDacCode )
{
    uchar ucBuff[2];
    
    if ( SpiIsBusy() ){
        pgmputs( "Spi Busy\n" );
    } else {
        unDacCode <<= 2;
        ucBuff[0] = unDacCode >> 8;
        ucBuff[1] = unDacCode & 0xff;
        
        PORTB &= ~_BV(4);
        SpiTransmit( ucBuff, 2 );
    }
}


/*
 * kx1 + b = y1
 * kx2 + b = y2
 * =>
 *   k = (y2 - y1) / (x2 - x1)
 *   b = (x2y1 - x1y2) / (x2 - x1)
 */
void DacAdjust( uint unY1, uint unY2 )
{
#   define y1   (double)unY1
#   define y2   (double)unY2
#   define x1   (double)DAC_AJT_CODE1
#   define x2   (double)DAC_AJT_CODE2
    
    fDacCoefK = ( y2 - y1 ) / ( x2 - x1 );
    fDacCoefB = (x2 * y1 - x1 * y2) / (x2 - x1);
}


double GetDacCoefK( void )
{
    return fDacCoefK;    
}


double GetDacCoefB( void )
{
    return fDacCoefB;
}
