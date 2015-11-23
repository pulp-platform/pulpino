#include <stdio.h>
#include "pulpino.h"
#include "uart.h"
#include "gpio.h"
#include "utils.h"
#include "bench.h"


void check_uart(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "uart",                  .test = check_uart                 },
  {0, 0}
};

int main()
{
  run_suite(testcases);

  return 0;
}

void check_uart(testresult_t *result, void (*start)(), void (*stop)()) {
  int i;
  int j;
  char c;

  if(get_core_id() == 0) {

    for (j = 0; j < 10; j++) {
      uart_wait_tx_done();
      uart_send("01234567", 8);
      
      for (i = 0; i < 8; i++) {
        c = uart_getchar();

        if (c != '0' + i) {
          result->errors++;
          printf("Wrong character received: %c\n", c);
        }
      }
    }
  }

  synch_barrier();
}
