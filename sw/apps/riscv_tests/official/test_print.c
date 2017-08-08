// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


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
  
  // signal end of computation
  eoc(g_errors);
}
