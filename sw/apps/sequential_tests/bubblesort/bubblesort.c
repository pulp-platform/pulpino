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

#include "utils.h"
#include <stdio.h>
#include "bar.h"
#include "bench.h"

/* Bubble sort code */

#define N 20

void check_bubbleSort(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "bubbleSort",          .test = check_bubbleSort           },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}

void bubbleSort(volatile int*,int);


__attribute__ ((section(".heapsram"))) int array[N];

void check_bubbleSort(testresult_t *result, void (*start)(), void (*stop)()) {
  int i,j;
  int n = N;

  printf("Start bubblesort\n");

  start();

  for (i=0;i<n;i++)
    array[i] = 2*n-i;

  bubbleSort(array, n);

  stop();

  for (i=0;i<n;i++) {
    if (array[N-1-i] != 2*n-i) {
      printf("expected: %d, actual: %d\n",2*n-i,array[N-1-i]);
      result->errors++;
    }
  }
}


void bubbleSort(volatile int* array, int n)
{
  int c,d;
  int swap;

  for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (array[d] > array[d+1]) /* For decreasing order use < */
      {
        swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
}
