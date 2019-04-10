/*
 * TA0.c
 *
 *  Created on: 18.04.2018
 *      Author: Admin
 */
#include <msp430.h>
#include "..\base.h"

GLOBAL Void TA0_Init(Void) {
   TA0CCR0 = 0;                               // stop Timer A
   CLRBIT(TA0CTL, TAIFG);                     // clear overflow flag
   CLRBIT(TA0CCR0, CCIFG);                    // clear CCI flag
   TA0CTL  = TASSEL__ACLK + MC__UP + ID__4;   // set up Timer A
   TA0EX0  = TAIDEX_4;                        // set up expansion register
   TA0CCR0 = 50/2;                            // set up compare register
   SETBIT(TA0CTL, TACLR);                     // clear and start Timer
   SETBIT(TA0CCTL0, CCIE);                    // enable Timer A interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt Void TA0_ISR(Void) {
   TGLBIT(P2OUT, BIT7);                        // toggle led
}
