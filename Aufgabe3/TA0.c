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

const Char n = 6;
const Char num_buttons = 2;  // Button 1 und 2
const Char num_button_states = 2;  // State, Cnt (Folie 45)
static Bool states[2] = { 0, 1 };
static Char cnt[6] = { 0, 1, 2, 3, 4, 5 };

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
    TA0CCR0 = 2 * 48;                        // set up CCR0 for 10 ms

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

    static Bool btn1;
    static Bool btn2;
    static Bool btn3;
    static Bool btn4;
    static Bool btn5;
    static Bool btn6;

    static Bool* ptr_state1 = &states[0];
    static Char* ptr_cnt1 = &cnt[0];
    static Bool* ptr_state2 = &states[0];
    static Char* ptr_cnt2 = &cnt[0];
    static Bool* ptr_state3 = &states[0];
    static Char* ptr_cnt3 = &cnt[0];
    static Bool* ptr_state4 = &states[0];
    static Char* ptr_cnt4 = &cnt[0];
    static Bool* ptr_state5 = &states[0];
    static Char* ptr_cnt5 = &cnt[0];
    static Bool* ptr_state6 = &states[0];
    static Char* ptr_cnt6 = &cnt[0];

    btn1 = TSTBIT(P1IN, BIT1);
    btn2 = TSTBIT(P1IN, BIT0);
    btn3 = TSTBIT(P3IN, BIT0);
    btn4 = TSTBIT(P3IN, BIT1);
    btn5 = TSTBIT(P3IN, BIT2);
    btn6 = TSTBIT(P3IN, BIT3);

    /*Hysterese*/
    /* Button 1 ohne Funktion in Aufgabe 3
    if (btn1)
    {
        if (*ptr_cnt1 == n - 1 && *ptr_state1 == 0)
        {
            ptr_state1++;
            if (!tst_event(EVENT_BTN1))
            {
                set_event(EVENT_BTN1);
            }
        }
        if (*ptr_cnt1 == 0 && *ptr_state1 == 1)
        {
            ptr_state1--;
        }
        if (*ptr_cnt1 < n - 1)
        {
            ptr_cnt1++;
        }
    }
    else
    {
        if (*ptr_cnt1 > 0)
        {
            ptr_cnt1--;
        }
    }
    */

    /*Hysterese Button2*/
    if (btn2)
    {
        if (*ptr_cnt2 == n - 1 && *ptr_state2 == 0)
        {
            ptr_state2++;
            set_event(EVENT_BTN2);
            set_event(EVENT_7LED); // signal to switch mode in AS1108
            __low_power_mode_off_on_exit();
        }
        if (*ptr_cnt2 == 0 && *ptr_state2 == 1)
        {
            ptr_state2--;
        }
        if (*ptr_cnt2 < n - 1)
        {
            ptr_cnt2++;
        }
    }
    else
    {
        if (*ptr_cnt2 > 0)
        {
            ptr_cnt2--;
        }
    }
    if (btn3)
    {
        if (*ptr_cnt3 == n - 1 && *ptr_state3 == 0)
        {
            ptr_state3++;
            set_event(EVENT_BTN3);
            __low_power_mode_off_on_exit();
        }
        if (*ptr_cnt3 == 0 && *ptr_state3 == 1)
        {
            ptr_state3--;
        }
        if (*ptr_cnt3 < n - 1)
        {
            ptr_cnt3++;
        }
    }
    else
    {
        if (*ptr_cnt3 > 0)
        {
            ptr_cnt3--;
        }
    }
    if (btn4)
    {
        if (*ptr_cnt4 == n - 1 && *ptr_state4 == 0)
        {
            ptr_state4++;
            set_event(EVENT_BTN4);
            __low_power_mode_off_on_exit();
        }
        if (*ptr_cnt4 == 0 && *ptr_state4 == 1)
        {
            ptr_state4--;
        }
        if (*ptr_cnt4 < n - 1)
        {
            ptr_cnt4++;
        }
    }
    else
    {
        if (*ptr_cnt4 > 0)
        {
            ptr_cnt4--;
        }
    }
    if (btn5)
    {
        if (*ptr_cnt5 == n - 1 && *ptr_state5 == 0)
        {
            ptr_state5++;
            set_event(EVENT_BTN5);
            __low_power_mode_off_on_exit();
        }
        if (*ptr_cnt5 == 0 && *ptr_state5 == 1)
        {
            ptr_state5--;
        }
        if (*ptr_cnt5 < n - 1)
        {
            ptr_cnt5++;
        }
    }
    else
    {
        if (*ptr_cnt5 > 0)
        {
            ptr_cnt5--;
        }
    }
    if (btn6)
    {
        if (*ptr_cnt6 == n - 1 && *ptr_state6 == 0)
        {
            ptr_state6++;
            set_event(EVENT_BTN6);
            __low_power_mode_off_on_exit();
        }
        if (*ptr_cnt6 == 0 && *ptr_state6 == 1)
        {
            ptr_state6--;
        }
        if (*ptr_cnt6 < n - 1)
        {
            ptr_cnt6++;
        }
    }
    else
    {
        if (*ptr_cnt6 > 0)
        {
            ptr_cnt6--;
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
