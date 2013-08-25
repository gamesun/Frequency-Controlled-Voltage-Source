/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __DAC_H__
#define __DAC_H__

#define DAC_V_MIN           0       // 0.00V
#define DAC_V_REF           1000ul  // 1.000V
#define DAC_V_MAX           (DAC_V_REF * DAC_CODE_MAX / 512ul)    // Full Scale Range


#define DAC_CODE_MAX        1023ul

/*
 *  TLC5615 Offset Error is 3 LSB.
 *  (3 * 2 * Vref * / 1024) / (2 * Vref * 1023 / 1024)
 *  = 3 / 1023 = 0.3%FSR
 */
#define DAC_AJT_CODE1       102     // 10%FSR
#define DAC_AJT_CODE2       921     // 90%FSR


extern void DacInit( void );
extern void SetVoltageByValue( uint unVol1000times );
extern double IdealVoltToRealVolt( uint unVol1000times );
extern double DacCodeToRealVolt( uint unCode );
extern uint IdealVoltToDacCode( uint unVol1000times );
extern void DacAdjust( uint unY1, uint unY2 );
extern double GetDacCoefK( void );
extern double GetDacCoefB( void );
extern void SetVoltageByCode( uint unCode );

#endif /* __DAC_H__ */
