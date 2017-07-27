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
#include "pooling.h"
#include <stdio.h>

void check_Pool2x2_Scalar          (testresult_t *result, void (*start)(), void (*stop)());
void check_Pool2x2_Vector          (testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
#ifdef VEC
  { .name = "Pool2x2_Vector"    , .test = check_Pool2x2_Vector    },
#endif
  { .name = "Pool2x2_Scalar"    , .test = check_Pool2x2_Scalar    },
  {0, 0}
};

int main()
{
  int errors;
  if(get_core_id() == 0){
    errors = run_suite(testcases);
  }
  synch_barrier();

  return errors;
}

#define POOLING_DEBUG
static Pixel  __attribute__ ((section(".heapsram")))  Out[OUT_DIM];
static Pixel  __attribute__ ((section(".heapsram")))  In[IMG_DIM];

void check_Pool2x2_Scalar(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  printf("2x2 Pooling\n");
  InitData(In, IMG_DIM);
  InitZero(Out, IMG_DIM/4);

  start();
  Pool2x2_Scalar(In, Out, IMG_ROW, IMG_COL);
  stop();
  result->errors = checkresult(Out, Gold_Out_Img, OUT_DIM);
}

void check_Pool2x2_Vector(testresult_t *result, void (*start)(), void (*stop)()) {

  printf("2x2 Pooling\n");
  InitData(In, IMG_DIM);
  InitZero(Out, IMG_DIM/4);

  start();
  Pool2x2_Vector(In, Out, IMG_ROW, IMG_COL);
  stop();
  result->errors = checkresult(Out, Gold_Out_Img, OUT_DIM);
}

// load input img
void __attribute__ ((noinline)) InitData(Pixel * __restrict__ Img, int size)
{
  int i;

  for (i=0; i < size; i++) 
      Img[i] = In_Img[i];

}

// load initialize out to 0
void __attribute__ ((noinline)) InitZero(Pixel * __restrict__ Img, int size)
{
  int i;

  for (i=0; i < size; i++) 
      Img[i] = 0;

}

int  __attribute__ ((noinline)) checkresult(Pixel * __restrict__ Out, Pixel * __restrict__ OutGold, int N)
{
  int i;
  int err = 0;

  for (i = 0; i<N; i++) {
    if (Out[i]!=OutGold[i]) {
#ifdef POOLING_DEBUG
      printf("At index %d: Actual value (%x): %d: Expected: %d\n", i, &Out[i], Out[i],  OutGold[i]);
#endif
      err++;
    }
  }
  return err;
}

void print_image(Pixel* image, int N, int M) {
  for (int y = 0; y < M; y++) {
    for (int x = 0; x < N; x++)
      printf("%3x", (Pixel) image[M * y + x]);

    printf("\n");
  }

  printf("\n");
}

