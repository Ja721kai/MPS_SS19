#include <msp430.h> /* --> msp430fr5729.h */
#include "..\base.h"
#include "ta0.h"
#include "uca0.h"

/**
 * main.c
 */
GLOBAL Void main(Void) {
   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

   // set up Clock System
   CSCTL0 = CSKEY;                                       // enable clock system
   CSCTL1 = DCOFSEL_3;                                   // DCO frequency = 8,0 MHz
   CSCTL2 = SELA__XT1CLK | SELS__DCOCLK | SELM__DCOCLK;  // select clock sources
   CSCTL3 = DIVA__8      | DIVS__8      | DIVM__8;       // set frequency divider
   // XT2 disabled, XT1: HF mode,  low power, no bypass
   CSCTL4 = XT2OFF | XTS | XT1DRIVE_0;
   CSCTL0_H = 0;                                         // disable clock system

   TA0_Init();  // set up Timer A0
   UCA0_Init(); // set up Universal Serial Communication Interface A

   while(TRUE) {
      _low_power_mode_3 (); // enter low power mode LPM3, only ACLK active
   }
}
