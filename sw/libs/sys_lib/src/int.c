// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "utils.h"
#include "int.h"
#include "string_lib.h"
#include "uart.h"
#include "event.h"

/* int time cmp stub */
__attribute__ ((weak, interrupt))
void int_time_cmp (void) {

}

/* Interrupt handlers table */
struct ihnd int_handlers[MAX_INT_HANDLERS];

void handler_stub(void) {
  printf("Stub called\n");
}

/* Initialize routine */
void int_init(void)
{
  for(int i = 0; i < MAX_INT_HANDLERS; i++) {
    int_handlers[i].handler = (void *) handler_stub;
    int_handlers[i].arg = 0;
  }
}

/* Add interrupt handler */
int int_add(unsigned long irq, void (* handler)(void *), void *arg)
{
  if(irq >= MAX_INT_HANDLERS)
    return 0;

  int_handlers[irq].handler = handler;
  int_handlers[irq].arg = arg;

  return 1;
}

/* Main interrupt handler */
__attribute__((weak))
void int_main(void) {
  // select correct interrupt
  // read cause register to get pending interrupt
  // execute ISR.

#ifdef __riscv__
  int mcause;
  csrr(mcause, mcause);



  // printf("In ISR. cause = %u\n", mcause & 0x1F);

  if (mcause & (1 << 31)) {
    // interrupt handler called because of external IRQ
    int_handlers[(mcause & 0x1F)].handler(int_handlers[(mcause & 0x1F)].arg);
  }

  // printf("Leaving ISR.\n");
  // clear pending register
  ICP = (1 << mcause);
#endif
}
