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

#include "SPI.h"

SPIClass SPI;

uint8_t SPIClass::initialized = 0;
uint8_t SPIClass::interruptMode = 0;
unsigned int SPIClass::interruptMask = 0;
unsigned int SPIClass::interruptSave = 0;
#ifdef SPI_TRANSACTION_MISMATCH_LED
uint8_t SPIClass::inTransactionFlag = 0;
#endif

void SPIClass::begin()
{
  initialized++; // reference count
}

void SPIClass::end() {
  unsigned int oldMstatus;   
  csrr(mstatus, oldMstatus);
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  // Decrease the reference counter
  if (initialized)
    initialized--;
  // If there are no more references disable SPI
  if (!initialized) {
    interruptMode = 0;
    #ifdef SPI_TRANSACTION_MISMATCH_LED
    inTransactionFlag = 0;
    #endif
  }
  csrw(mstatus, oldMstatus);
}


void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
  unsigned int  mask = 0;
  unsigned int oldMstatus;
  csrr(mstatus, oldMstatus);
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  mask=(1<<interruptNumber);
  interruptMask |= mask;
  if (!interruptMode)
    interruptMode = 1;
  csrw(mstatus, oldMstatus);
}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
{
  // Once in mode 2 we can't go back to 0 without a proper reference count
  if (interruptMode == 2)
    return;
  unsigned int  mask = 0;
  unsigned int oldMstatus;
  csrr(mstatus, oldMstatus);
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  mask=(1<<interruptNumber);
  interruptMask &= ~mask;
  if (!interruptMask)
    interruptMode = 0;
  csrw(mstatus, oldMstatus);
}
