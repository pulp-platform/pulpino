#include "bench.h"
#include "utils.h"
#include "bar.h"
#include "timer.h"
#include "string_lib.h"

#include "matrixMul8.h"

void check_matrix_mul          (testresult_t *result, void (*start)(), void (*stop)());
void check_matrix_mul_dotp     (testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "matrixMul",          .test = check_matrix_mul           },
#ifdef DOTP
  { .name = "matrixMulDotp     ", .test = check_matrix_mul_dotp      },
#endif
  {0, 0}
};

int main() {
  run_suite(testcases);
  return 0;
}


void check_matrix_mul(testresult_t *result, void (*start)(), void (*stop)()) {
  
  int N = SIZE;
  int M = SIZE;
  
  signed char matA[N*M];
  signed char matB[N*M];
  signed int  matC[N*M];

  matrix_init(matA,matB,matC);

  // start benchmark
  start();

  matMul8(matA, matB, matC, N, M);
  
  stop();

  result->errors = matrix_check(matC);
}


void check_matrix_mul_dotp(testresult_t *result, void (*start)(), void (*stop)()) {

  int N = SIZE;
  int M = SIZE;

  signed char matA[N*M];
  signed char matB[N*M];
  signed int  matC[N*M];

  matrix_init(matA, matB, matC);

  // start benchmark
  start();

  matMul8_t_dot(matA, matB, matC, N, M);
  
  stop();

  result->errors = matrix_check(matC);
}
