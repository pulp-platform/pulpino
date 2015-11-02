
#include "bench.h"
#include "utils.h"
#include "bar.h"
#include "timer.h"
#include "string_lib.h"

#include "parMatrixMul16_stimuli.h"

void check_matrix_mul(testresult_t *result, void (*start)(), void (*stop)());
void check_matrix_mul_transpose(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "matrixMulTranspose", .test = check_matrix_mul_transpose },
  { .name = "matrixMul",          .test = check_matrix_mul           },
  {0, 0}
};

int num_cores = 1;

int main()
{
  run_suite(testcases);

  eoc(0);

  return 0;
}

void matrix_init();
unsigned int matrix_check();

void check_matrix_mul(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i, j, k;
  unsigned int chunk;
  unsigned int lb, ub;

  // number of rows each core has to multiply
  chunk = SIZE / num_cores;
  // lower bound
  lb = 0;
  // upper bound
  ub = lb + chunk;

  matrix_init();

  // start benchmark
  start();

  for(i = lb; i < ub; i++) {
    for(j = 0; j < SIZE; j++) {
      g_mC[i][j] = 0;

      for(k = 0; k < SIZE; k++) {
        g_mC[i][j] += g_mA[i][k] * g_mB[k][j];
      }
    }
  }


  stop();

  result->errors = matrix_check();


}

void check_matrix_mul_transpose(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i, j, k;
  unsigned int chunk;
  unsigned int lb, ub;

  // number of rows each core has to multiply
  chunk = SIZE / num_cores;
  // lower bound
  lb = 0;
  // upper bound
  ub = lb + chunk;


  matrix_init();


  // start benchmark
  start();

  // transpose array before using it
  for(i = lb; i < ub; i++) {
    for(j = 0; j < SIZE; j++) {
      g_mB_tmp[i][j] = g_mB[j][i];
    }
  }


  for(i = lb; i < ub; i++) {
    for(j = 0; j < SIZE; j++) {
      g_mC[i][j] = 0;

      for(k = 0; k < SIZE; k++) {
        g_mC[i][j] += g_mA[i][k] * g_mB_tmp[j][k];
      }
    }
  }


  stop();


  result->errors = matrix_check();


}

void matrix_init() {
  unsigned int i, j;

  // init, copy to TCDM
  for(i = 0; i < SIZE; i++) {
    for(j = 0; j < SIZE; j++) {
      g_mA[i][j] = m_a[i * SIZE + j];
      g_mB[i][j] = m_b[i * SIZE + j];
      g_mC[i][j] = 0;
    }
  }
}

unsigned int matrix_check() {
  unsigned int errors = 0;
  unsigned int i, j;
  // check
  for(i = 0; i < SIZE; i++) {
    for(j = 0; j < SIZE; j++) {
      if(g_mC[i][j] != m_exp[i * SIZE + j]) {
        printf("At index %d, %d\n", i, j, 0, 0);
        errors++;
      }
    }
  }

  return errors;
}
