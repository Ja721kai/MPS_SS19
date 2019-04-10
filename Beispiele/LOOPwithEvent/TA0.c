/*
 * TA0.c
 *
 *  Created on: 22.04.2018
 *      Author: Admin
 */

#include <msp430.h>
#include "..\base.h"
#include "event.h"

GLOBAL Void TA0_Init(Void) {
   TA0CCR0 = 0;                              // stopp Timer A
   CLRBIT(TA0CTL, TAIFG);                    // clear overflow flag
   CLRBIT(TA0CCR0, CCIFG);                   // clear CCI flag
   TA0CTL  = TASSEL__ACLK + MC__UP + ID__8;  // set up Timer A
   TA0EX0  = TAIDEX_7;                       // set up expansion register
   TA0CCR0 = 96;                             // set up CCR0 for 10 ms
   SETBIT(TA0CTL, TACLR);                    // clear and start Timer
   SETBIT(TA0CCTL0, CCIE);                   // enable Timer A interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt Void TA0_ISR(Void) {
   evn_Set(EVENT1);
   __low_power_mode_off_on_exit(); // restore Active Mode on return
}
