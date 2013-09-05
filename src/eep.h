/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __EEP_H__
#define __EEP_H__

// form 0x0000 to 0x01FF.
#define EEP_ADDR_BASE           0x0000U
#define EEP_ADDR_END            0x01FFU

#define EEP_ADDR_DAC_ZO         EEP_ADDR_BASE
#define EEP_DAC_ZO_LEN          2
#define EEP_ADDR_DAC_K          (EEP_ADDR_DAC_ZO + EEP_DAC_ZO_LEN)
#define EEP_DAC_K_LEN           2

#define EEP_ADDR_VOL1           (EEP_ADDR_DAC_K + EEP_DAC_K_LEN)
#define EEP_VOL1_LEN            2
#define EEP_ADDR_VOL2           (EEP_ADDR_VOL1 + EEP_VOL1_LEN)
#define EEP_VOL2_LEN            2
#define EEP_ADDR_VOL3           (EEP_ADDR_VOL2 + EEP_VOL2_LEN)
#define EEP_VOL3_LEN            2
#define EEP_ADDR_VOL4           (EEP_ADDR_VOL3 + EEP_VOL3_LEN)
#define EEP_VOL4_LEN            2
#define EEP_ADDR_VOL5           (EEP_ADDR_VOL4 + EEP_VOL4_LEN)
#define EEP_VOL5_LEN            2
#define EEP_ADDR_VOL6           (EEP_ADDR_VOL5 + EEP_VOL5_LEN)
#define EEP_VOL6_LEN            2

#define EEP_ADDR_C2             (EEP_ADDR_VOL6 + EEP_VOL6_LEN)
#define EEP_C2_LEN              2
#define EEP_ADDR_C3             (EEP_ADDR_C2 + EEP_C2_LEN)
#define EEP_C3_LEN              2
#define EEP_ADDR_C4             (EEP_ADDR_C3 + EEP_C3_LEN)
#define EEP_C4_LEN              2
#define EEP_ADDR_C5             (EEP_ADDR_C4 + EEP_C4_LEN)
#define EEP_C5_LEN              2

#define EEP_ADDR_T1             (EEP_ADDR_C5 + EEP_C5_LEN)
#define EEP_T1_LEN              2


#define EEP_ADDR_DBG1           (EEP_ADDR_BASE + 0x0100U)
#define EEP_DBG1_LEN            1
#define EEP_ADDR_DBG2           (EEP_ADDR_DBG1 + EEP_DBG1_LEN)
#define EEP_DBG2_LEN            1

extern void EepWtByte( uint unStartAddress, uchar ucData );
extern uchar EepRdByte( uint unStartAddress );
extern void EepWtWord( uint unStartAddress, uint unData );
extern uint EepRdWord( uint unStartAddress );
extern void EepWtDword( uint unStartAddress, ulong ulData );
extern ulong EepRdDword( uint unStartAddress );
extern double EepRdFloat( uint unStartAddress );
extern void EepWtFloat( uint unStartAddress, double fData );

extern void EepReadAll( void );
extern void EepWriteAll( void );

#endif /* __EEP_H__ */
