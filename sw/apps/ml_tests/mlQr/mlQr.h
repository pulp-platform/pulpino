// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#ifndef __ML_QR_H__
#define __ML_QR_H__

/////////////////////////////////////////////////////////
// defines
/////////////////////////////////////////////////////////

// include the shared header for ml kernels
#include "mlShared.h"

/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

void eml_xgeqrf(float A[100], float tau[10]);
float eml_xnrm2(int n, const float x[100], int ix0);
float rt_hypotf_snf(float u0, float u1);
float var(const float x[100]);
void mlQr(const float A[100], float Q[100], float R[100]);

/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[]);



#endif

