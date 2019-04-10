/*
 * TA0.c
 *
 *  Created on: 15.04.2018
 *      Author: Admin
 */
#include <msp430.h> /* --> msp430fr5729.h */
#include "..\base.h"
#include "ta0.h"

#define CCR0 96
#define LED1max 9600/CCR0

GLOBAL Void TA0_Init(Void) {
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

    // set up Timer A with Compare Register
    TA0CCR0 = 0;                                          // stop Timer A
    CLRBIT(TA0CTL, TAIFG);                                // clear overflow flag
    CLRBIT(TA0CCR0, CCIFG);                               // clear CCI flag
    TA0CTL  = TASSEL__ACLK | MC__UP | ID__8;              // set up Timer A
    TA0EX0  = TAIDEX_7;                                   // set up expansion register
    TA0CCR0 = CCR0;                                       // set up CCR0 for 10 ms
    SETBIT(TA0CTL, TACLR);                                // clear and start Timer
    SETBIT(TA0CCTL0, CCIE);                               // enable Timer A interrupt
}

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
