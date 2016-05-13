#include "matrixMul8.h"

typedef signed char   Filtc;
typedef signed char   FiltcV __attribute__((vector_size (4)));

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

void __attribute__ ((noinline)) matMul8_t_dot(Filtc * __restrict__ A, Filtc * __restrict__ B, signed int * __restrict__ C, int N, int M) {

  FiltcV VA;
  FiltcV VB;
  FiltcV *VectInA;
  FiltcV *VectInB;
  int S;
  
  Filtc Btmp[N*M];

  // transpose array before using it
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      Btmp[i*N+j] = B[j*N+i];
    }
  }
  for (int i = 0; i < N; i++) {
    VectInA = (FiltcV*)(&A[i*N]);
    for (int j = 0; j < M; j++) {
      S = 0;
      VectInB = (FiltcV*)(&Btmp[j*N]);
      for (int k = 0; k < (N>>2); k++) {
	VA = VectInA[k];
	VB = VectInB[k];
	S = SumDotp(VA, VB, S);
      }
      C[i*N+j] = S;
    }
  }
}


// helper functions
void __attribute__ ((noinline)) matrix_init(signed char * __restrict__ A, signed char * __restrict__ B, signed int * __restrict__ C) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      A[i*SIZE+j] = m_a[i * SIZE + j];
      B[i*SIZE+j] = m_b[i * SIZE + j];
      C[i*SIZE+j] = 0;
    }
  }
}

unsigned int __attribute__ ((noinline)) matrix_check(signed int * __restrict__ C) {
  unsigned int errors = 0;
  // check
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (C[i*SIZE+j] != m_exp[i * SIZE + j]) {
	//printf("At index %d, %d\n", i, j);
        errors++;
      }
    }
  }
  return errors;
}
