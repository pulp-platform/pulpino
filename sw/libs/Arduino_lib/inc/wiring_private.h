/*
  wiring_private.h - Internal header file.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
  
  Modified 14 June 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/

#ifndef WiringPrivate_h
#define WiringPrivate_h


#include <stdio.h>
//#include <stdarg.h>

#include "Arduino.h"
#include "int.h"
#include "timer.h"
#include "utils.h"
#include "gpio.h"


#define PWM_FREQ 200	//PWM frequency
#define	PWM_PRE 3	//PWM prescision if 0 precision will be 256, if 1 precision will be 255/2 and so on.
#define PWM_MAX_COUNT ((256 >> PWM_PRE)-1)	//maximum number ISR will count then overflow to zero

typedef struct{
uint8_t PWM_enabled;
uint8_t PWM_value;
}PWM_status;
extern PWM_status PWM_status_arr[NUM_DIGITAL_PINS];
extern uint8_t PWM_global_en;
extern uint8_t PWM_count;



#ifdef __cplusplus
extern "C"{
#endif


#define cbi(sfr, bit) ((sfr) &= ~(bit))
#define sbi(sfr, bit) ((sfr) |= (bit))
#define bit_is_clear(sfr,bit) \
	(!((sfr) & (bit))) 
#define bit_is_set(sfr,bit) \
	((sfr) & (bit)) 


typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
