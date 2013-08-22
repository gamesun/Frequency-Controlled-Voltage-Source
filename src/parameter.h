/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __PARAMETER_H__
#define __PARAMETER_H__


extern void SetCTable( uchar ucIndex, uint unData );
extern uint GetCTable( uchar ucIndex );

extern void SetVTable( uchar ucIndex, uint unVol1000times );
extern uint GetVTable( uchar ucIndex );

extern void SetTTable( uchar ucIndex, uint unTime );
extern uint GetTTable( uchar ucIndex );

#endif /* __PARAMETER_H__ */
