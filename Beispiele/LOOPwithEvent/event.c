/*
 * event.c
 *
 *  Created on: 28.04.2018
 *      Author: Admin
 */

#include <msp430.h>
#include "..\base.h"
#include "event.h"

#define SIZEFLAGS 3

LOCAL UShort flags[SIZEFLAGS];

GLOBAL Void evn_Init(Void) {
   UInt i;
   for (i=0; i LT SIZEFLAGS; i++) {
      flags[i] = 0;
   }
}

GLOBAL Void evn_Clr(const UInt arg) {
   if (arg GE SIZEFLAGS) {
      // runtime error
      return;
   }
   _disable_interrupt();
   flags[EVENT0] -= 1;
   flags[arg]    -= 1;
   _enable_interrupt();
}

GLOBAL inline Void evn_Set(const UInt arg) {
   if (arg GE SIZEFLAGS) {
      // runtime error
      return;
   }
   flags[EVENT0] += 1;
   flags[arg]    += 1;
}

GLOBAL inline UShort evn_Get(const UInt arg) {
   if (arg GE SIZEFLAGS) {
      // runtime error
      return 0;
   }
   return flags[arg];
}
