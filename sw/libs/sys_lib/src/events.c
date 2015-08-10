#include "events.h"
#include "events_ids.h"

// EVENT/IRQ MASK DESCRIPTION:
// ID  0       : BARRIER (highest priority)
// ID 1,2,3    : GP EVENTS
// ID 4,5,6,7  : TMR 
// ID 8        : DMA
// ID 9,10     : SPI (lowest priority)
// 
// the mask bits are active HIGH in all the following functions
//
//


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
To use event you should:
-set the event mask providing a bitmask.. this bitmask selects the events that the core will sense
-if you go to sleep, be sure to clear the event buffer related to the event you were waiting


It has changed the way to trigger GP events... now you should:
-provide the GP event ( ID 1 to 3 )
-provide a bitmask where the asserted bits correspond to the cores that will sense the event
-the selected GP event should also be active in the event mask ( it's a kind of redundancy )

												  */
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
			EVNT APIs
												  */
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void wait_event(void) {
}


unsigned int read_evnt_buff_low(int coreid){
  unsigned int buff;   
  buff=*(volatile unsigned int*) (EV_BUFFER_LOW_BASE  + 4*coreid);
  return buff;
}

unsigned int read_evnt_buff_high(int coreid){
  unsigned int buff;   
  buff=*(volatile unsigned int*) (EV_BUFFER_HIGH_BASE  + 4*coreid);
  return buff;
}

void clear_evnt_buff_low(unsigned int mask )  {
    *(volatile int*) (EV_BUFF_CLEAR) = mask ; 
}

void set_evnt_mask_low( int coreid , unsigned int mask )  {
    *(volatile int*) (EV_MASK_LOW_BASE + 4*coreid) = mask ; 
}

void set_evnt_mask_high( int coreid , unsigned int mask )  {
    *(volatile int*) (EV_MASK_HIGH_BASE + 4*coreid) =  mask ;  
}

unsigned int read_evnt_mask_low( int coreid  )  {  
  return *(volatile int*) (EV_MASK_LOW_BASE + 4*coreid) ; 
}

unsigned int read_evnt_mask_high( int coreid )  {
  return *(volatile int*) (EV_MASK_HIGH_BASE+ 4*coreid );
}

void set_emergency_evnt( unsigned int event_mask )
{
   *(volatile int*) (EMERGENCY_EVENT_SET) = event_mask; 
}

unsigned int read_emergency_evnt_status(){
  return *(volatile unsigned int*)(EMERGENCY_EVENT_SET);
}
void clear_emergency_evnt( int core_id )  { //id 0 to 15
    *(volatile int*) (EMERGENCY_EVNT_CLEAR) = (1<<core_id) ; 
}





///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
			INTERRUPTS APIs
												  */
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int read_irq_id(int coreid){
  unsigned int buff;   
  buff=*(volatile unsigned int*) (READ_IRQ_ID_BASE  + 4*coreid);
  return buff;
}


unsigned int read_irq_buff_low(int coreid){
  unsigned int buff;   
  buff=*(volatile unsigned int*) (IRQ_BUFFER_LOW_BASE  + 4*coreid);
  return buff;
}

unsigned int read_irq_buff_high(int coreid){
  unsigned int buff;   
  buff=*(volatile unsigned int*) (IRQ_BUFFER_HIGH_BASE  + 4*coreid);
  return buff;
}

void clear_irq_buff_low( unsigned int mask )  {
    *(volatile int*) (IRQ_BUFF_CLEAR) = mask ; 
}

void set_irq_mask_low( int coreid , unsigned int mask )  {
    *(volatile int*) (IRQ_MASK_LOW_BASE + 4*coreid) = mask ; 
}

void set_irq_mask_high( int coreid , unsigned int mask )  {
    *(volatile int*) (IRQ_MASK_HIGH_BASE + 4*coreid) =  mask ;  
}

unsigned int read_irq_mask_low( int coreid  )  {  
  return *(volatile int*) (IRQ_MASK_LOW_BASE + 4*coreid) ; 
}

unsigned int read_irq_mask_high( int coreid )  {
  return *(volatile int*) (IRQ_MASK_HIGH_BASE+ 4*coreid );
}

void set_emergency_irq( unsigned int irq_mask )
{
   *(volatile int*) (EMERGENCY_IRQ_SET) = irq_mask; 
}

unsigned int read_emergency_irq_status(){
  return *(volatile unsigned int*)(EMERGENCY_IRQ_SET);
}


void clear_emergency_irq( int core_id )  {
    *(volatile int*) (EMERGENCY_IRQ_CLEAR) =  (1<<core_id) ;  
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
			IN COMMON APIs
												  */
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


__attribute__((always_inline)) void trigg_barrier( unsigned int event_mask ){
   *(volatile int*) (TRIGG_BARRIER) = event_mask;
}

// id = trigg event for a specific core (one hot encoding)
// core0=1, core1=2, core2=4, core3=8
// event_mask = specify which event is triggered (GP0-GP3)
void trigg_GP( unsigned int event_mask, unsigned int id ){
   *(volatile int*) (GP_0+4*(id-1)) = event_mask;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
			HW_BARRIER
												  */
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


inline void wait_barrier_event_buff(unsigned int barrier_id) {
    *(volatile int*) (WAIT_BARRIER) =  barrier_id;
    *(volatile int*) (CORE_CLKGATE) =  0x1;
    // Flush the pipeline
    *(volatile int*) (EV_BUFF_CLEAR) = 0x1;
}

void notify_barrier(unsigned int barrier_id)  {
    *(volatile int*) (WAIT_BARRIER) =  barrier_id;
}

unsigned int read_barrier_counter(unsigned int barrier_id){
    return *(volatile unsigned int*)(SET_BARRIER_BASE+4*barrier_id);
}


inline void setup_barrier(unsigned int barrier_id, unsigned int num_threads, unsigned int mask_to_trigg)  {
    *(volatile int*) (SET_BARRIER_BASE+4*barrier_id) = (num_threads<<16)+mask_to_trigg;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
			CLOCKGATING
												  */
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int read_core_clkgate_status(int coreid) {  
  unsigned int status;   
  status=*(volatile int*) (CLKGATE_SET);
  status = ( status & (1 << coreid) )/(1 << coreid);
   /*switch(coreid){
		case 0:
        	status = status & 0x00000001;
		break;
		case 1:
        	status = (status & 0x00000002)/2;
		break;
		case 2:
        	status = (status & 0x00000004)/4;
		break;
		case 3:
        	status = (status & 0x00000008)/8;
		break;
   } */
  return status;
}

unsigned int read_clkgate_register() {
  unsigned int status;   
  status=*(volatile unsigned int*) (CLKGATE_SET);
  return status;
}

void set_memory_clockgating(unsigned int id){
//IDs:
/*
0 TCDM 0
1 TCDM 1
2 TCDM 2
3 TCDM 3
5 SCM
*/
   *(volatile int*) (CLKGATE_SET) = id; 
}

void clear_memory_clockgating(unsigned int id){
//IDs:
/*
0 TCDM 0
1 TCDM 1
2 TCDM 2
3 TCDM 3
5 SCM
*/
   *(volatile int*) (CLKGATE_CLEAR) = id; 
}
