/*******************************************************************
 File  : counter.h
 Coder : gamesun
*******************************************************************/

#pragma once

#ifndef __COUNTER_H__
#define __COUNTER_H__

extern void CounterInit( void );
extern void CounterStart( void );
extern void SetCTable( uchar ucIndex, uint unData );
extern uint GetCTable( uchar ucIndex );

#endif /* __COUNTER_H__ */
