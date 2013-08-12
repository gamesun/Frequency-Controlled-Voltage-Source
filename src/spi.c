/*******************************************************************
 File  : spi.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "spi.h"

void SpiInit( void )
{
    SPRC    =   0x00;               // close SPI
    
    SPDR    =   0x00;               // clear data.
    
    SPSR    =   _BV(SPI2X);         // Double SPI Speed
    
    SPRC    =   _BV(SPIE) |         // SPI Interrupt Enable
                _BV(SPE)  |         // SPI Enable
              /*_BV(DORD) |*/       // Data Order, 0: MSB transmitted first.
                _BV(MSTR) |         // Master/Slave Select 1: Master
              /*_BV(CPOL) |*/       // Clock Polarity 0: 
              /*_BV(CPHA) |*/       // Clock Phase 0: 
              /*_BV(SPR1) |*/       // SPI Clock Rate Select 0:
              /*_BV(SPR0) |*/       // 
                0;

}


void SpiTransmit( uchar* pucData, uchar ucLen )
{
    
}
