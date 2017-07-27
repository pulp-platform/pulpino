/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */
#ifndef __ML_EIG_H__
#define __ML_EIG_H__

/////////////////////////////////////////////////////////
// defines
/////////////////////////////////////////////////////////

// include the shared header for ml kernels
#include "mlShared.h"

/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

float b_eml_xnrm2(int n, const float x[3]);
void b_eml_xrot(int n, float x[25], int ix0, int iy0, float c, float s);
int eml_dlahqr(float h[25], float z[25]);
void eml_dlanv2(float *a, float *b, float *c, float *d, float *rt1r,
  float *rt1i, float *rt2r, float *rt2i, float *cs, float *sn);
float eml_matlab_zlarfg(int n, float *alpha1, float x[3]);
void eml_xgehrd(float a[25], float tau[4]);
void eml_xgerc(int m, int n, float alpha1, int ix0, const float y[5],
                      float A[25], int ia0);
float eml_xnrm2(int n, const float x[25], int ix0);
void eml_xrot(int n, float x[25], int ix0, int iy0, float c, float s);
void eml_xunghr(int n, int ilo, int ihi, float A[25], int ia0, const
  float tau[4], int itau0);
float rt_hypotf_snf(float u0, float u1);

/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[]);




#endif

