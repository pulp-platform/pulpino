#include <stdio.h>
#include "timer.h"
#include "utils.h"

#include "bench.h"

#define NUM_ITER 5
static unsigned samples[NUM_ITER];

extern void test_setup();
extern void test_clear();
extern void test_run(int);
extern int test_check();

int main() {

  test_setup();
  perf_start();

  for (int i = 0; i < NUM_ITER; ++i) {
    test_clear();

    //reset_timer();
    //start_timer();

    test_run(i);

    //stop_timer();

    samples[i] = get_time();
  }

  perf_stop();
  int check = test_check();

  printf("Correct: %d\n", check);
  for (int i = 0; i < NUM_ITER; ++i)
    printf("TS[%d]: %d\n", i, samples[i]);

  perf_print_all();

  return 0;
}
