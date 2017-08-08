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

void conv2d(int16_t *restrict mat, int16_t * restrict res,
            const int16_t * restrict coeffs,
            int W, int H, int K, uint16_t scf) {
  int R = K >> 1;
  int index = R * W;

  for(int j = R; j < H - R; j++) {
    for(int i = R; i < W - R; i++) {
      int index2 = index - R * W;
      int c = 0;
      int val = 0;
      for(int y = -R; y <= R; ++y) {
        val += coeffs[c++] * mat[index2 + i - 2];
        val += coeffs[c++] * mat[index2 + i -1];
        val += coeffs[c++] * mat[index2 + i];
        val += coeffs[c++] * mat[index2 + i + 1];
        val += coeffs[c++] * mat[index2 + i + 2];
        index2 += W;
      }
      res[index+i] = val >> scf;
    }
    index += W;
  }
}
