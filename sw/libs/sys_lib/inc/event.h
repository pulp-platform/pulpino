#include <pulp.h>

/* TODO: */
#define REG_IRQ_ENABLE         	0x00 //BASEADDR+0x00
#define REG_IRQ_PENDING      	0x04 //BASEADDR+0x04
#define REG_IRQ_ACK   			0x08 //BASEADDR+0x08
#define REG_EVENT_ENABLE 		0x0C //BASEADDR+0x0C
#define REG_EVENT_PENDING    	0x10 //BASEADDR+0x10
#define REG_EVENT_ACK      		0x14 //BASEADDR+0x14
#define REG_SLEEP_CTRL        	0x18 //BASEADDR+0x18
#define REG_SLEEP_STATUS		0x1C //BASEADDR+0x1C


// pointer to mem of event unit - PointerEventunit
#define __PE__(a) *(volatile int*) (EVENT_UNIT_BASE_ADDR + a)

// interrupt enable register
#define IER __PE__(REG_IRQ_ENABLE)

// interrupt pending register
#define IPR __PE__(REG_IRQ_PENDING)

// interrupt acknowledge register
#define IAR __PE__(REG_IRQ_ACK)