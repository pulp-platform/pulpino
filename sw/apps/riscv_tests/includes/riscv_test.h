#ifndef RISCV_TESTS_H
#define RISCV_TESTS_H

#include <stdint.h>


unsigned int g_num_checks = 0;
unsigned int g_errors = 0;

#define test_init() test_init_int(__FILE__)
// since the global variables are not initialized we have to do that in a special function here
void test_init_int(const char* filename) {
  g_num_checks = 0;
  g_errors = 0;

#ifdef RISCV_TESTS_PRINTF
  printf("Starting %s\n", filename);
#endif
}

void test_check(const char* str, uint32_t act, uint32_t exp) {
  g_num_checks++;

  if(act != exp) {
    g_errors++;
#ifdef RISCV_TESTS_PRINTF
    printf("%s: Is %X, expected %X\n", str, act, exp);
#endif
  }
}

void test_check_flags(const char* str, uint32_t spr, uint32_t exp, uint32_t flags) {
  g_num_checks++;

  if((spr & flags) != (exp & flags)) {
    g_errors++;
#ifdef RISCV_TESTS_PRINTF
    printf("%s: SPR is %X, is %X, expected %X\n", str, spr, spr & flags, exp & flags);
#endif
  }
}

unsigned int test_get_num_errors(void) {
  return g_errors;
}

void test_report(void) {
#ifdef RISCV_TESTS_PRINTF
  printf("%d of %d checks failed\n", g_errors, g_num_checks);

  if(g_errors == 0)
    printf ("OOOOOOK!!!!!!\n");
  else
    printf("NOT OK!!!!!\n");
#endif
}

#endif
