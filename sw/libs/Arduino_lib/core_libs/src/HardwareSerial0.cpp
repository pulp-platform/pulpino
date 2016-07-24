/*
  HardwareSerial0.cpp - Hardware serial library for Wiring
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

#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"
// Each HardwareSerial is defined in its own file, sine the linker pulls
// in the entire file when any element inside is used. --gc-sections can
// additionally cause unused symbols to be dropped, but ISRs have the
// "used" attribute so are never dropped and they keep the
// HardwareSerial instance in as well. Putting each instance in its own
// file prevents the linker from pulling in any unused instances in the
// first place.



#if defined(HAVE_HWSERIAL0)

#if defined(RBR_UART) 
  HardwareSerial Serial(RBR_UART, DLL_UART, THR_UART, DLM_UART, IER_UART, IIR_UART,
			FCR_UART, LCR_UART, MCR_UART, LSR_UART, MSR_UART, SCR_UART);
#else
  HardwareSerial Serial(RBR_UART0, DLL_UART0, THR_UART0, DLM_UART0, IER_UART0, IIR_UART0,
			FCR_UART0, LCR_UART0, MCR_UART0, LSR_UART0, MSR_UART0, SCR_UART0);
#endif

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial0_available() {
  return Serial.available();
}

#endif // HAVE_HWSERIAL0
