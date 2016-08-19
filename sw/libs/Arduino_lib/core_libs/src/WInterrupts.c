/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Wiring project - http://wiring.uniandes.edu.co

  Copyright (c) 2004-05 Hernando Barragan

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
  
  Modified 24 November 2006 by David A. Mellis
  Modified 1 August 2010 by Mark Sproul
  Modified 25 June 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/

#include <stdio.h>
#include "wiring_private.h"
#include "pins_arduino.h"
#include "int.h"
#include "event.h"
#include "gpio.h"

static void nothing(void) {
}

static volatile voidFuncPtr intFunc[NUM_DIGITAL_PINS]= {
#if NUM_DIGITAL_PINS > 31
    nothing,
#endif
#if NUM_DIGITAL_PINS > 30
    nothing,
#endif
#if NUM_DIGITAL_PINS > 29
    nothing,
#endif
#if NUM_DIGITAL_PINS > 28
    nothing,
#endif
#if NUM_DIGITAL_PINS > 27
    nothing,
#endif
#if NUM_DIGITAL_PINS > 26
    nothing,
#endif
#if NUM_DIGITAL_PINS > 25
    nothing,
#endif
#if NUM_DIGITAL_PINS > 24
    nothing,
#endif
#if NUM_DIGITAL_PINS > 23
    nothing,
#endif
#if NUM_DIGITAL_PINS > 22
    nothing,
#endif
#if NUM_DIGITAL_PINS > 21
    nothing,
#endif
#if NUM_DIGITAL_PINS > 20
    nothing,
#endif
#if NUM_DIGITAL_PINS > 19
    nothing,
#endif
#if NUM_DIGITAL_PINS > 18
    nothing,
#endif
#if NUM_DIGITAL_PINS > 17
    nothing,
#endif
#if NUM_DIGITAL_PINS > 16
    nothing,
#endif
#if NUM_DIGITAL_PINS > 15
    nothing,
#endif
#if NUM_DIGITAL_PINS > 14
    nothing,
#endif
#if NUM_DIGITAL_PINS > 13
    nothing,
#endif
#if NUM_DIGITAL_PINS > 12
    nothing,
#endif
#if NUM_DIGITAL_PINS > 11
    nothing,
#endif
#if NUM_DIGITAL_PINS > 10
    nothing,
#endif
#if NUM_DIGITAL_PINS > 9
    nothing,
#endif
#if NUM_DIGITAL_PINS > 8
    nothing,
#endif
#if NUM_DIGITAL_PINS > 7
    nothing,
#endif
#if NUM_DIGITAL_PINS > 6
    nothing,
#endif
#if NUM_DIGITAL_PINS > 5
    nothing,
#endif
#if NUM_DIGITAL_PINS > 4
    nothing,
#endif
#if NUM_DIGITAL_PINS > 3
    nothing,
#endif
#if NUM_DIGITAL_PINS > 2
    nothing,
#endif
#if NUM_DIGITAL_PINS > 1
    nothing,
#endif
#if NUM_DIGITAL_PINS > 0
    nothing
#endif
};

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode) {
  unsigned int bit = digitalPinToBitMask(interruptNum);
  unsigned int oldMstatus;

  if(interruptNum < NUM_DIGITAL_PINS) {
    intFunc[interruptNum] = userFunc;
    
    // Configure the interrupt mode (trigger on low input, any change, rising
    // edge, or falling edge).  The mode constants were chosen to correspond
    // to the configuration bits in the hardware register, so we simply shift
    // the mode into place.
      
    // Enable the interrupt.
    csrr(mstatus, oldMstatus);  
    switch(mode) {

        case LOW  :
                *INTTYPE1 &= ~bit;
                *INTTYPE0 |= bit;
                break; 
	case HIGH  :
                *INTTYPE1 &= ~bit;
                *INTTYPE0 &= ~bit; 
                break;
        case FALLING  :
                *INTTYPE1 |= bit;
                *INTTYPE0 |= bit;
                break;
        default  :      //Rising is the default
                *INTTYPE1 |= bit;
                *INTTYPE0 &= ~bit;
                break;  
    }
  csrw(mstatus, oldMstatus);

  *INTEN |= bit;     //Enable the specific GPIO pin interrupt
  IER |= (1<<25);                 //Enable Global GPIO interrupt
  }
}

void detachInterrupt(uint8_t interruptNum) {
  if(interruptNum < NUM_DIGITAL_PINS) {
    // Disable the interrupt. 
        *INTEN &= ~(1<<interruptNum);      //Disable the specific GPIO pin interrupt
        intFunc[interruptNum] = nothing;
  }
}


//redefining the GPIO handler
void ISR_GPIO (void) {
        voidFuncPtr func = intFunc[BitMaskToDigitalPin(*INTSTATUS)]; 
        if (func != NULL)
                func();

        ICP |= (1<<25);         //clear interrupt pending for GPIO
}

