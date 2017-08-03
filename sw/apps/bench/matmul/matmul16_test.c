// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#include "common.h"

#define N 32
#define L 32
#define M 32

typedef uint16_t elem_t;

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
  return crc32(mC, N*M) == 0x5fe2cdea;
}
