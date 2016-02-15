// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef __OR10N_CPU_HAL_H__
#define __OR10N_CPU_HAL_H__

#include "spr-defs.h"
#include <stdint.h>

#ifdef __riscv__

#define CSR_PCMR_ACTIVE 0x1

/* Configure the active events. eventMask is an OR of events got through SPR_PCER_EVENT_MASK */
static inline void cpu_perf_conf_events(unsigned int eventMask)
{
  asm volatile ("csrw 0x7A0, %0" : "+r" (eventMask));
}

/* Configure the mode. confMask is an OR of all SPR_PCMR_* macros */
static inline void cpu_perf_conf(unsigned int confMask)
{
  asm volatile ("csrw 0x7A1, %0" :: "r" (confMask));
}

/* Starts counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_start(unsigned int conf) {
  cpu_perf_conf(conf | CSR_PCMR_ACTIVE); // TODO
}

/* Stops counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_stop(unsigned int conf) {
  cpu_perf_conf(conf); // TODO
}

/* Set the specified counter to the specified value */
void cpu_perf_set(unsigned int counterId, unsigned int value);

/* Set all counters to the specified value */
static inline void cpu_perf_setall(unsigned int value) {
  asm volatile ("csrw 0x79F, %0" :: "r" (value));
}

/* Return the value of the specified counter */
unsigned int cpu_perf_get(unsigned int counterId);
#else
/*
 * PERFORMANCE COUNTERS
 * 
 * API for accessing performance counters registers.
 * Have a look at file spr-defs.h to speficy registers through defines
 * SPR_PCER_* and SPR_PCMR_*
 */

/* Configure the active events. eventMask is an OR of events got through SPR_PCER_EVENT_MASK */
static inline void cpu_perf_conf_events(unsigned int eventMask)
{
  mtspr(SPR_PCER, eventMask);
}

/* Configure the mode. confMask is an OR of all SPR_PCMR_* macros */
static inline void cpu_perf_conf(unsigned int confMask)
{
  mtspr(SPR_PCMR, confMask);
}

/* Starts counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_start(unsigned int conf) {
  mtspr(SPR_PCMR, SPR_PCMR_ACTIVE | conf);
}

/* Stops counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_stop(unsigned int conf) {
  mtspr(SPR_PCMR, conf);
}

/* Set the specified counter to the specified value */
static inline void cpu_perf_set(unsigned int counterId, unsigned int value) {
  mtspr(SPR_PCCR(counterId), value);
}

/* Set all counters to the specified value */
static inline void cpu_perf_setall(unsigned int value) {
  mtspr(SPR_PCCR(31), value);
}

/* Return the value of the specified counter */
static inline unsigned int cpu_perf_get(unsigned int counterId) {
  return mfspr(SPR_PCCR(counterId));
}
#endif


#endif
