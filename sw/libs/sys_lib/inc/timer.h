#ifndef	__TIMER_H__
#define __TIMER_H__

#include "pulp.h"

#define START_TIME_ADDR    (TIMER_BASE_ADDR + 0x0)
#define STOP_TIME_ADDR     (TIMER_BASE_ADDR + 0x04)
#define RESET_TIME_ADDR    (TIMER_BASE_ADDR + 0x08)
#define GET_TIME_LO_ADDR   (TIMER_BASE_ADDR + 0x0C)
#define GET_TIME_HI_ADDR   (TIMER_BASE_ADDR + 0x10)

#define START_TIME_T0_ADDR (TIMER_BASE_ADDR + 0x40)
#define STOP_TIME_T0_ADDR  (TIMER_BASE_ADDR + 0x44)
#define RESET_TIME_T0_ADDR (TIMER_BASE_ADDR + 0x48)
#define GET_TIME_T0_ADDR   (TIMER_BASE_ADDR + 0x4C)
#define START_TIME_T1_ADDR (TIMER_BASE_ADDR + 0x80)
#define STOP_TIME_T1_ADDR  (TIMER_BASE_ADDR + 0x84)
#define RESET_TIME_T1_ADDR (TIMER_BASE_ADDR + 0x88)
#define GET_TIME_T1_ADDR   (TIMER_BASE_ADDR + 0x8C)
#define START_TIME_T2_ADDR (TIMER_BASE_ADDR + 0xC0)
#define STOP_TIME_T2_ADDR  (TIMER_BASE_ADDR + 0xC4)
#define RESET_TIME_T2_ADDR (TIMER_BASE_ADDR + 0xC8)
#define GET_TIME_T2_ADDR   (TIMER_BASE_ADDR + 0xCC)
#define START_TIME_T3_ADDR (TIMER_BASE_ADDR + 0x100)
#define STOP_TIME_T3_ADDR  (TIMER_BASE_ADDR + 0x104)
#define RESET_TIME_T3_ADDR (TIMER_BASE_ADDR + 0x108)
#define GET_TIME_T3_ADDR   (TIMER_BASE_ADDR + 0x10C)

void start_timer();
void stop_timer();
void reset_timer();

int get_time();
int get_time_hi();

void start_core_timer(int timernum);
void stop_core_timer(int timernum);
void reset_core_timer(int timernum);
int  get_core_time(int timernum);
void set_core_time(int timernum,int timerval);

#endif
