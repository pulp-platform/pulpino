// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

typedef signed short shortV __attribute__((vector_size (4)));

#define SumDotp(a, b, c) __builtin_pulp_sdotsp2(a, b, c)
#define Dotp(a, b)       __builtin_pulp_dotsp2(a, b)

void __attribute__ ((noinline)) matMul16(signed short * __restrict__ A, signed short * __restrict__ B, signed int * __restrict__ C, int N, int M) {
  
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
void __attribute__ ((noinline)) matMul16_t_dot(short * __restrict__ A, short * __restrict__ B, signed int * __restrict__ C, int N, int M) {

  shortV VA;
  shortV VB;
  shortV *VectInA;
  shortV *VectInB;
  int S;
  
  short Btmp[N*M];

  // transpose array before using it
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      Btmp[i*N+j] = B[j*N+i];
    }
  }
  for (int i = 0; i < N; i++) {
    VectInA = (shortV*)(&A[i*N]);
    for (int j = 0; j < M; j++) {
      S = 0;
      VectInB = (shortV*)(&Btmp[j*N]);
      for (int k = 0; k < (N>>1); k++) {
          VA = VectInA[k];
          VB = VectInB[k];
          S = SumDotp(VA, VB, S);
      }
      C[i*N+j] = S;
    }
  }
}
#endif