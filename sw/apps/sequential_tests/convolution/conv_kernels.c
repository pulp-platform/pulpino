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
        S = S >> FILT_SHIFT; //divide by 2^FILT_SHIFT
        //saturation TODO: FIX IT
        S = clipu(S,0,127);
        Out_Img[t] = (Pixel)(S);
    }
  }
}

void  __attribute__ ((noinline)) Conv5x5_Byte_Scalar  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
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
        S = S >> FILT_SHIFT; //divide by 2^FILT_SHIFT
        //saturation TODO: FIX IT
        S = clipu(S,0,127);
        Out_Img[t] = (Pixel)S;
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
        //saturation, S in (0, 2^(9-1) -1) -> (0,255) TODO: FIX IT use 255 instead of 255
        S = clipu(S,0,127);

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
    S = clipu(S,0,127);
    t = r*R + c+1;
    Out_Img[t] = (Pixel)S;
  }
}

void  __attribute__ ((noinline)) Conv5x5_Byte_Vector  (Pixel * In_Img, Pixel * Out_Img, int R, int C, Filtc  * Kernel)
{

  FiltcV coeff_0, coeff_1, coeff_2, coeff_3, coeff_4, coeff_5, coeff_6;
  PixelV Img_0, Img_1, Img_2, Img_3, Img_4, Img_5, Img_6;
  PixelV new_data1, new_data2;
  int r, c, k, i, j, w, t;
  PixelV mask0;
  int S;

  coeff_0 = (FiltcV)  {  Kernel[0],  Kernel[1],  Kernel[2],  Kernel[3]};
  coeff_1 = (FiltcV)  {  Kernel[5],  Kernel[6],  Kernel[7],  Kernel[8]};
  coeff_2 = (FiltcV)  { Kernel[10], Kernel[11], Kernel[12], Kernel[13]};
  coeff_3 = (FiltcV)  { Kernel[15], Kernel[16], Kernel[17], Kernel[18]};
  coeff_4 = (FiltcV)  { Kernel[20], Kernel[21], Kernel[22], Kernel[23]};
  coeff_5 = (FiltcV)  {  Kernel[4],  Kernel[9], Kernel[14], Kernel[19]};
  coeff_6 = (FiltcV)  { Kernel[24],          0,          0,          0};

  mask0 = (PixelV){5, 6, 7, 0};

  //image board is black
  for (c=0; c < C-4; c++) {
    r = 0;
    Img_0   = (PixelV)  {       In_Img[c],     In_Img[c+1],     In_Img[c+2],     In_Img[c+3] };
    Img_1   = (PixelV)  {     In_Img[c+R],   In_Img[c+1+R],   In_Img[c+2+R],   In_Img[c+3+R] };
    Img_2   = (PixelV)  {   In_Img[c+2*R], In_Img[c+1+2*R], In_Img[c+2+2*R], In_Img[c+3+2*R] };
    Img_3   = (PixelV)  {   In_Img[c+3*R], In_Img[c+1+3*R], In_Img[c+2+3*R], In_Img[c+3+3*R] };
    Img_4   = (PixelV)  {   In_Img[c+4*R], In_Img[c+1+4*R], In_Img[c+2+4*R], In_Img[c+3+4*R] };
    Img_5   = (PixelV)  {     In_Img[c+4],   In_Img[c+4+R], In_Img[c+4+2*R], In_Img[c+4+3*R] };
    Img_6   = (PixelV)  { In_Img[c+4+4*R],               0,               0,               0 };


    for (r=1; r < R-4; r++) {

        S = 0;
        S = Dotp(Img_0,coeff_0);
        S = SumDotp(Img_1,coeff_1, S);
        S = SumDotp(Img_2,coeff_2, S);
        S = SumDotp(Img_3,coeff_3, S);
        S = SumDotp(Img_4,coeff_4, S);
        S = SumDotp(Img_5,coeff_5, S);
        S = SumDotp(Img_6,coeff_6, S);
        S = S >> FILT_SHIFT; //divide by 2^FILT_SHIFT
        //saturation, S in (0, 2^(9-1) -1) -> (0,255)
        S = clipu(S,0,127);

        t = (r+1)*R + c+2;
        Out_Img[t] = (Pixel)S;


        new_data1 = *((PixelV *)(&In_Img[c+(r+4)*R]));
        new_data2 =   (PixelV)  {In_Img[c+4+(r+4)*R], 0, 0, 0};

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
    //last iteration, r=R-2
    S = 0;
    S = Dotp(Img_0,coeff_0);
    S = SumDotp(Img_1,coeff_1, S);
    S = SumDotp(Img_2,coeff_2, S);
    S = SumDotp(Img_3,coeff_3, S);
    S = SumDotp(Img_4,coeff_4, S);
    S = SumDotp(Img_5,coeff_5, S);
    S = SumDotp(Img_6,coeff_6, S);
    S = S >> FILT_SHIFT; //divide by 2^FILT_SHIFT
    //saturation, S in (0, 2^(9-1) -1) -> (0,255)
    S = clipu(S,0,127);
    t = (r+1)*R + c+2;
    Out_Img[t] = (Pixel)S;
  }
}