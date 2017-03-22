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
void __attribute__ ((noinline)) CNN_layer_Vector_Fused (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{
  FiltcV coeff_0, coeff_1, coeff_2, coeff_3, coeff_4, coeff_5, coeff_6, coeff_7, coeff_8, coeff_9, coeff_10, coeff_11, coeff_12;
  PixelV c0Img_0, c0Img_1, c0Img_2, c0Img_3, c0Img_4, c0Img_5, c0Img_6, c0Img_7, c0Img_8, c0Img_9, c0Img_10, c0Img_11, c0Img_12;
  PixelV c1Img_0, c1Img_1, c1Img_2, c1Img_3, c1Img_4, c1Img_5, c1Img_6, c1Img_7, c1Img_8, c1Img_9, c1Img_10, c1Img_11, c1Img_12;
  PixelV new_data1, new_data2, new_data3;
  int r, c, i, j, t, k;
  PixelV mask0;
  int S00, S01, S10, S11;
  Pixel max0;

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

  k = 0;
  //image board is black
  for (c=0; c < C-4; c+=2,k++) {
    t = k;
    //first column window
    c0Img_0   = *((PixelV *) (&In_Img[c])      );
    c0Img_1   = *((PixelV *) (&In_Img[c+2])    );
    c0Img_2   = *((PixelV *) (&In_Img[c+R])    );
    c0Img_3   = *((PixelV *) (&In_Img[c+2+R])  );
    c0Img_4   = *((PixelV *) (&In_Img[c+2*R])  );
    c0Img_5   = *((PixelV *) (&In_Img[c+2+2*R]));
    c0Img_6   = *((PixelV *) (&In_Img[c+3*R])  );
    c0Img_7   = *((PixelV *) (&In_Img[c+2+3*R]));
    c0Img_8   = *((PixelV *) (&In_Img[c+4*R])  );
    c0Img_9   = *((PixelV *) (&In_Img[c+2+4*R]));
    c0Img_10  = (PixelV)     { In_Img[c+4],     In_Img[c+4+R] };
    c0Img_11  = (PixelV)     { In_Img[c+4+2*R], In_Img[c+4+3*R] };
    c0Img_12  = (PixelV)     { In_Img[c+4+4*R], 0  };

    //second column window
    c1Img_0   = *((PixelV *) (&In_Img[c+1])      );
    c1Img_1   = *((PixelV *) (&In_Img[c+1+2])    );
    c1Img_2   = *((PixelV *) (&In_Img[c+1+R])    );
    c1Img_3   = *((PixelV *) (&In_Img[c+1+2+R])  );
    c1Img_4   = *((PixelV *) (&In_Img[c+1+2*R])  );
    c1Img_5   = *((PixelV *) (&In_Img[c+1+2+2*R]));
    c1Img_6   = *((PixelV *) (&In_Img[c+1+3*R])  );
    c1Img_7   = *((PixelV *) (&In_Img[c+1+2+3*R]));
    c1Img_8   = *((PixelV *) (&In_Img[c+1+4*R])  );
    c1Img_9   = *((PixelV *) (&In_Img[c+1+2+4*R]));
    c1Img_10  = (PixelV)     { In_Img[c+1+4],     In_Img[c+1+4+R] };
    c1Img_11  = (PixelV)     { In_Img[c+1+4+2*R], In_Img[c+1+4+3*R] };
    c1Img_12  = (PixelV)     { In_Img[c+1+4+4*R], 0  };

    for (r=1; r < R-4; r+=2) {

        //first column window
        S00 = dotp(c0Img_0,coeff_0);
        S00 = sumdotp(c0Img_1,  coeff_1, S00);
        S00 = sumdotp(c0Img_2,  coeff_2, S00);
        S00 = sumdotp(c0Img_3,  coeff_3, S00);
        S00 = sumdotp(c0Img_4,  coeff_4, S00);
        S00 = sumdotp(c0Img_5,  coeff_5, S00);
        S00 = sumdotp(c0Img_6,  coeff_6, S00);
        S00 = sumdotp(c0Img_7,  coeff_7, S00);
        S00 = sumdotp(c0Img_8,  coeff_8, S00);
        S00 = sumdotp(c0Img_9,  coeff_9, S00);
        S00 = sumdotp(c0Img_10,coeff_10, S00);
        S00 = sumdotp(c0Img_11,coeff_11, S00);
        S00 = sumdotp(c0Img_12,coeff_12, S00);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S00 = addnr(S00,DATA_WIDTH-1,ROUNDBIT);

        //second column window
        S01 = dotp(c1Img_0,coeff_0);
        S01 = sumdotp(c1Img_1,  coeff_1, S01);
        S01 = sumdotp(c1Img_2,  coeff_2, S01);
        S01 = sumdotp(c1Img_3,  coeff_3, S01);
        S01 = sumdotp(c1Img_4,  coeff_4, S01);
        S01 = sumdotp(c1Img_5,  coeff_5, S01);
        S01 = sumdotp(c1Img_6,  coeff_6, S01);
        S01 = sumdotp(c1Img_7,  coeff_7, S01);
        S01 = sumdotp(c1Img_8,  coeff_8, S01);
        S01 = sumdotp(c1Img_9,  coeff_9, S01);
        S01 = sumdotp(c1Img_10,coeff_10, S01);
        S01 = sumdotp(c1Img_11,coeff_11, S01);
        S01 = sumdotp(c1Img_12,coeff_12, S01);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S01 = addnr(S01,DATA_WIDTH-1,ROUNDBIT);

        //next line, first column

        new_data1 = *((PixelV *) (&In_Img[c+(r+4)*R]));
        new_data2 = *((PixelV *) (&In_Img[c+2+(r+4)*R]));
        new_data3 =   (PixelV  ) {In_Img[c+4+(r+4)*R], 0};

        c0Img_0  = c0Img_2;
        c0Img_1  = c0Img_3;
        c0Img_2  = c0Img_4;
        c0Img_3  = c0Img_5;
        c0Img_4  = c0Img_6;
        c0Img_5  = c0Img_7;
        c0Img_6  = c0Img_8;
        c0Img_7  = c0Img_9;
        c0Img_8  = new_data1;
        c0Img_9  = new_data2;
        c0Img_10 = shuffle(c0Img_10, c0Img_11, mask0);
        c0Img_11 = shuffle(c0Img_11, c0Img_12, mask0);
        c0Img_12 = new_data3;

        //next line, second column

        new_data1 = *((PixelV *) (&In_Img[c+1+(r+4)*R]));
        new_data2 = *((PixelV *) (&In_Img[c+1+2+(r+4)*R]));
        new_data3 =   (PixelV  ) {In_Img[c+1+4+(r+4)*R], 0};

        c1Img_0  = c1Img_2;
        c1Img_1  = c1Img_3;
        c1Img_2  = c1Img_4;
        c1Img_3  = c1Img_5;
        c1Img_4  = c1Img_6;
        c1Img_5  = c1Img_7;
        c1Img_6  = c1Img_8;
        c1Img_7  = c1Img_9;
        c1Img_8  = new_data1;
        c1Img_9  = new_data2;
        c1Img_10 = shuffle(c1Img_10, c1Img_11, mask0);
        c1Img_11 = shuffle(c1Img_11, c1Img_12, mask0);
        c1Img_12 = new_data3;

        //first column window
        S10 = dotp(c0Img_0,coeff_0);
        S10 = sumdotp(c0Img_1,  coeff_1, S10);
        S10 = sumdotp(c0Img_2,  coeff_2, S10);
        S10 = sumdotp(c0Img_3,  coeff_3, S10);
        S10 = sumdotp(c0Img_4,  coeff_4, S10);
        S10 = sumdotp(c0Img_5,  coeff_5, S10);
        S10 = sumdotp(c0Img_6,  coeff_6, S10);
        S10 = sumdotp(c0Img_7,  coeff_7, S10);
        S10 = sumdotp(c0Img_8,  coeff_8, S10);
        S10 = sumdotp(c0Img_9,  coeff_9, S10);
        S10 = sumdotp(c0Img_10,coeff_10, S10);
        S10 = sumdotp(c0Img_11,coeff_11, S10);
        S10 = sumdotp(c0Img_12,coeff_12, S10);

        // Rounding and Normalization: Data are Q2.2*(DATA_WIDTH-1), now Q2.DATA_WIDTH-1
        S10 = addnr(S10,DATA_WIDTH-1,ROUNDBIT);

        //second column window
        S11 = dotp(c1Img_0,coeff_0);
        S11 = sumdotp(c1Img_1,  coeff_1, S11);
        S11 = sumdotp(c1Img_2,  coeff_2, S11);
        S11 = sumdotp(c1Img_3,  coeff_3, S11);
        S11 = sumdotp(c1Img_4,  coeff_4, S11);
        S11 = sumdotp(c1Img_5,  coeff_5, S11);
        S11 = sumdotp(c1Img_6,  coeff_6, S11);
        S11 = sumdotp(c1Img_7,  coeff_7, S11);
        S11 = sumdotp(c1Img_8,  coeff_8, S11);
        S11 = sumdotp(c1Img_9,  coeff_9, S11);
        S11 = sumdotp(c1Img_10,coeff_10, S11);
        S11 = sumdotp(c1Img_11,coeff_11, S11);
        S11 = sumdotp(c1Img_12,coeff_12, S11);

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

        //next line, first column
        new_data1 = *((PixelV *) (&In_Img[c+(r+5)*R]));
        new_data2 = *((PixelV *) (&In_Img[c+2+(r+5)*R]));
        new_data3 =   (PixelV  ) {In_Img[c+4+(r+5)*R], 0};

        c0Img_0  = c0Img_2;
        c0Img_1  = c0Img_3;
        c0Img_2  = c0Img_4;
        c0Img_3  = c0Img_5;
        c0Img_4  = c0Img_6;
        c0Img_5  = c0Img_7;
        c0Img_6  = c0Img_8;
        c0Img_7  = c0Img_9;
        c0Img_8  = new_data1;
        c0Img_9  = new_data2;
        c0Img_10 = shuffle(c0Img_10, c0Img_11, mask0);
        c0Img_11 = shuffle(c0Img_11, c0Img_12, mask0);
        c0Img_12 = new_data3;

        //next line, second column

        new_data1 = *((PixelV *) (&In_Img[c+1+(r+5)*R]));
        new_data2 = *((PixelV *) (&In_Img[c+1+2+(r+5)*R]));
        new_data3 =   (PixelV  ) {In_Img[c+1+4+(r+5)*R], 0};

        c1Img_0  = c1Img_2;
        c1Img_1  = c1Img_3;
        c1Img_2  = c1Img_4;
        c1Img_3  = c1Img_5;
        c1Img_4  = c1Img_6;
        c1Img_5  = c1Img_7;
        c1Img_6  = c1Img_8;
        c1Img_7  = c1Img_9;
        c1Img_8  = new_data1;
        c1Img_9  = new_data2;
        c1Img_10 = shuffle(c1Img_10, c1Img_11, mask0);
        c1Img_11 = shuffle(c1Img_11, c1Img_12, mask0);
        c1Img_12 = new_data3;

    }

  }
}
#endif