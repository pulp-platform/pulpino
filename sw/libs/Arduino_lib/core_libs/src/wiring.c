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

   Modified 14 June 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
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
	
	return ((m << 14) + t) * (1.0F / clockCyclesPerMicrosecond());
}

void delay(unsigned int ms)
{
	unsigned int start = micros();

	while (ms > 0) {
		yield();
		while ( ms > 0 && (micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}

//This function should be hardcoded according to frequency to busy wait until delay is over (it should work even if interrupts are disabled)
//This is a simple implementation for now and should be replaced later 
//### Check Later ###
void delayMicroseconds(unsigned int us)
{
unsigned int startMicros = micros();

	while ((micros() - startMicros) < us)
	{
		//*	do nothing
	}
}


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
