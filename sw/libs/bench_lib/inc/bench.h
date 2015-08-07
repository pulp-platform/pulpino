#ifndef BENCH_H
#define BENCH_H

#include "string_lib.h"
#include "bar.h"
#include "timer.h"
#include <stdint.h>

typedef struct _testresult_t {
  int time;
  int errors;
} testresult_t;

typedef struct _testcase_t {
  int id;
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
 * @param[in] num the number of benchmarks contained in \a tests.
 */
void run_suite(testcase_t *tests, size_t num);

/**
 * @brief Checks if actual == expected and if not, prints fail_msg and
 * increases the error counter in the testresult struct.
 * This function should be used when a fine grained error reporting is needed
 */
void check_uint32(testresult_t* result, const char* fail_msg, uint32_t actual, uint32_t expected);

#endif
