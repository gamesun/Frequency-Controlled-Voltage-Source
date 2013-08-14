/*******************************************************************
 File  : cmd.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "cmd.h"
#include "uart.h"

#define CMD_NUM         8
#define CMD_LEN         16

#define ARG_NUM         8
#define ARG_LEN         8


#define CMD_BUFF_LEN    RECV_BUF_SIZE

typedef struct cmd {
    char szCmd[CMD_LEN];
    uint unArgs[ARG_NUM];
    uchar ucArgsCnt;
} ST_CMD, * PST_CMD;

typedef struct cmd_Matrix {
    char szCmd[CMD_LEN];
    void (*pf_HandleFunc)( PST_CMD );
} ST_CMD_MATRIX;

static void CmdExecute( PST_CMD );
static uchar CmdCnvArgs( char * pcStr, PST_CMD pstCmd );
static void CmdStart( PST_CMD );
static void CmdSetCnt( PST_CMD );
static void CmdSetTime( PST_CMD );

static ST_CMD_MATRIX stCmdMatrix[] = {
    
    { "start",      CmdStart        },
    { "setcnt",     CmdSetCnt       },
    { "settime",    CmdSetTime      },
    
    { "",           NULL            }  // Placing NULL at the end.
};



void CmdHandle( void )
{
    uchar i;
    char szBuff[CMD_BUFF_LEN];
    ST_CMD stCmd;
    
    if ( bIsCmdExist ){
        for ( i = 0; ucCmdBuff[i] != 0; i++ ){
            szBuff[i] = ucCmdBuff[i];
        }
        bIsCmdExist = FALSE;

        CmdCnvArgs( szBuff, &stCmd );

        CmdExecute( &stCmd );
    }
}


static void CmdExecute( PST_CMD pstCmd )
{
    uchar i;
    
    for ( i = 0; stCmdMatrix[i].pf_HandleFunc != NULL; i++ ){
        if ( 0 ){
            stCmdMatrix[i].pf_HandleFunc( pstCmd );
            break;
        }
    }
    
    if ( stCmdMatrix[i].pf_HandleFunc == NULL ){
        pgmputs( "invalid command.\n" );
    }
}

const char szSpace  PROGMEM = 32;
const char szTab  PROGMEM = 9;

static uchar CmdCnvArgs( char * pcStr, PST_CMD pstCmd )
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
                        pgmputs( "too many arguments.(>" );
                        putuc( (uchar)ARG_NUM );
                        pgmputs( ")" );
                    }
                }
                
                ucArgsIdx++;
            }
            
            if ( 0 == ucArgsIdx ){
                if ( ucCmdCnt < CMD_LEN ){
                    pstCmd->szCmd[ucCmdCnt++] = *pcStr;
                } else {
                    pgmputs( "too long command.(>8)" );
                    putuc( (uchar)CMD_LEN );
                    pgmputs( ")" );
                }
            }
        }
    }
    
    return ucArgsIdx;
}


static void CmdStart( PST_CMD pstCmd )
{
    
}


static void CmdSetCnt( PST_CMD pstCmd )
{
    
}


static void CmdSetTime( PST_CMD pstCmd )
{
    
}

