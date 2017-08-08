// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#ifndef __ML_SVD_H__
#define __ML_SVD_H__

/////////////////////////////////////////////////////////
// defines
/////////////////////////////////////////////////////////

// include the shared header for ml kernels
#include "mlShared.h"


/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

void b_eml_xaxpy(int n, float a, const float x[25], int ix0, float y[5], int iy0);
float b_eml_xnrm2(int n, const float x[5], int ix0);
void c_eml_xaxpy(int n, float a, const float x[5], int ix0, float y[25], int iy0);
void eml_xaxpy(int n, float a, int ix0, float y[25], int iy0);
float eml_xdotc(int n, const float x[25], int ix0, const float y[25], int iy0);
void eml_xgesvd(const float A[25], float U[25], float S[5], float V[25]);
float eml_xnrm2(int n, const float x[25], int ix0);
void eml_xrot(float x[25], int ix0, int iy0, float c, float s);
void eml_xrotg(float *a, float *b, float *c, float *s);
void eml_xscal(float a, float x[25], int ix0);
void eml_xswap(float x[25], int ix0, int iy0);

/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[]);


#endif

