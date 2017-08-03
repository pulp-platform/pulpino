// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
/*
 * This file contains some common defines, includes and helper functions for the
 * matlab generated FP kernels
 *
 */

#ifndef __ML_SHARED_H__
#define __ML_SHARED_H__

/////////////////////////////////////////////////////////
// defines
/////////////////////////////////////////////////////////
#include <stdio.h>
#include "math_fns.h"
#ifndef LINUX
#include "utils.h"
#include "bar.h"
#include "bench.h"
#endif
// number of kernel iterations
#define KERNEL_ITS 10

// number of cores in the cluster
#define N_CORES 4



// platform dependent includes

/////////////////////////////////////////////////////////
// helper functions
/////////////////////////////////////////////////////////
#ifdef LINUX
static inline void synch_barrier(){}
static inline void perf_reset(){}
static inline void perf_stop(){}
static inline void perf_start(){}
static inline void perf_print_all(){}
static inline int cpu_perf_get(){
  return 0;}
static inline int get_core_id(){
  return 0;}
static inline void init_fp_regs(){}



#else




#endif

// prints 8 digits...
void printFloat(float f)
{
  int j = 100000000;
  int i = j;
  float fi;
  float fj;
  char sign;

  if(fIsNan(f))
  {
    printf("NaN\t");
    return;
  }

  sign = (f<0.0F)?'-':'+';
  
  if(fIsInf(f))
  {
    printf("%cInf\t",sign);
    return;
  }

  f    = fAbs(f);
  fi   = (float) i;
  fj   = (float)((int) f);
  i    = (int) (fi * (f - fj));
  
  // fix in case the cast behaved as a ceil() instead of floor()
  // can happen in the LNS case since we employ faithful rounding...
  if (i < 0)
  {
    i += j;
    j  = ((int)f) - 1;
  }
  else
  {
    j = (int)f;
  }

  printf("%c%8d.%d",sign,j,i);
  return;
}

#ifndef LINUX
static inline void mem_fp_cpy(float * dest, float * src, int size){
  float tmp;
  for (int k=0;k<size;k++) {
    asm volatile ("flw %[ld],0x0(%[ad])": [ld] "=f"(tmp) : [ad] "r" (src));
    asm volatile ("fsw %[st],0x0(%[ad])": [st] "=f"(tmp) : [ad] "r" (dest));
    src++;
    dest++;
  }
}

static inline void mem_fp_set(float * dest, int size){
  float tmp = 0.0F;
  for (int k=0;k<size;k++) {
    asm volatile ("fsw %[st],0x0(%[ad])": [st] "=f"(tmp) : [ad] "r" (dest));
    dest++;
  }
}
static inline void init_fp_regs(void) {
  asm volatile ("fmv.s.x f0, x0");
  asm volatile ("fmv.s.x f1, x0");
  asm volatile ("fmv.s.x f2, x0");
  asm volatile ("fmv.s.x f3, x0");
  asm volatile ("fmv.s.x f4, x0");
  asm volatile ("fmv.s.x f5, x0");
  asm volatile ("fmv.s.x f6, x0");
  asm volatile ("fmv.s.x f7, x0");
  asm volatile ("fmv.s.x f8, x0");
  asm volatile ("fmv.s.x f9, x0");
  asm volatile ("fmv.s.x f10, x0");
  asm volatile ("fmv.s.x f11, x0");
  asm volatile ("fmv.s.x f12, x0");
  asm volatile ("fmv.s.x f13, x0");
  asm volatile ("fmv.s.x f14, x0");
  asm volatile ("fmv.s.x f15, x0");
  asm volatile ("fmv.s.x f16, x0");
  asm volatile ("fmv.s.x f17, x0");
  asm volatile ("fmv.s.x f18, x0");
  asm volatile ("fmv.s.x f19, x0");
  asm volatile ("fmv.s.x f20, x0");
  asm volatile ("fmv.s.x f21, x0");
  asm volatile ("fmv.s.x f22, x0");
  asm volatile ("fmv.s.x f23, x0");
  asm volatile ("fmv.s.x f24, x0");
  asm volatile ("fmv.s.x f25, x0");
  asm volatile ("fmv.s.x f26, x0");
  asm volatile ("fmv.s.x f27, x0");
  asm volatile ("fmv.s.x f28, x0");
  asm volatile ("fmv.s.x f29, x0");
  asm volatile ("fmv.s.x f30, x0");
  asm volatile ("fmv.s.x f31, x0");
}
#endif
static inline int getKernelIts(void)
{
#ifdef TESTER
  //  *(volatile int*)(0x10003fe8) = 1;
  int kernel_its = *(volatile int*)(0x10003fe8);
  return kernel_its;
#else
  return (int)KERNEL_ITS;
#endif
}

static inline void flagPassFail(unsigned char pass, int coreid)
{
  if(!pass)
  {
    printf("kernel did not pass on core %d !!\n",coreid);
  }
  else
  {
    printf("kernel passed on core  %d\n",coreid);
  }
  return;
}

static inline void cycleCount()
{
  printf("Kernel completed in %d cycles!\n",cpu_perf_get(0));
}

static inline void printErrors(unsigned char error, int id, float is, float exp_hi, float exp_lo)
{
  int coreid=get_core_id();
  if (error) {
    printf("Error occured!\n");
    printf("ID: %d \t", id*4+coreid);
    printFloat(exp_lo);
    printf("\t < ");
    printFloat(is);
    printf("\t < ");
    printFloat(exp_hi);
    printf("\n");
  }
}

//#define VCD_TRIGGER
static inline void perf_begin() {
  if(get_core_id() == 0) {
#ifdef VCD_TRIGGER
    set_pin_function(PIN_CAM_I2S_SDI1+2, FUNC_GPIO);
    set_gpio_pin_direction(PIN_CAM_I2S_SDI1+1, DIR_OUT);
    set_gpio_pin_value(PIN_CAM_I2S_SDI1+1, 0);
#endif
    perf_reset();
    perf_start();
#ifdef VCD_TRIGGER
    set_gpio_pin_value(PIN_CAM_I2S_SDI1+1, 1);
#endif
  }
}

static inline void perf_end() {
  if(get_core_id() == 0) {
#ifdef VCD_TRIGGER
    set_gpio_pin_value(PIN_CAM_I2S_SDI1+1, 0);
#endif
    perf_stop();
    cycleCount();
    perf_print_all();
  }
}

/////////////////////////////////////////////////////////
// matlab coder specifics
/////////////////////////////////////////////////////////


#ifndef __TMWTYPES__
#define __TMWTYPES__

/*=======================================================================*
 * Target hardware information
 *   Device type: Generic->MATLAB Host Computer
 *   Number of bits:     char:   8    short:   16    int:  32
 *                       long:  64    long long:  64
 *                       native word size:  64
 *   Byte ordering: LittleEndian
 *   Signed integer division rounds to: Zero
 *   Shift right on a signed integer as arithmetic shift: on
 *=======================================================================*/

/*=======================================================================*
 * Fixed width word size data types:                                     *
 *   int8_T, int16_T, int32_T     - signed 8, 16, or 32 bit integers     *
 *   uint8_T, uint16_T, uint32_T  - unsigned 8, 16, or 32 bit integers   *
 *   real32_T, real64_T           - 32 and 64 bit floating point numbers *
 *=======================================================================*/
typedef signed char int8_T;
typedef unsigned char uint8_T;
typedef short int16_T;
typedef unsigned short uint16_T;
typedef int int32_T;
typedef unsigned int uint32_T;
typedef long int64_T;
typedef unsigned long uint64_T;
typedef float real32_T;
typedef double real64_T;

/*===========================================================================*
 * Generic type definitions: real_T, time_T, boolean_T, int_T, uint_T,       *
 *                           ulong_T, ulonglong_T, char_T and byte_T.        *
 *===========================================================================*/
typedef double real_T;
typedef double time_T;
typedef unsigned char boolean_T;
typedef int int_T;
typedef unsigned int uint_T;
typedef unsigned long ulong_T;
typedef unsigned long long ulonglong_T;
typedef char char_T;
typedef char_T byte_T;

/*===========================================================================*
 * Complex number type definitions                                           *
 *===========================================================================*/
#define CREAL_T

typedef struct {
  real32_T re;
  real32_T im;
} creal32_T;

typedef struct {
  real64_T re;
  real64_T im;
} creal64_T;

typedef struct {
  real_T re;
  real_T im;
} creal_T;

typedef struct {
  int8_T re;
  int8_T im;
} cint8_T;

typedef struct {
  uint8_T re;
  uint8_T im;
} cuint8_T;

typedef struct {
  int16_T re;
  int16_T im;
} cint16_T;

typedef struct {
  uint16_T re;
  uint16_T im;
} cuint16_T;

typedef struct {
  int32_T re;
  int32_T im;
} cint32_T;

typedef struct {
  uint32_T re;
  uint32_T im;
} cuint32_T;

typedef struct {
  int64_T re;
  int64_T im;
} cint64_T;

typedef struct {
  uint64_T re;
  uint64_T im;
} cuint64_T;

/*=======================================================================*
 * Min and Max:                                                          *
 *   int8_T, int16_T, int32_T     - signed 8, 16, or 32 bit integers     *
 *   uint8_T, uint16_T, uint32_T  - unsigned 8, 16, or 32 bit integers   *
 *=======================================================================*/
#define MAX_int8_T                     ((int8_T)(127))
#define MIN_int8_T                     ((int8_T)(-128))
#define MAX_uint8_T                    ((uint8_T)(255))
#define MIN_uint8_T                    ((uint8_T)(0))
#define MAX_int16_T                    ((int16_T)(32767))
#define MIN_int16_T                    ((int16_T)(-32768))
#define MAX_uint16_T                   ((uint16_T)(65535))
#define MIN_uint16_T                   ((uint16_T)(0))
#define MAX_int32_T                    ((int32_T)(2147483647))
#define MIN_int32_T                    ((int32_T)(-2147483647-1))
#define MAX_uint32_T                   ((uint32_T)(0xFFFFFFFFU))
#define MIN_uint32_T                   ((uint32_T)(0))
#define MAX_int64_T                    ((int64_T)(9223372036854775807L))
#define MIN_int64_T                    ((int64_T)(-9223372036854775807L-1L))
#define MAX_uint64_T                   ((uint64_T)(0xFFFFFFFFFFFFFFFFUL))
#define MIN_uint64_T                   ((uint64_T)(0UL))

/* Logical type definitions */
#if !defined(__cplusplus) && !defined(__true_false_are_keywords)
#  ifndef false
#   define false                       (0U)
#  endif

#  ifndef true
#   define true                        (1U)
#  endif
#endif

/*
 * Maximum length of a MATLAB identifier (function/variable)
 * including the null-termination character. Referenced by
 * rt_logging.c and rt_matrx.c.
 */
#define TMW_NAME_LENGTH_MAX            64
#endif
#endif


//boolean_T rtIsInfF(real32_T value);
/* static inline boolean_T rtIsNaNF(real32_T value) */
/* { */
/*   return (value!=value)? 1U:0U; */
/* } */

/*
 * File trailer for rtwtypes.h
 *
 * [EOF]
 */
