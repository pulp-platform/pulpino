#include "convolution.h"

#define shuffle(a, b, c)     __builtin_shuffle(a, b, c)
#define clipu(a, zero, max)  __builtin_pulp_clipu(a, zero, max)
#define SumDotp(a, b, c)     __builtin_pulp_sdotusp4(a, b, c)
#define Dotp(a, b)           __builtin_pulp_dotusp4(a, b)


void  __attribute__ ((noinline)) Conv3x3_Byte_Scalar  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
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
                S += (int)(coeff*data);
	    }
	}
	S          = S >> FILT_SHIFT; //divide by 2^FILT_SHIFT
        //saturation
	S = (S > 255) ? 255 : S;
	S = (S < 0)   ?   0 : S;

	Out_Img[t] = (Pixel)(S);
    }
  }
}

void  __attribute__ ((noinline)) Conv3x3_Byte_Vector  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{

  FiltcV coeff_0, coeff_1, coeff_2;
  PixelV Img_0, Img_1, Img_2;
  PixelV new_data;
  int r, c, k, i, j, w, t;
  PixelV mask0,mask1,mask2;
  int S;
  
  coeff_0 = (FiltcV)  {Kernel[0], Kernel[1], Kernel[2], Kernel[3]};
  coeff_1 = (FiltcV)  {Kernel[4], Kernel[5], Kernel[6], Kernel[7]};
  coeff_2 = (FiltcV)  {Kernel[8], 0, 0, 0};

  mask0 = (PixelV){4, 0, 1, 2};
  mask1 = (PixelV){7, 0, 1, 2};
  mask2 = (PixelV){7,0,0,0};

  //image board is black
  for (c=0; c < C-2; c++) {
    r = 0;
    Img_0   = (PixelV)  {In_Img[c], In_Img[c+1], In_Img[c+2], In_Img[c+R]};
    Img_1   = (PixelV)  {In_Img[c+1+R], In_Img[c+2+R], In_Img[c+2*R], In_Img[c+1+2*R]};
    Img_2   = (PixelV)  {In_Img[c+2+2*R], 0, 0, 0};

    for (r=1; r < R-2; r++) {

        S = 0;
        S = Dotp(Img_0,coeff_0);
        S = SumDotp(Img_1,coeff_1, S);
        S = SumDotp(Img_2,coeff_2, S);

        S = S >> FILT_SHIFT; //divide by 2^FILT_SHIFT
        //saturation, S in (0, 2^(9-1) -1) -> (0,255)
        S = clipu(S,0,255);

        t = r*R + c+1;

        Out_Img[t] = (Pixel)S;

        new_data = (PixelV)  {In_Img[(r+2)*R+c], In_Img[(r+2)*R+c+1], In_Img[(r+2)*R+c+2], 0};

        // Move the window 
        /*
          Three vectors:

          Img_0 = {A0, A1, A2, A3}
          Img_1 = {B0, B1, B2, B3}
          Img_2 = {C0, 0,  0,  0}

          Current Windonw:
          XX XX XX
          A0 A1 A2
          A3 B0 B1
          B2 B3 C0
          D0 D1 D2
          XX XX XX

          We want to load next line (D0, D1, D2) in vector new_data
          new_data = {D0, D1, D2, 0} 

          Shuffle:

          1) Img_2 = {C0, D0, D1, D2}
          2) Img_0 = {A3, B0, B1, B2}
          3) Img_1 = {B3, C0, D0, D1}
          4) Img_2 = {D2, 0,  0,  0}
        */

        Img_2 = shuffle(Img_2, new_data,  mask0);
        Img_0 = shuffle(Img_0, Img_1,     mask1);
        Img_1 = shuffle(Img_1, Img_2,     mask1);
        Img_2 = shuffle(Img_2, (PixelV)0, mask2);

    }
    //last iteration, r=R-2
    S = 0;
    S = Dotp(Img_0,coeff_0);
    S = SumDotp(Img_1,coeff_1, S);
    S = SumDotp(Img_2,coeff_2, S);
    S = S >> FILT_SHIFT; //divide by 2^FILT_SHIFT
    //saturation, S in (0, 2^(9-1) -1) -> (0,255)
    S = clipu(S,0,255);
    t = r*R + c+1;
    Out_Img[t] = (Pixel)S;
  }
}
