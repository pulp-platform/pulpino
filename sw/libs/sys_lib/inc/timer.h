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
 * @brief Timer library.
 *
 * Provides Timer function like writing the appropriate
 * timer registers and uttility functions to cycle count
 * certain events. Used in bench.h.
 *
 * @author Florian Zaruba
 *
 * @version 1.0
 *
 * @date 2/10/2015
 *
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include "pulpino.h"

#define TIMERA_ADDR         0x00
#define TIMERA_CTRL         0x04
#define TIMERA_OUPUT_CMP    0x08

#define TIMERB_ADDR         0x10
#define TIMERB_CTRL         0x14
#define TIMERB_OUPUT_CMP    0x18

/* pointer to mem of timer unit - PointerTimer */
#define __PT__(a) *(volatile int*) (TIMER_BASE_ADDR + a)

/** timer A register - contains the actual cycle counter */
#define TIRA __PT__(TIMERA_ADDR)

/** timer A control register */
#define TPRA __PT__(TIMERA_CTRL)

/** timer A output compare register */
#define TOCRA __PT__(TIMERA_OUPUT_CMP)

/** timer A register - contains the actual cycle counter */
#define TIRB __PT__(TIMERB_ADDR)

/** timer A control register */
#define TPRB __PT__(TIMERB_CTRL)

/** timer A output compare register */
#define TOCRB __PT__(TIMERB_OUPUT_CMP)


void reset_timer(void);

void start_timer(void);

void stop_timer(void);

int get_time(void);

#endif
