/*******************************************************************
 File  : spi.h
 Coder : gamesun
*******************************************************************/

#pragma once

#ifndef __SPI_H__
#define __SPI_H__

extern void SpiInit( void );
extern void SpiTransmit( uchar* pucData, uchar ucLen );
extern bool SpiIsBusy( void );
extern void SpiTransmit( uchar* pucData, uchar ucLen );

#endif /* __SPI_H__ */
