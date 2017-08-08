// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "pooling.h"
#include <stdio.h>
#include "utils.h"
#include "bench.h"

void __attribute__ ((noinline)) Pool2x2_Scalar  (Pixel * In_Img, Pixel * Out_Img, int R, int C)
{
  int r, c, t0, t1,k0, k1, r0, r1, c0, c1, i;
  Pixel max0,max1;
  Pixel w0,w1,w2,w3,v0,v1,v2,v3;

  i=0;
  r0 = 0;
  r1 = 1;
  c0 = 0;
  c1 = 2;

  for (r=0; r < R; r+=2) {

    t0 = r0*R;
    t1 = r1*R;

    for (c=0; c < C>>2; c++) {

        k0 = t0 + c0;
        k1 = t1 + c0;

        w0 = In_Img[k0];
        w1 = In_Img[k0+1];
        w2 = In_Img[k1];
        w3 = In_Img[k1+1];

        k0 = t0 + c1;
        k1 = t1 + c1;

        c0+=4; c1+=4;

        v0 = In_Img[k0];
        v1 = In_Img[k0+1];
        v2 = In_Img[k1];
        v3 = In_Img[k1+1];

        max0 =   w0 > w1 ?   w0 : w1;
        max0 = max0 > w2 ? max0 : w2;
        max0 = max0 > w3 ? max0 : w3;

        Out_Img[i++] = (Pixel)(max0);

        max1 =   v0 > v1 ?   v0 : v1;
        max1 = max1 > v2 ? max1 : v2;
        max1 = max1 > v3 ? max1 : v3;

        Out_Img[i++] = (Pixel)(max1);

    }

    r0+= 2;
    r1+= 2;
    c0 = 0;
    c1 = 2;

  }

}
#ifdef VEC
void __attribute__ ((noinline)) Pool2x2_Vector (Pixel * In_Img, Pixel * Out_Img, int R, int C)
{
  int r, c, t0, t1,k0, k1, r0, r1, c0, c1, i;
  int max0,max1;
  int a,b;
  int a0,b0,a1,b1;
  PixelV *w0,*w1,*v0,*v1;
  PixelV maxw0, maxv0;
  int* _out_imgV = &(Out_Img[0]);

  i=0;
  r0 = 0;
  r1 = 1;
  c0 = 0;
  c1 = 2;

  for (r=0; r < R; r+=2) {

    t0 = r0*R;
    t1 = r1*R;

    for (c=0; c < C>>2; c++) {

        k0 = t0 + c0;
        k1 = t1 + c0;

        w0 = (PixelV* )(&In_Img[k0]);
        w1 = (PixelV* )(&In_Img[k1]);

        k0 = t0 + c1;
        k1 = t1 + c1;

        c0+=4; c1+=4;

        v0 = (PixelV* )(&In_Img[k0]);
        v1 = (PixelV* )(&In_Img[k1]);

        maxw0 = max_vec(*w0,*w1);
        maxv0 = max_vec(*v0,*v1);

        a  = *((int *)&maxw0);
        a0 = a & 0xFFFF;
        a1 = a >> 16;
        max0 = a0 > a1 ? a0 : a1;

        b  = *((int *)&maxv0);
        b0 = b & 0xFFFF;
        b1 = b >> 16;

        max1 = b0 > b1 ? b0 : b1;
        //TODO: check how to modify infer it without sign/zero extension instruction
        asm volatile ("pv.insert.h %[b], %[a], 1\n" : [b] "+r" (max0) : [a] "r"  (max1));
        _out_imgV[i] = max0;
        i++;
    }

    r0+= 2;
    r1+= 2;
    c0 = 0;
    c1 = 2;

  }
}
#endif