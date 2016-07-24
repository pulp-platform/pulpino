/*
  wiring_digital.c - digital input and output functions
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

  Modified 28 September 2010 by Mark Sproul
  Modified 14 June 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)

*/

#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"


void pinMode(uint8_t pin, uint8_t mode)
{
	unsigned int bit = digitalPinToBitMask(pin);
	volatile unsigned int *reg;	 
	unsigned int oldMstatus;

	if (pin >= NUM_DIGITAL_PINS) return;	//check for number of pin

	reg = PADDIR;

	if (mode == INPUT) { 	
		csrr(mstatus, oldMstatus);
		int_disable(); 		//disable interrupt as the next instruction is not atomic
		*reg &= ~bit;
		csrw(mstatus, oldMstatus);
	}

//We don't have pullup and pulldown for FPGA but have it for imperio, ###check later###

	else {
		csrr(mstatus, oldMstatus);
		int_disable(); 		//disable interrupt as the next instruction is not atomic
		*reg |= bit;
		csrw(mstatus, oldMstatus);
	}
}


static inline void turnOffPWM(uint8_t pin)
{
	if (PWM_status_arr[pin].PWM_enabled==1){ 		
			PWM_status_arr[pin].PWM_enabled=0;
			PWM_global_en--;
  			if (PWM_global_en==0)
                		IER &= ~(1<<31);	//disable Timer B compare match interrupt
		}
}


void digitalWrite(uint8_t pin, uint8_t val)
{
	unsigned int bit = digitalPinToBitMask(pin);	
	volatile unsigned int *out;
      	unsigned int oldMstatus;

	if (pin >= NUM_DIGITAL_PINS) return;	//check for number of pin

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	turnOffPWM(pin);
	
	out = PADOUT;	
	csrr(mstatus, oldMstatus);
	int_disable(); 		//disable interrupt as the next instruction is not atomic	
	if (val == LOW) {
		*out &= ~bit;
	} else {
		*out |= bit;
	}
	csrw(mstatus, oldMstatus);
}

int digitalRead(uint8_t pin)
{
	unsigned int bit = digitalPinToBitMask(pin);

	if (pin >= NUM_DIGITAL_PINS) return LOW;	//check for number of pin

	// If the pin that support PWM output, we need to turn it off
	// before getting a digital reading.
	turnOffPWM(pin);

	if (*PADIN & bit) return HIGH;
	return LOW;
}
