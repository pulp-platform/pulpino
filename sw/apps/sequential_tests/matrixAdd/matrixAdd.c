
#include "utils.h"
#include "timer.h"
#include "string_lib.h"
#include "bench.h"

#if WORD == 32
#include "matrixAdd32.h"
#elif WORD == 16
#include "matrixAdd16.h"
#elif WORD == 8
#include "matrixAdd8.h"
#else
#error No word size specified
#endif

void check_matrix_add(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "matrix_add",   .test = check_matrix_add       },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}


void check_matrix_add(testresult_t *result, void (*start)(), void (*stop)()) {

  start();

  unsigned int i, j;
  for(i = 0; i < HEIGHT; i++) {
    for(j = 0; j < WIDTH; j++) {
      m_c[i][j] = m_a[i][j] + m_b[i][j];
    }
  }

  stop();

  // check
  for(i = 0; i < WIDTH; i++) {
    for(j = 0; j < HEIGHT; j++) {
      if(m_c[i][j] != m_exp[i][j]) {
        result->errors++;
        printf("Error at index %d, %d, expected %d, got %d\n", i, j, m_exp[i][j], m_c[i][j]);
      }
    }
  }

}
