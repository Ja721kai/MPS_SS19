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
#define OVERFLOW BASE*BASE*BASE*BASE
#define BASE_POW2 BASE*BASE
#define BASE_POW3 BASE*BASE*BASE

// es sind geeignete Datenstrukturen für den Datenaustausch
// zwischen den Handlern festzulegen.

typedef Void (* VoidFunc)(Void);

LOCAL unsigned int selectedValue = 0;
LOCAL unsigned char displayedValue[4] = { 0, 0, 0, 0 };
GLOBAL Bool mode = 1;  // mode 1 -> increment, mode 0 -> decrement

LOCAL unsigned char digit;
LOCAL char ch;  // warning is fine, only used for dummy reads
LOCAL Bool jump_to_S3;
LOCAL VoidFunc state = State0;

LOCAL Void AS1108_Write(UChar adr, UChar arg)
{
    Char ch = UCA1RXBUF;   // dummy read, UCRXIFG := 0, UCOE := 0
    CLRBIT(P2OUT, BIT3);  // Select aktivieren
    UCA1TXBUF = adr;       // Adresse ausgeben
    while (TSTBIT(UCA1IFG, UCRXIFG) EQ 0)
        ;
    ch = UCA1RXBUF;        // dummy read
    UCA1TXBUF = arg;       // Datum ausgeben
    while (TSTBIT(UCA1IFG, UCRXIFG) EQ 0)
        ;
    ch = UCA1RXBUF;        // dummy read
    SETBIT(P2OUT, BIT3);  // Select deaktivieren
}

GLOBAL Void SPI_Init(Void)
{
    // set up Pin 3 at Port 2 => CS output, High
    SETBIT(P2OUT, BIT3);
    SETBIT(P2DIR, BIT3);
    CLRBIT(P2SEL0, BIT3);
    CLRBIT(P2SEL1, BIT3);
    CLRBIT(P2REN, BIT3);

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
GLOBAL Void AS1108_Init(Void)
{

    AS1108_Write(0x0C, 0x01);  // reset feature register to default setting

    /* set digits to "HEHE"
     AS1108_Write(0x03, 0x0B);
     AS1108_Write(0x04, 0x0C);
     AS1108_Write(0x01, 0x0B);
     AS1108_Write(0x02, 0x0C);*/

    AS1108_Write(0x03, 0x00);
    AS1108_Write(0x04, 0x00);
    AS1108_Write(0x01, 0x00);
    AS1108_Write(0x02, 0x00);

    AS1108_Write(0x09, 0xFF);  // decode mode, B-/HEX for all digits

    AS1108_Write(0x0A, 0x03);  // set brightness to maximum brightness

    AS1108_Write(0x0B, 0x03);  // display all digits (0x03)

    AS1108_Write(0x0C, 0x81);  // enter operation mode with default register

    //AS1108_Write(0x0F, 0x01);  // display test mode
}

// ----------------------------------------------------------------------------

// der Button-Handler beinhaltet keine Zustandsmaschine
GLOBAL Void Button_Handler(Void)
{
    // switch modes?
    if (tst_event(EVENT_7LED))
    {
        clr_event(EVENT_7LED);
        mode ^= 1; // toggle mode bit
    }
    // button 6-3 pressed?
    if (tst_event(EVENT_BTN3))
    {
        clr_event(EVENT_BTN3);
        selectedValue = 1;
        set_event(EVENT_10); // Handler 2 Event
    }
    if (tst_event(EVENT_BTN4))
    {
        clr_event(EVENT_BTN4);
        selectedValue = BASE;
        set_event(EVENT_10); // Handler 2 Event
    }
    if (tst_event(EVENT_BTN5))
    {
        clr_event(EVENT_BTN5);
        selectedValue = BASE_POW2;
        set_event(EVENT_10); // Handler 2 Event
    }
    if (tst_event(EVENT_BTN6))
    {
        clr_event(EVENT_BTN6);
        selectedValue = BASE_POW3;
        set_event(EVENT_10); // Handler 2 Event
    }
}

// ----------------------------------------------------------------------------

LOCAL Void Calculate_Displayed_Value() {
    LOCAL int currentValue;
    if (mode)
    {
        currentValue += selectedValue;
    }
    else
    {
        currentValue -= selectedValue;
    }
    // handle overflow:
    if (currentValue GE OVERFLOW)
    {
        currentValue -= OVERFLOW;
    }
    if (currentValue LT 0)
    {
        currentValue += OVERFLOW;
    }

    displayedValue[3] = currentValue / (BASE_POW3);
    displayedValue[2] = (currentValue % (BASE_POW3)) / (BASE_POW2);
    displayedValue[1] = (currentValue % (BASE_POW2)) / BASE;
    displayedValue[0] = currentValue % BASE;
}


// der Number-Handler beinhaltet keine Zustandsmaschine
GLOBAL Void Number_Handler(Void)
{
    if (tst_event(EVENT_10))
    {
        clr_event(EVENT_10);

        Calculate_Displayed_Value();

        set_event(EVENT_11);  // Handler 3 Event
    }
}

// ----------------------------------------------------------------------------

LOCAL Void State0(Void) {
    if (tst_event(EVENT_11))
    {
        jump_to_S3 = 0;
        digit = 0x01;  // initialize with index at digit 0x01
        state = State1;
    }
}

LOCAL Void State1(Void) {
    ch = UCA1RXBUF;   // dummy read, UCRXIFG := 0, UCOE := 0
    CLRBIT(P2OUT, BIT3);  // Select aktivieren
    UCA1TXBUF = digit;       // Adresse ausgeben
    state = State4;
}

LOCAL Void State2(Void) {
    ch = UCA1RXBUF;        // dummy read
    UCA1TXBUF = displayedValue[digit - 1];   // Datum ausgeben, stattdessen pointer?
    jump_to_S3 = 1;
    state = State4;
}

LOCAL Void State3(Void) {
    ch = UCA1RXBUF;        // dummy read
    SETBIT(P2OUT, BIT3);  // Select deaktivieren
    digit++;
    if (digit EQ 0x05)
    {
        clr_event(EVENT_11);
        state = State0;
    }
    else
    {
        state = State1;
    }
}

LOCAL Void State4(Void) {
    if (TSTBIT(UCA1IFG, UCRXIFG) NE 0)
    {
        if (jump_to_S3)
        {
            state = State3;
            jump_to_S3 = 0;
        }
        else
        {
            state = State2;
        }
    }
}


// der AS1108_Hander beinhaltet eine Zustandsmaschine
GLOBAL Void AS1108_Handler(Void)
{
    (*state)();
}

