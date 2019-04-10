/*
 * TA1.c
 *
 *  Created on: 28.04.2018
 *      Author: Admin
 */

#include <msp430.h>
#include "ta1.h"

// Der Ausgang des Timer TA1 ist am Pin 2 vom Port 1
// ausgeführt. Somit ist es möglich, den Timer TA1 so zu
// konfigurieren, dass er eine LED ohne eine ISR blinken
// lässt.

// XT1CLK = 4.9152 MHz
// ACLK = XT1CLK / 8 = 614.4 kHz
// input divider: ACLK / 8 = 76.8 kHz
// expansion register: IDEX := 76.8 kHz / 8 = 9600
// compare register CCR0: 9600 / 2 = 4800 => (500 ms)
// compare register CCR1: im toggle Mode ist der Wert egal.
GLOBAL Void TA1_Init(Void) {
   // set up Pin 2 at Port 1
   SETBIT(P1DIR,  BIT2); // direction: output
   SETBIT(P1SEL0, BIT2); // TA1.1
   CLRBIT(P1SEL1, BIT2); // TA1.1
   CLRBIT(P1OUT,  BIT2);
   CLRBIT(P1REN,  BIT2); // without pull up

   TA1CCR0 = 0;                             // stop Timer A1
   CLRBIT(TA1CTL, TAIFG);                   // clear overflow flag
   CLRBIT(TA1CCR0, CCIFG);                  // clear CCI flag
   TA1CCTL1 = 0x0000;                       //
   SETBIT(TA1CCTL1, OUTMOD_4);              // output mode: toggle
   TA1CTL  = TASSEL__ACLK + MC__UP + ID_3;  // set up Timer A1 (/8)
   TA1EX0  = TBIDEX_7;                      // set up expansion register (/8)
   TA1CCR0 = 4800;                          // set up compare register
   TA1CCR1 = 2400;                          // set up compare register
   SETBIT(TA1CTL, TACLR);                   // clear and start Timer
   CLRBIT(TA1CCTL0, CCIE);                  // disable Timer A1 interrupt
}
