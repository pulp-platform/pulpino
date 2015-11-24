#include "string_lib.h"
#include "uart.h"
#include "utils.h"

unsigned int g_errors = 0;

void print_okay(const int testnr, const char *testsuite) {
  printf("Test suite OK: %s\n", testsuite);
}

void print_fail(const int testnr, const char *testsuite) {
  g_errors++;
  printf("Test FAIL: %d (suite %s)\n", testnr, testsuite);
}


void print_test_summary()
{

  printf("==== SUMMARY: ");

  if (g_errors == 0) {
    printf("OOOOOOK!!!!!!\n");
  } else {
    printf("NOT OK!!!!!\n");
  }

  uart_wait_tx_done();
}
