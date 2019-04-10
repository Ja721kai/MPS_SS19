#include <msp430.h> /* --> msp430fr5729.h */
#include "..\base.h"
#include "uca0.h"

/**
 * main.c
 */
GLOBAL Void main(Void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // set up Pin 7 at Port 2 => output
    SETBIT(P2DIR,  BIT7);
    CLRBIT(P2SEL0, BIT7);
    CLRBIT(P2SEL1, BIT7);
    CLRBIT(P2OUT,  BIT7);
    CLRBIT(P2REN,  BIT7);

    // set up Pin 2 at Port 1 => output
    SETBIT(P1DIR,  BIT2);
    CLRBIT(P1SEL0, BIT2);
    CLRBIT(P1SEL1, BIT2);
    CLRBIT(P1OUT,  BIT2);
    CLRBIT(P1REN,  BIT2);

    // set up Clock System
    CSCTL0 = CSKEY;                                       // enable clock system
    CSCTL1 = DCOFSEL_3;                                   // DCO frequency = 8,0 MHz
    CSCTL2 = SELA__XT1CLK | SELS__DCOCLK | SELM__DCOCLK;  // select clock sources
    CSCTL3 = DIVA__8      | DIVS__8      | DIVM__8;       // set frequency divider
    // XT2 disabled, XT1: HF mode,  low power, no bypass
    CSCTL4 = XT2OFF | XTS | XT1DRIVE_0;
    CSCTL0_H = 0;                                         // disable clock system

    // set up Timer A with Compare Register
    TA0CCR0 = 0;                                          // stop Timer A
    CLRBIT(TA0CTL, TAIFG);                                // clear overflow flag
    CLRBIT(TA0CCR0, CCIFG);                               // clear CCI flag
    TA0CTL  = TASSEL__ACLK | MC__UP | ID__8;              // set up Timer A
    TA0EX0  = TAIDEX_7;                                   // set up expansion register
    TA0CCR0 = 96;                                         // set up CCR0 for 10 ms
    SETBIT(TA0CTL, TACLR);                                // clear and start Timer
    SETBIT(TA0CCTL0, CCIE);                               // enable Timer A interrupt

    UCA0_Init(); // set up Universal Serial Communication Interface A

    while(TRUE) {
       _low_power_mode_3 (); // enter low power mode LPM1, only ACLK active
    }
}

#define LED1max 100

#pragma vector = TIMER0_A0_VECTOR
__interrupt Void TA0_ISR(Void) {
    static UInt LED1cnt = 0;
    if (++LED1cnt GE LED1max) {
        LED1cnt = 0;
    }
    if ((LED1cnt EQ (LED1max/2-1)) OR (LED1cnt EQ (LED1max-1))) {
       TGLBIT(P2OUT, BIT7);            // toggle led
    }
    if (TSTBIT(P1IN, BIT1 | BIT0)) {
       SETBIT(P1OUT, BIT2);
    } else {
       CLRBIT(P1OUT, BIT2);
    }
 //   __low_power_mode_off_on_exit(); // restore Active Mode on return
}
