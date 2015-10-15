#ifndef	__TIMER_H__
#define __TIMER_H__

#include "pulpino.h"

#define TIMERA_ADDR 	    0x0
#define TIMERA_PRESCALER    0x04
#define TIMERA_OUPUT_CMP    0x08

// pointer to mem of timer unit - PointerTimer
#define __PT__(a) *(volatile int*) (TIMER_BASE_ADDR + a)

// timer A register - contains the actual cycle counter
#define TIRA __PT__(TIMERA_ADDR)

// timer A prescaler register
#define TPRA __PT__(TIMERA_PRESCALER)

// timer A output compare register
#define TOCRA __PT__(TIMERA_OUPUT_CMP)
#endif
