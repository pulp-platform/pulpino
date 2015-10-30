
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "event.h"
#include "timer.h"
#include "int.h"


volatile int timer_triggered = 0;

void timer_overflow_isr(void) {
  timer_triggered++;
  printf("In ISR\n");
  //int_disable();
}


int main() {

  printf("Hello World!!!!!\n");
  //asm volatile (".byte 0,0,0,0");
  //printf("Post ILLINSN\n");

  // Configure ISRs
  int_init();
  int_add(TIMER_A_OUTPUT_CMP, (void *) timer_overflow_isr, 0);
  int_enable();

  EER = 0xff;
  IER = 0xC0000000; // enable all timer interrupts

  /* Setup Timer A */
  TOCRA = TOCRB = 0x800;
  TPRA = TPRB = 0x3F; // set prescaler, enable interrupts and start timer.
  sleep();

  while(timer_triggered < 3)
  asm volatile ( "nop" );

  int_disable();

  return 0;
}
