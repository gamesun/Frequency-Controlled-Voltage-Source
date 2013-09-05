/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <math.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <ctype.h>
#include <stdlib.h>


#define MIN(a, b)       ( (a < b) ? a : b )
#define MAX(a, b)       ( (a > b) ? a : b )

typedef enum {
    false = 0,
    true  = 1
}BOOLEAN;


// Reference to <stdint.h>
typedef unsigned char           uchar;          // 8-bit unsigned type.  short as uc
typedef   signed char           schar;          // 8-bit signed type.    short as sc
typedef unsigned int            uint;           // 16-bit unsigned type. short as un
typedef   signed int            sint;           // 16-bit signed type.   short as sn
typedef unsigned long int       ulong;          // 32-bit unsigned type. short as ul
typedef   signed long int       slong;          // 32-bit signed type.   short as sl
typedef BOOLEAN                 bool;           //

// note
// This type is not available when the compiler option -mint8 is in effect.
typedef unsigned long long int  ulonglong;      // 64-bit unsigned type. short as ull
typedef   signed long long int  slonglong;      // 64-bit signed type.   short as sll


typedef union {
    uchar   c[4];
    double  d;
}D2C;

typedef union {
    uchar   byte[2];
    uint    word;
} W2B;

typedef union {
    uchar   byte[4];
    ulong   dword;
} DW2B;


extern bool bIsDebug;

#endif /* __COMMON_H__ */
