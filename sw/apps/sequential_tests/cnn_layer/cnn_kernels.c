// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "cnn_layer.h"
#include <stdio.h>

#define ROUNDBIT   (1 << (DATA_WIDTH -1 -1))
#define SATURATION ((1 << (DATA_WIDTH -1)) -1)

static Pixel __attribute__ ((section(".heapsram")))  Conv_Img[CONV_DIM];

void __attribute__ ((noinline)) CNN_layer_Scalar  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{

  Conv5x5_Scalar(In_Img,Conv_Img,R,C,Kernel);
  Pool2x2(Conv_Img,Out_Img,R-4,C-4);

}

void __attribute__ ((noinline)) Conv5x5_Scalar  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
  int r, c, k, i, j, w, t;
  Filtc coeff;
  Pixel data;
  int S;
  t = 0;

  //image board is black
  for (r=2; r < R-2; r++) {
    for (c=2; c < C-2; c++) {

        S = 0;
        //move in the window
        /* Coordinate window
            (-2;-2) (-2;-1) (-2; 0) (-2;+1) (-2;+2)
            (-1;-2) (-1;-1) (-1; 0) (-1;+1) (-1;+2)
            ( 0;-2) ( 0;-1) ( 0; 0) ( 0;+1) ( 0;+2)
            (+1;-2) (+1;-1) (+1; 0) (+1;+1) (+1;+2)
            (+2;-2) (+2;-1) (+2; 0) (+2;+1) (+2;+2)
        */
        for (i = -2; i <= 2; i++) {
            for (j = -2; j <= 2; j++) {

                k = (r+i)*R + (c+j); //coeff for one dimension matrix
                data = In_Img[k];
                w = (i+2)*FILT_WIN + (j+2);
                coeff = Kernel[w];
                S += (int)(coeff*data);
            }
        }

        // Rounding
        S = S + ROUNDBIT;
        // Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S = S >> (DATA_WIDTH-1);

        // Non linearity
        S = S > 0 ? S : 0;

        Out_Img[t++] = (Pixel)S;
    }
  }
}

#ifdef VEC
void __attribute__ ((noinline)) CNN_layer_Vector  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{

  Conv5x5_Vector(In_Img,Conv_Img,R,C,Kernel);
  Pool2x2(Conv_Img,Out_Img,R-4,C-4);

}

void __attribute__ ((noinline)) Conv5x5_Vector  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
  FiltcV coeff_0, coeff_1, coeff_2, coeff_3, coeff_4, coeff_5, coeff_6, coeff_7, coeff_8, coeff_9, coeff_10, coeff_11, coeff_12;
  PixelV Img_0, Img_1, Img_2, Img_3, Img_4, Img_5, Img_6, Img_7, Img_8, Img_9, Img_10, Img_11, Img_12;
  PixelV new_data1, new_data2, new_data3;
  int r, c, i, j, t;
  PixelV mask0;
  int S;

  coeff_0  = *((FiltcV *) (&Kernel[0]));
  coeff_1  = *((FiltcV *) (&Kernel[2]));
  coeff_2  = *((FiltcV *) (&Kernel[5]));
  coeff_3  = *((FiltcV *) (&Kernel[7]));
  coeff_4  = *((FiltcV *) (&Kernel[10]));
  coeff_5  = *((FiltcV *) (&Kernel[12]));
  coeff_6  = *((FiltcV *) (&Kernel[15]));
  coeff_7  = *((FiltcV *) (&Kernel[17]));
  coeff_8  = *((FiltcV *) (&Kernel[20]));
  coeff_9  = *((FiltcV *) (&Kernel[22]));
  coeff_10 = (FiltcV)     { Kernel[4],  Kernel[9] };
  coeff_11 = (FiltcV)     { Kernel[14], Kernel[19] };
  coeff_12 = (FiltcV)     { Kernel[24], 0 };

  mask0 = (PixelV){1, 2};

  //image board is black
  for (c=0; c < C-4; c++) {

    Img_0   = *((PixelV *) (&In_Img[c])      );
    Img_1   = *((PixelV *) (&In_Img[c+2])    );
    Img_2   = *((PixelV *) (&In_Img[c+R])    );
    Img_3   = *((PixelV *) (&In_Img[c+2+R])  );
    Img_4   = *((PixelV *) (&In_Img[c+2*R])  );
    Img_5   = *((PixelV *) (&In_Img[c+2+2*R]));
    Img_6   = *((PixelV *) (&In_Img[c+3*R])  );
    Img_7   = *((PixelV *) (&In_Img[c+2+3*R]));
    Img_8   = *((PixelV *) (&In_Img[c+4*R])  );
    Img_9   = *((PixelV *) (&In_Img[c+2+4*R]));
    Img_10  = (PixelV)     { In_Img[c+4],     In_Img[c+4+R] };
    Img_11  = (PixelV)     { In_Img[c+4+2*R], In_Img[c+4+3*R] };
    Img_12  = (PixelV)     { In_Img[c+4+4*R], 0  };

    for (r=1; r < R-4; r++) {

        t = (r-1)*(R-4) + c;
        S = dotp(Img_0,coeff_0);
        S = sumdotp(Img_1,  coeff_1, S);
        S = sumdotp(Img_2,  coeff_2, S);
        S = sumdotp(Img_3,  coeff_3, S);
        S = sumdotp(Img_4,  coeff_4, S);
        S = sumdotp(Img_5,  coeff_5, S);
        S = sumdotp(Img_6,  coeff_6, S);
        S = sumdotp(Img_7,  coeff_7, S);
        S = sumdotp(Img_8,  coeff_8, S);
        S = sumdotp(Img_9,  coeff_9, S);
        S = sumdotp(Img_10,coeff_10, S);
        S = sumdotp(Img_11,coeff_11, S);
        S = sumdotp(Img_12,coeff_12, S);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S = addnr(S,DATA_WIDTH-1,ROUNDBIT);
        // Non linearity
        S = S > 0 ? S : 0;

        Out_Img[t] = (Pixel)S;

        new_data1 = *((PixelV *) (&In_Img[c+(r+4)*R]));
        new_data2 = *((PixelV *) (&In_Img[c+2+(r+4)*R]));
        new_data3 =   (PixelV  ) {In_Img[c+4+(r+4)*R], 0};

        // Move the window
        /*
          thirteen vectors:

          Img_0  = {A0, A1}
          Img_1  = {B0, B1}
          Img_2  = {C0, C1}
          Img_3  = {D0, D1}
          Img_4  = {E0, E1}
          Img_5  = {F0, F1}
          Img_6  = {G0, G1}
          Img_7  = {H0, H1}
          Img_8  = {I0, I1}
          Img_9  = {J0, J1}
          Img_10 = {K0, K1}
          Img_11 = {L0, L1}
          Img_12 = {M0,  0}

          Current Windonw:
          XX XX XX XX XX
          A0 A1 B0 B1 K0
          C0 C1 D0 D1 K1
          E0 E1 F0 F1 L0
          G0 G1 H0 H1 L1
          I0 I1 J0 J1 M0
          N0 N1 P0 P1 M1
          XX XX XX XX XX

          We want to load next line (N0, N1, P0, P1, M1)
          in vectors new_data1 and new_data2
          new_data1 = {N0, N1}
          new_data2 = {P0, P1}
          new_data3 = {M1,  0}

          Move each vector one line down and shuffle the vertical vector

          Img_0  = Img_2
          Img_1  = Img_3
          Img_2  = Img_4
          Img_3  = Img_5
          Img_4  = Img_6
          Img_5  = Img_7
          Img_6  = Img_8
          Img_7  = Img_9
          Img_8  = new_data1
          Img_9  = new_data2
          Img_10 = {K1, L0}
          Img_11 = {L1, M0}
          Img_12 = new_data3
        */

        Img_0  = Img_2;
        Img_1  = Img_3;
        Img_2  = Img_4;
        Img_3  = Img_5;
        Img_4  = Img_6;
        Img_5  = Img_7;
        Img_6  = Img_8;
        Img_7  = Img_9;
        Img_8  = new_data1;
        Img_9  = new_data2;
        Img_10 = shuffle(Img_10, Img_11, mask0);
        Img_11 = shuffle(Img_11, Img_12, mask0);
        Img_12 = new_data3;

    }
    //last iteration
    t = (r-1)*(R-4) + c;
    S = dotp(Img_0,coeff_0);
    S = sumdotp(Img_1,  coeff_1, S);
    S = sumdotp(Img_2,  coeff_2, S);
    S = sumdotp(Img_3,  coeff_3, S);
    S = sumdotp(Img_4,  coeff_4, S);
    S = sumdotp(Img_5,  coeff_5, S);
    S = sumdotp(Img_6,  coeff_6, S);
    S = sumdotp(Img_7,  coeff_7, S);
    S = sumdotp(Img_8,  coeff_8, S);
    S = sumdotp(Img_9,  coeff_9, S);
    S = sumdotp(Img_10,coeff_10, S);
    S = sumdotp(Img_11,coeff_11, S);
    S = sumdotp(Img_12,coeff_12, S);
    // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
    S = addnr(S,DATA_WIDTH-1,ROUNDBIT);
    // Non linearity
    S = S > 0 ? S : 0;

    Out_Img[t] = (Pixel)S;

  }
}
#endif
void __attribute__ ((noinline)) Pool2x2 (Pixel * In_Img, Pixel * Out_Img, int R, int C)
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
