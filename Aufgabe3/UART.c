/*
 * UART.c
 *
 *  Created on: 02.07.2019
 *      Author: Jan Kaiser
 */

#include <msp430.h>
#include "base.h"
#include "event.h"
#include "AS1108.h"
#include <math.h>

// DCOCLK := 8.0 MHz, XT1CLK := 4.9152 MHz
// SMCLK := DCOCLK / 8
// MCLK := DCOCLK / 8
// ACLK := XT1CLK / 8
#define BAUD 14400

static int lut[10] = { 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x11, 0x21,
                       0x22 };

LOCAL Void SETUCBRSW_Settings(Void)
{
    // setup baud rate: (baudrate 14,400 kbit/s)
    // ACLK: 614,4 kHz
    SETBIT(UCA0CTLW0, UCSSEL__ACLK);
    // N = 614,4kHz / 14,400kHz = 42,67
    float n = 42.67;
    float p = n / 16;
    int pt = (int) p;
    UCA0BRW = pt;

    float q = (p - pt) * 16;
    int qt = (int) q;
    UCA0MCTLW_L = qt;

    int idx = ceil((q - qt) * 36);
    UCA0BRW_H = lut[idx];

    SETBIT(UCA0MCTLW, UCOS16);
    // ------------------
}

GLOBAL Void UART_Init(Void)
{
    SETBIT(UCA0CTLW0, UCSWRST); // UCA0 software reset
    UCA0CTLW1 = 0x0002;         // deglitch time approximately 100ns
    UCA0CTLW0_H = 0xC0;         // even parity, LSB first

    SETUCBRSW_Settings();

    CLRBIT(P2SEL0, BIT1 + BIT0); // set up Port 2: Pin0 => TXD, ...
    SETBIT(P2SEL1, BIT1 + BIT0); // ... Pin1 <= RXD
    CLRBIT(P2REN, BIT1 + BIT0);  // without pull up
    CLRBIT(UCA0CTLW0, UCSWRST);  // release the UCA0 for operation
}


#pragma vector = USCI_A0_VECTOR
__interrupt Void UCA0_ISR(Void)
{
    Char c = '0'; // debug dummy declaration
    switch (__even_in_range(UCA0IV, 0x08))
    {
    case 0x02: // Vector 2: Receive buffer full
        if (TSTBIT(UCA0STATW, UCBRK + UCRXERR))
        {
            Char ch = UCA0RXBUF; // dummy read
        }
        else
        {
            UCA0TXBUF = UCA0RXBUF;
        }
        break;
    case 0x04: // Vector 4: Transmit buffer empty
        break;
    case 0x06: // Vector 6: Start bit received
        break;
    case 0x08: // Vector 8: Transmit complete
        break;
    default:
        break;
    }
}
