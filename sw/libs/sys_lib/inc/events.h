#ifndef	__EVENTS_H__
#define __EVENTS_H__

#include "pulp.h"
#include "events_ids.h"

#define BB_TCDM0              BBMUX_CLKGATE_BASE_ADDR + 0X000
#define BB_TCDM1              BBMUX_CLKGATE_BASE_ADDR + 0X004
#define BB_TCDM2              BBMUX_CLKGATE_BASE_ADDR + 0X008
#define BB_TCDM3              BBMUX_CLKGATE_BASE_ADDR + 0X00C
#define BB_SCM                BBMUX_CLKGATE_BASE_ADDR + 0X010
#define BB_CLUSTER            BBMUX_CLKGATE_BASE_ADDR + 0X014
#define BB_CORE_BASE          BBMUX_CLKGATE_BASE_ADDR + 0X018
#define BBMUX_SEL             BBMUX_CLKGATE_BASE_ADDR + 0X058

//////////////			\\\\\\\\\\\\\\\\
//////		CORE CLKGATING	   \\\\\\\\\\\\\
/////////////			\\\\\\\\\\\\\\\

//////////////
////////////	EVENT MASK
/////////////

#define	EV_MASK_LOW_BASE      BBMUX_CLKGATE_BASE_ADDR + 0X100
#define EV_MASK_HIGH_BASE     BBMUX_CLKGATE_BASE_ADDR + 0X140
//////////////
/////////////	EVENT BUFFERS
//////////////

#define EV_BUFF_CLEAR         EVENT_UNIT_DEMUX_BASE_ADDR + 0x004
#define EV_BUFFER_LOW_BASE    BBMUX_CLKGATE_BASE_ADDR + 0X180
#define EV_BUFFER_HIGH_BASE   BBMUX_CLKGATE_BASE_ADDR + 0X1C0
#define EMERGENCY_EVENT_SET   BBMUX_CLKGATE_BASE_ADDR + 0X30C
#define EMERGENCY_EVNT_CLEAR  BBMUX_CLKGATE_BASE_ADDR + 0X314
////////////
//////////	INTERRUPTS MASK
////////////


#define IRQ_BUFF_CLEAR        EVENT_UNIT_DEMUX_BASE_ADDR + 0x008
#define IRQ_MASK_LOW_BASE     BBMUX_CLKGATE_BASE_ADDR + 0X200
#define IRQ_MASK_HIGH_BASE    BBMUX_CLKGATE_BASE_ADDR + 0X240
////////////
//////////	INTERRUPT BUFFERS
////////////

#define IRQ_BUFFER_LOW_BASE   BBMUX_CLKGATE_BASE_ADDR + 0X280
#define IRQ_BUFFER_HIGH_BASE  BBMUX_CLKGATE_BASE_ADDR + 0X2C0
#define EMERGENCY_IRQ_SET     BBMUX_CLKGATE_BASE_ADDR + 0X310
#define EMERGENCY_IRQ_CLEAR   BBMUX_CLKGATE_BASE_ADDR + 0X318

#define READ_IRQ_ID_BASE      BBMUX_CLKGATE_BASE_ADDR + 0X31C

#define CLKGATE_SET           BBMUX_CLKGATE_BASE_ADDR + 0X300
#define CLKGATE_CLEAR         BBMUX_CLKGATE_BASE_ADDR + 0X304
#define CORE_CLKGATE          EVENT_UNIT_DEMUX_BASE_ADDR + 0X000

#define TRIGG_BARRIER         BBMUX_CLKGATE_BASE_ADDR + 0X35C
#define GP_0                  BBMUX_CLKGATE_BASE_ADDR + 0X360
#define GP_1                  BBMUX_CLKGATE_BASE_ADDR + 0X364
#define GP_2                  BBMUX_CLKGATE_BASE_ADDR + 0X368
#define WAIT_BARRIER          BBMUX_CLKGATE_BASE_ADDR + 0X36C
#define SET_BARRIER_BASE      BBMUX_CLKGATE_BASE_ADDR + 0X374
#define CLEAR_BARRIER         BBMUX_CLKGATE_BASE_ADDR + 0X370



//////////////
/////IN COMMON
//////////////
void trigg_GP( unsigned int event_mask, unsigned int id );
__attribute__((always_inline)) void trigg_barrier( unsigned int event_mask );
unsigned int read_clkgate_register();
unsigned int read_core_clkgate_status(int coreid);
/*
0000 0->  all cores ON
0001 1->  OFF: 0  	
0010 2->  OFF: 1  	
0011 3->  OFF: 0,1
0100 4->  OFF: 2
0101 5->  OFF: 0,2
0110 6->  OFF: 1,2
0111 7->  OFF: 1,2,3 
1000 8->  OFF: 3
1001 9->  OFF: 0,3
1010 10-> OFF: 1,3
1011 11-> OFF: 0,1,3
1100 12-> OFF: 2,3
1101 13-> OFF: 0,2,3
1110 14-> OFF: 1,2,3
1111 15-> all cores OFF 
*/
//////////////
/////CLOCK GATING
//////////////
void set_memory_clockgating(unsigned int id);
void clear_memory_clockgating(unsigned int id);

//////////////
/////EVENTS APIs
//////////////
void wait_event(void);
unsigned int read_evnt_buff_low(int coreid);
void clear_evnt_buff_low(unsigned int mask ); 
void set_evnt_mask_low( int coreid , unsigned int mask );
unsigned int read_evnt_mask_low( int coreid  );
void set_emergency_evnt( unsigned int event_mask );
unsigned int read_emergency_evnt_status();
void clear_emergency_evnt( int core_id );

//////////////
/////HW BARRIER 
//////////////
extern void setup_barrier(unsigned int barrier_id, unsigned int num_threads, unsigned int mask_to_trigg);
unsigned int read_barrier_counter(unsigned int barrier_id);
void notify_barrier(unsigned int barrier_id);
extern void wait_barrier_event_buff(unsigned int barrier_id);

//////////////
/////INTERRUPTS APIs
//////////////
unsigned int read_irq_buff_low(int coreid);
void clear_irq_buff_low(unsigned int mask );
void set_irq_mask_low( int coreid , unsigned int mask ); 
unsigned int read_irq_mask_low( int coreid  );
unsigned int read_irq_id(int coreid);
void set_emergency_irq( unsigned int irq_mask );
unsigned int read_emergency_irq_status();
void clear_emergency_irq( int core_id );

#endif
