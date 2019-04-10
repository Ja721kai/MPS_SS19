#include <msp430.h>
#include "..\base.h"
#include "TA0.h"

/**
 * main.c
 */

LOCAL Void CS_Init(Void);
LOCAL Void Port_Init(Void);


GLOBAL Void main(Void) {
   // stop watchdog timer
   WDTCTL = WDTPW + WDTHOLD;

   CS_Init();   // set up Clock System
   Port_Init(); // set up Port
   TA0_Init();  // set Timer A with Compare Register

   while(TRUE) {
      _low_power_mode_3 (); // do nothing
   }
}

LOCAL Void CS_Init(Void) {
   CSCTL0 = CSKEY;                                       // enable clock system
   CSCTL1 = DCOFSEL_3;                                   // frequency = 8,0 MHz
   CSCTL2 = SELA__VLOCLK + SELS__DCOCLK + SELM__DCOCLK;  // select clock sources
   CSCTL3 = DIVA__8      + DIVS__32     + DIVM__8;       // set frequency divider
   CSCTL0_H = 0;                                         // disable clock system
}

LOCAL Void Port_Init(Void) {
   CLRBIT(P2OUT,  BIT7);
   SETBIT(P2DIR,  BIT7);
   CLRBIT(P2SEL0, BIT7);
   CLRBIT(P2SEL1, BIT7);
   CLRBIT(P2REN,  BIT7);
}
