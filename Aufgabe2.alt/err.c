/*
 * err.c
 *
 *  Created on: 25.04.2018
 *      Author: Admin
 */
#include <msp430.h>
#include "..\base.h"
#include "err.h"

GLOBAL volatile UInt err_Flags;

GLOBAL Void err_Init(Void) {
   err_Flags = NO_ERROR;
}

GLOBAL UInt err_GetAndClear(Void) {
   UInt tmp;
   UShort sreg = _get_SR_register();
   if (TSTBIT(sreg, GIE)) {
      _disable_interrupt();
      tmp = err_Flags;
      err_Flags = NO_ERROR;
      _enable_interrupt();
   } else {
      tmp = err_Flags;
      err_Flags = NO_ERROR;
   }
   return tmp;
}

LOCAL const Char nibble2hex[] = "0123456789ABCDEF";
/*
 * diese Funktion wandelt einen ganzzahligen 16-Bit-Wert
 * in eine 4-stellige Zeichenkette mit hexadezimalen
 * Symbolen um.
 * Der Buffer muss zur Aufnahme der Zeichenkette
 * ausgelegt sein.
 * Es wird kein Null-Byte eingeführt
 */
GLOBAL Void err_String(UInt err, Char *buf) {
   Int i = 12;
   while (i GE 0) {
      *buf++ = nibble2hex[(err >> i) BAND 0x000F];
      i -= 4;
   }
}
