/*
 * SPI_AS1108.c
 *
 *  Created on: 26.12.2018
 *      Author: Admin
 */


#include <msp430.h>
#include "base.h"
#include "event.h"
#include "AS1108.h"

// Basis des Zahlensystems
// Einstellung zwischen 2 und 10 soll möglich sein
#define BASE 10

// es sind geeignete Datenstrukturen für den Datenaustausch
// zwischen den Handlern festzulegen.

LOCAL Void AS1108_Write(UChar adr, UChar arg) {
   Char ch = UCA1RXBUF;   // dummy read, UCRXIFG := 0, UCOE := 0
   CLRBIT(P2OUT,  BIT3);  // Select aktivieren
   UCA1TXBUF = adr;       // Adresse ausgeben
   while (TSTBIT(UCA1IFG, UCRXIFG) EQ 0);
   ch = UCA1RXBUF;        // dummy read
   UCA1TXBUF = arg;       // Datum ausgeben
   while (TSTBIT(UCA1IFG, UCRXIFG) EQ 0);
   ch = UCA1RXBUF;        // dummy read
   SETBIT(P2OUT,  BIT3);  // Select deaktivieren
}


GLOBAL Void SPI_Init(Void) {
   // set up Pin 3 at Port 2 => CS output, High
   SETBIT(P2OUT,  BIT3);
   SETBIT(P2DIR,  BIT3);
   CLRBIT(P2SEL0, BIT3);
   CLRBIT(P2SEL1, BIT3);
   CLRBIT(P2REN,  BIT3);

   // set up Pins 4, 5 and 6 at Port 2
   CLRBIT(P2SEL0, BIT4 + BIT5 + BIT6);
   SETBIT(P2SEL1, BIT4 + BIT5 + BIT6);

   // set up Universal Serial Communication Interface A
   SETBIT(UCA1CTLW0, UCSWRST);        // UCA1 software reset

   // in Übereinstimung mit dem SPI-Timing-Diagramm von AS1108
   UCA1CTLW0_H = 0b10101001;          // clock phase: rising edge, ...
                                      // ... clock polarity: inactive low ...
                                      // ... MSB first, 8-bit data, SPI master mode, ...
                                      // ... 3-pin SPI, synchronous mode

   CLRBIT(UCA1CTLW0, UCSSEL1);        // select ACLK ...
   SETBIT(UCA1CTLW0, UCSSEL0);        // .. as source clock
   UCA1BRW = 0;                       // prescaler

   CLRBIT(UCA1CTLW0, UCSWRST);        // release the UCA0 for operation
}


// der Treiberbaustein AS1108 ist hier über die SPI-Schnittstelle zu initialisieren
GLOBAL Void AS1108_Init(Void) {
    AS1108_Write(0x0C, 0x01);  // reset feature register to default setting
    AS1108_Write(0x0C, 0x81);  // enter operation mode with default register

    //AS1108_Write(adr, arg);  // set brightness to xx

    AS1108_Write(0x0B, 0x03);  // display all digits (0x03)

    AS1108_Write(0x09, 0xFF);  // decode mode, B-/HEX for all digits

    // set all 4 digits to ZERO
    AS1108_Write(0x01, 0x00);
    AS1108_Write(0x02, 0x00);
    AS1108_Write(0x03, 0x00);
    AS1108_Write(0x04, 0x00);
}

// ----------------------------------------------------------------------------

// der Button-Handler beinhaltet keine Zustandsmaschine
GLOBAL Void Button_Handler(Void) {

}

// ----------------------------------------------------------------------------

// der Number-Handler beinhaltet keine Zustandsmaschine
GLOBAL Void Number_Handler(Void) {

}

// ----------------------------------------------------------------------------

// der AS1108_Hander beinhaltet eine Zustandsmaschine
GLOBAL Void AS1108_Handler(Void) {

}

