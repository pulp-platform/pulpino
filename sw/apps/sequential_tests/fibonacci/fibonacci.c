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

#include "bench.h"
#include "utils.h"
#include "string_lib.h"
#include "bar.h"

/* Fibonacci Series c language */


#define N 10
const int RESULT_FIB[N] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};

void check(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "fibonacci",   .test = check },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}

int recFib(int);
void fib(int*, int);
int array[N];

void check(testresult_t *result, void (*start)(), void (*stop)()) {
  int i=0,j, c;
  int n=N;
  printf("Start fibonacci\n");

  start();
    fib(array, n);
  stop();

  for (i=0;i<n;i++) {
    if (RESULT_FIB[i] != array[i]) {
      printf("expected: %d, actual: %d\n",RESULT_FIB[i],array[i]);
      result->errors++;
    }
  }

}

int recFib(int n)
{
   if ( n == 0 )
      return 0;
   else if ( n == 1 )
      return 1;
   else
      return (recFib(n-1) + recFib(n-2));
}

void fib(int* array, int n)
{
  int i=0;
  int c;

  for (c=0; c<=(n-1); c++ )
    {
      array[c] = recFib(i);
      i++;
    }
}
