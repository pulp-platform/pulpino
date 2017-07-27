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
#include "matmul_impl.h"
#include "common.h"
#include "matmul_impl.h"

typedef uint8_t elem_t;

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
