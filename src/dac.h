/*******************************************************************
 File  : dac.h
 Coder : gamesun
*******************************************************************/

#pragma once

#ifndef __DAC_H__
#define __DAC_H__

extern void SetVoltageByVTable( uchar ucIndex );
extern void SetVTable( uchar ucIndex, uint unVol100times );
extern uint GetVTable( uchar ucIndex );
extern void SetVoltageByValue( uint unVol100times );

#endif /* __DAC_H__ */
