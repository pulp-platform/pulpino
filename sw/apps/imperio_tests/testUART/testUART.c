// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


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
