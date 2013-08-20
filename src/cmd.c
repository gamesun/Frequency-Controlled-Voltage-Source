/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "cmd.h"
#include "uart.h"
#include "counter.h"
#include "dac.h"
#include "setting.h"
#include "eep.h"

#define CMD_LEN         10
#define ARG_NUM         4
#define ARG_LEN         8
#define CMD_BUFF_LEN    RECV_BUF_SIZE

#define CMD_CNV_SUCCEED         0
#define ERR_CMD_TOO_LONG        -1
#define CMD_ARG_TOO_LONG        -2
#define CMD_ARG_TOO_MANY        -3

#define TABLE_LEN               STAGE_NUM

typedef struct cmd {
    char szCmd[CMD_LEN];
    uint unArgs[ARG_NUM];
    uchar ucArgsCnt;
} ST_CMD, * PST_CMD;

typedef struct cmd_Matrix {
    char szCmd[CMD_LEN];
    void (*pf_HandleFunc)( PST_CMD );
} ST_CMD_MATRIX;


extern uchar ucCmdBuff[RECV_BUF_SIZE];
extern bool bIsCmdExist;


static void CmdMatchAndExecute( PST_CMD );
static schar CmdCnvArgs( char * pcStr, PST_CMD pstCmd );
static bool IsInRange( uint unData, uint unMin, uint unMax );
static void dbgPutCmd( PST_CMD pstCmd );
static void CmdHelp( PST_CMD );
static void CmdList( PST_CMD );
static void CmdSetCnt( PST_CMD );
static void CmdSetVol( PST_CMD );
static void CmdSave( PST_CMD );
static void CmdVolt( PST_CMD );
static void CmdVoltCode( PST_CMD );
static void CmdDacAdjust( PST_CMD );

static ST_CMD_MATRIX stCmdMatrix[] = {
    
    { "help",       CmdHelp         },
    { "list",       CmdList         },
    { "setcnt",     CmdSetCnt       },
    { "setvol",     CmdSetVol       },
    { "save",       CmdSave         },
    { "volt",       CmdVolt         },
    { "voltcode",   CmdVoltCode     },
    { "dacajt",     CmdDacAdjust    },
    
    { "",           NULL            }  // Placing NULL at the end.
};

const char szSpace  PROGMEM = 32;
const char szTab    PROGMEM = 9;

void CmdHandle( void )
{
    schar scResult;
    uchar i;
    char szBuff[CMD_BUFF_LEN];
    ST_CMD stCmd;
    
    if ( bIsCmdExist ){
        for ( i = 0; ucCmdBuff[i] != 0; i++ ){
            szBuff[i] = ucCmdBuff[i];
        }
        szBuff[i] = 0;
        bIsCmdExist = FALSE;

        scResult = CmdCnvArgs( szBuff, &stCmd );
        
        switch ( scResult ){
        case CMD_CNV_SUCCEED:
            CmdMatchAndExecute( &stCmd );
            break;
        case ERR_CMD_TOO_LONG:
            pgmputs( "your command is too long.(longer than " );
            putuc( CMD_LEN );
            pgmputs( ")\n" );
            break;
        case CMD_ARG_TOO_LONG:
            pgmputs( "an argument is too long.(longer than " );
            putuc( ARG_LEN );
            pgmputs( ")\n" );
            break;
        case CMD_ARG_TOO_MANY:
            pgmputs( "arguments are too many.(more than " );
            putuc( ARG_NUM );
            pgmputs( ")\n" );
            break;
        default:
            break;
        }
        
        pgmputs( "\n>" );
    }
}


static void CmdMatchAndExecute( PST_CMD pstCmd )
{
    uchar i;
    
    for ( i = 0; stCmdMatrix[i].pf_HandleFunc != NULL; i++ ){
        if ( 0 == strcmp( stCmdMatrix[i].szCmd, pstCmd->szCmd ) ){
            stCmdMatrix[i].pf_HandleFunc( pstCmd );
            break;
        }
    }
    
    if ( stCmdMatrix[i].pf_HandleFunc == NULL ){
        pgmputs( "invalid command.\n" );
    }
}


static schar CmdCnvArgs( char * pcStr, PST_CMD pstCmd )
{
    uchar ucArgsIdx;
    bool bIsBlank;
    uchar ucCmdCnt;
    uchar ucBuffCnt;
    char szBuff[ARG_LEN + 1];   // add one to store the '\0'.
    
    ucCmdCnt = 0;
    ucBuffCnt = 0;
    ucArgsIdx = 0;
    bIsBlank = FALSE;
    for ( ; 0 != *pcStr; pcStr++ ){
        if ( ( szTab == *pcStr ) || ( szSpace == *pcStr ) ){
            bIsBlank = TRUE;
        } else {
            if ( bIsBlank ){
                bIsBlank = FALSE;
                if ( 0 < ucArgsIdx ){
                    if ( ucArgsIdx - 1 < ARG_NUM ){
                        szBuff[ucBuffCnt] = 0;
                        pstCmd->unArgs[ucArgsIdx - 1] = atoi( szBuff );
                        ucBuffCnt = 0;
                    } else {
                        return CMD_ARG_TOO_MANY;
                    }
                }
                
                ucArgsIdx++;
            }
            
            if ( 0 == ucArgsIdx ){
                if ( ucCmdCnt < CMD_LEN ){
                    pstCmd->szCmd[ucCmdCnt++] = *pcStr;
                } else {
                    return ERR_CMD_TOO_LONG;
                }
            } else {
                if ( ucBuffCnt < ARG_LEN ){
                    szBuff[ucBuffCnt++] = *pcStr;
                } else {
                    return CMD_ARG_TOO_LONG;
                }
            }
        }
    }
    
    pstCmd->szCmd[ucCmdCnt] = 0;

    // the last argument
    if ( 0 < ucArgsIdx ){
        if ( ucArgsIdx - 1 < ARG_NUM ){
            szBuff[ucBuffCnt] = 0;
            pstCmd->unArgs[ucArgsIdx - 1] = atoi( szBuff );
            ucBuffCnt = 0;
        } else {
            return CMD_ARG_TOO_MANY;
        }
    }
    pstCmd->ucArgsCnt = ucArgsIdx;

    return CMD_CNV_SUCCEED;
}


static bool IsInRange( uint unData, uint unMin, uint unMax )
{
    if ( ( unMin <= unData ) && ( unData <= unMax ) ){
        return TRUE;
    } else {
        return FALSE;
    }
}


static void CmdHelp( PST_CMD pstCmd )
{
    pgmputs( "  help       display this message.\n" );
    pgmputs( "  list       list all the parameters.\n" );
    pgmputs( "  setcnt     set counters' values of each stage.\n" );
    pgmputs( "  setvol     set voltages of each stage.(Unit:mV)\n" );
    pgmputs( "  volt       set the DAC output voltage directly by voltage value.\n" );
    pgmputs( "  voltcode   set the DAC output voltage directly by DAC Code.\n" );
    pgmputs( "  dacajt     Adjust the DAC's Coefficient-K & Coefficient-B.\n" );
    pgmputs( "\n" );
    pgmputs( "Type a command will display it's usage.\n" );
}


static void CmdList( PST_CMD pstCmd )
{
//    uchar i;
    uint (*C)( uchar );
    uint (*V)( uchar );
#   define ps   pgmputs
#   define pc   putch
#   define RV   CnvToRealVoltage
    
    void p6( uint unData )
    {
        putunAppendSpace( unData, 6 );
    }
    
    void p9( uint unData )
    {
        putunAppendSpace( unData, 9 );
    }
    
    void pRV( float fData )
    {
        putfAppendSpace( fData, 10 );
    }
    
    C = GetCTable;
    V = GetVTable;
    
    ps( "|No.| Count | Set Volt | Real Volt |\n" );
    ps( "+---+-------+----------+-----------+\n" );
    ps( "| 1 | " );ps("  -   ");ps("| ");p9( V( 1 ) );ps("| ");pRV( RV(V(1)) );ps("|\n");
    ps( "| 2 | " );p6( C( 2 ) );ps("| ");p9( V( 2 ) );ps("| ");pRV( RV(V(2)) );ps("|\n");
    ps( "| 3 | " );p6( C( 3 ) );ps("| ");p9( V( 3 ) );ps("| ");pRV( RV(V(3)) );ps("|\n");
    ps( "| 4 | " );p6( C( 4 ) );ps("| ");p9( V( 4 ) );ps("| ");pRV( RV(V(4)) );ps("|\n");
    ps( "| 5 | " );p6( C( 5 ) );ps("| ");p9( V( 5 ) );ps("| ");pRV( RV(V(5)) );ps("|\n");
    ps( "| 6 | " );ps("  -   ");ps("| ");p9( V( 6 ) );ps("| ");pRV( RV(V(6)) );ps("|\n");
}


static void CmdSetCnt( PST_CMD pstCmd )
{
    uint unTmp;
    
    dbgPutCmd( pstCmd );
    
    if ( ( 2 == pstCmd->ucArgsCnt ) && IsInRange( pstCmd->unArgs[0], 2, 5 ) ){
        SetCTable( (uchar)pstCmd->unArgs[0], pstCmd->unArgs[1] );
        unTmp = GetCTable( (uchar)pstCmd->unArgs[0] );
        
        pgmputs( "you have set C" );
        putuc( pstCmd->unArgs[0] );
        pgmputs( " = " );
        putun( unTmp );
        pgmputs( "\n" );
    } else {
        pgmputs( "bad arguments.\n\n" );
        pgmputs( "usage: setcnt i c\n" );
        pgmputs( "  i -> Index of one count, from 2 to 5.\n" );
        pgmputs( "  c -> value of the Count, from 1 to 65535.\n" );
    }
}


static void CmdSetVol( PST_CMD pstCmd )
{
    uint unTmp;
    
    if ( ( 2 == pstCmd->ucArgsCnt ) && IsInRange( pstCmd->unArgs[0], 1, 6 ) ){
        SetVTable( (uchar)pstCmd->unArgs[0], pstCmd->unArgs[1] );
        unTmp = GetVTable( (uchar)pstCmd->unArgs[0] );
        
        pgmputs( "you have set V" );
        putuc( pstCmd->unArgs[0] );
        pgmputs( " = " );
        putun( unTmp );
        pgmputs( "mV\n" );
        pgmputs( "The Real Voltage will be " );
        putf( CnvToRealVoltage( unTmp ) );
        pgmputs( "mV\n" );
    } else {
        pgmputs( "bad arguments.\n\n" );
        pgmputs( "usage: setvol i v\n" );
        pgmputs( "  i -> Index of one voltage, from 1 to 6.\n" );
        pgmputs( "  v -> the Voltage, from 0 to 4995 (mV).\n" );
    }
}


static void CmdSave( PST_CMD pstCmd )
{
    if ( 0 == pstCmd->ucArgsCnt ){
        EepWriteAll();
        pgmputs( "Saved to EEPROM" );
    } else {
        pgmputs( "'save' command need no argument." );
    }
}


static void CmdVolt( PST_CMD pstCmd )
{
    if ( 1 == pstCmd->ucArgsCnt ){    
        SetVoltageByValue( pstCmd->unArgs[0] );
        
        pgmputs( "you have set DAC to output " );
        putun( pstCmd->unArgs[0] );
        pgmputs( "mV\n" );
        pgmputs( "The Real output will be " );
        putf( CnvToRealVoltage( pstCmd->unArgs[0] ) );
        pgmputs( "mV\n" );
    } else {
        pgmputs( "bad arguments.\n\n" );
        pgmputs( "usage: volt v\n" );
        pgmputs( "  v -> from 0 to 4995 (mV).\n" );
    }
}


static void CmdVoltCode( PST_CMD pstCmd )
{
    if ( ( 1 == pstCmd->ucArgsCnt ) &&
        ( pstCmd->unArgs[0] <= DAC_CODE_MAX ) ){    
        SetVoltageByValue( pstCmd->unArgs[0] );
        
        pgmputs( "you have set DAC's output by using Code = " );
        putun( pstCmd->unArgs[0] );
        pgmputs( "\n" );
    } else {
        pgmputs( "bad arguments.\n\n" );
        pgmputs( "usage: voltcode d\n" );
        pgmputs( "  d -> a 10-bits number in Decimal(from 0 to 1023).\n" );
    }
}

static void CmdDacAdjust( PST_CMD pstCmd )
{
    if ( 2 == pstCmd->ucArgsCnt ){
        DacAdjust( pstCmd->unArgs[0], pstCmd->unArgs[1] );
        
        pgmputs( "Adjustment Result:\n  Coef K = " );
        putf( GetDacCoefK() );
        pgmputs( "\n  Coef B = " );
        putf( GetDacCoefB() );
        pgmputs( "\n" );
    } else {
#       define p1   putun( (uint)DAC_AJT_CODE1 )
#       define p2   putun( (uint)DAC_AJT_CODE2 )
        
        pgmputs( "" );
        pgmputs( "bad arguments.\n\n" );
        pgmputs( "usage: dacajt V" );p1;
        pgmputs( " V" );p2;pgmputs( "\n" );
        pgmputs( "  V" );p1;
        pgmputs( " -> the real voltage measuring after \"voltcode " );p1;pgmputs( "\" using a voltmeter.\n" );
        pgmputs( "  V" );p2;
        pgmputs( " -> the real voltage measuring after \"voltcode " );p2;pgmputs( "\" using a voltmeter.\n" );
        pgmputs( "(Unit: mV)\n" );
    }
}


static void dbgPutCmd( PST_CMD pstCmd )
{
    uchar i;

    if ( 0 < pstCmd->ucArgsCnt ){        
        pgmputs( "args:" );
        
        for ( i = 0; i < pstCmd->ucArgsCnt - 1; i++ ){
            putun( pstCmd->unArgs[i] );
            pgmputs( "," );
        }
        
        putun( pstCmd->unArgs[i] );
        pgmputs( "\n" );
    }
}
