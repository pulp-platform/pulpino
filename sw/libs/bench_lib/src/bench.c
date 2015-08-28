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
  printf("== test: %s -> ", test->name);

  if (result->errors == 0)
    printf("success, ");
  else
    printf("fail, ");

  printf("nr. of errors: %d", result->errors);

  if(result->time == 0)
    printf("\n");
  else
    printf(", execution time: %d\n", result->time);
}

void print_summary(unsigned int errors)
{
  volatile int* ptr = (int*)(0x10001000+get_core_id()*4*2);

  printf("==== SUMMARY: ");
  ptr[1] = errors;

  if (errors == 0) {
    printf("SUCCESS\n");
    printf("OOOOOOK!!!!!!\n"); // TODO: remove this one
    ptr[0] = 0xABBAABBA;                // and this
  } else {
    printf("FAIL\n");
    printf("NOT OK!!!!!\n");   // TODO: remove this one
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

void run_suite(testcase_t *tests)
{
  // figure out how many tests should be run
  size_t num = 0;
  while(tests[num].name != 0) num++;

  // setup uart
  uart_set_cfg(0, 26);

  unsigned int errors = 0;
  size_t i;

  // perform the tests
  for (i = 0; i < num; i++) {
    testresult_t result;
    run_benchmark(&tests[i], &result);
    print_result(&tests[i], &result);

    errors += result.errors;
  }

  print_summary(errors);

  // wait for UART transmit to be done, this ensures that the output of the
  // test is actually captured by the testbench before we terminate
  uart_wait_tx_done();
}

void check_uint32(testresult_t* result, const char* fail_msg, uint32_t actual, uint32_t expected)
{
  if(actual != expected) {
    result->errors += 1;
    printf("%s: Actual %X, expected %X\n", fail_msg, actual, expected);
  }
}

void perf_print_all(void) {
#ifdef __riscv__
  printf("Perf CYCLES: %d\n",      cpu_perf_get(0));
  printf("Perf INSTR: %d\n",       cpu_perf_get(1));
  printf("Perf LD_STALL: %d\n",    cpu_perf_get(2));
  printf("Perf JR_STALL: %d\n",    cpu_perf_get(3));
  printf("Perf IMISS: %d\n",       cpu_perf_get(4));
  printf("Perf LD: %d\n",          cpu_perf_get(5));
  printf("Perf ST: %d\n",          cpu_perf_get(6));
  printf("Perf JUMP: %d\n",        cpu_perf_get(7));
  printf("Perf BRANCH: %d\n",      cpu_perf_get(8));
#else
  printf("Perf CYCLES: %d\n",      cpu_perf_get(SPR_PCER_CYCLES));
  printf("Perf INSTR: %d\n",       cpu_perf_get(SPR_PCER_INSTR));
  printf("Perf LD_STALL: %d\n",    cpu_perf_get(SPR_PCER_LD_STALL));
  printf("Perf JMP_STALL: %d\n",   cpu_perf_get(SPR_PCER_JMP_STALL));
  printf("Perf IMISS: %d\n",       cpu_perf_get(SPR_PCER_IMISS));
  printf("Perf WBRANCH: %d\n",     cpu_perf_get(SPR_PCER_WBRANCH));
  printf("Perf WBRANCH_CYC: %d\n", cpu_perf_get(SPR_PCER_WBRANCH_CYC));
  printf("Perf LD: %d\n",          cpu_perf_get(SPR_PCER_LD));
  printf("Perf ST: %d\n",          cpu_perf_get(SPR_PCER_ST));
  printf("Perf JUMP: %d\n",        cpu_perf_get(SPR_PCER_JUMP));
  printf("Perf BRANCH: %d\n",      cpu_perf_get(SPR_PCER_BRANCH));
  printf("Perf DELAY NOP: %d\n",   cpu_perf_get(SPR_PCER_DELAY_NOP));
#endif
}

// use weak attribute here, so we can overwrite this function to provide custom exception handlers, e.g. for tests
//__attribute__((interrupt))
__attribute__((weak))
void illegal_insn_handler_c(void)
{
  unsigned int exception_address, insn;
#ifdef __riscv__
  asm("csrr %0, 0x341" : "=r" (exception_address) : );
#else
  exception_address = mfspr(SPR_EPCR_BASE);
#endif
  insn = *((unsigned int*)(exception_address));
  printf("Illegal instruction encountered at address 0x%08X: %X\n", exception_address, insn);
  eoc(0);
}
