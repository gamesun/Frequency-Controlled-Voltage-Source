/*******************************************************************
 File  : cmd.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "cmd.h"
#include "uart.h"
#include "counter.h"
#include "dac.h"

#define CMD_LEN         10
#define ARG_NUM         4
#define ARG_LEN         8


#define CMD_BUFF_LEN    RECV_BUF_SIZE


#define CMD_CNV_SUCCEED         0
#define ERR_CMD_TOO_LONG        -1
#define CMD_ARG_TOO_LONG        -2
#define CMD_ARG_TOO_MANY        -3


typedef struct cmd {
    char szCmd[CMD_LEN];
    uint unArgs[ARG_NUM];
    uchar ucArgsCnt;
} ST_CMD, * PST_CMD;

typedef struct cmd_Matrix {
    char szCmd[CMD_LEN];
    void (*pf_HandleFunc)( PST_CMD );
} ST_CMD_MATRIX;

static void CmdMatchAndExecute( PST_CMD );
static schar CmdCnvArgs( char * pcStr, PST_CMD pstCmd );
static void dbgPutCmd( PST_CMD pstCmd );
static void CmdHelp( PST_CMD );
static void CmdList( PST_CMD );
static void CmdSetCnt( PST_CMD );
static void CmdSetTime( PST_CMD );
static void CmdSetVol( PST_CMD );
static void CmdVolt( PST_CMD );

static ST_CMD_MATRIX stCmdMatrix[] = {
    
    { "help",       CmdHelp         },
    { "list",       CmdList         },
    { "setcnt",     CmdSetCnt       },
    { "settime",    CmdSetTime      },
    { "setvol",     CmdSetVol       },
    { "volt",       CmdVolt         },
    
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
    char szBuff[ARG_LEN];
    
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
            pstCmd->unArgs[ucArgsIdx - 1] = atoi( szBuff );
            ucBuffCnt = 0;
        } else {
            return CMD_ARG_TOO_MANY;
        }
    }
    pstCmd->ucArgsCnt = ucArgsIdx;

    return CMD_CNV_SUCCEED;
}


static void CmdHelp( PST_CMD pstCmd )
{
    pgmputs( "  help       display this message.\n" );
    pgmputs( "  list       list all the parameters.\n" );
    pgmputs( "  setcnt     set counters' values of each stage.\n" );
    pgmputs( "  setvol     set voltages of each stage.\n" );
    pgmputs( "  volt       set the DAC output voltage directly.\n" );
    pgmputs( "\n" );
    pgmputs( "Type a command will display it's usage.\n" );
}


static void CmdList( PST_CMD pstCmd )
{
    
}


static void CmdSetCnt( PST_CMD pstCmd )
{
    dbgPutCmd( pstCmd );
    
    if ( 2 != pstCmd->ucArgsCnt ){
        pgmputs( "bad arguments.\n\n" );
        pgmputs( "usage: setcnt i c\n" );
        pgmputs( "  i -> Index of one count, from 2 to 5.\n" );
        pgmputs( "  c -> value of the Count, from 1 to 65535.\n" );
    }
    
    SetCTable( (uchar)pstCmd->unArgs[0], pstCmd->unArgs[1] );
}


static void CmdSetTime( PST_CMD pstCmd )
{
    dbgPutCmd( pstCmd );
}


static void CmdSetVol( PST_CMD pstCmd )
{
    uint unTmp;
    
    if ( ( 2 == pstCmd->ucArgsCnt ) && 
            ( 1 <= pstCmd->unArgs[0] ) &&
            ( pstCmd->unArgs[0] <= 6 ) ){
        SetVTable( pstCmd->unArgs[0], pstCmd->unArgs[1] );
        unTmp = GetVTable( pstCmd->unArgs[0] );
        
        pgmputs( "you have set V" );
        putuc( pstCmd->unArgs[0] );
        pgmputs( " = " );
        putun( unTmp );
        pgmputs( "\n" );
        pgmputs( "The Real Voltage will be " );
        putf( CnvToRealVoltage( unTmp ) );
        pgmputs( "\n" );
    } else {
        pgmputs( "bad arguments.\n\n" );
        pgmputs( "usage: setvol i v\n" );
        pgmputs( "  i -> Index of one voltage, from 1 to 6.\n" );
        pgmputs( "  v -> the Voltage, from 0 to 500 (500 means 5.00V).\n" );
    }
}


static void CmdVolt( PST_CMD pstCmd )
{
    dbgPutCmd( pstCmd );
    
    if ( 1 != pstCmd->ucArgsCnt ){
        pgmputs( "bad arguments.\n\n" );
        pgmputs( "usage: volt v\n" );
        pgmputs( "  v -> from 0 to 500 (500 means 5.00V).\n" );
    }
    
    SetVoltageByValue( pstCmd->unArgs[0] );
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
