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
*/

#include "wiring_private.h"

// this is so I can support Attiny series and any other chip without a uart
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) 
#if !defined(RBR0)
#if defined(RBR)
// If there is only one UART name all registers with 0 at the end.
#define RBR0 RBR 
#define DLL0 DLL 
#define THR0 THR 
#define DLM0 DLM
#define IER0 IER 
#define IIR0 IIR 
#define FCR0 FCR
#define LCR0 LCR
#define MCR0 MCR
#define LSR0 LSR
#define MSR0 MSR
#define SCR0 SCR
#else
#error No UART found in HardwareSerial.cpp
#endif //defined RBR
#endif // !defined RBR0
#endif

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
    _rx_buffer_head(0), _rx_buffer_tail(0),
    _tx_buffer_head(0), _tx_buffer_tail(0)
{
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial::_rx_complete_irq(void)
{
  if (bit_is_clear(*_ucsra, UPE0)) {
    // No Parity error, read byte and store it in the buffer if there is
    // room
    unsigned char c = *_udr;
    rx_buffer_index_t i = (unsigned int)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer_tail) {
      _rx_buffer[_rx_buffer_head] = c;
      _rx_buffer_head = i;
    }
  } else {
    // Parity error, read byte but discard it
    *_udr;
  };
}

#endif // whole file
