/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
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

  $Id$
*/

#include "wiring_private.h"

// the prescaler is set so that timerA ticks every 1 clock cycles, and the
// the compare match handler is called every 16384 ticks.
#define MICROSECONDS_PER_TIMERA_COMPARE (clockCyclesToMicroseconds(16384)) //16384 is chosen to be compatible with the same arduino ticks, but could be modified

// the whole number of milliseconds per timerA compare match
#define MILLIS_INC (MICROSECONDS_PER_TIMERA_COMPARE / 1000)

// the fractional number of milliseconds per timerA compare match. we shift right
// by three to fit these numbers into a byte. 
#define FRACT_INC ((MICROSECONDS_PER_TIMERA_COMPARE % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned int timerA_cmp_count = 0;
volatile unsigned int timerA_millis = 0;
static unsigned char timerA_fract = 0;


void ISR_TA_CMP(void) 
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned int m = timerA_millis;
	unsigned char f = timerA_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timerA_fract = f;
	timerA_millis = m;
	timerA_cmp_count++;

	ICP|= (1<<29);		//clear Timer A compare match interrupt pending
}

unsigned int millis()
{
	unsigned int m,oldMstatus;
	csrr(mstatus, oldMstatus);

	// disable interrupts while we read timerA_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timerA_millis)
	int_disable();
	m = timerA_millis;
	csrw(mstatus, oldMstatus);

	return m;
}

unsigned int micros() {
	unsigned int m, t, oldMstatus;
	csrr(mstatus, oldMstatus);
	int_disable();

	m = timerA_cmp_count;
	t = TIRA;

	csrw(mstatus, oldMstatus);
	
	return ((m << 14) + t) * (1.0 / clockCyclesPerMicrosecond());
}

void delay(unsigned int ms)
{
	unsigned int start = micros();

	while (ms > 0) {
		yield();
		if ((micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}

//////////////////////// Supression///////////////////////
//Need to test the exact cycles, ###check later###
/*

// Delay for the given number of microseconds.  Assumes a 1, 8, 12, 16, 20 or 24 MHz clock. 
void delayMicroseconds(unsigned int us)
{
	// call = 4 cycles + 2 to 4 cycles to init us(2 for constant delay, 4 for variable)

	// calling avrlib's delay_us() function with low values (e.g. 1 or
	// 2 microseconds) gives delays longer than desired.
	//delay_us(us);
#if F_CPU >= 24000000L
	// for the 24 MHz clock for the aventurous ones, trying to overclock

	// zero delay fix
	if (!us) return; //  = 3 cycles, (4 when true)

	// the following loop takes a 1/6 of a microsecond (4 cycles)
	// per iteration, so execute it six times for each microsecond of
	// delay requested.
	us *= 6; // x6 us, = 7 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 22 (24) cycles above, remove 5, (5*4=20)
	// us is at least 6 so we can substract 5
	us -= 5; //=2 cycles

#elif F_CPU >= 20000000L
	// for the 20 MHz clock on rare Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call takes 18 (20) cycles, which is 1us
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop"); //just waiting 4 cycles
	if (us <= 1) return; //  = 3 cycles, (4 when true)

	// the following loop takes a 1/5 of a microsecond (4 cycles)
	// per iteration, so execute it five times for each microsecond of
	// delay requested.
	us = (us << 2) + us; // x5 us, = 7 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 26 (28) cycles above, remove 7, (7*4=28)
	// us is at least 10 so we can substract 7
	us -= 7; // 2 cycles

#elif F_CPU >= 16000000L
	// for the 16 MHz clock on most Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 1us
	if (us <= 1) return; //  = 3 cycles, (4 when true)

	// the following loop takes 1/4 of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2; // x4 us, = 4 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 19 (21) cycles above, remove 5, (5*4=20)
	// us is at least 8 so we can substract 5
	us -= 5; // = 2 cycles,

#elif F_CPU >= 12000000L
	// for the 12 MHz clock if somebody is working with USB

	// for a 1 microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 1.5us
	if (us <= 1) return; //  = 3 cycles, (4 when true)

	// the following loop takes 1/3 of a microsecond (4 cycles)
	// per iteration, so execute it three times for each microsecond of
	// delay requested.
	us = (us << 1) + us; // x3 us, = 5 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 20 (22) cycles above, remove 5, (5*4=20)
	// us is at least 6 so we can substract 5
	us -= 5; //2 cycles

#elif F_CPU >= 8000000L
	// for the 8 MHz internal clock

	// for a 1 and 2 microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 2us
	if (us <= 2) return; //  = 3 cycles, (4 when true)

	// the following loop takes 1/2 of a microsecond (4 cycles)
	// per iteration, so execute it twice for each microsecond of
	// delay requested.
	us <<= 1; //x2 us, = 2 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 17 (19) cycles above, remove 4, (4*4=16)
	// us is at least 6 so we can substract 4
	us -= 4; // = 2 cycles

#else
	// for the 1 MHz internal clock (default settings for common Atmega microcontrollers)

	// the overhead of the function calls is 14 (16) cycles
	if (us <= 16) return; //= 3 cycles, (4 when true)
	if (us <= 25) return; //= 3 cycles, (4 when true), (must be at least 25 if we want to substract 22)

	// compensate for the time taken by the preceeding and next commands (about 22 cycles)
	us -= 22; // = 2 cycles
	// the following loop takes 4 microseconds (4 cycles)
	// per iteration, so execute it us/4 times
	// us is at least 4, divided by 4 gives us 1 (no zero delay bug)
	us >>= 2; // us div 4, = 4 cycles
	

#endif

	// busy wait
	__asm__ __volatile__ (
		"1: sbiw %0,1" "\n\t" // 2 cycles
		"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
	// return = 4 cycles
}



*/
////////////////////////End///////////////////////

void init()
{
	// this needs to be called before setup() or some functions won't
	// work there
	int_enable();	//enable global interrupt
	
	//###check later### Data sheet of Florian says that I should target 31 not 29
	IER |= (1<<29);	//enable Timer A compare match interrupt
	TIRA = 0;	//reset Timer A
	TOCRA= 0x4000;	//set the compare value to be 16834 to be compatible with Arduino cycles for interrupt
	TPRA  = 0x05;	//enable timer, enable output compare, set prescaler to be 1
 
	

	IER |= (1<<31);	//enable Timer B compare match interrupt
	TIRB = 0;	//reset Timer B
	TOCRB= F_CPU/(PWM_FREQ*(256>>PWM_PRE));	//set the compare value to match the predifined PWM frequency and precision
	TPRB  = 0x05;	//enable timer, enable output compare, set prescaler to be 1
 
	//###check later###
	// intialize ADC


	//###check later###
	// the bootloader connects pins 0 and 1 to the USART; disconnect them
	// here so they can be used as normal digital i/o; they will be
	// reconnected in Serial.begin()


}
