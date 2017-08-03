// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "convolution.h"
#include <stdio.h>

#define ROUNDBIT   (1 << (DATA_WIDTH -1 -1))
#define SATURATION ((1 << (DATA_WIDTH -1)) -1)

void __attribute__ ((noinline)) Conv3x3_Scalar  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
  int r, c, k, i, j, w, t;
  Filtc coeff;
  Pixel data;
  int S;

  //image board is black
  for (r=1; r < R-1; r++) {
    for (c=1; c < C-1; c++) {

        S = 0;
        t = r*R + c;
        //move in the window
        /* Coordinate window
            (-1;-1) (-1;0) (-1;+1)
            ( 0;-1) ( 0;0) ( 0;+1)
            (+1;-1) (+1;0) (+1;+1)
        */
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                k = (r+i)*R + (c+j); //coeff for one dimension matrix
                data = In_Img[k];
                w = (i+1)*FILT_WIN + (j+1);
                coeff = Kernel[w];
                S = S + (int)(coeff*data);
            }
        }

        // Rounding
        S = S + ROUNDBIT;
        // Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S = S >> (DATA_WIDTH-1);
        // Saturation
        S = S > SATURATION ? SATURATION : S;
        S = S <          0 ?          0 : S;

        Out_Img[t] = (Pixel)(S);
    }
  }
}

void __attribute__ ((noinline)) Conv5x5_Scalar  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
  int r, c, k, i, j, w, t;
  Filtc coeff;
  Pixel data;
  int S;

  //image board is black
  for (r=2; r < R-2; r++) {
    for (c=2; c < C-2; c++) {

        S = 0;
        t = r*R + c;
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
        // Saturation
        S = S > SATURATION ? SATURATION : S;
        S = S <          0 ?          0 : S;

        Out_Img[t] = (Pixel)S;
    }
  }
}

#if DATA_TYPE == 8

void __attribute__ ((noinline)) Conv3x3_Vector  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
#ifdef DOTP
  FiltcV coeff_0, coeff_1, coeff_2;
  PixelV Img_0, Img_1, Img_2;
  PixelV new_data;
  int r, c, i, j, t;
  int S;

  coeff_0 = (FiltcV)  {Kernel[0], Kernel[1], Kernel[2], 0 };
  coeff_1 = (FiltcV)  {Kernel[3], Kernel[4], Kernel[5], 0 };
  coeff_2 = (FiltcV)  {Kernel[6], Kernel[7], Kernel[8], 0 };

  //image board is black
  for (c=0; c < C-2; c++) {

    Img_0   = (PixelV)  {In_Img[c],     In_Img[c+1],     In_Img[c+2],     0};
    Img_1   = (PixelV)  {In_Img[c+R],   In_Img[c+1+R],   In_Img[c+2+R],   0};
    Img_2   = (PixelV)  {In_Img[c+2*R], In_Img[c+1+2*R], In_Img[c+2+2*R], 0};

    for (r=1; r < R-2; r++) {

        t = r*R + c+1;
        S = dotp(Img_0,coeff_0);
        S = sumdotp(Img_1,coeff_1, S);
        S = sumdotp(Img_2,coeff_2, S);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S = addnr(S,DATA_WIDTH-1,ROUNDBIT);
        // Saturation
        S = clipu(S,0,SATURATION);

        Out_Img[t] = (Pixel)S;

        new_data = (PixelV)  {In_Img[(r+2)*R+c], In_Img[(r+2)*R+c+1], In_Img[(r+2)*R+c+2], 0};

        // Move the window
        /*
          Three vectors:

          Img_0 = {A0, A1, A2, 0}
          Img_1 = {B0, B1, B2, 0}
          Img_2 = {C0, C1, C2, 0}

          Current Windonw:
          XX XX XX
          A0 A1 A2
          B0 B1 B2
          C0 C1 C2
          D0 D1 D2
          XX XX XX

          We want to load next line (D0, D1, D2) in vector new_data
          new_data = {D0, D1, D2, 0} 

          Move each vector one line down

          Img_0 = Img_1
          Img_1 = Img_2
          Img_2 = new_data
        */

        Img_0 = Img_1;
        Img_1 = Img_2;
        Img_2 = new_data;

    }
    //last iteration
    t = r*R + c+1;
    S = dotp(Img_0,coeff_0);
    S = sumdotp(Img_1,coeff_1, S);
    S = sumdotp(Img_2,coeff_2, S);

    // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
    S = addnr(S,DATA_WIDTH-1,ROUNDBIT);
    // Saturation
    S = clipu(S,0,SATURATION);

    Out_Img[t] = (Pixel)S;
  }
#endif
}

void __attribute__ ((noinline)) Conv5x5_Vector  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
#ifdef DOTP
  FiltcV coeff_0, coeff_1, coeff_2, coeff_3, coeff_4, coeff_5, coeff_6;
  PixelV Img_0, Img_1, Img_2, Img_3, Img_4, Img_5, Img_6;
  PixelV new_data1, new_data2;
  int r, c, i, j, t;
  PixelV mask0;
  int S;

  coeff_0 = *((FiltcV *) (&Kernel[ 0]));
  coeff_1 = *((FiltcV *) (&Kernel[ 5]));
  coeff_2 = *((FiltcV *) (&Kernel[10]));
  coeff_3 = *((FiltcV *) (&Kernel[15]));
  coeff_4 = *((FiltcV *) (&Kernel[20]));
  coeff_5 = (FiltcV)     { Kernel[4],  Kernel[9], Kernel[14], Kernel[19] };
  coeff_6 = (FiltcV)     { Kernel[24], 0,         0,          0 };
  mask0 = (PixelV){1, 2, 3, 4};

  //image board is black
  for (c=0; c < C-4; c++) {

    Img_0   = *((PixelV *) (&In_Img[c])     );
    Img_1   = *((PixelV *) (&In_Img[c+R])   );
    Img_2   = *((PixelV *) (&In_Img[c+2*R]) );
    Img_3   = *((PixelV *) (&In_Img[c+3*R]) );
    Img_4   = *((PixelV *) (&In_Img[c+4*R]) );
    Img_5   = (PixelV)     { In_Img[c+4],     In_Img[c+4+R], In_Img[c+4+2*R], In_Img[c+4+3*R] };
    Img_6   = (PixelV)     { In_Img[c+4+4*R], 0,             0,               0 };

    for (r=1; r < R-4; r++) {

        t = (r+1)*R + c+2;
        S = dotp(Img_0,coeff_0);
        S = sumdotp(Img_1,coeff_1, S);
        S = sumdotp(Img_2,coeff_2, S);
        S = sumdotp(Img_3,coeff_3, S);
        S = sumdotp(Img_4,coeff_4, S);
        S = sumdotp(Img_5,coeff_5, S);
        S = sumdotp(Img_6,coeff_6, S);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S = addnr(S,DATA_WIDTH-1,ROUNDBIT);
        // Saturation
        S = clipu(S,0,SATURATION);

        Out_Img[t] = (Pixel)S;

        new_data1 = *((PixelV *) (&In_Img[c+(r+4)*R]));
        new_data2 =   (PixelV  ) {In_Img[c+4+(r+4)*R], 0, 0, 0};

        // Move the window
        /*
          Seven vectors:

          Img_0 = {A0, A1, A2, A3}
          Img_1 = {B0, B1, B2, B3}
          Img_2 = {C0, C1, C2, C3}
          Img_3 = {D0, D1, D2, D3}
          Img_4 = {E0, E1, E2, E3}
          Img_5 = {F0, F1, F2, F3}
          Img_6 = {G0,  0,  0,  0}

          Current Windonw:
          XX XX XX XX XX
          A0 A1 A2 A3 F0
          B0 B1 B2 B3 F1
          C0 C1 C2 C3 F2
          D0 D1 D2 D3 F3
          E0 E1 E2 E3 G0
          H0 H1 H2 H3 G1
          XX XX XX XX XX

          We want to load next line (H0, H1, H2, H3, G1)
          in vectors new_data1 and new_data2
          new_data1 = {H0, H1, H2, H3}
          new_data2 = {G1,  0,  0,  0}

          Move each vector one line down and shuffle the vertical vector

          Img_0 = {B0, B1, B2, B3}
          Img_1 = {C0, C1, C2, C3}
          Img_2 = {D0, D1, D2, D3}
          Img_3 = {E0, E1, E2, E3}
          Img_4 = {H0, H1, H2, H3}
          Img_5 = {F1, F2, F3, G0}
          Img_5 = {G1,  0,  0,  0}
        */

        Img_0 = Img_1;
        Img_1 = Img_2;
        Img_2 = Img_3;
        Img_3 = Img_4;
        Img_4 = new_data1;
        Img_5 = shuffle(Img_5, Img_6,  mask0);
        Img_6 = new_data2;

    }
    //last iteration
    t = (r+1)*R + c+2;
    S = dotp(Img_0,coeff_0);
    S = sumdotp(Img_1,coeff_1, S);
    S = sumdotp(Img_2,coeff_2, S);
    S = sumdotp(Img_3,coeff_3, S);
    S = sumdotp(Img_4,coeff_4, S);
    S = sumdotp(Img_5,coeff_5, S);
    S = sumdotp(Img_6,coeff_6, S);

    // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
    S = addnr(S,DATA_WIDTH-1,ROUNDBIT);
    // Saturation
    S = clipu(S,0,SATURATION);

    Out_Img[t] = (Pixel)S;
  }
#endif
}

#else

void __attribute__ ((noinline)) Conv3x3_Vector  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{

#ifdef DOTP
  FiltcV coeff_0, coeff_1, coeff_2, coeff_3, coeff_4;
  PixelV Img_0, Img_1, Img_2, Img_3, Img_4;
  PixelV new_data1,new_data2;
  int r, c, i, j, t;
  PixelV mask0;
  int S;

  coeff_0 = *((FiltcV *) (&Kernel[0]));
  coeff_1 = *((FiltcV *) (&Kernel[3]));
  coeff_2 = *((FiltcV *) (&Kernel[6]));
  coeff_3 = (FiltcV)     { Kernel[2], Kernel[5] };
  coeff_4 = (FiltcV)     { Kernel[8], 0 };

  mask0 = (PixelV){1, 2};

  //image board is black
  for (c=0; c < C-2; c++) {

    Img_0   = *((PixelV *) (&In_Img[c])     );
    Img_1   = *((PixelV *) (&In_Img[c+R])   );
    Img_2   = *((PixelV *) (&In_Img[c+2*R]) );
    Img_3   = (PixelV)     { In_Img[c+2],     In_Img[c+2+R]   };
    Img_4   = (PixelV)     { In_Img[c+2+2*R], 0               };

    for (r=1; r < R-2; r++) {

        t = r*R + c+1;
        S = dotp(Img_0,coeff_0);
        S = sumdotp(Img_1,coeff_1, S);
        S = sumdotp(Img_2,coeff_2, S);
        S = sumdotp(Img_3,coeff_3, S);
        S = sumdotp(Img_4,coeff_4, S);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S = addnr(S,DATA_WIDTH-1,ROUNDBIT);
        // Saturation
        S = clipu(S,0,SATURATION);

        Out_Img[t] = (Pixel)S;

        new_data1 = *((PixelV *) (&In_Img[c+(r+2)*R]));
        new_data2 = (PixelV)  {In_Img[c+2+(r+2)*R], 0 };

        // Move the window
        /*
          Five vectors:

          Img_0 = {A0, A1}
          Img_1 = {B0, B1}
          Img_2 = {C0, C1}
          Img_3 = {D0, D1}
          Img_4 = {E0,  0}

          Current Windonw:
          XX XX XX
          A0 A1 D0
          B0 B1 D1
          C0 C1 E0
          F0 F1 E1
          XX XX XX

          We want to load next line (F0, F1, E1) in vector new_data1 and new_data2
          new_data1 = {F0, F1}
          new_data2 = {E1,  0}

          Move each vector one line down and shuffle the vertical vector

          1) Img_0 = Img_1
          2) Img_1 = Img_2
          3) Img_2 = new_data1
          4) Img_3 = {D1, E0}
          5) Img_4 = new_data2
        */

        Img_0 = Img_1;
        Img_1 = Img_2;
        Img_2 = new_data1;
        Img_3 = shuffle(Img_3,Img_4,mask0);
        Img_4 = new_data2;

    }
    //last iteration
    t = r*R + c+1;
    S = dotp(Img_0,coeff_0);
    S = sumdotp(Img_1,coeff_1, S);
    S = sumdotp(Img_2,coeff_2, S);
    S = sumdotp(Img_3,coeff_3, S);
    S = sumdotp(Img_4,coeff_4, S);

    // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
    S = addnr(S,DATA_WIDTH-1,ROUNDBIT);
    // Saturation
    S = clipu(S,0,SATURATION);

    Out_Img[t] = (Pixel)S;
  }
#endif
}

void __attribute__ ((noinline)) Conv5x5_Vector  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
 #ifdef DOTP
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

        t = (r+1)*R + c+2;
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
        // Saturation
        S = clipu(S,0,SATURATION);

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
    t = (r+1)*R + c+2;
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
    // Saturation
    S = clipu(S,0,SATURATION);

    Out_Img[t] = (Pixel)S;
  }
#endif
}

#endif
