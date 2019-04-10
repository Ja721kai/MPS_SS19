#include <msp430.h> 
#include <stdio.h>
#include "..\base.h"
#include "uca0.h"
#include "err.h"

/**
 * main.c
 */

LOCAL Void CS_Init(Void);

LOCAL const Char Prompt[] = "> ";
LOCAL const Char OK[] = " OK\r\n> ";
LOCAL Char msg1[] = " ? #....\r\n> ";

GLOBAL Void main(Void) {
   UInt tmp;
   WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer

   CS_Init();   // set up Clock System
   err_Init();  // set up error flags
   UCA0_Init(); // set up UCA0

   UCA0_printf(Prompt);
   while(TRUE) {
      _low_power_mode_3(); // do nothing
      if (UCA0_Flag EQ 1) {
         if (err_Flags EQ NO_ERROR) {
             UCA0_printf(OK);
         } else {
        	    tmp = err_GetAndClear();
             err_String(tmp, &msg1[4]);
             UCA0_printf(msg1);
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
