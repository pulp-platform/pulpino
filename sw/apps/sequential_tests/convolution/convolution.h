// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#ifndef _CONF_HEADER_
#define _CONF_HEADER_

#include "config.h" //generate by matlab

#if DATA_TYPE == 8

    typedef signed char      Filtc;
    typedef signed char      Pixel;
    typedef signed char      FiltcV     __attribute__((vector_size (4)));
    typedef signed char      PixelV     __attribute__((vector_size (4)));
 #ifdef DOTP
    #define sumdotp(a, b, c)            __builtin_pulp_sdotsp4(a, b, c)
    #define dotp(a, b)                  __builtin_pulp_dotsp4(a, b)
    #define shuffle(a, b, c)            __builtin_pulp_shuffle2b(a, b, c)
 #endif
#else

    typedef signed short      Filtc;
    typedef signed short      Pixel;
    typedef signed short      FiltcV    __attribute__((vector_size (4)));
    typedef signed short      PixelV    __attribute__((vector_size (4)));
 #ifdef DOTP
    #define sumdotp(a, b, c)            __builtin_pulp_sdotsp2(a, b, c)
    #define dotp(a, b)                  __builtin_pulp_dotsp2(a, b)
    #define shuffle(a, b, c)            __builtin_pulp_shuffle2h(a, b, c)
 #endif
#endif

#include "data_image.h" //generate by matlab

#define clipu(a, zero, max)         __builtin_pulp_clipu(a, zero, max)
#define addnr(S, Norm, Round)       __builtin_pulp_addRN(S, 0, Norm, Round);

void __attribute__ ((noinline))  Conv3x3_Scalar     (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  Conv3x3_Vector     (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  Conv5x5_Scalar     (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  Conv5x5_Vector     (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);

void __attribute__ ((noinline))  perf_enable_id         (int eventid, char all);

void __attribute__ ((noinline))  InitData               (Pixel * __restrict__ Img,    int size);
void __attribute__ ((noinline))  InitZero               (Pixel * __restrict__ Img,    int size);
void __attribute__ ((noinline))  InitKernel             (Filtc * __restrict__ Kernel, int size);

int  __attribute__ ((noinline))  checkresult            (Pixel * __restrict__ Out, Pixel * __restrict__ OutGold, int N);

/*  Profiling */
/* #define SPR_PCER_CYCLES      0 */
/* #define SPR_PCER_INSTR       1 */
/* #define SPR_PCER_LD_STALL    2 */
/* #define SPR_PCER_JMP_STALL   3 */
/* #define SPR_PCER_IMISS       4 */
/* #define SPR_PCER_WBRANCH     5 */
/* #define SPR_PCER_WBRANCH_CYC 6 */
/* #define SPR_PCER_LD          7 */
/* #define SPR_PCER_ST          8 */
/* #define SPR_PCER_JUMP        9 */
/* #define SPR_PCER_BRANCH      10 */
/* #define SPR_PCER_DELAY_NOP   11 */
/* #define SPR_PCER_LD_EXT      12 */
/* #define SPR_PCER_ST_EXT      13 */
/* #define SPR_PCER_LD_EXT_CYC  14 */
/* #define SPR_PCER_ST_EXT_CYC  15 */
/* #define SPR_PCER_TCDM_CONT   16 */

// define an event you want to profile
#define EVENT_ID 1


#endif