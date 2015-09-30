#include <pulp.h>

/* TODO: 
`define REG_IRQ_ENABLE         	3'b000 //BASEADDR+0x00
`define REG_IRQ_PENDING      	3'b001 //BASEADDR+0x04
`define REG_IRQ_ACK   			3'b010 //BASEADDR+0x08
`define REG_EVENT_ENABLE 		3'b011 //BASEADDR+0x0C
`define REG_EVENT_PENDING    	3'b100 //BASEADDR+0x10
`define REG_EVENT_ACK      		3'b101 //BASEADDR+0x14
`define REG_SLEEP_CTRL        	3'b110 //BASEADDR+0x18
`define REG_SLEEP_STATUS		3'b111 //BASEADDR+0x1C
*/

// pointer to mem of event unit - PointerEventunit
#define __PE__(a) *(volatile int*) (EVENT_UNIT_BASE_ADDR + a)

// interrupt enable register
#define IER __PE__(0)