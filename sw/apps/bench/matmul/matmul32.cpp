#include "matmul_impl.h"
#include "common.h"
#include "matmul_impl.h"

typedef uint32_t elem_t;

#ifndef TRANSPOSE_B
extern "C"
void matmul(const elem_t * __restrict__ A, const elem_t * __restrict__ B,
            elem_t * __restrict__ C,
            unsigned N, unsigned L, unsigned M) {
  matmul<elem_t>(A, B, C, N, L, M);
}
#else
extern "C"
void matmul(const elem_t * __restrict__ A, const elem_t * __restrict__ B,
            elem_t * __restrict__ Bt, elem_t * __restrict__ C,
            unsigned N, unsigned L, unsigned M) {
  matmul_t<elem_t>(A, B, Bt, C, N, L, M);
}
#endif
