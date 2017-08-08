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
#include "utils.h"
#include "bench.h"

void check_compressed_vs_normal(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "vs_normal",          .test = check_compressed_vs_normal },
  {0, 0}
};

int main()
{
  run_suite(testcases);

  return 0;
}

// belongs to compressed vs normal tests
unsigned int g_error;

void compressed_vs_normal_tests();

void check_compressed_vs_normal(testresult_t *result, void (*start)(), void (*stop)()) {
  g_error = 0;

  compressed_vs_normal_tests();

  result->errors = g_error;
}

void helper_check(const char* str, uint32_t compressed, uint32_t normal)
{
  if(compressed != normal) {
    g_error++;
    printf("Instruction %s: Compressed %X, Normal %X\n", str, compressed, normal);
  }
}

void helper_error(const char* str) {
  g_error++;
  printf("Error in executing %s\n", str);
}

void helper_link_check(const char* str, uint32_t should_be_zero, uint32_t act) {
  if((should_be_zero && act != 0) || (!should_be_zero && act == 0)) {
    g_error++;
    printf("Error in executing %s\n", str);
  }
}
