// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


#include "bench.h"
#include "utils.h"
#include "bar.h"
#include "timer.h"
#include "string_lib.h"
#include "gpio.h"

#include "parMatrixMul8_stimuli.h"

void check_matrix_mul(testresult_t *result, void (*start)(), void (*stop)());
void check_matrix_mul_transpose(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "matrixMul",          .test = check_matrix_mul           },
  { .name = "matrixMulTranspose", .test = check_matrix_mul_transpose },
  {0, 0}
};

int main() {
  return run_suite(testcases);
}

void matrix_init();
unsigned int matrix_check();

void check_matrix_mul(testresult_t *result, void (*start)(), void (*stop)()) {
  matrix_init();

  // start benchmark
  start();

  set_gpio_pin_direction(0, DIR_OUT);
  set_gpio_pin_value(0, 1);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      g_mC[i][j] = 0;

      for (int k = 0; k < SIZE; k++) {
        g_mC[i][j] += g_mA[i][k] * g_mB[k][j];
      }
    }
  }


  set_gpio_pin_value(0, 0);
  stop();

  result->errors = matrix_check();
}

void check_matrix_mul_transpose(testresult_t *result, void (*start)(), void (*stop)()) {

  matrix_init();

  // start benchmark
  start();

  // transpose array before using it
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      g_mB_tmp[i][j] = g_mB[j][i];
    }
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      g_mC[i][j] = 0;

      for (int k = 0; k < SIZE; k++) {
        g_mC[i][j] += g_mA[i][k] * g_mB_tmp[j][k];
      }
    }
  }

  stop();

  result->errors = matrix_check();
}

void matrix_init() {
  // init, reindex - legacy from PULP
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      g_mA[i][j] = m_a[i * SIZE + j];
      g_mB[i][j] = m_b[i * SIZE + j];
      g_mC[i][j] = 0;
    }
  }
}

unsigned int matrix_check() {
  unsigned int errors = 0;
  // check
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (g_mC[i][j] != m_exp[i * SIZE + j]) {
        printf("result[%d][%d] is %d, expected %d\n", i, j, g_mC[i][j], m_exp[i * SIZE + j]);
        errors++;
      }
    }
  }

  return errors;
}
