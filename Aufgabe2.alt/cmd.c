#include <string.h>
#include "..\base.h"
#include "cmd.h"
#include "err.h"

LOCAL Int base = 10; // Basis des Zahlensystems

#define STACKSIZE 16
LOCAL UInt idx; // zeigt auf den freien Platz auf dem Stack
LOCAL Int  stack[STACKSIZE];

LOCAL Int  store(Void);
LOCAL Int  fetch(Void);
LOCAL Int  emit(Void);
LOCAL Int  base10(Void);
LOCAL Int  base16(Void);
LOCAL Int  empty(Void);

#define CMDSIZE 5

LOCAL const struct {
   Char * const name;
   Int (* const code)(Void);
} words[CMDSIZE+1] = {
   {"!",     store},
   {"@",     fetch},
   {".",     emit},
   {"DEC",   base10},
   {"HEX",   base16},
   {"",      empty}
};

GLOBAL Void cmd_Init(Void) {
   idx   = 0;
   base  = 10;
}

/*
 * cmd - Pointer auf Input-Buffer mit Commandos
 */
GLOBAL Int cmd_Handler(Char *cmd) {
   // diese Funktion ist zu implementieren,
   // und zwar so, dass sie andere Abläufe in der
   // Main-Schleife nicht blockiert
   // damit dies gewährleistet ist, darf diese Funktion
   // und auch die aufgerufenen Funktionen, KEINE Schleifen
   // beinhalten. => Zustandsmaschine
   // eine Ausnahme von dieser Regel sind Funktionen
   // der Bibliothekk string sowie eine FUnktion
   // zur Umwandlung von Stringliteralen auf Zahlen
   return 0;
}

LOCAL Void push(Int arg) {
   stack[idx] = arg;
   if (++idx GE STACKSIZE) {
      idx = 0;
      SETBIT(err_Flags, CMD_OVERFLOW_ERROR);
   }
}

LOCAL UInt pop(Void) {
   if (idx EQ 0) {
      idx = STACKSIZE;
      SETBIT(err_Flags, CMD_UNDERFLOW_ERROR);
   }
   return stack[--idx];
}

// ! (n a -- )
LOCAL Int store(Void) {
   UInt *adr  = (UInt *)pop();
   UInt  val  = pop();
   if (NOT TSTBIT(err_Flags, STACK_ERROR)) {
      *adr = val;
      return 0;
   }
   return -1;
}

// @ (a -- n)
LOCAL Int fetch(Void) {
   UInt *adr  = (UInt *)pop();
   if (NOT TSTBIT(err_Flags, STACK_ERROR)) {
      push(*adr);
      return 0;
   }
   return -1;
}

// . (n -- )
LOCAL Int emit(Void) {
   // diese Funktion ist zu implementieren
   return 0;
}

LOCAL Int base10(Void) {
   base = 10;
   return 0;
}

LOCAL Int base16(Void) {
   base = 16;
   return 0;
}

LOCAL Int empty(Void) {
   return -1;
}


