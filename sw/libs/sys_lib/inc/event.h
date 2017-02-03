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
 * @brief Event library for PULPino.
 *
 * Contains event manipulating functions and event related
 * registers.
 *
 * @author Florian Zaruba
 *
 * @version 1.0
 *
 * @date 2/10/2015
 *
 */

#ifndef __EVENT_H__
#define __EVENT_H__

#include <pulpino.h>

#define REG_IRQ_ENABLE          0x00
#define REG_IRQ_PENDING         0x04
#define REG_IRQ_SET_PENDING     0x08
#define REG_IRQ_CLEAR_PENDING   0x0C

#define REG_EVENT_ENABLE        0x10
#define REG_EVENT_PENDING       0x14
#define REG_EVENT_SET_PENDING   0x18
#define REG_EVENT_CLEAR_PENDING 0x1C

#define REG_SLEEP_CTRL          0x20
#define REG_SLEEP_STATUS        0x24


// pointer to mem of event unit - PointerEventunit
#define __PE__(a) *(volatile int*) (EVENT_UNIT_BASE_ADDR + a)

// interrupt enable register
#define IER __PE__(REG_IRQ_ENABLE)

// interrupt pending register
#define IPR __PE__(REG_IRQ_PENDING)

// interrupt set pending register
#define ISP __PE__(REG_IRQ_SET_PENDING)

// interrupt clear pending register
#define ICP __PE__(REG_IRQ_CLEAR_PENDING)

// event enable register
#define EER __PE__(REG_EVENT_ENABLE)

// event pending register
#define EPR __PE__(REG_EVENT_PENDING)

// event set pending register
#define ESP __PE__(REG_EVENT_SET_PENDING)

// event clear pending register
#define ECP __PE__(REG_EVENT_CLEAR_PENDING)

// sleep control register
#define SCR __PE__(REG_SLEEP_CTRL)

// sleep status register
#define SSR __PE__(REG_SLEEP_STATUS)


// ISRS
#define GPIO_EVENT              25
#define TIMER_A_OVERFLOW        0x1C
#define TIMER_A_OUTPUT_CMP      0x1D
#define TIMER_B_OVERFLOW        0x1E
#define TIMER_B_OUTPUT_CMP      0x1F


#endif
