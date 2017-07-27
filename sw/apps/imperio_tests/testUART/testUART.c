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


#include <stdio.h>
#include "pulpino.h"
#include "uart.h"
#include "gpio.h"
#include "utils.h"
#include "bench.h"


void check_uart(testresult_t *result, void (*start)(), void (*stop)());
void check_uart_basic(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  // { .name = "uart_basic",            .test = check_uart_basic           },
  { .name = "uart",                  .test = check_uart                 },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}
void check_uart_basic(testresult_t *result, void (*start)(), void (*stop)()) {
    char c;

    // *(volatile unsigned int*)(UART_REG_FCR) = 0x00; // disable 16byte FIFO and clear FIFOs
    uart_send("a", 1);

    uart_wait_tx_done();

    c = uart_getchar();
    if (c != 'a')
      result->errors++;

    // test clearing fifos
    return;
}

void check_uart(testresult_t *result, void (*start)(), void (*stop)()) {
  int i;
  int j;
  char c;
  // *(volatile unsigned int*)(UART_REG_FCR) = 0x00; // disable 16byte FIFO and clear FIFOs

  for (j = 0; j < 10; j++) {
    uart_wait_tx_done();
    uart_send("01234567", 8);

    for (i = 0; i < 8; i++) {
      c = uart_getchar();

      if (c != '0' + i) {
        result->errors++;
        printf("Error: act: %c; exp: %c\n", c, '0' + i);
        return;
      }
    }
  }
}
