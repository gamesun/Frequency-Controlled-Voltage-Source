/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __SPI_H__
#define __SPI_H__

extern void SpiInit( void );
extern void SpiTransmit( uchar* pucData, uchar ucLen );
extern bool SpiIsBusy( void );
extern void SpiTransmit( uchar* pucData, uchar ucLen );

#endif /* __SPI_H__ */
