
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "event.h"
#include "timer.h"
#include "int.h"

static volatile int timer_triggered = 0;

void timer_overflow_isr(void) {
	timer_triggered++;
	printf("In ISR\n");
}


int main() {
  uart_set_cfg(0, 0);

  uart_send("Hello!\n", 7);

  uart_wait_tx_done();

  printf("Hello World!!!!!\n");

  // Configure ISRs
  int_init();
  int_add(TIMER_A_OUTPUT_CMP, (void *) timer_overflow_isr, 0);
  int_enable();

  EER = 0xff;
  IER = 0xC0000000; // enable all timer interrupts
  
  /* Setup Timer A */
  TOCRA = 0xe80;
  TPRA = 0x0f; // set prescaler

  while(!timer_triggered)
  	asm volatile ( "nop" );

  int_disable();

  return 0;
}
