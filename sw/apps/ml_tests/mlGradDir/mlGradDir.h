// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#ifndef __ML_GRADDIR_H__
#define __ML_GRADDIR_H__

/////////////////////////////////////////////////////////
// defines
/////////////////////////////////////////////////////////

// include the shared header for ml kernels
#include "mlShared.h"

/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

void mlGrad(const float img[225], float filt[169]);

float var(const float x[169]);

float sum(const float x[169]);

boolean_T checkRes(const float check[2], const float golden[4], int coreid);

/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[]);




#endif

