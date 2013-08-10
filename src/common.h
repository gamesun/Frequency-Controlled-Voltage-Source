/*******************************************************************
 File  : common.h
 Coder : gamesun
*******************************************************************/

#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <math.h>
#include <string.h>

typedef enum {
    FALSE = 0,
    TRUE  = 1
}BOOLEAN;


typedef unsigned char uchar;
typedef   signed char schar;
typedef unsigned int  uint;
typedef   signed int  sint;
typedef unsigned long ulong;
typedef   signed long slong;
typedef BOOLEAN       bool;

typedef union {
	uchar	c[4];
	double	d;
}D2C;


#endif /* __COMMON_H__ */
