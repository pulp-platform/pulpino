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

void __attribute__ ((noinline)) CNN_layer_Scalar_Fused (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{

  int r, c, k, i, j, w, t;
  Filtc coeff;
  Pixel data;
  int S00,S01,S10,S11;
  Pixel max0;

  t = 0;

  //image board is black
  for (r=2; r < R-2; r+=2) {
    for (c=2; c < C-2; c+=2) {

        S00 = 0;
        S01 = 0;
        S10 = 0;
        S11 = 0;

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

                k = (r+i)*R + (c+j);
                data = In_Img[k];
                w = (i+2)*FILT_WIN + (j+2);
                coeff = Kernel[w];
                S00 += (int)(coeff*data);

                k = (r+i)*R + (c+1+j);
                data = In_Img[k];
                w = (i+2)*FILT_WIN + (j+2);
                coeff = Kernel[w];
                S01 += (int)(coeff*data);

                k = (r+1+i)*R + (c+j);
                data = In_Img[k];
                w = (i+2)*FILT_WIN + (j+2);
                coeff = Kernel[w];
                S10 += (int)(coeff*data);

                k = (r+1+i)*R + (c+1+j);
                data = In_Img[k];
                w = (i+2)*FILT_WIN + (j+2);
                coeff = Kernel[w];
                S11 += (int)(coeff*data);
            }
        }


        // Rounding
        S00 = S00 + ROUNDBIT;
        S01 = S01 + ROUNDBIT;
        S10 = S10 + ROUNDBIT;
        S11 = S11 + ROUNDBIT;
        // Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S00 = S00 >> (DATA_WIDTH-1);
        S01 = S01 >> (DATA_WIDTH-1);
        S10 = S10 >> (DATA_WIDTH-1);
        S11 = S11 >> (DATA_WIDTH-1);

        // Non linearity
        S00 = S00 > 0 ? S00 : 0;
        S01 = S01 > 0 ? S01 : 0;
        S10 = S10 > 0 ? S10 : 0;
        S11 = S11 > 0 ? S11 : 0;

        // Pooling
        max0 =  S00 > S01 ?  S00 : S01;
        max0 = max0 > S10 ? max0 : S10;
        max0 = max0 > S11 ? max0 : S11;

        Out_Img[t++] = (Pixel)max0;
    }
  }

}

#ifdef VEC
//using the SHUFFLE version makes the kernel slower
//#define USE_SHUFFLE
void __attribute__ ((noinline)) CNN_layer_Vector_Fused (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
  FiltcV coeff_0, coeff_1, coeff_2, coeff_3, coeff_4, coeff_5, coeff_6, coeff_7, coeff_8, coeff_9, coeff_10, coeff_11, coeff_12, coeff_13, coeff_14;
  PixelV pImg0, pImg1, pImg2, pImg3, pImg4, pImg5, pImg6, pImg7, pImg8, pImg9, pImg10, pImg11, pImg12, pImg13, pImg14, pImg15, pImg16, pImg17;
  int r, c, t, k;
  int S00, S01, S10, S11;
  Pixel max0;
  PixelV mask_left, mask_right;
  PixelV zeroV;

  /*
 ________________
|        |       |
|        |       |
|    0   |   1   |
|________|_______|
|        |       |
|    2   |   3   |
|        |       |
|________|_______|

  */

  mask_right = (PixelV){3, 0};
  mask_left  = (PixelV){1, 2};
  zeroV      = (PixelV){0, 0};

#ifdef USE_SHUFFLE
  coeff_0  = *((FiltcV *) (&Kernel[0]));
  coeff_1  = *((FiltcV *) (&Kernel[2]));
  coeff_2  = (FiltcV) { Kernel[4],  0 };

  coeff_3  = *((FiltcV *) (&Kernel[5]));
  coeff_4  = *((FiltcV *) (&Kernel[7]));
  coeff_5  = (FiltcV) { Kernel[9],  0 };

  coeff_6  = *((FiltcV *) (&Kernel[10]));
  coeff_7  = *((FiltcV *) (&Kernel[12]));
  coeff_8  = (FiltcV) { Kernel[14], 0 };

  coeff_9  = *((FiltcV *) (&Kernel[15]));
  coeff_10 = *((FiltcV *) (&Kernel[17]));
  coeff_11 = (FiltcV) { Kernel[19], 0 };

  coeff_12 = *((FiltcV *) (&Kernel[20]));
  coeff_13 = *((FiltcV *) (&Kernel[22]));
  coeff_14 = (FiltcV) { Kernel[24], 0 };
#endif

  k = 0;
  //image board is black
  for (c=0; c < C-4; c+=2,k++) {
    t = k;

    pImg0   = *((PixelV *) (&In_Img[c])      );
    pImg1   = *((PixelV *) (&In_Img[c+2])    );
    pImg2   = *((PixelV *) (&In_Img[c+4])    );

    pImg3   = *((PixelV *) (&In_Img[c+R])    );
    pImg4   = *((PixelV *) (&In_Img[c+2+R])  );
    pImg5   = *((PixelV *) (&In_Img[c+4+R])  );

    pImg6   = *((PixelV *) (&In_Img[c+2*R])  );
    pImg7   = *((PixelV *) (&In_Img[c+2+2*R]));
    pImg8   = *((PixelV *) (&In_Img[c+4+2*R]));

    pImg9   = *((PixelV *) (&In_Img[c+3*R])  );
    pImg10  = *((PixelV *) (&In_Img[c+2+3*R]));
    pImg11  = *((PixelV *) (&In_Img[c+4+3*R]));

    pImg12  = *((PixelV *) (&In_Img[c+4*R])  );
    pImg13  = *((PixelV *) (&In_Img[c+2+4*R]));
    pImg14  = *((PixelV *) (&In_Img[c+4+4*R]));

    pImg15  = *((PixelV *) (&In_Img[c+5*R])  );
    pImg16  = *((PixelV *) (&In_Img[c+2+5*R]));
    pImg17  = *((PixelV *) (&In_Img[c+4+5*R]));


    for (r=1; r < R-6; r+=2) {

#ifndef USE_SHUFFLE
        coeff_0  = *((FiltcV *) (&Kernel[0]));
        coeff_1  = *((FiltcV *) (&Kernel[2]));
        coeff_2  = (FiltcV) { Kernel[4],  0 };

        coeff_3  = *((FiltcV *) (&Kernel[5]));
        coeff_4  = *((FiltcV *) (&Kernel[7]));
        coeff_5  = (FiltcV) { Kernel[9],  0 };

        coeff_6  = *((FiltcV *) (&Kernel[10]));
        coeff_7  = *((FiltcV *) (&Kernel[12]));
        coeff_8  = (FiltcV) { Kernel[14], 0 };

        coeff_9  = *((FiltcV *) (&Kernel[15]));
        coeff_10 = *((FiltcV *) (&Kernel[17]));
        coeff_11 = (FiltcV) { Kernel[19], 0 };

        coeff_12 = *((FiltcV *) (&Kernel[20]));
        coeff_13 = *((FiltcV *) (&Kernel[22]));
        coeff_14 = (FiltcV) { Kernel[24], 0 };
#endif
        //WINDOW 0
        S00 = dotp(pImg0,coeff_0);
        S00 = sumdotp(pImg1,  coeff_1,  S00);
        S00 = sumdotp(pImg2,  coeff_2,  S00);

        S00 = sumdotp(pImg3,  coeff_3,  S00);
        S00 = sumdotp(pImg4,  coeff_4,  S00);
        S00 = sumdotp(pImg5,  coeff_5,  S00);

        S00 = sumdotp(pImg6,  coeff_6,  S00);
        S00 = sumdotp(pImg7,  coeff_7,  S00);
        S00 = sumdotp(pImg8,  coeff_8,  S00);

        S00 = sumdotp(pImg9,  coeff_9,  S00);
        S00 = sumdotp(pImg10, coeff_10, S00);
        S00 = sumdotp(pImg11, coeff_11, S00);

        S00 = sumdotp(pImg12, coeff_12, S00);
        S00 = sumdotp(pImg13, coeff_13, S00);
        S00 = sumdotp(pImg14, coeff_14, S00);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S00 = addnr(S00,DATA_WIDTH-1,ROUNDBIT);

        //WINDOW 2
        S10 = dotp(pImg3,coeff_0);
        S10 = sumdotp(pImg4,  coeff_1,  S10);
        S10 = sumdotp(pImg5,  coeff_2,  S10);

        S10 = sumdotp(pImg6,  coeff_3,  S10);
        S10 = sumdotp(pImg7,  coeff_4,  S10);
        S10 = sumdotp(pImg8,  coeff_5,  S10);

        S10 = sumdotp(pImg9,  coeff_6,  S10);
        S10 = sumdotp(pImg10, coeff_7,  S10);
        S10 = sumdotp(pImg11, coeff_8,  S10);

        S10 = sumdotp(pImg12, coeff_9,  S10);
        S10 = sumdotp(pImg13, coeff_10, S10);
        S10 = sumdotp(pImg14, coeff_11, S10);

        S10 = sumdotp(pImg15, coeff_12, S10);
        S10 = sumdotp(pImg16, coeff_13, S10);
        S10 = sumdotp(pImg17, coeff_14, S10);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S10 = addnr(S10,DATA_WIDTH-1,ROUNDBIT);

        //windows 1-3
#ifndef USE_SHUFFLE
        coeff_0  = (FiltcV) { 0, Kernel[0]  } ;
        coeff_1  = *((FiltcV *) (&Kernel[1])) ;
        coeff_2  = *((FiltcV *) (&Kernel[3])) ;

        coeff_3  = (FiltcV) { 0, Kernel[5]  } ;
        coeff_4  = *((FiltcV *) (&Kernel[6])) ;
        coeff_5  = *((FiltcV *) (&Kernel[8])) ;

        coeff_6  = (FiltcV) { 0, Kernel[10] } ;
        coeff_7  = *((FiltcV *) (&Kernel[11]));
        coeff_8 = *((FiltcV *) (&Kernel[13]));

        coeff_9  = (FiltcV) { 0, Kernel[15] } ;
        coeff_10 = *((FiltcV *) (&Kernel[16]));
        coeff_11 = *((FiltcV *) (&Kernel[18]));

        coeff_12 = (FiltcV) { 0, Kernel[20] } ;
        coeff_13 = *((FiltcV *) (&Kernel[21]));
        coeff_14 = *((FiltcV *) (&Kernel[23]));
#else
        coeff_2 = shuffle(coeff_2, coeff_1, mask_right);
        coeff_1 = shuffle(coeff_1, coeff_0, mask_right);
        coeff_0 = shuffle(coeff_0, zeroV, mask_right);

        coeff_5 = shuffle(coeff_5, coeff_4, mask_right);
        coeff_4 = shuffle(coeff_4, coeff_3, mask_right);
        coeff_3 = shuffle(coeff_3, zeroV, mask_right);

        coeff_8 = shuffle(coeff_8, coeff_7, mask_right);
        coeff_7 = shuffle(coeff_7, coeff_6, mask_right);
        coeff_6 = shuffle(coeff_6, zeroV, mask_right);

        coeff_11 = shuffle(coeff_11, coeff_10, mask_right);
        coeff_10 = shuffle(coeff_10, coeff_9, mask_right);
        coeff_9  = shuffle(coeff_9, zeroV, mask_right);

        coeff_14 = shuffle(coeff_14, coeff_13, mask_right);
        coeff_13 = shuffle(coeff_13, coeff_12, mask_right);
        coeff_12 = shuffle(coeff_12, zeroV, mask_right);
#endif

        //WINDOW 1
        S01 = dotp(pImg0,coeff_0);
        S01 = sumdotp(pImg1,  coeff_1,  S01);
        S01 = sumdotp(pImg2,  coeff_2,  S01);

        S01 = sumdotp(pImg3,  coeff_3,  S01);
        S01 = sumdotp(pImg4,  coeff_4,  S01);
        S01 = sumdotp(pImg5,  coeff_5,  S01);

        S01 = sumdotp(pImg6,  coeff_6,  S01);
        S01 = sumdotp(pImg7,  coeff_7,  S01);
        S01 = sumdotp(pImg8,  coeff_8,  S01);

        S01 = sumdotp(pImg9,  coeff_9,  S01);
        S01 = sumdotp(pImg10, coeff_10, S01);
        S01 = sumdotp(pImg11, coeff_11, S01);

        S01 = sumdotp(pImg12, coeff_12, S01);
        S01 = sumdotp(pImg13, coeff_13, S01);
        S01 = sumdotp(pImg14, coeff_14, S01);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S01 = addnr(S01,DATA_WIDTH-1,ROUNDBIT);

        //WINDOW 3
        S11 = dotp(pImg3,coeff_0);
        S11 = sumdotp(pImg4,  coeff_1,  S11);
        S11 = sumdotp(pImg5,  coeff_2,  S11);

        S11 = sumdotp(pImg6,  coeff_3,  S11);
        S11 = sumdotp(pImg7,  coeff_4,  S11);
        S11 = sumdotp(pImg8,  coeff_5,  S11);

        S11 = sumdotp(pImg9,  coeff_6,  S11);
        S11 = sumdotp(pImg10, coeff_7,  S11);
        S11 = sumdotp(pImg11, coeff_8,  S11);

        S11 = sumdotp(pImg12, coeff_9,  S11);
        S11 = sumdotp(pImg13, coeff_10, S11);
        S11 = sumdotp(pImg14, coeff_11, S11);

        S11 = sumdotp(pImg15, coeff_12, S11);
        S11 = sumdotp(pImg16, coeff_13, S11);
        S11 = sumdotp(pImg17, coeff_14, S11);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S11 = addnr(S11,DATA_WIDTH-1,ROUNDBIT);

        // Non linearity
        S00 = S00 > 0 ? S00 : 0;
        S01 = S01 > 0 ? S01 : 0;
        S10 = S10 > 0 ? S10 : 0;
        S11 = S11 > 0 ? S11 : 0;

        max0 =  S00 > S01 ?  S00 : S01;
        max0 = max0 > S10 ? max0 : S10;
        max0 = max0 > S11 ? max0 : S11;

        Out_Img[t] = (Pixel)max0;
        t+= (R - 4)>>1;

        //next line
        pImg0  = pImg6;
        pImg1  = pImg7;
        pImg2  = pImg8;

        pImg3  = pImg9;
        pImg4  = pImg10;
        pImg5  = pImg11;

        pImg6  = pImg12;
        pImg7  = pImg13;
        pImg8  = pImg14;

        pImg9  = pImg15;
        pImg10 = pImg16;
        pImg11 = pImg17;

        pImg12 = *((PixelV *) (&In_Img[c+(r+5)*R])  );
        pImg13 = *((PixelV *) (&In_Img[c+2+(r+5)*R]));
        pImg14 = *((PixelV *) (&In_Img[c+4+(r+5)*R]));

        pImg15 = *((PixelV *) (&In_Img[c+(r+6)*R])  );
        pImg16 = *((PixelV *) (&In_Img[c+2+(r+6)*R]));
        pImg17 = *((PixelV *) (&In_Img[c+4+(r+6)*R]));

#ifdef USE_SHUFFLE
        coeff_0 = shuffle(coeff_0, coeff_1, mask_left);
        coeff_1 = shuffle(coeff_1, coeff_2, mask_left);
        coeff_2 = shuffle(coeff_2, zeroV, mask_left);

        coeff_3 = shuffle(coeff_3, coeff_4, mask_left);
        coeff_4 = shuffle(coeff_4, coeff_5, mask_left);
        coeff_5 = shuffle(coeff_5, zeroV, mask_left);

        coeff_6 = shuffle(coeff_6, coeff_7, mask_left);
        coeff_7 = shuffle(coeff_7, coeff_8, mask_left);
        coeff_8 = shuffle(coeff_8, zeroV, mask_left);

        coeff_9 = shuffle(coeff_9, coeff_10, mask_left);
        coeff_10 = shuffle(coeff_10, coeff_11, mask_left);
        coeff_11 = shuffle(coeff_11, zeroV, mask_left);

        coeff_12 = shuffle(coeff_12, coeff_13, mask_left);
        coeff_13 = shuffle(coeff_13, coeff_14, mask_left);
        coeff_14 = shuffle(coeff_14, zeroV, mask_left);
#endif
    }

    //last two rows

#ifndef USE_SHUFFLE
    coeff_0  = *((FiltcV *) (&Kernel[0]));
    coeff_1  = *((FiltcV *) (&Kernel[2]));
    coeff_2  = (FiltcV) { Kernel[4],  0 };

    coeff_3  = *((FiltcV *) (&Kernel[5]));
    coeff_4  = *((FiltcV *) (&Kernel[7]));
    coeff_5  = (FiltcV) { Kernel[9],  0 };

    coeff_6  = *((FiltcV *) (&Kernel[10]));
    coeff_7  = *((FiltcV *) (&Kernel[12]));
    coeff_8  = (FiltcV) { Kernel[14], 0 };

    coeff_9  = *((FiltcV *) (&Kernel[15]));
    coeff_10 = *((FiltcV *) (&Kernel[17]));
    coeff_11 = (FiltcV) { Kernel[19], 0 };

    coeff_12 = *((FiltcV *) (&Kernel[20]));
    coeff_13 = *((FiltcV *) (&Kernel[22]));
    coeff_14 = (FiltcV) { Kernel[24], 0 };
#endif

    //WINDOW 0
    S00 = dotp(pImg0,coeff_0);
    S00 = sumdotp(pImg1,  coeff_1,  S00);
    S00 = sumdotp(pImg2,  coeff_2,  S00);

    S00 = sumdotp(pImg3,  coeff_3,  S00);
    S00 = sumdotp(pImg4,  coeff_4,  S00);
    S00 = sumdotp(pImg5,  coeff_5,  S00);

    S00 = sumdotp(pImg6,  coeff_6,  S00);
    S00 = sumdotp(pImg7,  coeff_7,  S00);
    S00 = sumdotp(pImg8,  coeff_8,  S00);

    S00 = sumdotp(pImg9,  coeff_9,  S00);
    S00 = sumdotp(pImg10, coeff_10, S00);
    S00 = sumdotp(pImg11, coeff_11, S00);

    S00 = sumdotp(pImg12, coeff_12, S00);
    S00 = sumdotp(pImg13, coeff_13, S00);
    S00 = sumdotp(pImg14, coeff_14, S00);

    // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
    S00 = addnr(S00,DATA_WIDTH-1,ROUNDBIT);

    //WINDOW 2
    S10 = dotp(pImg3,coeff_0);
    S10 = sumdotp(pImg4,  coeff_1,  S10);
    S10 = sumdotp(pImg5,  coeff_2,  S10);

    S10 = sumdotp(pImg6,  coeff_3,  S10);
    S10 = sumdotp(pImg7,  coeff_4,  S10);
    S10 = sumdotp(pImg8,  coeff_5,  S10);

    S10 = sumdotp(pImg9,  coeff_6,  S10);
    S10 = sumdotp(pImg10, coeff_7,  S10);
    S10 = sumdotp(pImg11, coeff_8,  S10);

    S10 = sumdotp(pImg12, coeff_9,  S10);
    S10 = sumdotp(pImg13, coeff_10, S10);
    S10 = sumdotp(pImg14, coeff_11, S10);

    S10 = sumdotp(pImg15, coeff_12, S10);
    S10 = sumdotp(pImg16, coeff_13, S10);
    S10 = sumdotp(pImg17, coeff_14, S10);

    // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
    S10 = addnr(S10,DATA_WIDTH-1,ROUNDBIT);

    //windows 1-3
#ifndef USE_SHUFFLE
    coeff_0  = (FiltcV) { 0, Kernel[0]  } ;
    coeff_1  = *((FiltcV *) (&Kernel[1])) ;
    coeff_2  = *((FiltcV *) (&Kernel[3])) ;

    coeff_3  = (FiltcV) { 0, Kernel[5]  } ;
    coeff_4  = *((FiltcV *) (&Kernel[6])) ;
    coeff_5  = *((FiltcV *) (&Kernel[8])) ;

    coeff_6  = (FiltcV) { 0, Kernel[10] } ;
    coeff_7  = *((FiltcV *) (&Kernel[11]));
    coeff_8 = *((FiltcV *) (&Kernel[13]));

    coeff_9  = (FiltcV) { 0, Kernel[15] } ;
    coeff_10 = *((FiltcV *) (&Kernel[16]));
    coeff_11 = *((FiltcV *) (&Kernel[18]));

    coeff_12 = (FiltcV) { 0, Kernel[20] } ;
    coeff_13 = *((FiltcV *) (&Kernel[21]));
    coeff_14 = *((FiltcV *) (&Kernel[23]));
#else
    coeff_2 = shuffle(coeff_2, coeff_1, mask_right);
    coeff_1 = shuffle(coeff_1, coeff_0, mask_right);
    coeff_0 = shuffle(coeff_0, zeroV, mask_right);

    coeff_5 = shuffle(coeff_5, coeff_4, mask_right);
    coeff_4 = shuffle(coeff_4, coeff_3, mask_right);
    coeff_3 = shuffle(coeff_3, zeroV, mask_right);

    coeff_8 = shuffle(coeff_8, coeff_7, mask_right);
    coeff_7 = shuffle(coeff_7, coeff_6, mask_right);
    coeff_6 = shuffle(coeff_6, zeroV, mask_right);

    coeff_11 = shuffle(coeff_11, coeff_10, mask_right);
    coeff_10 = shuffle(coeff_10, coeff_9, mask_right);
    coeff_9  = shuffle(coeff_9, zeroV, mask_right);

    coeff_14 = shuffle(coeff_14, coeff_13, mask_right);
    coeff_13 = shuffle(coeff_13, coeff_12, mask_right);
    coeff_12 = shuffle(coeff_12, zeroV, mask_right);
#endif

    //WINDOW 1
    S01 = dotp(pImg0,coeff_0);
    S01 = sumdotp(pImg1,  coeff_1,  S01);
    S01 = sumdotp(pImg2,  coeff_2,  S01);

    S01 = sumdotp(pImg3,  coeff_3,  S01);
    S01 = sumdotp(pImg4,  coeff_4,  S01);
    S01 = sumdotp(pImg5,  coeff_5,  S01);

    S01 = sumdotp(pImg6,  coeff_6,  S01);
    S01 = sumdotp(pImg7,  coeff_7,  S01);
    S01 = sumdotp(pImg8,  coeff_8,  S01);

    S01 = sumdotp(pImg9,  coeff_9,  S01);
    S01 = sumdotp(pImg10, coeff_10, S01);
    S01 = sumdotp(pImg11, coeff_11, S01);

    S01 = sumdotp(pImg12, coeff_12, S01);
    S01 = sumdotp(pImg13, coeff_13, S01);
    S01 = sumdotp(pImg14, coeff_14, S01);

    // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
    S01 = addnr(S01,DATA_WIDTH-1,ROUNDBIT);

    //WINDOW 3
    S11 = dotp(pImg3,coeff_0);
    S11 = sumdotp(pImg4,  coeff_1,  S11);
    S11 = sumdotp(pImg5,  coeff_2,  S11);

    S11 = sumdotp(pImg6,  coeff_3,  S11);
    S11 = sumdotp(pImg7,  coeff_4,  S11);
    S11 = sumdotp(pImg8,  coeff_5,  S11);

    S11 = sumdotp(pImg9,  coeff_6,  S11);
    S11 = sumdotp(pImg10, coeff_7,  S11);
    S11 = sumdotp(pImg11, coeff_8,  S11);

    S11 = sumdotp(pImg12, coeff_9,  S11);
    S11 = sumdotp(pImg13, coeff_10, S11);
    S11 = sumdotp(pImg14, coeff_11, S11);

    S11 = sumdotp(pImg15, coeff_12, S11);
    S11 = sumdotp(pImg16, coeff_13, S11);
    S11 = sumdotp(pImg17, coeff_14, S11);

    // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
    S11 = addnr(S11,DATA_WIDTH-1,ROUNDBIT);

    // Non linearity
    S00 = S00 > 0 ? S00 : 0;
    S01 = S01 > 0 ? S01 : 0;
    S10 = S10 > 0 ? S10 : 0;
    S11 = S11 > 0 ? S11 : 0;

    max0 =  S00 > S01 ?  S00 : S01;
    max0 = max0 > S10 ? max0 : S10;
    max0 = max0 > S11 ? max0 : S11;

    Out_Img[t] = (Pixel)max0;
#ifdef USE_SHUFFLE
    coeff_0 = shuffle(coeff_0, coeff_1, mask_left);
    coeff_1 = shuffle(coeff_1, coeff_2, mask_left);
    coeff_2 = shuffle(coeff_2, zeroV, mask_left);

    coeff_3 = shuffle(coeff_3, coeff_4, mask_left);
    coeff_4 = shuffle(coeff_4, coeff_5, mask_left);
    coeff_5 = shuffle(coeff_5, zeroV, mask_left);

    coeff_6 = shuffle(coeff_6, coeff_7, mask_left);
    coeff_7 = shuffle(coeff_7, coeff_8, mask_left);
    coeff_8 = shuffle(coeff_8, zeroV, mask_left);

    coeff_9 = shuffle(coeff_9, coeff_10, mask_left);
    coeff_10 = shuffle(coeff_10, coeff_11, mask_left);
    coeff_11 = shuffle(coeff_11, zeroV, mask_left);

    coeff_12 = shuffle(coeff_12, coeff_13, mask_left);
    coeff_13 = shuffle(coeff_13, coeff_14, mask_left);
    coeff_14 = shuffle(coeff_14, zeroV, mask_left);
#endif
  }
}
#endif