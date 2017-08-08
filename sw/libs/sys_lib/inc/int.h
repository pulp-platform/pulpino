// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
/**
 * @file
 * @brief Interrupt library for PULPino.
 *
 * Contains various interrupt manipulating functions.
 *
 * @author Florian Zaruba
 *
 * @version 1.0
 *
 * @date 11/9/2015
 *
 */

#ifndef _INT_H_
#define _INT_H_

#ifndef __riscv__
#include "spr-defs.h"
#endif

/* Number of interrupt handlers - really depends on PIC width in OR1200*/
#define MAX_INT_HANDLERS  32


/**
 * \brief Disables interrupts globally.
 * \param void
 * \return void
 *
 * By writing 1 to the ie (interruptenable) bit
 * interrupts are globally disable.
 */
static inline void int_disable(void) {
#ifdef __riscv__
  // read-modify-write
  int mstatus;
  asm volatile ("csrr %0, mstatus": "=r" (mstatus));
  mstatus &= 0xFFFFFFF7;
  asm volatile ("csrw mstatus, %0" : /* no output */ : "r" (mstatus));
  asm("csrw 0x300, %0" : : "r" (0x0) );
#else
  mtspr(SPR_SR, mfspr(SPR_SR) & (~SPR_SR_IEE));
#endif
}



/**
 * \brief Enables interrupts globally.
 * \param void
 * \return void
 *
 * By writing 1 to the ie (interruptenable) bit
 * interrupts are globally enabled.
 */
static inline void int_enable(void) {
#ifdef __riscv__
  // read-modify-write
  int mstatus;
  asm volatile ("csrr %0, mstatus": "=r" (mstatus));
  mstatus |= 0x08;
  asm volatile ("csrw mstatus, %0" : /* no output */ : "r" (mstatus));
#else
  mtspr(SPR_SR, mfspr(SPR_SR) | (SPR_SR_IEE));
#endif
}



//declearing all interrupt handelrs
//these functions can be redefined by users

void ISR_I2C (void);	// 23: i2c
void ISR_UART (void);	// 23: i2c
void ISR_GPIO (void); 	// 25: gpio
void ISR_SPIM0 (void);  // 26: spim end of transmission
void ISR_SPIM1 (void);  // 27: spim R/T finished
void ISR_TA_OVF (void); // 28: timer A overflow
void ISR_TA_CMP (void); // 29: timer A compare
void ISR_TB_OVR (void); // 30: timer B overflow
void ISR_TB_CMP (void); // 31: timer B compare



#endif // _INT_H_
