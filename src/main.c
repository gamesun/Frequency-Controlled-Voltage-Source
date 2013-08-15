/*******************************************************************
 File  : main.c
 Coder : gamesun
*******************************************************************/

#include "common.h"
#include "main.h"
#include "uart.h"
#include "counter.h"
#include "spi.h"
#include "cmd.h"

static void InitDevices( void );
static void IOPortInit( void );

int main( void )
{
    
    InitDevices();
    
    
    while ( 1 ){
        UartHandle();
        CmdHandle();
    }
    
    return 0;
}


static void InitDevices( void )
{
    cli();

    _delay_ms( 10 );

    MCUCR = 0x00;                           // sleep mode off
    IOPortInit();
    CounterInit();
    SpiInit();
    UartInit();


    sei();
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
static void IOPortInit( void )
{
    DDRA  = 0xFF;                           // PA0..7 Output
    DDRB  = 0xFF;                           // PB0..7 Output
    DDRC  = 0xFF;                           // PC0..7 Output
    DDRD  = 0b10111110;                     // PD7,5,4,3,2 Output,,PD1-TxD,PD0-RxD

    PORTA = 0b00000000;                     // PA Low
    PORTB = 0b00010000;                     // PB4 High
    PORTC = 0b00000000;                     // PC Low
    PORTD = 0b00000000;                     // PD Low
}


