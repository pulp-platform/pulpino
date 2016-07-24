/*
  HardwareSerial_private.h - Hardware serial library for Wiring
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
  Modified 2 July 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/

#include "wiring_private.h"

// this is so I can support Attiny series and any other chip without a uart
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) 
#if !defined(RBR_UART0)
#if defined(RBR_UART)
// If there is only one UART name all registers with 0 at the end.
#define RBR_UART0 RBR_UART 
#define DLL_UART0 DLL_UART 
#define THR_UART0 THR_UART 
#define DLM_UART0 DLM_UART
#define IER0_UART IER_UART 
#define IIR0_UART IIR_UART 
#define FCR0_UART FCR_UART
#define LCR0_UART LCR_UART
#define MCR0_UART MCR_UART
#define LSR0_UART LSR_UART
#define MSR0_UART MSR_UART
#define SCR0_UART SCR_UART
#else
#error No UART found in HardwareSerial.cpp
#endif //defined RBR
#endif // !defined RBR0


// Constructors ////////////////////////////////////////////////////////////////


HardwareSerial::HardwareSerial(
      volatile uint8_t *rbr, volatile uint8_t *dll,
      volatile uint8_t *thr, volatile uint8_t *dlm,
      volatile uint8_t *ier, volatile uint8_t *iir,
      volatile uint8_t *fcr, volatile uint8_t *lcr,
      volatile uint8_t *mcr, volatile uint8_t *lsr,
      volatile uint8_t *msr, volatile uint8_t *scr) :
    _rbr(rbr), _dll(dll),_thr(thr), _dlm(dlm),
    _ier(ier), _iir(iir),_fcr(fcr), _lcr(lcr),
    _mcr(mcr), _lsr(lsr),_msr(msr), _scr(scr),
    _rx_buffer(0)
{
}

#endif // whole file
