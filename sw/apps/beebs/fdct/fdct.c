/* Forward discrete cosine transform

   Copyright (C) 2014 Embecosm Limited

   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file is part of BEEBS

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* The original version of this program is part of the Worst-Case Execution
   Time (WCET) analysis project at Mälardalen Real-Time Research Center
   (MRTC).  The benchmarks are freely available but without any explicit
   licensing.

   This version, modified by Embecosm to be robust on 16-bit architectures and
   compliant with the C99 standard is licensed explicitly under the GPL to
   enforce those freedoems explicitly. */

/******************************************************************************
				    FDCT.C

   Forward Discrete Cosine Transform

   Used on 8x8 image blocks to reassemble blocks in order to ease quantization
   compressing image information on the more significant frequency components

   Expected Result -> short int block[64] =
     { 699,164,-51,-16, 31,-15,-19,  8,
        71, 14,-61, -2, 11,-12,  7, 12,
       -58,-55, 13, 28,-20, -7, 14,-18,
        29, 22,  3,  3,-11,  7, 11,-22,
        -1,-28,-27, 10,  0, -7, 11,  6,
         7,  6, 21, 21,-10, -8,  2,-14,
         1, -7,-15,-15,-10, 15, 16,-10,
         0, -1,  0, 15,  4,-13, -5,  4 };

   Hexadecimal results: Block ->

     02bb00a4 ffcdfff0 001ffff1 ffed0008 0047000e ffc3fffe 000bfff4 0007000c
     ffc6ffc9 000d001c ffecfff9 000effee 001d0016 00030003 fff50007 000bffea
     ffffffe4 ffe5000a 0000fff9 000b0006 00070006 00150015 fff6fff8 0002fff2
     0001fff9 fff1fff1 fff6000f 0010fff6 0000ffff 0000000f 0004fff3 fffb0004

    Number of clock cycles (with these inputs) -> 2132
*******************************************************************************/

#define NULL 0

#include "support.h"

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)

#ifdef IO
#include "libp.c"
#include "arith.c"
#include "string.c"
#endif

/* Cosine Transform Coefficients */

#define W1 2841                 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676                 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408                 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609                 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108                 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565                  /* 2048*sqrt(2)*cos(7*pi/16) */

/* Other FDCT Parameters */
#define CONST_BITS  13
#define PASS1_BITS  2

int out;

/* Image block to be transformed: */
short int block[64] =
  {  99, 104, 109, 113, 115, 115,  55,  55,
    104, 111, 113, 116, 119,  56,  56,  56,
    110, 115, 120, 119, 118,  56,  56,  56,
    119, 121, 122, 120, 120,  59,  59,  59,
    119, 120, 121, 122, 122,  55,  55,  55,
    121, 121, 121, 121,  60,  57,  57,  57,
    122, 122,  61,  63,  62,  57,  57,  57,
     62,  62,  61,  61,  63,  58,  58,  58 };

/* Fast Discrete Cosine Transform */

void fdct(short int *blk, int lx)
{
   long int tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
   long int tmp10, tmp11, tmp12, tmp13;
   long int z1, z2, z3, z4, z5;
   int i;
   short int *block;

   long int constant;

   /* Pass 1: process rows. */
   /* Note results are scaled up by sqrt(8) compared to a true DCT; */
   /* furthermore, we scale the results by 2**PASS1_BITS. */

   block=blk;

   for (i=0; i<8; i++)
   {
      tmp0 = block[0] + block[7];
      tmp7 = block[0] - block[7];
      tmp1 = block[1] + block[6];
      tmp6 = block[1] - block[6];
      tmp2 = block[2] + block[5];
      tmp5 = block[2] - block[5];
      tmp3 = block[3] + block[4];
      tmp4 = block[3] - block[4];

      /* Even part per LL&M figure 1 --- note that published figure is faulty;
       * rotator "sqrt(2)*c1" should be "sqrt(2)*c6".
       */

      tmp10 = tmp0 + tmp3;
      tmp13 = tmp0 - tmp3;
      tmp11 = tmp1 + tmp2;
      tmp12 = tmp1 - tmp2;

      block[0] = ((tmp10+tmp11) << PASS1_BITS);
      block[4] = ((tmp10-tmp11) << PASS1_BITS);

      constant= 4433;
      z1 = (tmp12 + tmp13) * constant;
      constant= 6270;
      block[2] = (z1 + (tmp13 * constant)) >> (CONST_BITS-PASS1_BITS);
      constant= -15137;
      block[6] = (z1 + (tmp12 * constant)) >> (CONST_BITS-PASS1_BITS);

      /* Odd part per figure 8 --- note paper omits factor of sqrt(2).
       * cK represents cos(K*pi/16).
       * i0..i3 in the paper are tmp4..tmp7 here.
       */

      z1 = tmp4 + tmp7;
      z2 = tmp5 + tmp6;
      z3 = tmp4 + tmp6;
      z4 = tmp5 + tmp7;
      constant= 9633;
      z5 = ((z3 + z4) * constant); /* sqrt(2) * c3 */

      constant= 2446;
      tmp4 = (tmp4 * constant); /* sqrt(2) * (-c1+c3+c5-c7) */
      constant= 16819;
      tmp5 = (tmp5 * constant); /* sqrt(2) * ( c1+c3-c5+c7) */
      constant= 25172;
      tmp6 = (tmp6 * constant); /* sqrt(2) * ( c1+c3+c5-c7) */
      constant= 12299;
      tmp7 = (tmp7 * constant); /* sqrt(2) * ( c1+c3-c5-c7) */
      constant= -7373;
      z1 = (z1 * constant); /* sqrt(2) * (c7-c3) */
      constant= -20995;
      z2 = (z2 * constant); /* sqrt(2) * (-c1-c3) */
      constant= -16069;
      z3 = (z3 * constant); /* sqrt(2) * (-c3-c5) */
      constant= -3196;
      z4 = (z4 * constant); /* sqrt(2) * (c5-c3) */

      z3 += z5;
      z4 += z5;

      block[7] = (tmp4 + z1 + z3) >> (CONST_BITS-PASS1_BITS);
      block[5] = (tmp5 + z2 + z4) >> (CONST_BITS-PASS1_BITS);
      block[3] = (tmp6 + z2 + z3) >> (CONST_BITS-PASS1_BITS);
      block[1] = (tmp7 + z1 + z4) >> (CONST_BITS-PASS1_BITS);



      /* advance to next row */
      block += lx;

   }

   /* Pass 2: process columns. */

   block=blk;

   for (i = 0; i<8; i++)
   {
      tmp0 = block[0] + block[7*lx];
      tmp7 = block[0] - block[7*lx];
      tmp1 = block[lx] + block[6*lx];
      tmp6 = block[lx]- block[6*lx];
      tmp2 = block[2*lx] + block[5*lx];
      tmp5 = block[2*lx] - block[5*lx];
      tmp3 = block[3*lx] + block[4*lx];
      tmp4 = block[3*lx] - block[4*lx];

      /* Even part per LL&M figure 1 --- note that published figure is faulty;
       * rotator "sqrt(2)*c1" should be "sqrt(2)*c6".
       */

      tmp10 = tmp0 + tmp3;
      tmp13 = tmp0 - tmp3;
      tmp11 = tmp1 + tmp2;
      tmp12 = tmp1 - tmp2;

      block[0] = (tmp10 + tmp11) >> (PASS1_BITS+3);
      block[4*lx] = (tmp10 - tmp11) >> (PASS1_BITS+3);

      constant = 4433;
      z1 = ((tmp12 + tmp13) * constant);
      constant= 6270;
      block[2*lx] = (z1 + (tmp13 * constant)) >> (CONST_BITS+PASS1_BITS+3);
      constant=-15137;
      block[6*lx] = (z1 + (tmp12 * constant)) >> (CONST_BITS+PASS1_BITS+3);

      /* Odd part per figure 8 --- note paper omits factor of sqrt(2).
       * cK represents cos(K*pi/16).
       * i0..i3 in the paper are tmp4..tmp7 here.
       */

      z1 = tmp4 + tmp7;
      z2 = tmp5 + tmp6;
      z3 = tmp4 + tmp6;
      z4 = tmp5 + tmp7;
      constant=9633;
      z5 = ((z3 + z4) * constant); /* sqrt(2) * c3 */

      constant=2446;
      tmp4 = (tmp4 * constant); /* sqrt(2) * (-c1+c3+c5-c7) */
      constant=16819;
      tmp5 = (tmp5 * constant); /* sqrt(2) * ( c1+c3-c5+c7) */
      constant=25172;
      tmp6 = (tmp6 * constant); /* sqrt(2) * ( c1+c3+c5-c7) */
      constant=12299;
      tmp7 = (tmp7 * constant); /* sqrt(2) * ( c1+c3-c5-c7) */
      constant=-7373;
      z1 = (z1 * constant); /* sqrt(2) * (c7-c3) */
      constant= -20995;
      z2 = (z2 * constant); /* sqrt(2) * (-c1-c3) */
      constant=-16069;
      z3 = (z3 * constant); /* sqrt(2) * (-c3-c5) */
      constant=-3196;
      z4 = (z4 * constant); /* sqrt(2) * (c5-c3) */

      z3 += z5;
      z4 += z5;

      block[7*lx] = (tmp4 + z1 + z3) >> (CONST_BITS+PASS1_BITS+3);
      block[5*lx] = (tmp5 + z2 + z4) >> (CONST_BITS+PASS1_BITS+3);
      block[3*lx] = (tmp6 + z2 + z3) >> (CONST_BITS+PASS1_BITS+3);
      block[lx] =  (tmp7 + z1 + z4) >> (CONST_BITS+PASS1_BITS+3);

      /* advance to next column */
      block++;
   }
}

int benchmark()
{
  fdct(block, 8);
  return 0;
}


/* vim: set ts=3 sw=3 et: */
