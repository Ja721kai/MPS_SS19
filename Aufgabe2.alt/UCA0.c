/*
 * UCA0.c
 *
 *  Created on: 10.04.2018
 *      Author: Admin
 */

#include <msp430.h>
#include "..\base.h"
#include "uca0.h"
#include "err.h"

GLOBAL Int   UCA0_Flag;
GLOBAL Char *UCA0_buf;

#define IBUFSIZE 16
LOCAL Char ibuf[IBUFSIZE];


GLOBAL Void UCA0_Init(Void) {

    UCA0_Flag  = 0;
    UCA0_buf   = ibuf;

    // set up Universal Serial Communication Interface A
    // deglitch time approximately 100 ns
    // UART mode, Asynchronous mode
    // even parity, LSB first,  8-bit data,  One stop bit,
    // select clock source: ACLK with 614.4 kHz
    // set clock prescaler for 9600 baud

}

LOCAL const Char *ptr;

#pragma vector = USCI_A0_VECTOR
__interrupt Void UCA0_ISR(Void) {
   volatile Char ch;
   switch (__even_in_range(UCA0IV, 0x04)) {
      case 0x02:  // Vector 2: Receive buffer full

         /*
          * die Funktionalität in der IST ist zu implementieren
          */

         break;
      case 0x04:  // Vector 4: Transmit buffer empty
         if (*ptr NE '\0') {
            UCA0TXBUF = *ptr++;
         } else {
            CLRBIT(UCA0IE, UCTXIE);   // disable transmit interrupt
            ch = UCA0RXBUF;           // dummy read
            SETBIT(UCA0IE, UCRXIE);   // enable receive interrupt
         }
         break;
      default:
    	 _never_executed();
         break;
   }
}

GLOBAL Int UCA0_printf(const Char *str) {
   if (str EQ NULL)
      return -1;
   ptr = str;
   SETBIT(UCA0IFG, UCTXIFG); // set UCTXIFG
   SETBIT(UCA0IE,  UCTXIE);  // enable transmit interrupt
   return 0;
}
