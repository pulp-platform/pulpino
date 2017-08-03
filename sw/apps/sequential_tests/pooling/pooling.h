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
