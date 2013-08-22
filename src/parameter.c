/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "setting.h"
#include "uart.h"
#include "dac.h"

#define CTABLE_LEN          STAGE_NUM
#define VTABLE_LEN          STAGE_NUM
#define TTABLE_LEN         1

#define CNT_VALUE_MIN       1

static uint unCTable[CTABLE_LEN] = { 0 };

static uint unVTable[VTABLE_LEN] = { 0 };

static uint unTTable[TTABLE_LEN] = { 0 };

void SetCTable( uchar ucIndex, uint unData )
{
    if ( ucIndex <= CTABLE_LEN ){
        unCTable[ucIndex - 1] = MAX( unData, CNT_VALUE_MIN );
    }
}


uint GetCTable( uchar ucIndex )
{
    if ( ( CTABLE_LEN < ucIndex ) || ( ucIndex < 1 ) ){
        pgmputs( "IllegalAccess at GetCTable() [parameter.c]\n" );
        return -1;
    }
    
    return unCTable[ucIndex - 1];
}


// ucIndex: 1 - VTABLE_LEN
void SetVTable( uchar ucIndex, uint unVol1000times )
{
    if ( ucIndex <= VTABLE_LEN ){
        unVTable[ucIndex - 1] = MIN( unVol1000times, DAC_V_MAX );
    }
}


// ucIndex: 1 - VTABLE_LEN
uint GetVTable( uchar ucIndex )
{
    if ( ( VTABLE_LEN < ucIndex ) || ( ucIndex < 1 ) ){
        pgmputs( "IllegalAccess at GetVTable() [parameter.c]\n" );
        return -1;
    }
    
    return unVTable[ucIndex - 1];
}


void SetTTable( uchar ucIndex, uint unTime )
{
    if ( ucIndex <= TTABLE_LEN ){
        unTTable[ucIndex - 1] = unTime;
    }
}


uint GetTTable( uchar ucIndex )
{
    if ( ( TTABLE_LEN < ucIndex ) || ( ucIndex < 1 ) ){
        pgmputs( "IllegalAccess at GetTTable() [parameter.c]\n" );
        return -1;
    }
    
    return unTTable[ucIndex - 1];
}
