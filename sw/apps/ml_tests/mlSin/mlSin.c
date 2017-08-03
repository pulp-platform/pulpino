// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////
// includes
/////////////////////////////////////////////////////////
#include "mlSin.h"
#include "math_fns.h"
#include "bar.h"
#include "utils.h"
#include "bench.h"

/////////////////////////////////////////////////////////
// shared globals
/////////////////////////////////////////////////////////

  __attribute__ ((section(".heapsram"))) static const float fv0[16] = { 4.24753476F, 4.24724483F, 0.495684743F,
    0.495674849F, 9.03945351F, 9.03927231F, 0.496419042F, 0.496409088F,
    4.96837616F, 4.96827698F, 0.499342769F, 0.499332756F, 1.63826454F,
    1.63823187F, 0.504582584F, 0.504572451F };

  __attribute__ ((section(".heapsram"))) static const float fv1[4] = { 0.13089969749999999515F,    // 2pi/48kHz*1000Hz
                                                                       0.20629792325999998792F,    // 2pi/48kHz*1576Hz
                                                                       0.33327062983499999849F,    // 2pi/48kHz*2546Hz
                                                                       0.49440815745749999710F };  // 2pi/48kHz*3777Hz
  
  __attribute__ ((section(".heapsram"))) static const float fv2[4] = { 1.0F,    // 1.0
                                                                       1.0F,    // 1.0
                                                                       1.0F,    // 1.0
                                                                       1.0F };  // 1.0
  
  
  static const float pi2[1] =  {6.283185307179586F};
  
/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  boolean_T pass, flag;
  int coreid;
  
  float omega, ampl, runningPhase;
  float sig[200];
  int k;
  int i;
  float y;
  int ix;
  float xbar;
  float r;
  float b_y;
  float tmp[2];

  float golden[4];
  boolean_T c_y;
  boolean_T exitg1;
  
  /////////////////////////////////////////////////////////
  // main test loop 
  // each core loops over a kernel instance
  /////////////////////////////////////////////////////////
  
  coreid = get_core_id();

  printf("starting %d kernel iterations... (coreid = %d)\n",KERNEL_ITS,coreid);
  
  if (coreid>3)
    coreid=coreid-4;

  synch_barrier();
  
  perf_begin();

  omega =  fv1[coreid];
  ampl  =  fv2[coreid];
  
  for(k = 0; k < getKernelIts(); k++)
  {
    runningPhase = omega;
  
    // matlab kernel
    for (i = 0; i < 200; i++) {
      sig[i] = ampl * fSin(runningPhase);
      runningPhase += omega;

      if(runningPhase > pi2[0])
      {
	runningPhase -= pi2[0];
      }    
    }
  }

  synch_barrier();

  perf_end();

  /////////////////////////////////////////////////////////
  // check results
  /////////////////////////////////////////////////////////

  synch_barrier();


  y = sig[0];
  ix = 0;
  xbar = sig[0];
  for (k = 0; k < 199; k++) {
    y += sig[k + 1];
    ix++;
    xbar += sig[ix];
  }

  xbar = fDiv(xbar,200.0F);
  ix = 0;
  r = sig[0] - xbar;
  b_y = r * r;
  for (k = 0; k < 199; k++) {
    ix++;
    r = sig[ix] - xbar;
    b_y += r * r;
  }

  b_y = fDiv(b_y,199.0F);
  tmp[0] = y;
  tmp[1] = b_y;

  pass = true;
  for (k = 0; k < 2; k++) {
    for (ix = 0; ix < 2; ix++) {
      golden[ix + (k << 1)] = fv0[(ix + (k << 1)) + (coreid << 2)];
    }
    flag = true;
    flag = flag && (tmp[k] <= golden[k << 1]);
    flag = flag && (tmp[k] >= golden[1 + (k << 1)]);
    printErrors(!flag, k, tmp[k] ,golden[k << 1], golden[1 + (k << 1)]);
    pass = pass && flag;
  }

  flagPassFail(pass, get_core_id());

/////////////////////////////////////////////////////////
// synchronize and exit
/////////////////////////////////////////////////////////

  return !pass;
}

