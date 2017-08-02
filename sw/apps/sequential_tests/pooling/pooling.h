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

#include "config.h"

typedef signed short      Pixel;
typedef signed short      PixelV    __attribute__((vector_size (4)));

#ifdef VEC
    #define shuffle(a, b, c)            __builtin_pulp_shuffle2h(a, b, c)
    #define max_vec(a, b)               __builtin_pulp_max2(a, b)
#endif

void __attribute__ ((noinline)) Pool2x2_Scalar (Pixel * In_Img, Pixel * Out_Img, int R, int C);
void __attribute__ ((noinline)) Pool2x2_Vector (Pixel * In_Img, Pixel * Out_Img, int R, int C);

void __attribute__ ((noinline))  InitData               (Pixel * __restrict__ Img,    int size);
void __attribute__ ((noinline))  InitZero               (Pixel * __restrict__ Img,    int size);

int  __attribute__ ((noinline))  checkresult            (Pixel * __restrict__ Out, Pixel * __restrict__ OutGold, int N);
void print_image(Pixel* image, int N, int M);

#include "data.h"

#endif
