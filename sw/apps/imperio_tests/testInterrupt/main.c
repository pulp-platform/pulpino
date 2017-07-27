/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */


#include <string.h>
#include "utils.h"
#include "uart.h"
#include "event.h"
#include "timer.h"
#include "int.h"
#include "bench.h"
#include "gpio.h"

volatile int timer_triggered = 0;


void ISR_TA_CMP(void) {
  ICP = (1 << 29);
  timer_triggered++;
}


int main() {
  // Configure ISRs
  int_enable();

  EER = 0xF0000000; // enable all timer events;
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
