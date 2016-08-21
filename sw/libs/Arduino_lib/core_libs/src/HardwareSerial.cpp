/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
  Modified 4 July 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

// this next line disables the entire HardwareSerial.cpp if no UART existed in a particular chip
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1)

// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
// The Serialx_available is just a wrapper around Serialx.available(),
// but we can refer to it weakly so we don't pull in the entire
// HardwareSerial instance if the user doesn't also refer to it.
#if defined(HAVE_HWSERIAL0)
  void serialEvent() __attribute__((weak));
  bool Serial0_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL1)
  void serialEvent1() __attribute__((weak));
  bool Serial1_available() __attribute__((weak));
#endif


void serialEventRun(void)
{
#if defined(HAVE_HWSERIAL0)
  if (Serial0_available && serialEvent && Serial0_available()) serialEvent();
#endif
#if defined(HAVE_HWSERIAL1)
  if (Serial1_available && serialEvent1 && Serial1_available()) serialEvent1();
#endif
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned int baud, byte config)
{
  // calculating divisor for baud rate
  uint16_t baud_setting = (F_CPU / (16* baud)) -1;	
 
  sbi(*_lcr, DLAB);		//set DLAB bit in lcr reg to enable divisor modification
  // assign the baud_setting, to the divisor registers
  *_dlm = baud_setting >> 8;
  *_dll = baud_setting;

  _written = false;
  _peek = false;

  //set the data bits, parity, and stop bits
  uint8_t lcrMasked=*_lcr & 0b11100000; 		//keep the most 3 bits unchanged
  *_lcr= lcrMasked | config;				//config represents the least 5 bits
  

  *_fcr = 0xA7;		           //enable and reset FIFOs

  cbi(*_lcr, DLAB);		   //clear DLAB bit in lcr reg to enable reading and writing
	

  *_ier = 0x00;			   //disable all UART interrupts
  cbi(IER, 1<<24);                 //Ensure diabling Global UART interrupt
  }

void HardwareSerial::end()
{ 
  flush();
  //if the pad mux is implemented, seitch the mux to GPIO
  *_fcr = 0x06;			//Disable FIFO
  cbi(IER, 1<<24);                 //Ensure diabling Global UART interrupt
}

int HardwareSerial::available(void)
{ 
   if (bit_is_set(*_lsr, DR))
   	return 1; 		// no buffer is used so always return 1
   else 
	return 0;
}

int HardwareSerial::peek(void)
{
  	if (_peek == false)
		_rx_buffer = read();
	_peek= true;
	return _rx_buffer;
}

int HardwareSerial::read(void)
{
	while(bit_is_clear(*_lsr, DR));
	if (bit_is_clear(*_lsr, PE)){
		_peek= false;
  		return *_rbr;
 	}
	else{
 		_peek= false;
  		*_rbr;
  		return -1;
	}
}

int HardwareSerial::availableForWrite(void)
{
   if (bit_is_set(*_lsr, THRE))
   	return 1; 		// no buffer is used so always return 1
   else 
	return 0;
}

void HardwareSerial::flush()
{
    //no need for flush as we are not using interrupt approach
    return;
}

size_t HardwareSerial::write(uint8_t c)
{
  _written = true;
  while(bit_is_clear(*_lsr, THRE));
  *_thr = c;
  return 1;
}

#endif // whole file
