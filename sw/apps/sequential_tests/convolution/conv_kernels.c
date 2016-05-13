#include "convolution.h"

#define SumDotp(a, b, c) __builtin_pulp_sdotsp4(a, b, c)
#define Dotp(a, b)       __builtin_pulp_dotsp4(a, b)

void  __attribute__ ((noinline)) Conv5x5_Byte(Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M, FiltcV * __restrict__ Coeff)
{
  int i, j;
  FiltcV V0, V1, V2, V3, V4, V5, V6;
  /* Move all Kernel coeffs into registers */
  FiltcV C0 = Coeff[0];
  FiltcV C1 = Coeff[1];
  FiltcV C2 = Coeff[2];
  FiltcV C3 = Coeff[3];
  FiltcV C4 = Coeff[4];
  FiltcV C5 = Coeff[5];
  FiltcV C6 = Coeff[6];
  FiltcV Mask  = {5,6,7,0};
  FiltcV Mask1 = {4,5,0,1};
  FiltcV Mask2 = {4,5,6,0};
  /* For fresh data */
  FiltcV *VectIn;
  Filtc *O;
  int NormFactor = 32767/25;	/* 1/25 in Q15 */
  int S;

  N = COL;	/* To force static value in N, simpler loop setup */
  M = LINE;	/* To force static value in M, simpler loop setup */

  /* Half kernel size has to be removed left, right, top and bottom for proper and safe operations */
  for (i=0; i<(N-4); i++) {
    /* Prime data cache, all in register */
    V1 = __builtin_shuffle(*((FiltcV *)(In+i)), (FiltcV) {0, 0, 1, 2});
    V2 = *((FiltcV *)(In+3     +i));
    V3 = *((FiltcV *)(In+N     +i));
    V2 = __builtin_shuffle(V2, V3, Mask1);
    V3 = *((FiltcV *)(In+N+2   +i));
    V4 = *((FiltcV *)(In+2*N   +i));
    V3 = __builtin_shuffle(V3, V4, Mask2);
    V4 = *((FiltcV *)(In+2*N+1 +i));
    V5 = *((FiltcV *)(In+3*N   +i));
    V6 = *((FiltcV *)(In+3*N+4 +i));
    /* VectIn points, as a vector to the top left corner of the current vertical strip */
    VectIn = (FiltcV *) (In + 4*N + i);
    /* O points, to the center of the first kernel to be evaluated at the top of current vertical strip */
    O = Out + 2*N + i + 2;
    /* Process one full vertical strip, width = kernel width */
    for (j=0; j<(M-4); j++) {
      /* Retire first line of the data set under the kernel: V0 and first element of V1 */
      V0 = __builtin_shuffle(V1, V2, Mask);
      V1 = __builtin_shuffle(V2, V3, Mask);
      V2 = __builtin_shuffle(V3, V4, Mask);
      V3 = __builtin_shuffle(V4, V5, Mask);
      V4 = __builtin_shuffle(V5, V6, Mask);
      /* Bring in first part of fresh new line of data in the set under the kernel */
      V5 = *VectIn; VectIn++;
      V6 = *VectIn;
      /* Goes to next line of In, ready to parse next fresh 5 in data */
      VectIn += ((N>>2)-1);
      /* The convolution itself, first a dot product producing an accumulator that is used in subsequent
	 sum of products */
      S = Dotp(V0, C0);
      S = SumDotp(V1, C1, S);
      S = SumDotp(V2, C2, S);
      S = SumDotp(V3, C3, S);
      S = SumDotp(V4, C4, S);
      S = SumDotp(V5, C5, S);
      S = SumDotp(V6, C6, S);
      *O = (S*NormFactor)>>15;
      O+=N;
    }
  }
}

void  __attribute__ ((noinline)) Conv3x3_Byte(Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M, FiltcV * __restrict__ Coeff)

{
  int i, j;
  FiltcV V0, V1, V2;
  /* Move all Kernel coeffs into registers */
  FiltcV C0 = Coeff[0];
  FiltcV C1 = Coeff[1];
  FiltcV C2 = Coeff[2];
  /* Pointer to read fresh data */
  FiltcV *VectIn;
  /* Pointer to center of kernel */
  Filtc *O;
  int NormFactor = 32767/9;	/* 1/9 in Q15 */
  int S;

  N = COL;	/* To force static value in N, simpler loop setup */
  M = LINE;	/* To force static value in M, simpler loop setup */

  /* Half kernel size has to be removed left, right, top and bottom for proper and safe operations */
  for (i=0; i<(N-2); i++) {
	  
    // load first 2 lines (2*3 pixels)
    V1 = (FiltcV) {In[M*(0)+0+i], In[M*(0)+1+i], In[M*(0)+2+i], 0};
    V2 = (FiltcV) {In[M*(1)+0+i], In[M*(1)+1+i], In[M*(1)+2+i], 0};

    /* VectIn points, as a vector to the top left corner of the current vertical strip */
    VectIn = (FiltcV *) (In + 2*N + i);
    /* O points, to the center of the first kernel to be evaluated at the top of current vertical strip */
    O = Out + N + i + 1;
    /* Process one full vertical strip, width = kernel width */
    for (j=0; j<(M-2); j++) {
      /* Retire first line of the data set under the kernel: 3 first elements of V0 */
      V0 = V1;
      V1 = V2;
      V2 = *VectIn;
      /* Goes to next line of In, ready to parse next fresh 5 in data */
      VectIn += (N>>2);
      /* The convolution itself, first a dot product producing an accumulator that is used in subsequent
	 sum of products */
      S = Dotp(V0, C0);
      S = SumDotp(V1, C1, S);
      S = SumDotp(V2, C2, S);
      *O = (S*NormFactor)>>15;
      O+=N;
    }
  }
}

void  __attribute__ ((noinline)) Conv5x5_Byte_Scalar(Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M, Filtc * __restrict__ Coeff)

{
  int i, j, k;
  int NormFactor = 32767/25;	/* 1/25 in Q15 */
  int S;

  N = COL;	/* To force static value in N, simpler loop setup */
  M = LINE;	/* To force static value in M, simpler loop setup */
  for (i=0; i<(M-4); i++) {
    for (j=0; j<(N-4); j++) {
      S = 0;
      for (k=0; k<5;k++) {
	S += In[M*(j  ) + i + k ]*Coeff[k ];
      }
      for (k=0; k<5;k++) {
	S += In[M*(j+1  ) + i + k ]*Coeff[5+k ];
      }
      for (k=0; k<5;k++) {
	S += In[M*(j+2  ) + i + k ]*Coeff[10+k ];
      }
      for (k=0; k<5;k++) {
	S += In[M*(j+3  ) + i + k ]*Coeff[15+k ];
      }
      for (k=0; k<5;k++) {
	S += In[M*(j+4  ) + i + k ]*Coeff[20+k ];
      }
      Out[M*(j+2)+(i+2)] = (S*NormFactor)>>15;
    }

		
  }
}

void  __attribute__ ((noinline)) Conv3x3_Byte_Scalar(Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M, Filtc * __restrict__ Coeff)

{
  int i, j;
  int NormFactor = 32767/9;	/* 1/9 in Q15 */
  int S;

  N = COL;	/* To force static value in N, simpler loop setup */
  M = LINE;	/* To force static value in M, simpler loop setup */

  for (i=0; i<(M-2); i++) {
    for (j=0; j<(N-2); j++) {
      S = In[M*(j  ) + i  ]*Coeff[0] + In[M*(j  ) +i +1]*Coeff[1] + In[M*(j  ) + i+2]*Coeff[2] +
	In[M*(j+1) + i  ]*Coeff[3] + In[M*(j+1) +i +1]*Coeff[4] + In[M*(j+1) + i+2]*Coeff[5] +
	In[M*(j+2) + i  ]*Coeff[6] + In[M*(j+2) +i +1]*Coeff[7] + In[M*(j+2) + i+2]*Coeff[8];
      Out[M*(j+1)+(i+1)] = (S*NormFactor)>>15;

    }
  }
}


