/*
 *
 * User interrupt handler software for OR1200
 *
 */

#include "utils.h"
#include "spr-defs.h"
#include "int.h"
#include "string_lib.h"
#include "uart.h"

/* Interrupt handlers table */
struct ihnd int_handlers[MAX_INT_HANDLERS];

/* Initialize routine */
void int_init()
{
  int i;

  for(i = 0; i < MAX_INT_HANDLERS; i++) {
    int_handlers[i].handler = 0;
    int_handlers[i].arg = 0;
  }
}

/* Add interrupt handler */ 
int int_add(unsigned long irq, void (* handler)(void *), void *arg)
{
  if(irq >= MAX_INT_HANDLERS)
    return -1;

  int_handlers[irq].handler = handler;
  int_handlers[irq].arg = arg;

  return 0;
}

/* Main interrupt handler */
__attribute__((interrupt)) __attribute__((weak))
void int_main()
{
  // unsigned int core_id = get_core_id();
  // unsigned int irq_id;

  // for(irq_id = read_irq_id(core_id); irq_id != 0xFF; irq_id = read_irq_id(core_id)) {
  //   if(int_handlers[irq_id].handler != 0) {
  //     // call interrupt handler
  //     (*int_handlers[irq_id].handler)(int_handlers[irq_id].arg);
  //   }

  //   // clear flag in buffer
  //   clear_irq_buff_low(1 << irq_id);
  // }
  //printf("In interrupt handler\n");
}

// use weak attribute here, so we can overwrite this function to provide custom exception handlers, e.g. for tests
__attribute__((interrupt)) __attribute__((weak))
void irq_emergency_handler_c(void)
{
  for(;;);
}
