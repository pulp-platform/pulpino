#include "bench.h"
#include "uart.h"
#include "spr-defs.h"

void bench_timer_start(void) {
  start_core_timer(get_core_id());
}

void bench_timer_stop(void) {
  stop_core_timer(get_core_id());
}

void bench_timer_reset(void) {
  reset_core_timer(get_core_id());
}

void print_result(testcase_t *test, testresult_t *result)
{
  qprintf("== test: %d, %s -> ", test->id, (int)test->name, 0, 0);

  if (result->errors == 0)
    qprintf("success, ", 0, 0, 0, 0);
  else
    qprintf("fail, ", 0, 0, 0, 0);

  qprintf("nr. of errors: %d", result->errors, 0, 0, 0);

  if(result->time == 0)
    qprintf("\n", 0, 0, 0, 0);
  else
    qprintf(", execution time: %d\n", result->time, 0, 0, 0);
}

void print_summary(unsigned int errors)
{
  volatile int* ptr = (int*)(0x10001000+get_core_id()*4*2);

  qprintf("==== SUMMARY: ", 0, 0, 0, 0);
  ptr[1] = errors;

  if (errors == 0) {
    qprintf("SUCCESS\n", 0, 0, 0, 0);
    qprintf("OOOOOOK!!!!!!\n",0,0,0,0); // TODO: remove this one
    ptr[0] = 0xABBAABBA;                // and this
  } else {
    qprintf("FAIL\n", 0, 0, 0, 0);
    qprintf("NOT OK!!!!!\n",0,0,0,0);   // TODO: remove this one
    ptr[0] = 0xDEADBEEF;                // and this
  }
}

void run_benchmark(testcase_t *test, testresult_t *result)
{
  result->errors = 0;

  bench_timer_reset();

  test->test(result, bench_timer_start, bench_timer_stop);

  result->time = get_core_time(get_core_id());
}

void run_suite(testcase_t *tests, size_t num)
{
  uart_set_cfg(0, 26);

  unsigned int errors = 0;
  size_t i;

  for (i = 0; i < num; i++) {
    testresult_t result;
    run_benchmark(&tests[i], &result);
    print_result(&tests[i], &result);

    errors += result.errors;
  }

  print_summary(errors);

  uart_wait_tx_done();
}

void check_uint32(testresult_t* result, const char* fail_msg, uint32_t actual, uint32_t expected)
{
  if(actual != expected) {
    result->errors += 1;
    qprintf("%s: Actual %X, expected %X\n", fail_msg, actual, expected, 0);
  }
}

// use weak attribute here, so we can overwrite this function to provide custom exception handlers, e.g. for tests
__attribute__((interrupt)) __attribute__((weak))
void illegal_insn_handler_c(void)
{
#ifndef __riscv__
  unsigned int exception_address = mfspr(SPR_EPCR_BASE);
  unsigned int insn = *((unsigned int*)(exception_address));
  qprintf("Illegal instruction encountered at address 0x%08X: %X\n", exception_address, insn, 0, 0);
#endif
  eoc(0);
}
