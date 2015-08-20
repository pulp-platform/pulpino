#ifndef __OR10N_CPU_HAL_H__
#define __OR10N_CPU_HAL_H__

#include "spr-defs.h"

#ifdef __riscv__
/* Configure the active events. eventMask is an OR of events got through SPR_PCER_EVENT_MASK */
static inline void cpu_perf_conf_events(unsigned int eventMask)
{
}

/* Configure the mode. confMask is an OR of all SPR_PCMR_* macros */
static inline void cpu_perf_conf(unsigned int confMask)
{
}

/* Starts counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_start(unsigned int conf) {
}

/* Stops counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_stop(unsigned int conf) {
}

/* Set the specified counter to the specified value */
static inline void cpu_perf_set(unsigned int counterId, unsigned int value) {
}

/* Set all counters to the specified value */
static inline void cpu_perf_setall(unsigned int value) {
}

/* Return the value of the specified counter */
static inline unsigned int cpu_perf_get(unsigned int counterId) {
  return 0;
}
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
