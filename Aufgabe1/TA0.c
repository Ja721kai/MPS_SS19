/*
 * TA0.c
 *
 *  Created on: 22.04.2018
 *      Author: Admin
 */

#include <msp430.h>
#include "base.h"
#include "TA0.h"
#include "event.h"

/*
 * Man soll sich geeignete Datenstrukturen überlegen,
 * die eine laufzeitefffiziente Auusführung der ISR
 * ermöglichen.
 */

//const int ten_millis = 10;
static Int muster = 0;
static Int counter = 0;

GLOBAL Void set_blink_muster(UInt arg)
{
    /*
     * Die Funktion muss so ertweitert werden,
     * dass ein Blinkmuster selektiert wird.
     */

    muster = arg;
    counter = 0;

}

// Der Timer A0 ist bereits initialisiert
GLOBAL Void TA0_Init(Void)
{
    TA0CCR0 = 0;                              // stopp Timer A
    CLRBIT(TA0CTL, TAIFG);                    // clear overflow flag
    CLRBIT(TA0CCR0, CCIFG);                   // clear CCI flag
    TA0CTL = TASSEL__ACLK + MC__UP + ID__8;  // set up Timer A
    TA0EX0 = TAIDEX_7;                       // set up expansion register
    TA0CCR0 = 2 * 48;                           // set up CCR0 for 10 ms
    SETBIT(TA0CTL, TACLR);                    // clear and start Timer
    SETBIT(TA0CCTL0, CCIE);                   // enable Timer A interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt Void TA0_ISR(Void)
{
    switch (muster)
    {
    case 0:                         // Muster 1: 2s / 0.5s
        if (counter < 200)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
            if (counter == 250)
            {
                counter = 0;
                set_event(EVENT_BTN1);
                __low_power_mode_off_on_exit();
            }
        }

        break;
    case 1:
        if (counter < 75)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
            if (counter == 150)
            {
                counter = 0;
                set_event(EVENT_BTN1);
                __low_power_mode_off_on_exit();
            }
        }
        break;
    case 2:
        if (counter < 25)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
            if (counter == 50)
            {
                counter = 0;
                set_event(EVENT_BTN1);
                __low_power_mode_off_on_exit();
            }
        }
        break;
    case 3:
        if (counter < 50)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
            if (counter == 250)
            {
                counter = 0;
                set_event(EVENT_BTN1);
                __low_power_mode_off_on_exit();
            }
        }
        break;
    case 4:
        if (counter < 50)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else if (counter < 100)
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
        }
        else if (counter < 150)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
            if (counter == 350)
            {
                counter = 0;
                set_event(EVENT_BTN1);
                __low_power_mode_off_on_exit();
            }
        }
        break;
    case 5:
        if (counter < 50)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else if (counter < 100)
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
        }
        else if (counter < 150)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else if (counter < 200)
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
        }
        else if (counter < 250)
        {
            SETBIT(P1OUT, BIT2);
            counter++;
        }
        else
        {
            CLRBIT(P1OUT, BIT2);
            counter++;
            if (counter == 450)
            {
                counter = 0;
                set_event(EVENT_BTN1);
                __low_power_mode_off_on_exit();
            }
        }
        break;
    default:
        SETBIT(P1OUT, BIT2);
        break;
    }

}
