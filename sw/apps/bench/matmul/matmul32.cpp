// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
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
