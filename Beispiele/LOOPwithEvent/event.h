/*
 * event.h
 *
 *  Created on: 28.04.2018
 *      Author: Admin
 */

#include "..\base.h"

#ifndef EVENT_H_
#define EVENT_H_

#define EVENT0  0  // global event
#define EVENT1  1
#define EVENT2  2

EXTERN Void evn_Init(Void);
EXTERN        Void   evn_Clr(const UInt arg);
EXTERN inline Void   evn_Set(const UInt arg);
GLOBAL inline UShort evn_Get(const UInt arg);

#endif /* EVENT_H_ */
