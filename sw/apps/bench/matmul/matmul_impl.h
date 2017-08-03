// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#ifndef MATMUL_IMPL_H
#define MATMUL_IMPL_H

template<typename ElemTy>
void matmul(const ElemTy *__restrict__ A, const ElemTy *__restrict__ B,
            ElemTy *__restrict__ C, unsigned N, unsigned L, unsigned M) {
  N&=0xFC;
  L&=0xFC;
  M&=0xFC;

  for (unsigned i = 0; i != N; ++i) {
    const ElemTy *rA = A + L * i;
    ElemTy *rC = C + M * i;
    for (unsigned j = 0; j != M; ++j) {
      const ElemTy *cB = B + j;
      
      ElemTy sum = 0;
      for (unsigned k = 0; k != L; ++k) {
        sum += rA[k] * cB[M * k];
      }

      rC[j] = sum;
    }
  }
}

template<typename ElemTy>
void matmul_t(const ElemTy *__restrict__ A, const ElemTy *__restrict__ B, ElemTy *__restrict__ Bt,
              ElemTy *__restrict__ C, unsigned N, unsigned L, unsigned M) {
  N&=~0x3;
  L&=~0x3;
  M&=~0x3;

  for (unsigned i = 0; i != L; ++i) {
    for (unsigned j = 0; j != M; ++j) {
      int idx = i * M  + j;
      int tidx = j * L + i;
      Bt[tidx] = B[idx];
    }
  }

  for (unsigned i = 0; i != N; ++i) {
    const ElemTy *rA = A + L * i;
    ElemTy *rC = C + M * i;
    for (unsigned j = 0; j != M; ++j) {
      const ElemTy *cBt = Bt + L * j;
      
      ElemTy sum = 0;
      for (unsigned k = 0; k != L; ++k) {
        sum += rA[k] * cBt[k];
      }

      rC[j] = sum;
    }
  }
}

#endif
