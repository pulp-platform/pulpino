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
#ifndef _CONF_HEADER_
#define _CONF_HEADER_

#include "config.h" //generate by matlab

typedef signed short      Filtc;
typedef signed short      Pixel;
typedef signed short      FiltcV    __attribute__((vector_size (4)));
typedef signed short      PixelV    __attribute__((vector_size (4)));

#ifdef VEC
    #define sumdotp(a, b, c)            __builtin_pulp_sdotsp2(a, b, c)
    #define dotp(a, b)                  __builtin_pulp_dotsp2(a, b)
    #define shuffle(a, b, c)            __builtin_pulp_shuffle2h(a, b, c)
    #define max_vec(a, b)               __builtin_pulp_max2(a, b)
    #define addnr(S, Norm, Round)       __builtin_pulp_addRN(S, 0, Norm, Round);
#endif

#include "data_image.h" //generate by matlab

void __attribute__ ((noinline))  CNN_layer_Scalar       (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  CNN_layer_Scalar_Fused (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);

void __attribute__ ((noinline))  CNN_layer_Vector       (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  CNN_layer_Vector_Fused (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);

void __attribute__ ((noinline))  Conv5x5_Scalar         (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  Conv5x5_Vector         (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);

void __attribute__ ((noinline))  Pool2x2                (Pixel * In_Img, Pixel * Out_Img, int R, int C);


void __attribute__ ((noinline))  perf_enable_id         (int eventid);

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