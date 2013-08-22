/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "port.h"
#include "dac.h"
#include "counter.h"
#include "setting.h"
#include "timer.h"
#include "parameter.h"

#define PORT_EN         PORTA
#define PORT_EN_BIT     _BV(0)

#define PORT_CLK        PORTB
#define PORT_CLK_BIT    _BV(0)

typedef enum {
    NONE_EDGE = 0,
    FALLING_EDGE,
    RISING_EDGE
} PIN_EDGE;


static PIN_EDGE signal_EN;
static PIN_EDGE signal_CLK;

static void EnSignalPolling( void );
static void ClkSignalPolling( void );
static void EnSignalTrigger( void );
static void ClkSignalTrigger( void );
static void Time1IsUp( void );

void PortHandle( void )
{
    EnSignalPolling();
    EnSignalTrigger();
    
    if ( GetStage() == STAGE_1 ){
        ClkSignalPolling();
        ClkSignalTrigger();
    }
}


static void EnSignalTrigger( void )
{
    if ( signal_EN == FALLING_EDGE ){
        signal_EN = NONE_EDGE;
        CounterStart();
        SetStageAndVolt( STAGE_1 );
        SetTimerByMillisecond( GetTTable( 1 ), Time1IsUp );
    } else if ( signal_EN == RISING_EDGE ){
        signal_EN = NONE_EDGE;
        SetStageAndVolt( STAGE_NONE );
    }
}


static void Time1IsUp( void )
{
    SetStageAndVolt( STAGE_2 );
}


static void ClkSignalTrigger( void )
{
    if ( signal_CLK == FALLING_EDGE ){
        signal_CLK = NONE_EDGE;
        SetStageAndVolt( STAGE_2 );
    } else if ( signal_CLK == RISING_EDGE ){
        signal_CLK = NONE_EDGE;
    }
}


static void EnSignalPolling( void )
{
    static uchar ucPreviousPortState = 0;
    uchar ucCurrentPortState;
    uchar ucDiffPort;
    
    ucCurrentPortState = PORT_EN;
    
    ucDiffPort = ucPreviousPortState ^ ucCurrentPortState; 
    if ( 0 != ucDiffPort ){
        if ( 0 != ( ucDiffPort & PORT_EN_BIT ) ){
            if ( 0 != ( ucPreviousPortState & PORT_EN_BIT ) ){
                signal_EN = FALLING_EDGE;
            } else {
                signal_EN = RISING_EDGE;
            }
        }
        ucPreviousPortState = ucCurrentPortState;
    }
}


static void ClkSignalPolling( void )
{
    static uchar ucPreviousPortState = 0;
    uchar ucCurrentPortState;
    uchar ucDiffPort;
    
    ucCurrentPortState = PORT_CLK;
    
    ucDiffPort = ucPreviousPortState ^ ucCurrentPortState; 
    if ( 0 != ucDiffPort ){
        if ( 0 != ( ucDiffPort & PORT_CLK_BIT ) ){
            if ( 0 != ( ucPreviousPortState & PORT_CLK_BIT ) ){
                signal_CLK = FALLING_EDGE;
            } else {
                signal_CLK = RISING_EDGE;
            }
        }
        ucPreviousPortState = ucCurrentPortState;
    }
}


/*
      ����������������������������
    1 ��(XCK/T0)PB0    PA0(ADC0)�� 40
    2 ��(T1)PB1        PA1(ADC1)�� 39
    3 ��(INT2/AIN0)PB2 PA2(ADC2)�� 38
    4 ��(OC0/AIN1)PB3  PA3(ADC3)�� 37
    5 ��(SS)PB4        PA4(ADC4)�� 36
    6 ��(MOSI)PB5      PA5(ADC5)�� 35
    7 ��(MISO)PB6      PA6(ADC6)�� 34
    8 ��(SCK)PB7       PA7(ADC7)�� 33
    9 ��RESET               AREF�� 32
   10 ��VCC                  GND�� 31
   11 ��GND                 AVCC�� 30
   12 ��XTAL2         PC7(TOSC2)�� 29
   13 ��XTAL1         PC6(TOSC1)�� 28
   14 ��(RXD)PD0        PC5(TDI)�� 27
   15 ��(TXD)PD1        PC4(TDO)�� 26
   16 ��(INT0)PD2       PC3(TMS)�� 25
   17 ��(INT1)PD3       PC2(TCK)�� 24
   18 ��(OC1B)PD4       PC1(SDA)�� 23
   19 ��(OC1A)PD5       PC0(SCL)�� 22
   20 ��(ICP1)PD6       PD7(OC2)�� 21
      ����������������������������
*/
void PortInit( void )
{
    DDRA  = 0b11111110;                     // PA0 Input, PA1..7 Output
    DDRB  = 0b11111110;                     // PB0 Input  PB1..7 Output
    DDRC  = 0b11111111;                     // PC0..7 Output
    DDRD  = 0b10111110;                     // PD7,5,4,3,2 Output,PD1-TxD,PD0-RxD

    PORTA = 0b00000000;                     // PA Low
    PORTB = 0b00010000;                     // PB4 High
    PORTC = 0b00000000;                     // PC Low
    PORTD = 0b00000000;                     // PD Low
}
