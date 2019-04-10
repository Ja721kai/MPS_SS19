/*
 * error.h
 *
 *  Created on: 25.04.2018
 *      Author: Admin
 */

#include "..\base.h"

#ifndef ERR_H_
#define ERR_H_

#define NO_ERROR               0x0000
#define UCA0_CHAR_ERROR        0x0001 // unknown character
#define UCA0_OVERFLOW_ERROR    0x0002 // input buffer overflow
#define UCA0_CANCEL_ERROR      0x0004 // canceling command
//#define RESERVED             0x0008
#define UCA0_PARITY_ERROR      0x0010 // UCPE
#define UCA0_OVERRUN_ERROR     0x0020 // UCOE
#define UCA0_FRAME_ERROR       0x0040 // UCFE
//#define RESERVED             0x0080
#define CMD_ERROR              0x0100 // unknown command
#define CMD_OVERFLOW_ERROR     0x0200 // stack overflow
#define CMD_UNDERFLOW_ERROR    0x0400 // stack underflow
#define CMD_CONV_ERROR         0x0800 // conversion error

#define TIME_OUT               0x8000 // time out

#define STACK_ERROR  (CMD_OVERFLOW_ERROR \
                     + CMD_UNDERFLOW_ERROR)

#define UCA0_ERRORS  (UCA0_CHAR_ERROR \
                    + UCA0_OVERFLOW_ERROR \
                    + UCA0_CANCEL_ERROR \
                    + UCA0_PARITY_ERROR \
                    + UCA0_OVERRUN_ERROR \
                    + UCA0_FRAME_ERROR)

EXTERN UInt volatile err_Flags;
EXTERN Void err_Init(Void);
EXTERN UInt err_GetAndClear(Void);
EXTERN Void err_String(UInt err, Char *buf);

#endif /* ERR_H_ */
