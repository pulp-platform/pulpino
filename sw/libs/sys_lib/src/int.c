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
void int_init(void)
{
  /*
  int i;

  for(i = 0; i < MAX_INT_HANDLERS; i++) {
    int_handlers[i].handler = 0;
    int_handlers[i].arg = 0;
  }
  */
}

/* Add interrupt handler */ 
int int_add(unsigned long irq, void (* handler)(void *), void *arg)
{
  /*
  if(irq >= MAX_INT_HANDLERS)
    return -1;

  int_handlers[irq].handler = handler;
  int_handlers[irq].arg = arg;
  */
  return 0;
}

/* Main interrupt handler */
__attribute__((interrupt)) __attribute__((weak))
void int_main(void) {

}

/** 
 * \brief Timer compare interrupt service routine.
 * \param void
 * \return void
 *
 * Interrupt service routine triggered when
 * 32 bit mtimecmp and mtime match. Only triggered
 * when interrupts are enabled and mtimecmp is not 0x00.
 *
 * Can be redefined by user software.
 */
 __attribute__((interrupt)) __attribute__((weak))
void int_time_cmp(void) {

}

/** 
 * \brief Timer compare interrupt service routine.
 * \param void
 * \return void
 *
 * Interrupt service routine for level-triggerd
 * external irq pin.
 *
 * Can be redefined by user software.
 */
__attribute__((interrupt)) __attribute__((weak)) 
// use weak attribute here, so we can overwrite this function to provide custom exception handlers, e.g. for tests
void irq_emergency_handler_c(void)
{
  for(;;);
}
