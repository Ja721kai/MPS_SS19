#include <msp430.h> 
#include <stdio.h>
#include "..\base.h"
#include "uca0.h"
#include "TA1.h"
#include "TA0.h"
#include "event.h"

/**
 * main.c
 */

LOCAL Void port_Init(Void);
LOCAL Void CS_Init(Void);
LOCAL UInt Handler1(Void);
LOCAL UInt Handler2(Void);

GLOBAL Void main(Void) {
   WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
	
   CS_Init();    // set up Clock System
   port_Init();  // set up ports
   TA0_Init();   // set up Port and Timer A0 with Compare Register
   TA1_Init();   // set up Port and Timer A1 with Compare Register
   UCA0_Init();  // set up Universal Serial Communication Interface A

   while(TRUE) {
      _disable_interrupt();
      if (evn_Get(EVENT0) EQ 0) {
         _low_power_mode_3(); // do nothing
      } else {
         _enable_interrupt();
         if (evn_Get(EVENT1) NE 0) {
            if (Handler1() EQ 0) {
                evn_Clr(EVENT1);
            }
         }
         if (evn_Get(EVENT2) NE 0) {
            if (Handler2() EQ 0) {
                evn_Clr(EVENT2);
            }
         }
      }
   }
}

LOCAL Void CS_Init(Void) {
   CSCTL0 = CSKEY;                                       // enable clock system
   CSCTL1 = DCOFSEL_3;                                   // DCO frequency = 8,0 MHz
   CSCTL2 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK;  // select clock sources
   CSCTL3 = DIVA__8      + DIVS__8      + DIVM__8;       // set frequency divider
   // XT2 disabled, XT1: HF mode,  low power, no bypass
   CSCTL4 = XT2OFF + XTS + XT1DRIVE_0;
   CSCTL0_H = 0;                                         // disable clock system
}

LOCAL Void port_Init(Void) {
   // set up Pin 7 at Port 2 => output
   SETBIT(P2DIR,  BIT7);
   CLRBIT(P2SEL0, BIT7);
   CLRBIT(P2SEL1, BIT7);
   CLRBIT(P2OUT,  BIT7);
   CLRBIT(P2REN,  BIT7);

   // set up Pin 0+1 at Port 1 => input
   CLRBIT(P1DIR,  BIT1 + BIT0);
   CLRBIT(P1SEL0, BIT1 + BIT0);
   CLRBIT(P1SEL1, BIT1 + BIT0);
   CLRBIT(P1OUT,  BIT1 + BIT0);
   CLRBIT(P1REN,  BIT1 + BIT0);
}

// Return-Wert 0 bedeutet, dass der Handler mit
// Verarbeitung der Daten fertig ist.
// Return-Wert 1 bedeutet, dass der Handler
// noch beschäftigt ist
LOCAL UInt Handler1(Void) {
   if (TSTBIT(P1IN, BIT1 + BIT0)) {
      SETBIT(P2OUT, BIT7);
   } else {
      CLRBIT(P2OUT, BIT7);
   }
   return 0;
}

LOCAL UInt Handler2(Void) {
   UCA0TXBUF = UCA0RXBUF;
   return 0;
}
