// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#include "../common.h"

void fir(const int16_t *arr, const int16_t *coeff, int16_t *output,
         unsigned arr_length, unsigned coeff_len) {

  for (unsigned i = 0; i < arr_length - coeff_len; ++i) {
    uint16_t sum = 0;
    for (unsigned j = 0; j < coeff_len; ++j) {
      sum += arr[i+j] * coeff[j];
    }
    
    output[i] = sum;
  }
}
