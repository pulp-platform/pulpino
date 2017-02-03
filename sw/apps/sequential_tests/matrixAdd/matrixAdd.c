
#include "utils.h"
#include "timer.h"
#include "string_lib.h"
#include "bench.h"

#if WORD == 32
#include "matrixAdd32.h"
#elif WORD == 16
#include "matrixAdd16.h"
#define VecSize 2
typedef short vector_type __attribute__((vector_size (4)));
#elif WORD == 8
#include "matrixAdd8.h"
#define VecSize 4
typedef char vector_type __attribute__((vector_size (4)));
#else
#error No word size specified
#endif



void check_matrix_add(testresult_t *result, void (*start)(), void (*stop)());
void check_matrix_addV(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "matrix_add",   .test = check_matrix_add       },
#ifdef USE_VEC
  { .name = "matrix_addV",  .test = check_matrix_addV      },
#endif
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
#ifdef USE_VEC
void check_matrix_addV(testresult_t *result, void (*start)(), void (*stop)()) {

  vector_type* v_a;
  vector_type* v_b;
  vector_type* v_c;

  start();

  unsigned int i, j;
  for(i = 0; i < HEIGHT; i+=VecSize) {
    v_a = (vector_type *)(&m_a[i]);
    v_b = (vector_type *)(&m_b[i]);
    v_c = (vector_type *)(&m_c[i]);
    for(j = 0; j < WIDTH; j+=VecSize) {
      v_c[j] = v_a[j] + v_b[j];
    }
  }
  // ... assume no corner case in matrix ... //

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
#endif