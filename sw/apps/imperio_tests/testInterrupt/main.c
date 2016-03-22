// Copyright 2015 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <string.h>
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
  int_add(29, (void *) int_time_cmp, 0);
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
