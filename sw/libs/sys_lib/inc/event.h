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

#ifndef	__EVENT_H__
#define __EVENT_H__

#include <pulpino.h>

#define REG_IRQ_ENABLE         	0x00
#define REG_IRQ_PENDING      	0x04
#define REG_IRQ_ACK   			0x08

#define REG_EVENT_ENABLE 		0x10
#define REG_EVENT_PENDING    	0x14
#define REG_EVENT_ACK      		0x1C

#define REG_SLEEP_CTRL        	0x20
#define REG_SLEEP_STATUS		0x24


// pointer to mem of event unit - PointerEventunit
#define __PE__(a) *(volatile int*) (EVENT_UNIT_BASE_ADDR + a)

// interrupt enable register
#define IER __PE__(REG_IRQ_ENABLE)

// interrupt pending register
#define IPR __PE__(REG_IRQ_PENDING)

// interrupt acknowledge register
#define IAR __PE__(REG_IRQ_ACK)

// event enable register
#define EER __PE__(REG_EVENT_ENABLE)

// event pending register
#define EPR __PE__(REG_EVENT_PENDING)

// event acknowledge register
#define EAR __PE__(REG_EVENT_ACK)

// sleep control register
#define SCR __PE__(REG_SLEEP_CTRL)

// sleep status register
#define SSR __PE__(REG_SLEEP_STATUS)


// ISRS
#define TIMER_A_OVERFLOW 		0x1e
#define TIMER_A_OUTPUT_CMP 		0x1f


#endif
