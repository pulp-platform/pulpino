// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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
