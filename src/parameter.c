/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "setting.h"
#include "uart.h"

#define CTABLE_LEN          STAGE_NUM

#define CNT_VALUE_MIN       1

static uint unCTable[CTABLE_LEN] = { 0 };


void SetCTable( uchar ucIndex, uint unData )
{
    if ( ucIndex <= CTABLE_LEN ){
        unCTable[ucIndex - 1] = MAX( unData, CNT_VALUE_MIN );
    }
}


uint GetCTable( uchar ucIndex )
{
    if ( CTABLE_LEN < ucIndex ){
        pgmputs( "IllegalAccess at GetCTable() [counter.c]\n" );
        return -1;
    }
    
    return unCTable[ucIndex - 1];
}

