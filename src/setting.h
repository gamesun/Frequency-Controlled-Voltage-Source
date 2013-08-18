/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#pragma once

#ifndef SETTING_H_
#define SETTING_H_

#define STAGE_NONE          -1
#define STAGE_1             0
#define STAGE_2             1
#define STAGE_3             2
#define STAGE_4             3
#define STAGE_5             4
#define STAGE_6             5

#define STAGE_NUM           6

extern uchar GetStage( void );
extern void SetStageAndVolt( uchar ucStage );

#endif /* SETTING_H_ */
