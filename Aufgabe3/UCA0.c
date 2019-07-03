/*
 * UCA0.c
 *
 *  Created on: 10.04.2018
 *      Author: Admin
 */

#include <msp430.h>
#include "base.h"
#include "uca0.h"
#include "event.h"
#include "AS1108.h"

GLOBAL const Int BUFSIZE = 16;
GLOBAL Char buf[BUFSIZE+1];

LOCAL const Char EndOfTExt = '\0';
LOCAL const Char * ptr = &EndOfTExt;

GLOBAL Void UCA0_Init(Void) {

   // set up Universal Serial Communication Interface A
   SETBIT(UCA0CTLW0, UCSWRST);        // UCA0 software reset
   UCA0CTLW1 = 0x0002;                // deglitch time approximately 100 ns
   UCA0CTLW0_H = 0xC0;                // even parity, LSB first,  ...
                                      // ... 8-bit data,  One stop bit,
                                      // ... UART mode, Asynchronous mode

   SETBIT(UCA0CTLW0, UCSSEL__ACLK);  // select ACLK with 614,4 kHz
   UCA0BRW = 2;                      // set clock prescaler for 14400 baud
   UCA0MCTLW_L = 10 << 4;            // first modulation stage
   UCA0MCTLW_H = 0xD6;               // second modulation stage
   SETBIT(UCA0MCTLW, UCOS16);        // times 16 oversampling

   CLRBIT(P2SEL0, BIT1 + BIT0);       // set up Port 2: Pin0 => TXD, ...
   SETBIT(P2SEL1, BIT1 + BIT0);       // ... Pin1 <= RXD
   CLRBIT(P2REN,  BIT1 + BIT0);       // without pull up
   CLRBIT(UCA0CTLW0, UCSWRST);        // release the UCA0 for operation
   SETBIT(UCA0IE, UCRXIE);            // enable receive interrupt
}

#pragma vector = USCI_A0_VECTOR
__interrupt Void UCA0_ISR(Void) {
   LOCAL Char ch  = '\0';
   LOCAL UInt idx = 0;
   switch (__even_in_range(UCA0IV, 0x04)) {
      case 0x02:  // Vector 2: Receive buffer full
         if (TSTBIT(UCA0STATW, UCBRK)) {
            set_error(BRK_ERROR); // break error
            set_event(EVENT_ERROR);
            ch = UCA0RXBUF; // dummy read
            idx = 0;
         } else if (TSTBIT(UCA0STATW, UCRXERR)) {
             set_error(ERR_ERROR); // framing, overrun, parity error
             set_event(EVENT_ERROR);
            ch = UCA0RXBUF; // dummy read
         } else {
            ch = UCA0RXBUF;
            if (ch EQ '\r') {
               buf[idx] = '\0';
               idx = 0;
               CLRBIT(UCA0IE, UCRXIE);   // disable rx interrupt
               set_event(EVENT_RXD);
               __low_power_mode_off_on_exit();
            } else if (between(0x20, ch, 0x7E)) {
               buf[idx] = ch;
               if (++idx EQ BUFSIZE) {
                  buf[idx] = '\0';
                  idx = 0;
                  CLRBIT(UCA0IE, UCRXIE);   // disable rx interrupt
                  set_event(EVENT_RXD);
                  __low_power_mode_off_on_exit();
               }
            }
         }
         break;
      case 0x04:  // Vector 4: Transmit buffer empty
          // error handling priority 1 and 4
          if (TSTBIT(UCA0STATW, UCBRK)) {
              set_error(BRK_ERROR);  // break error
              set_event(EVENT_ERROR);
           } else if (TSTBIT(UCA0STATW, UCRXERR)) {
               set_error(ERR_ERROR);  // framing, overrun, parity error
               set_event(EVENT_ERROR);
           }
          if (*ptr NE '\0') {
            UCA0TXBUF = *ptr++;
          } else {
              ptr = &EndOfTExt;
              CLRBIT(UCA0IE, UCTXIE);   // disable tx interrupt
              SETBIT(UCA0IE, UCRXIE);   // enable rx interrupt
              __low_power_mode_off_on_exit();
          }
      default:
         break;
   }
}

GLOBAL Int UCA0_printf(const Char * str) {
   if (str EQ NULL) {
      return -1;
   }
   if (*ptr EQ '\0') {
      ptr = str;
      SETBIT(UCA0IFG, UCTXIFG); // set UCTXIFG
      SETBIT(UCA0IE,  UCTXIE);  // enable tx interrupt
      return 0;
   }
   return -1;
}



