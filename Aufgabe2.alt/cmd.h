/*
 * cmd.h
 *
 *  Created on: 31.03.2018
 *      Author: Admin
 */
#include "..\base.h"
/*
 * ! (n a -- )  ein Wort n wird in eine Speicherzelle mit der Adresse a geschrieben, n muss gerade sein
 * @ (a -- n)   eine Speicherzelle mit der Adresse a wird gelesen und der Inhalt auf den Stack abgelegt
 * . (n -- )    gibt den Wert n über die serielle Schnittstelle aus
 * DEC ( -- )   die Basis des Zahlensystems wird auf 10 gesetzt (bei der Umwandlung von Literalen notwendig)
 * HEX ( -- )   die Basis des Zahlensystems wird aus 16 gesetzt
 */
#ifndef CMD_H_
#define CMD_H_

EXTERN Void cmd_Init(Void);
EXTERN Int  cmd_Handler(Char *cmd);

#endif /* CMD_H_ */
