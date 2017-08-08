// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "timer.h"
#include "bench.h"

/* Stencil code */
#define N 32
#define M 32

void stencil_vect0();
void stencil_vect1();

__attribute__ ((section(".heapsram"))) char C0[N+8][M+8];
__attribute__ ((section(".heapsram"))) char C1[N+8][M+8];
__attribute__ ((section(".heapsram"))) char A[N+8][M+8];


#define CHKSUM 60480

void check_stencil(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "stencil",          .test = check_stencil        },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}

void check_stencil(testresult_t *result, void (*start)(), void (*stop)())
{
  int i,j,k;

  printf("Start stencil\n");

  for (i=4;i<N+4;i++) {
    for (k=4;k<M+4;k++)
      A[i][k] = (i+k+1)%16;
  }

  for (j = 0; j<2; j++) {

    reset_timer();
    start_timer();

    stencil_vect0();

    stop_timer();

  }

  int time0 = get_time();

  for (j = 0; j<2; j++) {

    reset_timer();
    start_timer();

    stencil_vect1();

    stop_timer();

  }

  int time1 = get_time();

  // check results
  int check_sum0 = 0;
  int check_sum1 = 0;

  for (i=4;i<N+4;i++)
    for (k=4;k<M+4;k++) {
      check_sum0 += (int) C0[i][k];
      check_sum1 += (int) C1[i][k];
    }

  // check test 0
  printf("\nchecksum0 = %d\n",check_sum0);
  if (check_sum0 == CHKSUM) {
    if (time0 < 6000)
      printf("Part Vect OOOOOOK!!!!!! Time: %d\n",time0);
    else
      printf("Part Vect OOOOOOK!!!!!! But time=%d exceeded maximum of 6000 cycles\n",time0);
  }
  else {
    result->errors++;
    printf("Part Vect NOT OK!!!!! Time: %d\n",time0);
  }

  // check test 1
  printf("\nchecksum1 = %d\n",check_sum1);
  if (check_sum0 == CHKSUM) {
    if (time0 < 6000)
      printf("Full Vect OOOOOOK!!!!!! Time: %d\n",time0);
    else
      printf("Full Vect OOOOOOK!!!!!! But time=%d exceeded maximum of 6000 cycles\n",time0);
  }
  else {
    result->errors++;
    printf("Full Vect NOT OK!!!!! Time: %d\n",time0);
  }

}


void stencil_vect0()
{

  unsigned int c,d;

  for (c = 4 ; c < N+4; c++)
  {
    // aligned data
    for (d = 4 ; d < M+4; d++){
      C0[c][d] = 4*A[c][d] + A[c-1][d] + A[c+1][d];
    }
    // unaligned data
    for (d = 4; d < M+4; d++){
      C0[c][d] = C0[c][d] + A[c][d-1] + A[c][d-1];
    }
  }
}

void stencil_vect1()
{

  unsigned int c,d;

  for (c = 4 ; c < N+4; c++)
  {
    // aligned and unaligned data in one loop
    for (d = 4 ; d < M+4; d++){
      C1[c][d] = 4*A[c][d] + A[c-1][d] + A[c+1][d] + A[c][d-1] + A[c][d+1];
    }
  }
}
