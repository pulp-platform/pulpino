#include "bench.h"
#include "utils.h"
#include "bar.h"
#include "timer.h"
#include "string_lib.h"

#include "convolution.h"

void check_Conv3x3_Byte_Scalar          (testresult_t *result, void (*start)(), void (*stop)());
void check_Conv3x3_Byte_Vector          (testresult_t *result, void (*start)(), void (*stop)());
void check_Conv5x5_Byte_Scalar          (testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "Conv3x3_Byte_Vector"    , .test = check_Conv3x3_Byte_Vector    },
  { .name = "Conv3x3_Byte_Scalar"    , .test = check_Conv3x3_Byte_Scalar    },
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

static Pixel __attribute__ ((section(".heapsram"))) Out[IMG_DIM];

void check_Conv3x3_Byte_Scalar(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  Filtc Kernel3x3_Scalar[FILT_DIM];
  Pixel In[IMG_DIM];

  InitKernel(Kernel3x3_Scalar,FILT_WIN);
  InitData(In, IMG_DIM);
  InitZero(Out, IMG_DIM);

#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif

  start();
  Conv3x3_Byte_Scalar(In, Out, IMG_ROW, IMG_COL, Kernel3x3_Scalar);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif

  result->errors = checkresult(Out, Gold_Out_Img, IMG_DIM);

}

void check_Conv3x3_Byte_Vector(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  Filtc Kernel3x3_Scalar[FILT_DIM];
  Pixel In[IMG_DIM];

  InitKernel(Kernel3x3_Scalar,3);
  InitData(In, IMG_DIM);
  InitZero(Out, IMG_DIM);

#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif

  start();
  Conv3x3_Byte_Vector(In, Out, IMG_ROW, IMG_COL, Kernel3x3_Scalar);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif

  result->errors = checkresult(Out, Gold_Out_Img, IMG_DIM);

}

// load kernel to TCDM
void __attribute__ ((noinline)) InitKernel(Filtc * __restrict__ Kernel, int size)
{
  int i;
  int n = size*size;
  for (i=0; i < n; i++) {
      Kernel[i] = Filter_Kern[i];
  }
}

// load input img to TCDM
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

int  __attribute__ ((noinline)) checkresult(Pixel * __restrict__ Out, Pixel * __restrict__ OutGold, int N)
{
  int i;
  int err = 0;

  for (i = 0; i<N; i++) {
    if (Out[i]!=OutGold[i]) {
#ifdef DEBUG
      printf("Is: %d: Expected: %d\n", Out[i],  OutGold[i]);
#endif
      err++;
    }
  }
  return err;
}
