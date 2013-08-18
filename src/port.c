/*******************************************************************
 Frequency-Controlled Voltage Source
 
 Author: gamesun <gamesun*outlook.com>
 URL: https://github.com/gamesun/Frequency-Controlled-Voltage-Source
*******************************************************************/

#include "common.h"
#include "port.h"
#include "dac.h"
#include "counter.h"

#define PORT_EN         PORTA
#define PORT_EN_BIT     _BV(0)

typedef enum {
    NONE_EDGE = 0,
    FALLING_EDGE,
    RISING_EDGE
} PIN_EDGE;


static PIN_EDGE signal_EN;


static void PortPolling( void );
static void PortTrigger( void );


void PortHandle( void )
{
    PortPolling();
    PortTrigger();
}


static void PortTrigger( void )
{
    if ( signal_EN == FALLING_EDGE ){
        signal_EN = NONE_EDGE;
        SetVoltageByVTable( 1 );    // V1
        CounterStart();
    } else if ( signal_EN == RISING_EDGE ){
        signal_EN = NONE_EDGE;
        SetVoltageByValue( 0 );     // 0.00V
    }
}


static void PortPolling( void )
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


/*
      ©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·
    1 ©Ï(XCK/T0)PB0    PA0(ADC0)©Ç 40
    2 ©Ï(T1)PB1        PA1(ADC1)©Ç 39
    3 ©Ï(INT2/AIN0)PB2 PA2(ADC2)©Ç 38
    4 ©Ï(OC0/AIN1)PB3  PA3(ADC3)©Ç 37
    5 ©Ï(SS)PB4        PA4(ADC4)©Ç 36
    6 ©Ï(MOSI)PB5      PA5(ADC5)©Ç 35
    7 ©Ï(MISO)PB6      PA6(ADC6)©Ç 34
    8 ©Ï(SCK)PB7       PA7(ADC7)©Ç 33
    9 ©ÏRESET               AREF©Ç 32
   10 ©ÏVCC                  GND©Ç 31
   11 ©ÏGND                 AVCC©Ç 30
   12 ©ÏXTAL2         PC7(TOSC2)©Ç 29
   13 ©ÏXTAL1         PC6(TOSC1)©Ç 28
   14 ©Ï(RXD)PD0        PC5(TDI)©Ç 27
   15 ©Ï(TXD)PD1        PC4(TDO)©Ç 26
   16 ©Ï(INT0)PD2       PC3(TMS)©Ç 25
   17 ©Ï(INT1)PD3       PC2(TCK)©Ç 24
   18 ©Ï(OC1B)PD4       PC1(SDA)©Ç 23
   19 ©Ï(OC1A)PD5       PC0(SCL)©Ç 22
   20 ©Ï(ICP1)PD6       PD7(OC2)©Ç 21
      ©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿
*/
void PortInit( void )
{
    DDRA  = 0b11111110;                     // PA0 Input, PA1..7 Output
    DDRB  = 0b11111111;                     // PB0..7 Output
    DDRC  = 0b11111111;                     // PC0..7 Output
    DDRD  = 0b10111110;                     // PD7,5,4,3,2 Output,,PD1-TxD,PD0-RxD

    PORTA = 0b00000000;                     // PA Low
    PORTB = 0b00010000;                     // PB4 High
    PORTC = 0b00000000;                     // PC Low
    PORTD = 0b00000000;                     // PD Low
}
