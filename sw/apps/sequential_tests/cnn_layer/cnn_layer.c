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
#include "bar.h"
#include "timer.h"
#include "string_lib.h"
#include "cnn_layer.h"

void check_CNN_5x5_Scalar          (testresult_t *result, void (*start)(), void (*stop)());
void check_CNN_5x5_Vector          (testresult_t *result, void (*start)(), void (*stop)());
void check_CNN_5x5_Scalar_Fused    (testresult_t *result, void (*start)(), void (*stop)());
void check_CNN_5x5_Vector_Fused    (testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "CNN_5x5_Scalar"        , .test = check_CNN_5x5_Scalar        },
#ifdef VEC
  { .name = "CNN_5x5_Vector"        , .test = check_CNN_5x5_Vector        },
#endif
  { .name = "CNN_5x5_Scalar_Fused"  , .test = check_CNN_5x5_Scalar_Fused  },
#ifdef VEC
  { .name = "CNN_5x5_Vector_Fused"  , .test = check_CNN_5x5_Vector_Fused  },
#endif
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

static Pixel __attribute__ ((section(".heapsram")))  Out[OUT_DIM];
static Pixel __attribute__ ((section(".heapsram")))  In[IMG_DIM];
static Filtc __attribute__ ((section(".heapsram")))  Kernel[FILT_DIM];



void check_CNN_5x5_Scalar(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  Filtc Kernel5x5_Scalar[FILT_DIM];
  Pixel In[IMG_DIM];


  printf("CNN WINDOW=%d, DATA_WIDTH=%d\n",FILT_WIN,DATA_WIDTH);
  InitKernel(Kernel5x5_Scalar,FILT_WIN);
  InitData(In, IMG_DIM);
  InitZero(Out, OUT_DIM);

#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif

  start();
  CNN_layer_Scalar(In, Out, IMG_ROW, IMG_COL, Kernel5x5_Scalar);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif

  result->errors = checkresult(Out, Gold_Out_Img, OUT_DIM);
}

void check_CNN_5x5_Vector(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  Filtc Kernel5x5_Vector[FILT_DIM];
  Pixel In[IMG_DIM];


  printf("CNN WINDOW=%d, DATA_WIDTH=%d\n",FILT_WIN,DATA_WIDTH);
  InitKernel(Kernel5x5_Vector,FILT_WIN);
  InitData(In, IMG_DIM);
  InitZero(Out, OUT_DIM);

#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif

  start();
  CNN_layer_Vector(In, Out, IMG_ROW, IMG_COL, Kernel5x5_Vector);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif

  result->errors = checkresult(Out, Gold_Out_Img, OUT_DIM);
}

void check_CNN_5x5_Scalar_Fused(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  Filtc Kernel5x5_Scalar[FILT_DIM];
  Pixel In[IMG_DIM];


  printf("CNN WINDOW=%d, DATA_WIDTH=%d\n",FILT_WIN,DATA_WIDTH);
  InitKernel(Kernel5x5_Scalar,FILT_WIN);
  InitData(In, IMG_DIM);
  InitZero(Out, OUT_DIM);

#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif

  start();
  CNN_layer_Scalar_Fused(In, Out, IMG_ROW, IMG_COL, Kernel5x5_Scalar);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif

  result->errors = checkresult(Out, Gold_Out_Img, OUT_DIM);
}

void check_CNN_5x5_Vector_Fused(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  Filtc Kernel5x5_Vector[FILT_DIM];
  Pixel In[IMG_DIM];


  printf("CNN WINDOW=%d, DATA_WIDTH=%d\n",FILT_WIN,DATA_WIDTH);
  InitKernel(Kernel5x5_Vector,FILT_WIN);
  InitData(In, IMG_DIM);
  InitZero(Out, OUT_DIM);

#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif

  start();
  CNN_layer_Vector_Fused(In, Out, IMG_ROW, IMG_COL, Kernel5x5_Vector);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif

  result->errors = checkresult(Out, Gold_Out_Img, OUT_DIM);
}

// load kernel
void __attribute__ ((noinline)) InitKernel(Filtc * __restrict__ Kernel, int size)
{
  int i;
  int n = size*size;
  for (i=0; i < n; i++) {
      Kernel[i] = Filter_Kern[i];
  }
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


void perf_enable_id( int eventid){
  cpu_perf_conf_events(SPR_PCER_EVENT_MASK(eventid));
  cpu_perf_conf(SPR_PCMR_ACTIVE | SPR_PCMR_SATURATE);
};
#define CNN_DEBUG
int  __attribute__ ((noinline)) checkresult(Pixel * __restrict__ Out, Pixel * __restrict__ OutGold, int N)
{
  int i;
  int err = 0;

  for (i = 0; i<N; i++) {
    if (Out[i]!=OutGold[i]) {
#ifdef CNN_DEBUG
      printf("At index %d: Actual value: %d: Expected: %d\n", i, Out[i],  OutGold[i]);
#endif
      err++;
    }
  }
  return err;
}
