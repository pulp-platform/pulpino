/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 * Modified 13 August 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <Arduino.h>
#include "spi.h"
#include "gpio.h"
#include "utils.h"

// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_NOTUSINGINTERRUPT means that SPI has notUsingInterrupt() method
#define SPI_HAS_NOTUSINGINTERRUPT 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED 5

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C


class SPISettings { //the settings parameters are with no use for now as the current design is with only one mode, but left for further hardware modifications
public:
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    if (__builtin_constant_p(clock)) {
      init_AlwaysInline(clock, bitOrder, dataMode);
    } else {
      init_MightInline(clock, bitOrder, dataMode);
    }
  }
  SPISettings() {
    init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0);
  }
private:
  void init_MightInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    init_AlwaysInline(clock, bitOrder, dataMode);
  }
  void init_AlwaysInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
    __attribute__((__always_inline__)) {

    int32_t clockSetting = (F_CPU/(2*clock))-1;
    if (clockSetting < 0)
	clockSetting =0;
    else if (clockSetting > 255)
	clockSetting =255;


   //Initialize Mode and clock and data length, command and address length 
   status= (1<<4);	//reset FIFO
   clkdiv= clockSetting;
   spilen= (8<<16);	//set data length to be 8 bits, address and command lengths 0 bits
   spidum=0;		//set dummy cycles to be 0
   intcfg=0;		//disable interrupt

  }
  unsigned int status;
  unsigned int clkdiv;
  unsigned int spilen;
  unsigned int spidum;
  unsigned int intcfg;
  friend class SPIClass;
};


class SPIClass {
public:
  // Initialize the SPI library
  static void begin();

  // If SPI is used from within an interrupt, this function registers
  // that interrupt with the SPI library, so beginTransaction() can
  // prevent conflicts.  The input interruptNumber is the number used
  // with attachInterrupt.  If SPI is used from a different interrupt
  // (eg, a timer), interruptNumber should be 255.
  static void usingInterrupt(uint8_t interruptNumber);
  // And this does the opposite.
  static void notUsingInterrupt(uint8_t interruptNumber);
  // Note: the usingInterrupt and notUsingInterrupt functions should
  // not to be called from ISR context or inside a transaction.
  // For details see:
  // https://github.com/arduino/Arduino/pull/2381
  // https://github.com/arduino/Arduino/pull/2449

  // Before using SPI.transfer() or asserting chip select pins,
  // this function is used to gain exclusive access to the SPI bus
  // and configure the correct settings.

  inline static void beginTransaction(SPISettings settings) {
    if (interruptMode > 0) {
      unsigned int oldMstatus;
      csrr(mstatus, oldMstatus);
      noInterrupts();

      if (interruptMode == 1) {
        interruptSave = *INTEN;
        *INTEN &= ~interruptMask;
        csrw(mstatus, oldMstatus);
      } else
          interruptSave = oldMstatus;
    }


    #ifdef SPI_TRANSACTION_MISMATCH_LED
    if (inTransactionFlag) {
      pinMode(SPI_TRANSACTION_MISMATCH_LED, OUTPUT);
      digitalWrite(SPI_TRANSACTION_MISMATCH_LED, HIGH);
    }
    inTransactionFlag = 1;
    #endif
    SPI_STATUS= settings.status;
    SPI_CLKDIV= settings.clkdiv;
    SPI_SPILEN= settings.spilen;
    SPI_SPIDUM= settings.spidum;
    SPI_INTCFG= settings.intcfg;
  }

  // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
  inline static uint8_t transfer(uint8_t data) {
    while((SPI_STATUS & 0x01)==0);	//wait until SPI is idle
    SPI_SPILEN= (8<<16);	//set data length to be 8 bits, address and command lengths 0 bits
    SPI_STATUS = 0x0102;		//initiate a write operation with select CS0
    while (((SPI_STATUS >> 24) & 0xFF) >= 8);	//wait until tx buffer has available place
    SPI_TXFIFO=data<<24;

    while((SPI_STATUS & 0x01)==0);	//wait until SPI is idle
    //SPI_SPILEN= (8<<16);	//set data length to be 8 bits, address and command lengths 0 bitsS
    SPI_STATUS=0x0101;			//initiate a read operation with select CS0
    while (((SPI_STATUS >> 16) & 0xFF) == 0);	//wait until rx buffer has available place
    return (uint8_t)(SPI_RXFIFO);

  }
  inline static uint16_t transfer16(uint16_t data) {

    while((SPI_STATUS & 0x01)==0);	//wait until SPI is idle
    SPI_SPILEN= (16<<16);	//set data length to be 8 bits, address and command lengths 0 bits
    SPI_STATUS = 0x0102;		//initiate a write operation with select CS0
    while (((SPI_STATUS >> 24) & 0xFF) >= 8);	//wait until tx buffer has available place
    SPI_TXFIFO=data<<16;

    while((SPI_STATUS & 0x01)==0);	//wait until SPI is idle
    //SPI_SPILEN= (16<<16);	//set data length to be 8 bits, address and command lengths 0 bitsS
    SPI_STATUS=0x0101;			//initiate a read operation with select CS0
    while (((SPI_STATUS >> 16) & 0xFF) == 0);	//wait until rx buffer has available place
    return (uint16_t)(SPI_RXFIFO);

  }
  inline static void transfer(void *buf, size_t count) {
    if (count == 0) return;

    while((SPI_STATUS & 0x01)==0);	//wait until SPI is idle
    uint8_t *p = (uint8_t *)buf;
    SPI_SPILEN= (8<<16);	//set data length to be 8 bits, address and command lengths 0 bits
    SPI_STATUS=0x0102;		//initiate a write operation with select CS0
    while (((SPI_STATUS >> 24) & 0xFF) >= 8);	//wait until tx buffer has available place
    SPI_TXFIFO=(*p)<<24;
    while (--count > 0) {
      uint8_t out = *(p + 1);
      while((SPI_STATUS & 0x01)==0);	//wait until SPI is idle
      //SPI_SPILEN= (8<<16);	//set data length to be 8 bits, address and command lengths 0 bits
      SPI_STATUS=0x0101;		//initiate a read operation with select CS0
      while (((SPI_STATUS >> 16) & 0xFF) == 0);	//wait until rx buffer has available place
      uint8_t in= (uint8_t)SPI_RXFIFO;

      while((SPI_STATUS & 0x01)==0);	//wait until SPI is idle
      //SPI_SPILEN= (8<<16);	//set data length to be 8 bits, address and command lengths 0 bits
      SPI_STATUS=0x0102;		//initiate a write operation with select CS0
      while (((SPI_STATUS >> 24) & 0xFF) >= 8);	//wait until tx buffer has available place
      SPI_TXFIFO=out<<24;
      *p++ = in;
    }
    while((SPI_STATUS & 0x01)==0);	//wait until SPI is idle
    //SPI_SPILEN= (8<<16);	//set data length to be 8 bits, address and command lengths 0 bits
    SPI_STATUS=0x0101;		//initiate a read operation with select CS0
    while (((SPI_STATUS >> 16) & 0xFF) == 0);	//wait until rx buffer has available place
    *p = (uint8_t)SPI_RXFIFO;

  }

  // After performing a group of transfers and releasing the chip select
  // signal, this function allows others to access the SPI bus
  inline static void endTransaction(void) {
    #ifdef SPI_TRANSACTION_MISMATCH_LED
    if (!inTransactionFlag) {
      pinMode(SPI_TRANSACTION_MISMATCH_LED, OUTPUT);
      digitalWrite(SPI_TRANSACTION_MISMATCH_LED, HIGH);
    }
    inTransactionFlag = 0;
    #endif

    if (interruptMode > 0) {

      unsigned int oldMstatus;
      csrr(mstatus, oldMstatus);
      noInterrupts();

      if (interruptMode == 1) {
        *INTEN = interruptSave;
        csrw(mstatus, oldMstatus);
      } else
          csrw(mstatus, interruptSave);
    }
  }

  // Disable the SPI bus
  static void end();


private:
  static uint8_t initialized;
  static uint8_t interruptMode; // 0=none, 1=mask, 2=global
  static unsigned int interruptMask; // which interrupts to mask
  static unsigned int interruptSave; // temp storage, to restore state
  #ifdef SPI_TRANSACTION_MISMATCH_LED
  static uint8_t inTransactionFlag;
  #endif
};

extern SPIClass SPI;

#endif
