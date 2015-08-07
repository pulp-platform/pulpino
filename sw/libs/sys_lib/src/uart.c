#include "uart.h"
#include "events.h"
#include "events_ids.h"
#include "utils.h"

__attribute__ ((section(".scmlock"))) volatile char lock_uart_lock;

/**
 * Setup UART. The UART defaults to 8 bit character mode with 1 stop bit.
 *
 * parity       Enable/disable parity mode
 * clk_counter  Clock counter value that is used to derive the UART clock.
 *              There is a prescaler in place that already divides the SoC
 *              clock by 16.  Since this is a counter, a value of 0 means that
 *              the SoC clock divided by 16 is used. A value of 31 would mean
 *              that we use the SoC clock divided by 16*32 = 512.
 */
void uart_set_cfg(int parity, uint16_t clk_counter) {
  unsigned int i;

  // initialize the lock
  uart_unlock();

  *(volatile unsigned int*)(UART_REG_LCR) = 0x83; //sets 8N1 and set DLAB to 1
  *(volatile unsigned int*)(UART_REG_DLM) = (clk_counter >> 8) & 0xFF;
  *(volatile unsigned int*)(UART_REG_DLL) =  clk_counter       & 0xFF;
  *(volatile unsigned int*)(UART_REG_FCR) = 0xA7; //enables 16byte FIFO and clear FIFOs
  *(volatile unsigned int*)(UART_REG_LCR) = 0x03; //sets 8N1 and set DLAB to 0
  
  *(volatile unsigned int*)(UART_REG_IER) = ((*(volatile unsigned int*)(UART_REG_IER)) & 0xF0) | 0x02; // set IER (interrupt enable register) on UART
}

void uart_send(const char* str, unsigned int len) {
  unsigned int i;
  
  unsigned int core_id = get_core_id();

  // ensure that we are the only core that currently accesses the UART, this
  // ensures that messages "stay together"
  uart_lock();

  while(len > 0) {
    // wait until there is space in the fifo
    while( (*(volatile unsigned int*)(UART_REG_LSR) & 0x20) == 0);

    // process this in batches of 16 bytes to actually use the FIFO in the UART
    for(i = 0; (i < UART_FIFO_DEPTH) && (len > 0); i++) {
      // load FIFO
      *(volatile unsigned int*)(UART_REG_THR) = *str++;

      len--;
    }
  }

  uart_unlock();
}

void uart_sendchar(const char c) {
  // wait until there is space in the fifo
  while( (*(volatile unsigned int*)(UART_REG_LSR) & 0x20) == 0);

  // load FIFO
  *(volatile unsigned int*)(UART_REG_THR) = c;
}

void uart_wait_tx_done(void) {
  // wait until there is space in the fifo
  while( (*(volatile unsigned int*)(UART_REG_LSR) & 0x40) == 0);
}

