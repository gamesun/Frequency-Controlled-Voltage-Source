/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __TIMER_H__
#define __TIMER_H__

extern void TimerInit( void );
extern void SetTimerByMillisecond( ulong ulPreiodByMillisecond, void (*callback)(void) );
extern bool IsTimerLocking( void );
extern void TimerClear( void );

#endif /* __TIMER_H__ */
