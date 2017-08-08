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

/* Stencil code */
#include "stencil.h"

void stencil(int*,int*,int*);
void getEntries(int*,int*,int*,int*,unsigned int, unsigned int);

__attribute__ ((section(".heapsram"))) int h_R[N*M];
__attribute__ ((section(".heapsram"))) int A[N*M];
__attribute__ ((section(".heapsram"))) int W[N];

__attribute__ ((section(".heapsram"))) int neighbors[4];
__attribute__ ((section(".heapsram"))) int weights[4];

void check(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "stencil",   .test = check       },
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
  int i,j,k;
  int error = 0;

  printf("Start stencil\n");

  for (i=0;i<N;i++) {
    for (k=0;k<M;k++)
      A[i*M+k] = i+k+1;
      W[i] = i+2;
  }

  start();
  stencil(A, h_R, W);
  stop();

  for (i=0;i<N;i++) {
    for (k=0;k<M;k++) {
      if (RESULT_STENCIL[i*M+k] != h_R[i*M+k]) {
        result->errors++;
        printf("Error occurred at i=%d k=%d; Computed result R=%d does not match expected Result=%d\n",i,k,h_R[i*M+k],RESULT_STENCIL[i*M+k]);
      }
    }
  }
}
void stencil(int* A, int* h_R, int* W)
{

  unsigned int c,d;

  neighbors[0] = 0;
  neighbors[1] = 0;
  neighbors[2] = 0;
  neighbors[3] = 0;
  weights[0] = 0;
  weights[1] = 0;
  weights[2] = 0;
  weights[3] = 0;

  for (c = 0 ; c < N; c++)
  {
    for (d = 0 ; d < M; d++)
    {
      getEntries(neighbors, weights, A, W, c, d);
      h_R[c*M+d] = A[c*M+d] + neighbors[3]*weights[3] + neighbors[0]*weights[0] + neighbors[1]*weights[1] + neighbors[2]*weights[2];
    }
  }
}

void getEntries(int* neighbors, int* weights, int* A, int* W, unsigned int c, unsigned int d)
{
  // top&buttom pixel
  if (c==0) {
    neighbors[0] = 0;
    neighbors[3] = A[(c+1)*M+d];
    weights[0] = 0;
    weights[3] = W[c+1];
  }
  else if (c==N-1) {
    neighbors[0] = A[(c-1)*M+d];
    neighbors[3] = 0;
    weights[0] = W[c-1];
    weights[3] = 0;
  }
  else {
    neighbors[0] = A[(c-1)*M+d];
    neighbors[3] = A[(c+1)*M+d];
    weights[0] = W[c-1];
    weights[3] = W[c+1];
  }

  // left&right pixel
  if (d==0) {
    neighbors[1] = 0;
    neighbors[2] = A[c*M+d+1];
    weights[1] = 0;
    weights[2] = W[c];
  }
  else if (d==M-1) {
    neighbors[1] = A[c*M+d-1];
    neighbors[2] = 0;
    weights[1] = W[c];
    weights[2] = 0;
  }
  else {
    neighbors[1] = A[c*M+d-1];
    neighbors[2] = A[c*M+d+1];
    weights[1] = W[c];
    weights[2] = W[c];
  }
}
