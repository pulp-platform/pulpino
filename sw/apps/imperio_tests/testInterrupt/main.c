
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "event.h"
#include "timer.h"
#include "int.h"
#include "bench.h"
#include "gpio.h"

volatile int timer_triggered = 0;

__attribute__ ((interrupt))
void int_time_cmp(void) {
  ICP = (1 << 29);
  // if (timer_triggered == 1) {
  //   set_gpio_pin_direction(0, DIR_OUT);
  //   set_gpio_pin_value(0, 1);
  // }
  timer_triggered++;
  // clear pending register
}


int main() {
  // Configure ISRs
  int_init();
  //int_add(TIMER_A_OUTPUT_CMP, (void *) timer_overflow_isr, 0);
  int_enable();

  EER = 0xff;
  IER = 0xF0000000; // enable all timer interrupts

  /* Setup Timer A */
  TOCRA = 0x80;
  TPRA  = 0x3F; // set prescaler, enable interrupts and start timer.

  while (timer_triggered < 5) {
    printf("Loop Counter: %d\n", timer_triggered);
    sleep();
  }

  set_gpio_pin_value(0, 0);
  int_disable();

  print_summary(0);
  return 0;
}
