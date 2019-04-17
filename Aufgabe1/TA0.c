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
 * die eine laufzeiteffiziente Ausführung der ISR
 * ermöglichen.
 */

static Char muster = 0;
static Short counter = 0;

static Char cnt = 0; // Zähler für Hysterese-Funktion
static Char cnt2 = 0;
static Char state1 = 0; // Zustände für Hysterese-Funktion
static Char state2 = 0;
const Char n = 6;


const Char num_buttons = 2;  // Button 1 und 2
const Char num_button_states = 3;  // State, Taste, Cnt (Folie 45)
static Char buttons [num_buttons][num_button_states];




GLOBAL Void set_blink_muster(UInt arg)
{
    /*
     * Die Funktion muss so erweitert werden,
     * dass ein Blinkmuster selektiert wird.
     */
    muster = arg;
    counter = 0;

}

// Der Timer A0 ist bereits initialisiert
GLOBAL Void TA0_Init(Void)
{
    TA0CCR0 = 0;                              // stop Timer A
    CLRBIT(TA0CTL, TAIFG);                    // clear overflow flag
    CLRBIT(TA0CCR0, CCIFG);                   // clear CCI flag
    TA0CTL = TASSEL__ACLK + MC__UP + ID__8;  // set up Timer A
    TA0EX0 = TAIDEX_7;                       // set up expansion register
    TA0CCR0 = 2 * 48;                           // set up CCR0 for 10 ms

    CLRBIT(P1DIR, BIT1 + BIT0);             //set up Button 1 and 2
    CLRBIT(P1SEL0, BIT1 + BIT0);
    CLRBIT(P1SEL1, BIT1 + BIT0);
    CLRBIT(P1OUT, BIT1 + BIT0);
    CLRBIT(P1REN, BIT1 + BIT0);

    SETBIT(TA0CTL, TACLR);                    // clear and start Timer
    SETBIT(TA0CCTL0, CCIE);                   // enable Timer A interrupt

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt Void TA0_ISR(Void)
{

    Int btn1 = TSTBIT(P1IN, BIT1);
    Int btn2 = TSTBIT(P1IN, BIT0);


    /*Hysterese*/
    if (btn1) {
        if (cnt == n-1 && state1 != 1) {
            state1 = 1;
            set_event(EVENT_BTN1);
        }
        if (cnt == 0) {
            state1 = 0;
        }
        if (cnt < n-1) {
            cnt ++;
        }
    } else {
        if (cnt > 0) {
            cnt--;
        }
    }

    /*Hysterese Button2*/
    if (btn2) {
            if (cnt2 == n-1 && state2 != 1) {
                state2 = 1;
                set_event(EVENT_BTN2);
                __low_power_mode_off_on_exit();
            }
            if (cnt2 == 0) {
                state2 = 0;
            }
            if (cnt2 < n-1) {
                cnt2 ++;
            }
        } else {
            if (cnt2 > 0) {
                cnt2--;
            }
        }


    switch (muster)
    {
    case 0:                         // Muster 1: 2s/0.5s
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
                __low_power_mode_off_on_exit();
            }
        }
        break;
    default:
        SETBIT(P1OUT, BIT2);
        break;
    }

}
