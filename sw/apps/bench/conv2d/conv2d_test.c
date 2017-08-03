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

#define IMG_SIZE 10

static int16_t coeffs[] __sram = {
  1, 4, 6, 4, 1,
  4,16,24,16, 4,
  6,24,36,24, 6,
  4,16,24,16, 4,
  1, 4, 6, 4, 1 };

static const int16_t check[IMG_SIZE * IMG_SIZE] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0, 22, 23, 24, 25, 26, 27,  0,  0,
  0,  0, 32, 33, 34, 35, 36, 37,  0,  0,
  0,  0, 42, 43, 44, 45, 46, 47,  0,  0,
  0,  0, 52, 53, 54, 55, 56, 57,  0,  0,
  0,  0, 62, 63, 64, 65, 66, 67,  0,  0,
  0,  0, 72, 73, 74, 75, 76, 77,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

static int16_t img_in[IMG_SIZE * IMG_SIZE] __sram;
static int16_t img_out[IMG_SIZE * IMG_SIZE] __sram;

extern void conv2d(int16_t *, int16_t *, const int16_t *,
                   int, int, int, uint16_t);

void test_setup() {
  for (int i = 0; i != IMG_SIZE*IMG_SIZE; i++)
    img_in[i] = i;
}

void test_clear() {
  memset(img_out, 0, IMG_SIZE*IMG_SIZE);
}

void test_run() {
  conv2d(img_in, img_out, coeffs, IMG_SIZE, IMG_SIZE, 5, 8);
}

int test_check() {
  return 0 == memcmp(img_out, check, IMG_SIZE*IMG_SIZE);
}
