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
#include "mlSvd.h"
#include "math_fns.h"
#include "bar.h"
#include "utils.h"
#include "bench.h"

/////////////////////////////////////////////////////////
// shared globals
/////////////////////////////////////////////////////////


    __attribute__ ((section(".heapsram")))  static const float fv0[100] = { 11.6551533F, 3.06224823F, 2.69974494F,
    3.07209659F, 5.8402195F, 5.38518524F, 17.9067078F, 3.7271893F, 7.52009249F,
    6.52579117F, 5.51939058F, 8.50919819F, 14.2524872F, 5.74400616F, 7.68253946F,
    5.39514446F, 9.84270287F, 1.0558517F, 13.0100479F, 3.79853821F, 4.7271595F,
    2.22568727F, 3.20742488F, 2.26850915F, 10.2175779F, 10.1547337F, 7.83849859F,
    0.186470896F, 9.02727127F, 1.96604741F, 6.09034729F, 15.2287111F,
    7.06730318F, 3.53196239F, 7.58894396F, 5.05575705F, 7.65774393F, 13.6253195F,
    9.25017548F, 9.01099205F, 5.51714897F, 5.21025038F, 1.79024613F, 16.0892334F,
    7.27960157F, 8.75941467F, 4.73321819F, 7.52213764F, 1.81048298F, 10.2133579F,
    11.2927141F, 8.60052204F, 7.96709299F, 1.8759222F, 1.7121501F, 6.7556839F,
    13.9989452F, 9.35526848F, 4.76235104F, 4.02235794F, 1.90023458F, 3.37065792F,
    14.4098072F, 1.37983751F, 9.80338764F, 4.39650059F, 8.03841877F, 8.17665F,
    12.315279F, 6.41258383F, 2.5780983F, 1.24106932F, 4.07057762F, 0.676924825F,
    11.0976028F, 15.2955627F, 2.1150198F, 0.259674221F, 9.9411993F, 7.44640827F,
    9.82235146F, 19.5138206F, 4.7160778F, 9.87535572F, 8.52921104F, 3.09076977F,
    9.51736069F, 11.136487F, 1.17273784F, 0.00695414608F, 3.03156424F,
    3.69181323F, 1.20341122F, 17.1191216F, 9.57513714F, 3.22288203F, 7.88983774F,
    6.71485901F, 1.09614336F, 17.2396259F };

    __attribute__ ((section(".heapsram")))  static const float fv1[24] = { 10.298893F, 10.298687F, 68.5329056F,
    68.5315323F, 10.1729374F, 10.1727333F, 10.0122976F, 10.0120974F, 71.1426544F,
    71.1412354F, 10.4046917F, 10.4044838F, 9.66996193F, 9.66976929F, 66.3763275F,
    66.375F, 9.65051651F, 9.65032387F, 9.39816856F, 9.39798F, 85.7524719F,
    85.7507629F, 9.71934605F, 9.7191515F };

  

/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

void b_eml_xaxpy(int n, float a, const float x[25], int ix0, float y[5],
  int iy0)
{
  int ix;
  int iy;
  int k;
  if ((n < 1) || (a == 0.0F)) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

float b_eml_xnrm2(int n, const float x[5], int ix0)
{
  float y;
  float scale;
  int kend;
  int k;
  float absxk;
  float t;
  y = 0.0F;
  if (n < 1) {
  } else if (n == 1) {
    y = fAbs(x[ix0 - 1]);
  } else {
    scale = 1.17549435E-38F;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fAbs(x[k - 1]);
      if (absxk > scale) {
        t = fDiv(scale, absxk);
        y = 1.0F + y * t * t;
        scale = absxk;
      } else {
        t = fDiv(absxk, scale);
        y += t * t;
      }
    }

    y = scale * fSqrt(y);
  }

  return y;
}

void c_eml_xaxpy(int n, float a, const float x[5], int ix0, float y[25],
  int iy0)
{
  int ix;
  int iy;
  int k;
  if ((n < 1) || (a == 0.0F)) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}


void eml_xaxpy(int n, float a, int ix0, float y[25], int iy0)
{
  int ix;
  int iy;
  int k;
  if ((n < 1) || (a == 0.0F)) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * y[ix];
      ix++;
      iy++;
    }
  }
}

float eml_xdotc(int n, const float x[25], int ix0, const float y[25], int
  iy0)
{
  float d;
  int ix;
  int iy;
  int k;
  d = 0.0F;
  if (n < 1) {
  } else {
    ix = ix0;
    iy = iy0;
    for (k = 1; k <= n; k++) {
      d += x[ix - 1] * y[iy - 1];
      ix++;
      iy++;
    }
  }

  return d;
}

void eml_xgesvd(const float A[25], float U[25], float S[5], float V[25])
{
  float b_A[25];
  float s[5];
  float e[5];
  float work[5];
  int kase;
  int q;
  int iter;
  boolean_T apply_transform;
  float ztest0;
  int qp1jj;
  int qs;
  int m;
  float rt;
  float ztest;
  float snorm;
  int32_T exitg3;
  boolean_T exitg2;
  float f;
  float varargin_1[5];
  float mtmp;
  boolean_T exitg1;
  float sqds;
  float mtmp_inv;

  for (int k=0;k<25;k++)
    b_A[k] = A[k];
  //mem_fp_cpy(&b_A[0], &A[0], 25);
  //memcpy(&b_A[0], &A[0], 25U * sizeof(float));
  for (kase = 0; kase < 5; kase++) {
    s[kase] = 0.0F;
    e[kase] = 0.0F;
    work[kase] = 0.0F;
  }

  for (kase = 0; kase < 25; kase++) {
    U[kase] = 0.0F;
    V[kase] = 0.0F;
  }

  for (q = 0; q < 4; q++) {
    iter = q + 5 * q;
    apply_transform = false;
    ztest0 = eml_xnrm2(5 - q, b_A, iter + 1);
    if (ztest0 > 0.0F) {
      apply_transform = true;
      if (b_A[iter] < 0.0F) {
        s[q] = -ztest0;
      } else {
        s[q] = ztest0;
      }

      if (fAbs(s[q]) >= 9.86076132E-32F) {
        ztest0 = fDiv(1.0F,  s[q]);
        kase = (iter - q) + 5;
        for (qp1jj = iter; qp1jj + 1 <= kase; qp1jj++) {
          b_A[qp1jj] *= ztest0;
        }
      } else {
        kase = (iter - q) + 5;
        for (qp1jj = iter; qp1jj + 1 <= kase; qp1jj++) {
          b_A[qp1jj] = fDiv(b_A[qp1jj],s[q]);
        }
      }

      b_A[iter]++;
      s[q] = -s[q];
    } else {
      s[q] = 0.0F;
    }

    for (qs = q + 1; qs + 1 < 6; qs++) {
      kase = q + 5 * qs;
      if (apply_transform) {
        eml_xaxpy(5 - q, -(fDiv(eml_xdotc(5 - q, b_A, iter + 1, b_A, kase + 1), b_A[q + 5 * q])), iter + 1, b_A, kase + 1);
      }

      e[qs] = b_A[kase];
    }

    for (qp1jj = q; qp1jj + 1 < 6; qp1jj++) {
      U[qp1jj + 5 * q] = b_A[qp1jj + 5 * q];
    }

    if (q + 1 <= 3) {
      ztest0 = b_eml_xnrm2(4 - q, e, q + 2);
      if (ztest0 == 0.0F) {
        e[q] = 0.0F;
      } else {
        if (e[q + 1] < 0.0F) {
          e[q] = -ztest0;
        } else {
          e[q] = ztest0;
        }

        ztest0 = e[q];
        if (fAbs(e[q]) >= 9.86076132E-32F) {
          ztest0 = fDiv(1.0F, e[q]);
          for (qp1jj = q + 1; qp1jj + 1 < 6; qp1jj++) {
            e[qp1jj] *= ztest0;
          }
        } else {
          for (qp1jj = q + 1; qp1jj + 1 < 6; qp1jj++) {
            e[qp1jj] = fDiv(e[qp1jj],ztest0);
          }
        }

        e[q + 1]++;
        e[q] = -e[q];
        for (qp1jj = q + 1; qp1jj + 1 < 6; qp1jj++) {
          work[qp1jj] = 0.0F;
        }

        for (qs = q + 1; qs + 1 < 6; qs++) {
          b_eml_xaxpy(4 - q, e[qs], b_A, (q + 5 * qs) + 2, work, q + 2);
        }

        for (qs = q + 1; qs + 1 < 6; qs++) {
          c_eml_xaxpy(4 - q, -fDiv(e[qs], e[q + 1]), work, q + 2, b_A, (q + 5 * qs) +
                      2);
        }
      }

      for (qp1jj = q + 1; qp1jj + 1 < 6; qp1jj++) {
        V[qp1jj + 5 * q] = e[qp1jj];
      }
    }
  }

  m = 3;
  s[4] = b_A[24];
  e[3] = b_A[23];
  e[4] = 0.0F;
  for (qp1jj = 0; qp1jj < 5; qp1jj++) {
    U[20 + qp1jj] = 0.0F;
  }

  U[24] = 1.0F;
  for (q = 3; q > -1; q += -1) {
    iter = q + 5 * q;
    if (s[q] != 0.0F) {
      for (qs = q + 1; qs + 1 < 6; qs++) {
        kase = (q + 5 * qs) + 1;
        eml_xaxpy(5 - q, -(fDiv(eml_xdotc(5 - q, U, iter + 1, U, kase), U[iter])),
                  iter + 1, U, kase);
      }

      for (qp1jj = q; qp1jj + 1 < 6; qp1jj++) {
        U[qp1jj + 5 * q] = -U[qp1jj + 5 * q];
      }

      U[iter]++;
      for (qp1jj = 1; qp1jj <= q; qp1jj++) {
        U[(qp1jj + 5 * q) - 1] = 0.0F;
      }
    } else {
      for (qp1jj = 0; qp1jj < 5; qp1jj++) {
        U[qp1jj + 5 * q] = 0.0F;
      }

      U[iter] = 1.0F;
    }
  }

  for (q = 4; q > -1; q += -1) {
    if ((q + 1 <= 3) && (e[q] != 0.0F)) {
      kase = (q + 5 * q) + 2;
      for (qs = q + 1; qs + 1 < 6; qs++) {
        qp1jj = (q + 5 * qs) + 2;
        eml_xaxpy(4 - q, -(fDiv(eml_xdotc(4 - q, V, kase, V, qp1jj), V[kase - 1])),
                  kase, V, qp1jj);
      }
    }

    for (qp1jj = 0; qp1jj < 5; qp1jj++) {
      V[qp1jj + 5 * q] = 0.0F;
    }

    V[q + 5 * q] = 1.0F;
  }

  for (q = 0; q < 5; q++) {
    ztest0 = e[q];
    if (s[q] != 0.0F) {
      rt = fAbs(s[q]);
      ztest = fDiv(s[q], rt);
      s[q] = rt;
      if (q + 1 < 5) {
        ztest0 = fDiv(e[q], ztest);
      }

      eml_xscal(ztest, U, 1 + 5 * q);
    }

    if ((q + 1 < 5) && (ztest0 != 0.0F)) {
      rt = fAbs(ztest0);
      ztest = fDiv(rt, ztest0);
      ztest0 = rt;
      s[q + 1] *= ztest;
      eml_xscal(ztest, V, 1 + 5 * (q + 1));
    }

    e[q] = ztest0;
  }

  iter = 0;
  snorm = 0.0F;
  for (qp1jj = 0; qp1jj < 5; qp1jj++) {
    ztest0 = fAbs(s[qp1jj]);
    ztest = fAbs(e[qp1jj]);
    if ((ztest0 >= ztest) || fIsNan(ztest)) {
    } else {
      ztest0 = ztest;
    }

    if ((snorm >= ztest0) || fIsNan(ztest0)) {
    } else {
      snorm = ztest0;
    }
  }

  while ((m + 2 > 0) && (!(iter >= 75))) {
    qp1jj = m;
    do {
      exitg3 = 0;
      q = qp1jj + 1;
      if (qp1jj + 1 == 0) {
        exitg3 = 1;
      } else {
        ztest0 = fAbs(e[qp1jj]);
        if ((ztest0 <= 1.1920929E-7F * (fAbs(s[qp1jj]) + fAbs(s[qp1jj + 1]))) || (ztest0 <= 9.86076132E-32F) || ((iter > 20)
             && (ztest0 <= 1.1920929E-7F * snorm))) {
          e[qp1jj] = 0.0F;
          exitg3 = 1;
        } else {
          qp1jj--;
        }
      }
    } while (exitg3 == 0);

    if (qp1jj + 1 == m + 1) {
      kase = 4;
    } else {
      qs = m + 2;
      kase = m + 2;
      exitg2 = false;
      while ((!exitg2) && (kase >= qp1jj + 1)) {
        qs = kase;
        if (kase == qp1jj + 1) {
          exitg2 = true;
        } else {
          ztest0 = 0.0F;
          if (kase < m + 2) {
            ztest0 = fAbs(e[kase - 1]);
          }

          if (kase > qp1jj + 2) {
            ztest0 += fAbs(e[kase - 2]);
          }

          ztest = fAbs(s[kase - 1]);
          if ((ztest <= 1.1920929E-7F * ztest0) || (ztest <= 9.86076132E-32F)) {
            s[kase - 1] = 0.0F;
            exitg2 = true;
          } else {
            kase--;
          }
        }
      }

      if (qs == qp1jj + 1) {
        kase = 3;
      } else if (qs == m + 2) {
        kase = 1;
      } else {
        kase = 2;
        q = qs;
      }
    }

    switch (kase) {
     case 1:
      f = e[m];
      e[m] = 0.0F;
      for (qp1jj = m; qp1jj + 1 >= q + 1; qp1jj--) {
        ztest0 = s[qp1jj];
        eml_xrotg(&ztest0, &f, &ztest, &rt);
        s[qp1jj] = ztest0;
        if (qp1jj + 1 > q + 1) {
          f = -rt * e[qp1jj - 1];
          e[qp1jj - 1] *= ztest;
        }

        eml_xrot(V, 1 + 5 * qp1jj, 1 + 5 * (m + 1), ztest, rt);
      }
      break;

     case 2:
      f = e[q - 1];
      e[q - 1] = 0.0F;
      for (qp1jj = q; qp1jj + 1 <= m + 2; qp1jj++) {
        eml_xrotg(&s[qp1jj], &f, &ztest, &rt);
        f = -rt * e[qp1jj];
        e[qp1jj] *= ztest;
        eml_xrot(U, 1 + 5 * qp1jj, 1 + 5 * (q - 1), ztest, rt);
      }
      break;

     case 3:
      varargin_1[0] = fAbs(s[m + 1]);
      varargin_1[1] = fAbs(s[m]);
      varargin_1[2] = fAbs(e[m]);
      varargin_1[3] = fAbs(s[q]);
      varargin_1[4] = fAbs(e[q]);
      kase = 1;
      mtmp = varargin_1[0];
      if (fIsNan(varargin_1[0])) {
        qp1jj = 2;
        exitg1 = false;
        while ((!exitg1) && (qp1jj < 6)) {
          kase = qp1jj;
          if (!fIsNan(varargin_1[qp1jj - 1])) {
            mtmp = varargin_1[qp1jj - 1];
            exitg1 = true;
          } else {
            qp1jj++;
          }
        }
      }

      if (kase < 5) {
        while (kase + 1 < 6) {
          if (varargin_1[kase] > mtmp) {
            mtmp = varargin_1[kase];
          }

          kase++;
        }
      }

      mtmp_inv = fDiv(1.0F,mtmp);
      
      f = s[m + 1] * mtmp_inv;
      ztest0 = s[m] * mtmp_inv;
      ztest = e[m] * mtmp_inv;
      sqds = s[q] * mtmp_inv;
      rt = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) * 0.5F;
      ztest0 = f * ztest;
      ztest0 *= ztest0;
      if ((rt != 0.0F) || (ztest0 != 0.0F)) {
        ztest = fSqrt(rt * rt + ztest0);
        if (rt < 0.0F) {
          ztest = -ztest;
        }

        ztest = fDiv(ztest0, (rt + ztest));
      } else {
        ztest = 0.0F;
      }

      f = (sqds + f) * (sqds - f) + ztest;
      ztest0 = sqds * fDiv(e[q], mtmp);
      for (qp1jj = q + 1; qp1jj <= m + 1; qp1jj++) {
        eml_xrotg(&f, &ztest0, &ztest, &rt);
        if (qp1jj > q + 1) {
          e[qp1jj - 2] = f;
        }

        f = ztest * s[qp1jj - 1] + rt * e[qp1jj - 1];
        e[qp1jj - 1] = ztest * e[qp1jj - 1] - rt * s[qp1jj - 1];
        ztest0 = rt * s[qp1jj];
        s[qp1jj] *= ztest;
        eml_xrot(V, 1 + 5 * (qp1jj - 1), 1 + 5 * qp1jj, ztest, rt);
        s[qp1jj - 1] = f;
        eml_xrotg(&s[qp1jj - 1], &ztest0, &ztest, &rt);
        f = ztest * e[qp1jj - 1] + rt * s[qp1jj];
        s[qp1jj] = -rt * e[qp1jj - 1] + ztest * s[qp1jj];
        ztest0 = rt * e[qp1jj];
        e[qp1jj] *= ztest;
        eml_xrot(U, 1 + 5 * (qp1jj - 1), 1 + 5 * qp1jj, ztest, rt);
      }

      e[m] = f;
      iter++;
      break;

     default:
      if (s[q] < 0.0F) {
        s[q] = -s[q];
        eml_xscal(-1.0F, V, 1 + 5 * q);
      }

      kase = q + 1;
      while ((q + 1 < 5) && (s[q] < s[kase])) {
        rt = s[q];
        s[q] = s[kase];
        s[kase] = rt;
        eml_xswap(V, 1 + 5 * q, 1 + 5 * (q + 1));
        eml_xswap(U, 1 + 5 * q, 1 + 5 * (q + 1));
        q = kase;
        kase++;
      }

      iter = 0;
      m--;
      break;
    }
  }

  for (qp1jj = 0; qp1jj < 5; qp1jj++) {
    S[qp1jj] = s[qp1jj];
  }
}

float eml_xnrm2(int n, const float x[25], int ix0)
{
  float y;
  float scale;
  int kend;
  int k;
  float absxk;
  float t;
  y = 0.0F;
  if (n < 1) {
  } else if (n == 1) {
    y = fAbs(x[ix0 - 1]);
  } else {
    scale = 1.17549435E-38F;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fAbs(x[k - 1]);
      if (absxk > scale) {
        t = fDiv(scale, absxk);
        y = 1.0F + y * t * t;
        scale = absxk;
      } else {
        t = fDiv(absxk, scale);
        y += t * t;
      }
    }

    y = scale * fSqrt(y);
  }

  return y;
}

void eml_xrot(float x[25], int ix0, int iy0, float c, float s)
{
  int ix;
  int iy;
  int k;
  float temp;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 0; k < 5; k++) {
    temp = c * x[ix] + s * x[iy];
    x[iy] = c * x[iy] - s * x[ix];
    x[ix] = temp;
    iy++;
    ix++;
  }
}

void eml_xrotg(float *a, float *b, float *c, float *s)
{
  float roe;
  float absa;
  float absb;
  float scale;
  float ads;
  float bds;
  float inv_scale;
  roe = *b;
  absa = fAbs(*a);
  absb = fAbs(*b);
  if (absa > absb) {
    roe = *a;
  }

  scale = absa + absb;
  if (scale == 0.0F) {
    *s = 0.0F;
    *c = 1.0F;
    scale = 0.0F;
    *b = 0.0F;
  } else {
    inv_scale = fDiv(1.0F,scale);
    ads = absa * inv_scale;
    bds = absb * inv_scale;
    scale *= fSqrt(ads * ads + bds * bds);
    if (roe < 0.0F) {
      scale = -scale;
    }

    inv_scale = fDiv(1.0F,scale);
    

    *c = *a * inv_scale;
    *s = *b * inv_scale;
    if (absa > absb) {
      *b = *s;
    } else if (*c != 0.0F) {
      *b = fDiv(1.0F, *c);
    } else {
      *b = 1.0F;
    }
  }

  *a = scale;
}

void eml_xscal(float a, float x[25], int ix0)
{
  int k;
  for (k = ix0; k <= ix0 + 4; k++) {
    x[k - 1] *= a;
  }
}


void eml_xswap(float x[25], int ix0, int iy0)
{
  int ix;
  int iy;
  int k;
  float temp;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 0; k < 5; k++) {
    temp = x[ix];
    x[ix] = x[iy];
    x[iy] = temp;
    ix++;
    iy++;
  }
}

/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  int coreid, k;
  boolean_T pass;

  float V[25];
  float s[5];
  float U[25];
  
  int b_k;
  float y[25];
  float b_y;
  float c_y;
  float d_y;
  float tmp[3];
  
  init_fp_regs();

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

  for(k = 0; k < getKernelIts(); k++)
  {

    // call matlab kernel
    eml_xgesvd(*(float (*)[25])&fv0[25 * coreid], U, s, V);
    
  }

  synch_barrier();

  perf_end();

  /////////////////////////////////////////////////////////
  // check results
  /////////////////////////////////////////////////////////
  
  synch_barrier();

  for (b_k = 0; b_k < 25; b_k++) {
    y[b_k] = fAbs(U[b_k]);
  }

  b_y = y[0];
  c_y = s[0];
  
  for (b_k = 0; b_k < 4; b_k++) {
    c_y += s[b_k + 1];
  }

  for (b_k = 0; b_k < 24; b_k++) {
    b_y += y[b_k + 1];
  }

  for (b_k = 0; b_k < 25; b_k++) {
    y[b_k] = fAbs(V[b_k]);
  }

  d_y = y[0];
  for (b_k = 0; b_k < 24; b_k++) {
    d_y += y[b_k + 1];
  }

  tmp[0] = b_y;
  tmp[1] = c_y;
  tmp[2] = d_y;
  
  pass = true;
  for (b_k = 0; b_k < 3; b_k++) {
    pass = pass && (tmp[b_k] <= fv1[(0 + (b_k << 1)) + 6 * coreid]);
    pass = pass && (tmp[b_k] >= fv1[(1 + (b_k << 1)) + 6 * coreid]);
  }

  flagPassFail(pass, get_core_id());
  
  synch_barrier();

/////////////////////////////////////////////////////////
// synchronize and exit
/////////////////////////////////////////////////////////

  return !pass;
}

