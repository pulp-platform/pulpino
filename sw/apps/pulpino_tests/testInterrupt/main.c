
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "event.h"
#include "timer.h"
#include "int.h"
#include "bench.h"


volatile int timer_triggered = 0;

void timer_overflow_isr(void) {
  timer_triggered++;
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
  IER = 0xF0000000; // enable all timer interrupts

  /* Setup Timer A */
  TOCRA = 0x80;
  TPRA  = 0x3F; // set prescaler, enable interrupts and start timer.

  while(timer_triggered < 5)
    sleep();

  int_disable();

  print_summary(0);
  return 0;
}
