#include "timer.h"

void
start_timer()
{
  *(volatile int*) START_TIME_ADDR = 1;
}

void
stop_timer()
{
  *(volatile int*) STOP_TIME_ADDR = 1;
}

void
reset_timer()
{
  *(volatile int*) RESET_TIME_ADDR = 1;
}

int
get_time()
{
  volatile int time;
  
  time = *(volatile int*) GET_TIME_LO_ADDR;
  
  return time;
}

int
get_time_hi()
{
  
  volatile int time;
  
  time = *(volatile int*) GET_TIME_HI_ADDR;

  return  time;

}

void
start_core_timer(int timernum)
{
    switch(timernum) {
    case 0:
        *(volatile int*) START_TIME_T0_ADDR = 1;
        break;
    case 1:
        *(volatile int*) START_TIME_T1_ADDR = 1;
        break;
    case 2:
        *(volatile int*) START_TIME_T2_ADDR = 1;
        break;
    case 3:
        *(volatile int*) START_TIME_T3_ADDR = 1;
        break;
    }
}

void
stop_core_timer(int timernum)
{
    switch(timernum) {
    case 0:
        *(volatile int*) STOP_TIME_T0_ADDR = 1;
        break;
    case 1:
        *(volatile int*) STOP_TIME_T1_ADDR = 1;
        break;
    case 2:
        *(volatile int*) STOP_TIME_T2_ADDR = 1;
        break;
    case 3:
        *(volatile int*) STOP_TIME_T3_ADDR = 1;
        break;
    }
}

void
reset_core_timer(int timernum)
{
    switch(timernum) {
    case 0:
        *(volatile int*) RESET_TIME_T0_ADDR = 1;
        break;
    case 1:
        *(volatile int*) RESET_TIME_T1_ADDR = 1;
        break;
    case 2:
        *(volatile int*) RESET_TIME_T2_ADDR = 1;
        break;
    case 3:
        *(volatile int*) RESET_TIME_T3_ADDR = 1;
        break;
    }
}

int
get_core_time(int timernum)
{
  volatile int time;

    switch(timernum) {
    case 0:
        time = *(volatile int*) GET_TIME_T0_ADDR;
        break;
    case 1:
        time = *(volatile int*) GET_TIME_T1_ADDR;
        break;
    case 2:
        time = *(volatile int*) GET_TIME_T2_ADDR;
        break;
    case 3:
        time = *(volatile int*) GET_TIME_T3_ADDR;
        break;
    }
  return time;
}

void
set_core_time(int timernum,int timerval)
{

    switch(timernum) {
    case 0:
        *(volatile int*) GET_TIME_T0_ADDR = timerval;
        break;
    case 1:
        *(volatile int*) GET_TIME_T1_ADDR = timerval;
        break;
    case 2:
        *(volatile int*) GET_TIME_T2_ADDR = timerval;
        break;
    case 3:
        *(volatile int*) GET_TIME_T3_ADDR = timerval;
        break;
    }
}




