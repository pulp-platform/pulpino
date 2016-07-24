/*
  wiring_pulse.c - pulseIn() function
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
  
  Modified 22 July 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/

#include "wiring_private.h"
#include "pins_arduino.h"

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 *
 * This function performs better with short pulses in noInterrupt() context
 */

unsigned int pulseIn(uint8_t pin, uint8_t state, unsigned int timeout)
{
	// cache the port and bit of the pin in order to speed up the
	// pulse width measuring loop and achieve finer resolution.  calling
	// digitalRead() instead yields much coarser resolution.
	unsigned int bit = digitalPinToBitMask(pin);
	volatile unsigned int *reg;
	uint8_t stateMask = (state ? bit : 0);
	unsigned int width = 0; // keep initialization out of time critical area
	
	// convert the timeout from microseconds to a number of times through
	// the initial loop; it takes 16 clock cycles per iteration.
	unsigned long numloops = 0;
	unsigned long maxloops = microsecondsToClockCycles(timeout) / 9;
	
        reg=PADIN;
	       
	// wait for any previous pulse to end
	while ((*reg & bit) == stateMask)
		if (numloops++ == maxloops)
			return 0;

	// wait for the pulse to start
	while ((*reg & bit) != stateMask)
		if (numloops++ == maxloops)
			return 0;

	// wait for the pulse to stop
	while ((*reg & bit) == stateMask) {
		if (numloops++ == maxloops)
			return 0;
		width++;
	}

        
   
	// convert the reading to microseconds. The loop has been determined
	// to be 9 clock cycles long. There will be some error introduced by
	// the interrupt handlers.
	return clockCyclesToMicroseconds(width * 9); 
}

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 *
 * ATTENTION:
 * this function relies on micros() so cannot be used in noInterrupt() context
 */
unsigned int pulseInLong(uint8_t pin, uint8_t state, unsigned int timeout)
{

	// cache the port and bit of the pin in order to speed up the
	// pulse width measuring loop and achieve finer resolution.  calling
	// digitalRead() instead yields much coarser resolution.
	unsigned int bit = digitalPinToBitMask(pin);
	volatile unsigned int *reg;
	unsigned int stateMask = (state ? bit : 0);

	unsigned int startMicros = micros();

        reg=PADIN;

	// wait for any previous pulse to end
	while ((*reg & bit) == stateMask)
		if (micros() - startMicros > timeout)
			return 0;

	// wait for the pulse to start
	while ((*reg & bit) != stateMask)
		if (micros() - startMicros > timeout)
			return 0;
			
	unsigned int start = micros();
	// wait for the pulse to stop
	while ((*reg & bit) == stateMask) 
		if (micros() - startMicros > timeout)
			return 0;
	
	return micros() - start;
}
