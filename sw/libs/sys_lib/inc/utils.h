// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

/**
 * @file
 * @brief Utility functions for PULPino.
 *
 * Provides utility functions like writing and reading
 * special prupose register on both RISC-V and OpenRISC.
 *
 */
#ifndef _UTILS_H_
#define _UTILS_H_

#include "event.h"

/* For getting core ID. */
static inline int get_core_id()
{
  return 0;
}

// get number of cores
static inline int get_core_num()
{
  return 0;
}

#ifdef __riscv__
/**
 * @brief Write to CSR.
 * @param CSR register to write.
 * @param Value to write to CSR register.
 * @return void
 *
 * Function to handle CSR writes.
 *
 */
#define csrw(csr, value)  asm volatile ("csrw\t\t" #csr ", %0" : /* no output */ : "r" (value));

/**
 * @brief Read from CSR.
 * @param void
 * @return 32-bit unsigned int
 *
 * Function to handle CSR reads.
 *
 */
#define csrr(csr, value)  asm volatile ("csrr\t\t%0, " #csr "": "=r" (value));

/**
 * @brief Request to put the core to sleep.
 * @param void
 *
 * Set the core to sleep state and wait for events/interrupt to wake up.
 *
 */
static inline void sleep(void) {
  SCR = 0x01;
  asm volatile ("nop;nop;wfi");
}

#else // not __riscv__
/* For writing into SPR. */
static inline void mtspr(unsigned long spr, unsigned long value)
{
  asm volatile ("l.mtspr\t\t%0,%1,0": : "r" (spr), "r" (value));
}

/* For reading SPR. */
static inline unsigned long mfspr(unsigned long spr)
{
  unsigned long value;
  asm volatile ("l.mfspr\t\t%0,%1,0" : "=r" (value) : "r" (spr));
  return value;
}

static inline void sleep(void) {
  SCR = 0x01;
  asm volatile ("l.nop;l.nop;l.psync");
}
#endif

/* Loops/exits simulation */
void exit(int i);

/* end of computation */
void eoc(int i);

// sleep some cycles
void sleep_busy(volatile int);


#endif
