/*
  HardwareSerial.h - Hardware serial library for Wiring
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

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
  Modified 3 July 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Stream.h"
#include "uart.h"


// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0b00000
#define SERIAL_6N1 0b00001
#define SERIAL_7N1 0b00010
#define SERIAL_8N1 0b00011
#define SERIAL_5N2 0b00100
#define SERIAL_6N2 0b00101
#define SERIAL_7N2 0b00110
#define SERIAL_8N2 0b00111
#define SERIAL_5E1 0b11000
#define SERIAL_6E1 0b11001
#define SERIAL_7E1 0b11010
#define SERIAL_8E1 0b11011
#define SERIAL_5E2 0b11100
#define SERIAL_6E2 0b11101
#define SERIAL_7E2 0b11110
#define SERIAL_8E2 0b11111
#define SERIAL_5O1 0b01000
#define SERIAL_6O1 0b01001
#define SERIAL_7O1 0b01010
#define SERIAL_8O1 0b01011
#define SERIAL_5O2 0b01100
#define SERIAL_6O2 0b01101
#define SERIAL_7O2 0b01110
#define SERIAL_8O2 0b01111

class HardwareSerial : public Stream
{
  protected:
    volatile uint8_t * const _rbr;
    volatile uint8_t * const _dll;
    volatile uint8_t * const _thr;
    volatile uint8_t * const _dlm;
    volatile uint8_t * const _ier;
    volatile uint8_t * const _iir;
    volatile uint8_t * const _fcr;
    volatile uint8_t * const _lcr;
    volatile uint8_t * const _mcr;
    volatile uint8_t * const _lsr;
    volatile uint8_t * const _msr;
    volatile uint8_t * const _scr;
    
    // Has any byte been written to the UART since begin()
    bool _written,_peek;
    int _rx_buffer;

  public:
    inline HardwareSerial(
      volatile uint8_t *rbr, volatile uint8_t *dll,
      volatile uint8_t *thr, volatile uint8_t *dlm,
      volatile uint8_t *ier, volatile uint8_t *iir,
      volatile uint8_t *fcr, volatile uint8_t *lcr,
      volatile uint8_t *mcr, volatile uint8_t *lsr,
      volatile uint8_t *msr, volatile uint8_t *scr);

    void begin(unsigned int baud) { begin(baud, SERIAL_8N1); }
    void begin(unsigned int, uint8_t);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() { return true; }

};

// If in the future Pulpino may have many UARTs or none at all 
#if defined(RBR_UART) || defined(RBR_UART0)
  extern HardwareSerial Serial;
  #define HAVE_HWSERIAL0
#endif
#if defined(RBR_UART1)
  extern HardwareSerial Serial1;
  #define HAVE_HWSERIAL1
#endif

extern void serialEventRun(void) __attribute__((weak));

#endif
