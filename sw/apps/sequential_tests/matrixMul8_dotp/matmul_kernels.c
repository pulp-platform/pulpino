/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

typedef signed char charV __attribute__((vector_size (4)));

#define SumDotp(a, b, c) __builtin_pulp_sdotsp4(a, b, c)
#define Dotp(a, b)       __builtin_pulp_dotsp4(a, b)

void __attribute__ ((noinline)) matMul8(signed char * __restrict__ A, signed char * __restrict__ B, signed int * __restrict__ C, int N, int M) {
  
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      C[i*N+j] = 0;

      for (int k = 0; k < N; k++) {
        C[i*N+j] += A[i*N+k] * B[k*N+j];
      }
    }
  }
}
#ifdef USE_VEC
void __attribute__ ((noinline)) matMul8_t_dot(char * __restrict__ A, char * __restrict__ B, signed int * __restrict__ C, int N, int M) {

  charV VA;
  charV VB;
  charV *VectInA;
  charV *VectInB;
  int S;
  
  char Btmp[N*M];

  // transpose array before using it
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      Btmp[i*N+j] = B[j*N+i];
    }
  }
  for (int i = 0; i < N; i++) {
    VectInA = (charV*)(&A[i*N]);
    for (int j = 0; j < M; j++) {
      S = 0;
      VectInB = (charV*)(&Btmp[j*N]);
      for (int k = 0; k < (N>>2); k++) {
          VA = VectInA[k];
          VB = VectInB[k];
          S = SumDotp(VA, VB, S);
      }
      C[i*N+j] = S;
    }
  }
}
#endif