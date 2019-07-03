/*
 * SPI_AS1108.h
 *
 *  Created on: 26.12.2018
 *      Author: Admin
 */

#include "base.h"

#ifndef AS1108_H_
#define AS1108_H_

//GLOBAL char error_code;
EXTERN Void set_error(TEvent);
EXTERN Void SPI_Init(Void);
EXTERN Void AS1108_Init(Void);
EXTERN Void Button_Handler(Void);
EXTERN Void Number_Handler(Void);
EXTERN Void AS1108_Handler(Void);
LOCAL Void State0(Void);
LOCAL Void State1(Void);
LOCAL Void State2(Void);
LOCAL Void State3(Void);
LOCAL Void State4(Void);

#endif /* AS1108_H_ */