/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __DAC_H__
#define __DAC_H__

#define DAC_CODE_MAX        1023u

/*
 *  TLC5615 Offset Error is 3 LSB.
 *  (3 * 2 * Vref * / 1024) / (2 * Vref * 1023 / 1024)
 *  = 3 / 1023 = 0.3%FSR
 */
#define DAC_AJT_CODE1       102     // 10%FSR
#define DAC_AJT_CODE2       921     // 90%FSR


extern void DacInit( void );
extern void SetVoltageByVTable( uchar ucIndex );
extern void SetVTable( uchar ucIndex, uint unVol1000times );
extern uint GetVTable( uchar ucIndex );
extern void SetVoltageByValue( uint unVol1000times );
extern double CnvToRealVoltage( uint unVol1000times );
extern void DacAdjust( uint unY1, uint unY2 );
extern double GetDacCoefK( void );
extern double GetDacCoefB( void );

#endif /* __DAC_H__ */
