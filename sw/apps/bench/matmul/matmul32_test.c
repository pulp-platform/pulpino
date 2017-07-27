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
#include "common.h"

#define N 32
#define L 32
#define M 32

typedef uint32_t elem_t;

static elem_t mA[N*L] __sram;
static elem_t mB[L*M] __sram;
static elem_t mC[N*M] __sram;
#ifdef TRANSPOSE_B
static elem_t mBt[L*M] __sram;
#endif

#ifndef TRANSPOSE_B
extern void matmul(const elem_t *, const elem_t *, elem_t *,
                   unsigned, unsigned, unsigned);
#else
extern void matmul(const elem_t *, const elem_t *, elem_t *, elem_t *,
                   unsigned, unsigned, unsigned);
#endif

static void init(elem_t *m, unsigned len) {
  for (unsigned i = 0; i != len; ++i)
    m[i] = i % 4;
} 

void test_setup() {
  init(mA, N*L);
  init(mB, L*M);
  init(mC, N*M);
}

void test_clear() {}

void test_run() {
  #ifndef TRANSPOSE_B
  matmul(mA, mB, mC, N, L, M);
  #else
  matmul(mA, mB, mBt, mC, N, L, M);
  #endif
}

int test_check() {
  return crc32(mC, N*M) == 0x4e9cfec2;
}
