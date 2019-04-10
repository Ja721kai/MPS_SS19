#include <msp430.h> 
#include <msp430.h>
#include "..\base.h"

// die LED am Pin 7 vom Port 2 soll mit
// einer Frequenz von ca. 1 Hz blinken

// #define withXT1 0

LOCAL Void CS_Init(Void);
LOCAL Void Port_Init(Void);
LOCAL Void Timer_Init(Void);

/**
 * main.c
 */
GLOBAL Int main(Void) {
   // stop watchdog timer
   WDTCTL = WDTPW + WDTHOLD;

   CS_Init();
   Port_Init();
   Timer_Init();

   while(TRUE) {
      while (TSTBIT(TA0CTL, TAIFG) EQ 0) ;   // wait for overflow TAIFG
      CLRBIT(TA0CTL, TAIFG);                 // clear overflow flag
      TGLBIT(P2OUT, BIT7);                   // toggle led
   }
}

LOCAL Void CS_Init(Void) {
   CSCTL0 = CSKEY;                                       // enable clock system
#ifdef withXT1
   CSCTL1 = DCOFSEL_3;                                   // DCO frequency = 8,0 MHz
   CSCTL2 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK;  // select clock sources
   CSCTL3 = DIVA__8      + DIVS__8      + DIVM__8;       // set frequency divider
   // XT2 disabled, XT1: HF mode,  low power, no bypass
   CSCTL4 = XT2OFF + XTS + XT1DRIVE_0;
#else
   CSCTL1 = DCOFSEL_3;                                   // frequency = 8,0 MHz
   CSCTL2 = SELA__VLOCLK + SELS__DCOCLK + SELM__DCOCLK;  // select clock sources
   CSCTL3 = DIVA__8      + DIVS__32     + DIVM__8;       // set frequency divider
#endif
   CSCTL0_H = 0;                                         // disable clock system
}

LOCAL Void Port_Init(Void) {
   CLRBIT(P2OUT,  BIT7);
   SETBIT(P2DIR,  BIT7);
   CLRBIT(P2SEL0, BIT7);
   CLRBIT(P2SEL1, BIT7);
   CLRBIT(P2REN,  BIT7);
}


LOCAL Void Timer_Init(Void) {
   TA0CCR0 = 0;                               // stop Timer A0
   CLRBIT(TA0CTL, TAIFG);                     // clear overflow flag
   CLRBIT(TA0CCR0, CCIFG);                    // clear CCI flag
   TA0CTL  = TASSEL__SMCLK + MC__UP + ID__8;  // set up Timer A0
   TA0CCR0 = 31250/2;                         // set up compare register
   SETBIT(TA0CTL, TACLR);                     // clear and start Timer
}

/*
LOCAL Void Timer_Init(Void) {
   TB2CCR0 = 0;                               // stop Timer B2
   CLRBIT(TB2CTL, TBIFG);                     // clear overflow flag
   CLRBIT(TB2CCR0, CCIFG);                    // clear CCI flag
   // CNTL for 16-bit, Input divider /8
   TB2CTL  = TBSSEL__ACLK + MC__UP + ID_3;    // set up Timer B2
   TB2CCR0 = 38400;                           // set up compare register
   SETBIT(TB2CTL, TBCLR);                     // clear and start Timer
}
*/
/*
LOCAL Void Timer_Init(Void) {
   TA1CCR0 = 0;                               // stop Timer A1
   CLRBIT(TA1CTL, TBIFG);                     // clear overflow flag
   CLRBIT(TA1CCR0, CCIFG);                    // clear CCI flag
   // CNTL for 16-bit, Input divider /8
   TA1CTL  = TASSEL__ACLK + MC__UP + ID_3;    // set up Timer A1
   TA1CCR0 = 38400;                           // set up compare register
   SETBIT(TA1CTL, TBCLR);                     // clear and start Timer
}
*/
