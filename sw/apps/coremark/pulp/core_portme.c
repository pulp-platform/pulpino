/* 
  File : core_portme.c
*/
/*
  Author : Shay Gal-On, EEMBC
  Legal : TODO!
*/ 
//#include <stdio.h>
//#include <stdlib.h>
#include <utils.h>
#include "bar.h"
#include "timer.h"
#include "coremark.h"
#include "cpu_hal.h"

#if VALIDATION_RUN
  volatile ee_s32 seed1_volatile=0x3415;
  volatile ee_s32 seed2_volatile=0x3415;
  volatile ee_s32 seed3_volatile=0x66;
#endif
#if PERFORMANCE_RUN
  volatile ee_s32 seed1_volatile=0x0;
  volatile ee_s32 seed2_volatile=0x0;
  volatile ee_s32 seed3_volatile=0x66;
#endif
#if PROFILE_RUN
  volatile ee_s32 seed1_volatile=0x8;
  volatile ee_s32 seed2_volatile=0x8;
  volatile ee_s32 seed3_volatile=0x8;
#endif
  volatile ee_s32 seed4_volatile=ITERATIONS;
  volatile ee_s32 seed5_volatile=0;
/* Porting : Timing functions
  How to capture time and convert to seconds must be ported to whatever is supported by the platform.
  e.g. Read value from on board RTC, read value from cpu clock cycles performance counter etc. 
  Sample implementation for standard time.h and windows.h definitions included.
*/
/* Define : TIMER_RES_DIVIDER
  Divider to trade off timer resolution and total time that can be measured.

  Use lower values to increase resolution, but make sure that overflow does not occur.
  If there are issues with the return value overflowing, increase this value.
  */
#define NSECS_PER_SEC CLOCKS_PER_SEC
#define CORETIMETYPE clock_t 
#define GETMYTIME(_t) (*_t=get_time())
#define MYTIMEDIFF(fin,ini) ((fin)-(ini))
#define TIMER_RES_DIVIDER 1
#define SAMPLE_TIME_IMPLEMENTATION 1
#define EE_TICKS_PER_SEC (NSECS_PER_SEC / TIMER_RES_DIVIDER)

/** Define Host specific (POSIX), or target specific global time variables. */
static CORETIMETYPE start_time_val, stop_time_val;

/* Function : start_time
  This function will be called right before starting the timed portion of the benchmark.

  Implementation may be capturing a system timer (as implemented in the example code) 
  or zeroing some system parameters - e.g. setting the cpu clocks cycles to 0.
*/
void start_time(void) {
  cpu_perf_setall(0);
  cpu_perf_conf_events(SPR_PCER_ALL_EVENTS_MASK);
  //cpu_perf_conf(SPR_PCMR_ACTIVE | SPR_PCMR_SATURATE);

  start_timer();

  GETMYTIME(&start_time_val );
}
/* Function : stop_time
  This function will be called right after ending the timed portion of the benchmark.

  Implementation may be capturing a system timer (as implemented in the example code) 
  or other system parameters - e.g. reading the current value of cpu cycles counter.
*/
void stop_time(void) {
  stop_timer();
  //cpu_perf_conf(0);

  printf("Perf CYCLES: %d\n",     cpu_perf_get(SPR_PCER_CYCLES));
  printf("Perf INSTR: %d\n",      cpu_perf_get(SPR_PCER_INSTR));
  printf("Perf LOAD: %d\n",       cpu_perf_get(SPR_PCER_LOAD));
  printf("Perf JUMP: %d\n",       cpu_perf_get(SPR_PCER_JUMP));
  printf("Perf IMISS: %d\n",      cpu_perf_get(SPR_PCER_IMISS));
  printf("Perf BRANCH: %d\n",     cpu_perf_get(SPR_PCER_BRANCH));
  printf("Perf BRANCH_CYC: %d\n", cpu_perf_get(SPR_PCER_BRANCH_CYC));
  printf("Perf LD: %d\n",         cpu_perf_get(SPR_PCER_LD));
  printf("Perf ST: %d\n",         cpu_perf_get(SPR_PCER_ST));
  printf("Perf LD_EXT: %d\n",     cpu_perf_get(SPR_PCER_LD_EXT));
  printf("Perf ST_EXT: %d\n",     cpu_perf_get(SPR_PCER_ST_EXT));
  printf("Perf LD_EXT_CYC: %d\n", cpu_perf_get(SPR_PCER_LD_EXT_CYC));
  printf("Perf ST_EXT_CYC: %d\n", cpu_perf_get(SPR_PCER_ST_EXT_CYC));
  printf("Perf TCDM_CONT: %d\n",  cpu_perf_get(SPR_PCER_TCDM_CONT));
  GETMYTIME(&stop_time_val );
}
/* Function : time_in_secs
  Convert the value returned by get_time to seconds.

  The <secs_ret> type is used to accomodate systems with no support for floating point.
  Default implementation implemented by the EE_TICKS_PER_SEC macro above.
*/
secs_ret time_in_secs(CORE_TICKS ticks) {
  secs_ret retval=((secs_ret)ticks) / (secs_ret)EE_TICKS_PER_SEC;
  return retval;
}

ee_u32 default_num_contexts=MULTITHREAD;

/* Function : portable_init
  Target specific initialization code 
  Test for some common mistakes.
*/
void portable_init(core_portable *p, int *argc, char *argv[])
{
  if(get_core_id() >= MULTITHREAD) {
    while(1) {
      synch_barrier();
    }
  }

  if (sizeof(ee_ptr_int) != sizeof(ee_u8 *)) {
    ee_printf("ERROR! Please define ee_ptr_int to a type that holds a pointer!\n");
  }
  if (sizeof(ee_u32) != 4) {
    ee_printf("ERROR! Please define ee_u32 to a 32b unsigned type!\n");
  }
  p->portable_id=1;
}
/* Function : portable_fini
  Target specific final code 
*/
void portable_fini(core_portable *p)
{
  p->portable_id=0;
  synch_barrier();
  eoc(0);
}

void abort() {
  exit(0);
}

ee_u8 core_start_parallel(core_results *res) {
  iterate(res);
  return 0;
}
ee_u8 core_stop_parallel(core_results *res) {
  synch_barrier();
  return 0;
}
