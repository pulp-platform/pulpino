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
