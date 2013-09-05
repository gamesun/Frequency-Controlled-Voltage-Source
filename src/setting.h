/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef __SETTING_H__
#define __SETTING_H__

#define STAGE_NONE          255
#define STAGE_1             1
#define STAGE_2             2
#define STAGE_3             3
#define STAGE_4             4
#define STAGE_5             5
#define STAGE_6             6

#define STAGE_NUM           6

void SetStage( uchar ucStage );
extern uchar GetStage( void );
extern void SetStageAndVolt( uchar ucStage );

#endif /* __SETTING_H__ */
