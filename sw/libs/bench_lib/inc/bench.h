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

#ifndef BENCH_H
#define BENCH_H

#include "string_lib.h"
#include "bar.h"
#include "timer.h"
#include <stdint.h>
#include "cpu_hal.h"

typedef struct _testresult_t {
  int time;
  int errors;
} testresult_t;

typedef struct _testcase_t {
  char *name;
  void (*test)(testresult_t* result, void (*start)(), void (*stop)());
} testcase_t;

/**
 * @brief Prints the information and the result (success/fail, time elapsed)
 * of a benchmark.
 * @param[in] test the test.
 * @param[in] result the result of the test.
 */
void print_result(testcase_t *test, testresult_t *result);

void print_summary(unsigned int errors);

/**
 * @brief Runs a single benchmark.
 * @param[in] test the benchmark to run.
 * @param[out] result the result of the benchmark.
 */
void run_benchmark(testcase_t *test, testresult_t *result);


/**
 * @brief Runs a series of benchmarks and prints the results.
 * @param[in] tests an array with the benchmarks to run.
 */
unsigned int run_suite(testcase_t *tests);

/**
 * @brief Checks if actual == expected and if not, prints fail_msg and
 * increases the error counter in the testresult struct.
 * This function should be used when a fine grained error reporting is needed
 */
void check_uint32(testresult_t* result, const char* fail_msg, uint32_t actual, uint32_t expected);

/**
 * @brief Starts all performance counters
 */
static inline void perf_start(void) {
  cpu_perf_conf_events(SPR_PCER_ALL_EVENTS_MASK);
  cpu_perf_conf(SPR_PCMR_ACTIVE | SPR_PCMR_SATURATE);
}

/**
 * @brief Stops all performance counters
 */
static inline void perf_stop(void) {
  cpu_perf_conf(0);
}

/**
 * @brief Resets all performance counters to 0 without stopping them
 */
static inline void perf_reset(void) {
  cpu_perf_setall(0);
}

/**
 * @brief Prints all performance counters
 */
void perf_print_all(void);

#endif
