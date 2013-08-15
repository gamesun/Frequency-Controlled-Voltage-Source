/*******************************************************************
 File  : eep.h
 Coder : gamesun
*******************************************************************/

#pragma once

#ifndef __EEP_H__
#define __EEP_H__

// form 0x0000 to 0x01FF.
#define EEP_ADDR_BASE           0x0000U
#define EEP_ADDR_USER1          EEP_ADDR_BASE
#define EEP_USER1_LEN           4
#define EEP_ADDR_USER2          (EEP_ADDR_USER1 + EEP_USER1_LEN)
#define EEP_USER2_LEN           1

#define EEP_ADDR_END            0x01FFU



extern double EepRdFloat( uint unStartAddress );
extern void EepWtFloat( uint unStartAddress, double fData );
extern uchar EepRd( uint unAddress );
extern void EepWt( uint unAddress, uchar ucData );


#endif /* __EEP_H__ */
