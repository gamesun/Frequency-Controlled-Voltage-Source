/*******************************************************************
 File  : eep.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "eep.h"


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
uchar EepRd( uint unAddress )
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
void EepWt( uint unAddress, uchar ucData )
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

