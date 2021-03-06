/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "eep.h"
#include "dac.h"
#include "counter.h"
#include "parameter.h"

static uchar EepRd( uint unAddress );
static void EepWt( uint unAddress, uchar ucData );


void EepReadAll( void )
{
    SetVTable( 1, EepRdWord( EEP_ADDR_VOL1 ) );
    SetVTable( 2, EepRdWord( EEP_ADDR_VOL2 ) );
    SetVTable( 3, EepRdWord( EEP_ADDR_VOL3 ) );
    SetVTable( 4, EepRdWord( EEP_ADDR_VOL4 ) );
    SetVTable( 5, EepRdWord( EEP_ADDR_VOL5 ) );
    SetVTable( 6, EepRdWord( EEP_ADDR_VOL6 ) );

    SetCTable( 2, EepRdWord( EEP_ADDR_C2 ) );
    SetCTable( 3, EepRdWord( EEP_ADDR_C3 ) );
    SetCTable( 4, EepRdWord( EEP_ADDR_C4 ) );
    SetCTable( 5, EepRdWord( EEP_ADDR_C5 ) );
    
    SetTTable( 1, EepRdWord( EEP_ADDR_T1 ) );
}


void EepWriteAll( void )
{
    EepWtWord( EEP_ADDR_VOL1, GetVTable(1) );
    EepWtWord( EEP_ADDR_VOL2, GetVTable(2) );
    EepWtWord( EEP_ADDR_VOL3, GetVTable(3) );
    EepWtWord( EEP_ADDR_VOL4, GetVTable(4) );
    EepWtWord( EEP_ADDR_VOL5, GetVTable(5) );
    EepWtWord( EEP_ADDR_VOL6, GetVTable(6) );
    
    EepWtWord( EEP_ADDR_C2, GetCTable(2) );
    EepWtWord( EEP_ADDR_C3, GetCTable(3) );
    EepWtWord( EEP_ADDR_C4, GetCTable(4) );
    EepWtWord( EEP_ADDR_C5, GetCTable(5) );
    
    EepWtWord( EEP_ADDR_T1, GetTTable(1) );
}


void EepWtByte( uint unStartAddress, uchar ucData )
{
    if ( EEP_ADDR_END < unStartAddress ){
        return;
    }
    
    EepWt( unStartAddress, ucData );
}

uchar EepRdByte( uint unStartAddress )
{
    if ( EEP_ADDR_END < unStartAddress ){
        return -1;
    }
    
    return EepRd( unStartAddress );
}

void EepWtWord( uint unStartAddress, uint unData )
{
    W2B w2b;
    
    if ( EEP_ADDR_END < unStartAddress + 1 ){
        return;
    }
    
    w2b.word = unData;
    EepWt( unStartAddress,     w2b.byte[0] );
    EepWt( unStartAddress + 1, w2b.byte[1] );
}

uint EepRdWord( uint unStartAddress )
{
    W2B w2b;
    
    if ( EEP_ADDR_END < unStartAddress + 1 ){
        return -1;
    }
    
    w2b.byte[0] = EepRd( unStartAddress );
    w2b.byte[1] = EepRd( unStartAddress  + 1 );
    
    return (w2b.word);
}


void EepWtDword( uint unStartAddress, ulong ulData )
{
    DW2B dw2b;
    
    if ( EEP_ADDR_END < unStartAddress + 3 ){
        return;
    }
    
    dw2b.dword = ulData;
    EepWt( unStartAddress,     dw2b.byte[0] );
    EepWt( unStartAddress + 1, dw2b.byte[1] );
    EepWt( unStartAddress + 2, dw2b.byte[2] );
    EepWt( unStartAddress + 3, dw2b.byte[3] );
}

ulong EepRdDword( uint unStartAddress )
{
    DW2B dw2b;
    
    if ( EEP_ADDR_END < unStartAddress + 3 ){
        return -1;
    }
    
    dw2b.byte[0] = EepRd( unStartAddress );
    dw2b.byte[1] = EepRd( unStartAddress  + 1 );
    dw2b.byte[2] = EepRd( unStartAddress  + 2 );
    dw2b.byte[3] = EepRd( unStartAddress  + 3 );
    
    return (dw2b.dword);
}

double EepRdFloat( uint unStartAddress )
{
    D2C     UTmp;

    UTmp.c[0] = EepRd( unStartAddress     );
    UTmp.c[1] = EepRd( unStartAddress + 1 );
    UTmp.c[2] = EepRd( unStartAddress + 2 );
    UTmp.c[3] = EepRd( unStartAddress + 3 );

    return (UTmp.d);
}

void EepWtFloat( uint unStartAddress, double fData )
{
    D2C     UTmp;
    UTmp.d = fData;

    EepWt( unStartAddress,     UTmp.c[0] );
    EepWt( unStartAddress + 1, UTmp.c[1] );
    EepWt( unStartAddress + 2, UTmp.c[2] );
    EepWt( unStartAddress + 3, UTmp.c[3] );
}


// Read data from EEPROM
// Address: 0x0000 - 0x01FF (Size:512x8bits)
static uchar EepRd( uint unAddress )
{
    uchar ucBuf;
    uchar sreg;
    
    loop_until_bit_is_clear( EECR, EEWE );      // wait the last writing until complete.
    
    sreg = SREG;        // Save global interrupt flag
    cli();              // Disable interrupts
    
    EEAR = unAddress;
    EECR |= _BV(EERE);
    
    ucBuf = EEDR;
    
    SREG = sreg;        // Restore global interrupt flag
    
    return ucBuf;
}

// Write data to EEPROM
// Address: 0x0000 - 0x01FF (Size:512x8bits)
static void EepWt( uint unAddress, uchar ucData )
{
    uchar sreg;
    
    loop_until_bit_is_clear( EECR, EEWE );      // wait the last writing until complete.
    
    sreg = SREG;        // Save global interrupt flag
    cli();              // Disable interrupts
    
    EEAR = unAddress;
    EEDR = ucData;
    
    EECR |= _BV(EEMWE);
    
    EECR |= _BV(EEWE);
    
    SREG = sreg;        // Restore global interrupt flag
}

