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
#include "bench.h"

#define NR_PLATES 12

void ToH(int, char*, char*, char*);

void computeGold(int, char*, char*, char*);

static char A[NR_PLATES];
static char B[NR_PLATES];
static char C[NR_PLATES];

void check(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "towerofhanoi",   .test = check       },
  {0, 0}
};

int main()
{
  int error = 0;
  if(get_core_id()==0){
    error = run_suite(testcases);
  }
  return error;
}

void check(testresult_t *result, void (*start)(), void (*stop)()) {
  int i,j;
  printf("Start towerofhanoi\n");

  for (i=0;i<NR_PLATES;i++){
      A[i] = i;
      B[i] = 0;
      C[i] = 0;
  }
  start();
  computeGold(NR_PLATES, A, B, C);
  stop();

  for (i=0;i<NR_PLATES;i++) {
    if (B[i] != i) {
      printf("expected: %d, actual: %d\n",i,B[i]);
      result->errors++;
    }
  }
}
void computeGold(int n, char* A, char* B, char* C)
{
  ToH(n-1,A,B,C);
}

void ToH(int n, char* A, char* B, char* C)
{
  if(n==0)
    {
      B[n] = A[n];
      A[n] = 0;
    }
  else {
    ToH(n-1,A,C,B);
    B[n] = A[n];
    A[n] = 0;
    ToH(n-1,C,B,A);
  }
}
