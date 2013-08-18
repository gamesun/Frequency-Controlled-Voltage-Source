/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __DAC_H__
#define __DAC_H__

extern void DacInit( void );
extern void SetVoltageByVTable( uchar ucIndex );
extern void SetVTable( uchar ucIndex, uint unVol1000times );
extern uint GetVTable( uchar ucIndex );
extern void SetVoltageByValue( uint unVol1000times );
extern double CnvToRealVoltage( uint unVol1000times );

#endif /* __DAC_H__ */
