#include "utils.h"
#include <stdio.h>
#include "bar.h"
#include "bench.h"

/* Bubble sort code */

#define N 100

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

  for (j = 0; j<2; j++) {

    for (i=0;i<n;i++)
      array[i] = 2*n-i;

    bubbleSort(array, n);

  }

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
