/*******************************************************************
 File  : spi.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "spi.h"

#define SPI_TX_BUFF_MAX         4

static uchar ucSpiTxDataIdx       = 0;
static uchar ucSpiTxDataRemainLen = 0;
static uchar ucSpiTxDataRemain[SPI_TX_BUFF_MAX] = { 0 };

void SpiInit( void )
{
    SPCR    =   0x00;               // close SPI
    
    SPDR    =   0x00;               // clear data.
    
    SPSR    =   _BV(SPI2X);         // Double SPI Speed
    
    SPCR    =   _BV(SPIE) |         // SPI Interrupt Enable
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
    uchar i;
    
    if ( pucData ){
        if ( SPI_TX_BUFF_MAX < ucLen ){
            ucLen = SPI_TX_BUFF_MAX;
        }
        
        ucSpiTxDataRemainLen = ucLen - 1;
        ucSpiTxDataIdx = 0;
        
        for ( i = 0; i < ucLen - 1; i++ ){
            ucSpiTxDataRemain[i] = *( pucData + i + 1 );
        }
        
        SPDR = *pucData;
    }
}


bool SpiIsBusy( void )
{
    return (bool)(0 < ucSpiTxDataRemainLen);
}


/*----------------------------------------------- Serial Transfer Complete --*/
ISR( SPI_STC_vect )
{
    if ( 0 < ucSpiTxDataRemainLen ){
        ucSpiTxDataRemainLen--;
        SPDR = ucSpiTxDataRemain[ucSpiTxDataIdx++];
    } else {
        PORTB |= _BV(4);
    }
}

