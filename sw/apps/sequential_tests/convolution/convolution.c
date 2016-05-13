#include "bench.h"
#include "utils.h"
#include "bar.h"
#include "timer.h"
#include "string_lib.h"

#include "convolution.h"

void check_Conv5x5_Byte            (testresult_t *result, void (*start)(), void (*stop)());
void check_Conv5x5_Byte_Scalar     (testresult_t *result, void (*start)(), void (*stop)());
void check_Conv3x3_Byte            (testresult_t *result, void (*start)(), void (*stop)());
void check_Conv3x3_Byte_Scalar     (testresult_t *result, void (*start)(), void (*stop)());


testcase_t testcases[] = {
  { .name = "Conv5x5_Byte_Scalar"    , .test = check_Conv5x5_Byte_Scalar    },
  { .name = "Conv5x5_Byte"           , .test = check_Conv5x5_Byte           },
  { .name = "Conv3x3_Byte_Scalar"    , .test = check_Conv3x3_Byte_Scalar    },
  { .name = "Conv3x3_Byte"           , .test = check_Conv3x3_Byte           },
  {0, 0}
};

int main()
{
   
  run_suite(testcases);

  synch_barrier();

  return 0;
}

void check_Conv5x5_Byte(testresult_t *result, void (*start)(), void (*stop)()) {


  // start benchmark
  FiltcV Kernel5x5[7];
  Filtc In[LINE*(COL+4)];

  InitKernelVect(Kernel5x5,5);
  InitData(In, Out+get_core_id()*COL*LINE, COL, LINE);
#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif
  start();
  Conv5x5_Byte(In, Out+get_core_id()*COL*LINE, COL, LINE, Kernel5x5);
  stop();
#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif
  result->errors = checkRes(1 ,Out+get_core_id()*COL*LINE, COL, LINE);

  synch_barrier();

}

void check_Conv5x5_Byte_Scalar(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  Filtc Kernel5x5_Scalar[25];
  Filtc In[LINE*(COL+4)];

  InitKernelScalar(Kernel5x5_Scalar,5);
  InitData(In, Out+get_core_id()*COL*LINE, COL, LINE);
#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif
      
  start();
  Conv5x5_Byte_Scalar(In, Out+get_core_id()*COL*LINE, COL, LINE, Kernel5x5_Scalar);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif
  result->errors = checkRes(1 ,Out+get_core_id()*COL*LINE, COL, LINE);

synch_barrier();

}

void check_Conv3x3_Byte(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  FiltcV Kernel3x3[3];
  Filtc In[LINE*(COL+4)];
 
  InitKernelVect2(Kernel3x3,3);
  InitData(In, Out+get_core_id()*COL*LINE, COL, LINE);
#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif

  start();
  Conv3x3_Byte(In, Out+get_core_id()*COL*LINE, COL, LINE, Kernel3x3);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif
  result->errors = checkRes(0 ,Out+get_core_id()*COL*LINE, COL, LINE);
  synch_barrier();

}    
    
void check_Conv3x3_Byte_Scalar(testresult_t *result, void (*start)(), void (*stop)()) {

  // start benchmark
  Filtc Kernel3x3_Scalar[9];
  Filtc In[LINE*(COL+4)];

  InitKernelScalar(Kernel3x3_Scalar,3);
  InitData(In, Out+get_core_id()*COL*LINE, COL, LINE);
#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif

  start();
  Conv3x3_Byte_Scalar(In, Out+get_core_id()*COL*LINE, COL, LINE, Kernel3x3_Scalar);
  stop();

#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif
  result->errors = checkRes(0 ,Out+get_core_id()*COL*LINE, COL, LINE);
  synch_barrier();

}

void perf_enable_id( int eventid){
  cpu_perf_conf_events(SPR_PCER_EVENT_MASK(eventid));
  cpu_perf_conf(SPR_PCMR_ACTIVE | SPR_PCMR_SATURATE);
};


// initialize data with pseudo random data
void __attribute__ ((noinline)) InitData(Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M)
{
  int i;

  for (i=0; i<(N*(M+4)); i++) {
    if (i<N*4)
      In[i]  = (Filtc)i;
    else if (i<N*15)
      In[i]  = (Filtc)5;
    else if (i<N*15+17)
      In[i]  = (Filtc)1;
    else
      In[i]  = (Filtc)20;
  }
  for (i=0; i<(N*(M)); i++) {
    Out[i] = (Filtc)0;
  }
}

// load vectorial kernel to TCDM
void __attribute__ ((noinline)) InitKernelVect(FiltcV * __restrict__ Out, int size)
{
  int i;

  if (size == 5)
    for (i=0; i<7; i++) {
      Out[i] = Kernel5x5_init[i];
    }
  else
    for (i=0; i<3; i++) {
      Out[i] = Kernel3x3_init[i];
    } 
}

// load vectorial kernel to TCDM
void __attribute__ ((noinline)) InitKernelVect2(FiltcV * __restrict__ Out, int size)
{
  int i;

  if (size == 5)
    for (i=0; i<7; i++) {
      Out[i] = Kernel5x5_init2[i];
    }
  else
    for (i=0; i<3; i++) {
      Out[i] = Kernel3x3_init2[i];
    } 
}

// load kernel to TCDM
void __attribute__ ((noinline)) InitKernelScalar(Filtc * __restrict__ Out, int size)
{
  int i;

  if (size == 5) {
    for (i=0; i<25; i++) {
      Out[i] = Kernel5x5_Scalar_init[i];
    }
  }
  else {
    for (i=0; i<9; i++) {
      Out[i] = Kernel3x3_Scalar_init[i];
    }
  }
}


// checksum for 3x3 and 5x5 kernel
int CHECKSUM5x5 = 22462;
int CHECKSUM3x3 = 58954;

// check results
int checkRes(int is5x5 ,Filtc *Out, int N, int M)

{
  int checksum = 0;
  int i, j;

  for (i=0; i<M; i++) {
    for (j=0; j<N; j++) {
      checksum += (int)Out[i*M+j];
    }
  }
  if (is5x5) {
    if (CHECKSUM5x5 == checksum)
      return 0;
    else
      return 1;
  }
  else {
    if (CHECKSUM3x3 == checksum)
      return 0;
    else
      return 1;
  }
}
