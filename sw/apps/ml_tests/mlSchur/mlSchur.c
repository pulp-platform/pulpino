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
#include "mlSchur.h"
#include "mlShared.h"
#include "math_fns.h"
#ifndef LINUX
#include "bar.h"
#include "utils.h"
#include "bench.h"
#endif
/////////////////////////////////////////////////////////
// shared globals
/////////////////////////////////////////////////////////

    __attribute__ ((section(".heapsram"))) static const float fv0[100] = { 431.424896F, -332.915924F, -246.548111F,
    -332.485474F, -437.717865F, -332.915924F, 1423.53333F, -314.047241F,
    -230.521576F, 78.6693726F, -246.548111F, -314.047241F, 847.96228F,
    250.109161F, 320.915405F, -332.485474F, -230.521576F, 250.109161F,
    1819.52905F, 1293.37927F, -437.717865F, 78.6693726F, 320.915405F,
    1293.37927F, 1052.97021F, 394.013092F, 23.0439568F, -240.092178F,
    149.108078F, -583.700378F, 23.0439568F, 203.53624F, 57.9439774F, 133.19754F,
    227.439407F, -240.092178F, 57.9439774F, 230.038666F, 52.7716904F,
    509.824768F, 149.108078F, 133.19754F, 52.7716904F, 391.247711F, 74.889122F,
    -583.700378F, 227.439407F, 509.824768F, 74.889122F, 1419.64172F, 466.726288F,
    -187.057327F, 107.069092F, -122.094795F, 70.9483261F, -187.057327F,
    338.389038F, 60.1494713F, 237.151215F, -488.833F, 107.069092F, 60.1494713F,
    250.532532F, 249.328888F, -32.1080475F, -122.094795F, 237.151215F,
    249.328888F, 441.869965F, -171.971146F, 70.9483261F, -488.833F, -32.1080475F,
    -171.971146F, 974.27655F, 581.897339F, 285.430023F, -176.483063F,
    -39.7847519F, -124.96624F, 285.430023F, 282.335144F, -60.5030556F,
    120.271088F, -85.1179352F, -176.483063F, -60.5030556F, 459.508789F,
    289.082184F, 301.67804F, -39.7847519F, 120.271088F, 289.082184F, 947.621948F,
    -243.034912F, -124.96624F, -85.1179352F, 301.67804F, -243.034912F, 571.041F
  };

    __attribute__ ((section(".heapsram"))) static const float fv1[32] = { 8.79604149F, 8.79586601F, 5575.47559F,
    5575.36426F, 0.0793861672F, 0.0793845728F, 1.51855562E+6F, 1.51852538E+6F,
    9.29323387F, 9.29304886F, 2638.50391F, 2638.45117F, 0.0643965602F,
    0.0643952787F, 637568.688F, 637555.938F, 9.07298374F, 9.07280254F,
    2471.81909F, 2471.76978F, 0.0711383894F, 0.0711369738F, 308398.094F,
    308391.906F, 9.28204441F, 9.2818594F, 2842.43262F, 2842.37598F,
    0.0647429377F, 0.0647416413F, 253840.219F, 253835.156F };


/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

float b_eml_xnrm2(int n, const float x[3])
{
  float y;
  float scale;
  int k;
  float absxk;
  float t;
  y = 0.0F;
  if (n < 1) {
  } else if (n == 1) {
    y = fAbs(x[1]);
  } else {
    scale = 1.17549435E-38F;
    for (k = 2; k <= n + 1; k++) {
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

/*
 * Arguments    : int n
 *                float x[25]
 *                int ix0
 *                int iy0
 *                float c
 *                float s
 * Return Type  : void
 */
void b_eml_xrot(int n, float x[25], int ix0, int iy0, float c, float s)
{
  int ix;
  int iy;
  int k;
  float temp;
  if (n < 1) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 1; k <= n; k++) {
      temp = c * x[ix] + s * x[iy];
      x[iy] = c * x[iy] - s * x[ix];
      x[ix] = temp;
      iy++;
      ix++;
    }
  }
}

/*
 * Arguments    : float h[25]
 *                float z[25]
 * Return Type  : int
 */
int eml_dlahqr(float h[25], float z[25])
{
  int info;
  float v[3];
  int j;
  int i;
  boolean_T exitg1;
  int L;
  boolean_T goto150;
  int its;
  boolean_T exitg2;
  int k;
  boolean_T exitg4;
  float tst;
  boolean_T guard1 = false;
  float htmp1;
  float ab;
  float ba;
  float aa;
  float s;
  float f1;
  float rt1r;
  float rt2r;
  int m;
  boolean_T exitg3;
  int b_k;
  int nr;
  int hoffset;
  float sn;
  float s_inv;

  info = 0;
  for (j = 0; j < 2; j++) {
    h[(j + 5 * j) + 2] = 0.0F;
    h[(j + 5 * j) + 3] = 0.0F;
  }

  h[14] = 0.0F;
  i = 4;
  exitg1 = false;
  while ((!exitg1) && (i + 1 >= 1)) {
    L = 1;
    goto150 = false;
    its = 0;
    exitg2 = false;
    while ((!exitg2) && (its < 31)) {
      k = i;
      exitg4 = false;
      while ((!exitg4) && ((k + 1 > L) && (!(fAbs(h[k + 5 * (k - 1)]) <=
                4.93038066E-31F)))) {
        tst = fAbs(h[(k + 5 * (k - 1)) - 1]) + fAbs(h[k + 5 *
          k]);
        if (tst == 0.0F) {
          if (k - 1 >= 1) {
            tst = fAbs(h[(k + 5 * (k - 2)) - 1]);
          }

          if (k + 2 <= 5) {
            tst += fAbs(h[(k + 5 * k) + 1]);
          }
        }

        guard1 = false;
        if (fAbs(h[k + 5 * (k - 1)]) <= 1.1920929E-7F * tst) {
          htmp1 = fAbs(h[k + 5 * (k - 1)]);
          tst = fAbs(h[(k + 5 * k) - 1]);
          if (htmp1 > tst) {
            ab = htmp1;
            ba = tst;
          } else {
            ab = tst;
            ba = htmp1;
          }

          htmp1 = fAbs(h[k + 5 * k]);
          tst = fAbs(h[(k + 5 * (k - 1)) - 1] - h[k + 5 * k]);
          if (htmp1 > tst) {
            aa = htmp1;
            htmp1 = tst;
          } else {
            aa = tst;
          }

          s = aa + ab;
          tst = 1.1920929E-7F * (htmp1 * fDiv(aa, s));
          if ((4.93038066E-31F >= tst) || fIsNan(tst)) {
            f1 = 4.93038066E-31F;
          } else {
            f1 = tst;
          }

          if (ba * fDiv(ab, s) <= f1) {
            exitg4 = true;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }

        if (guard1) {
          k--;
        }
      }

      L = k + 1;
      if (k + 1 > 1) {
        h[k + 5 * (k - 1)] = 0.0F;
      }

      if (k + 1 >= i) {
        goto150 = true;
        exitg2 = true;
      } else {
        if (its == 10) {
          s = fAbs(h[(k + 5 * k) + 1]) + fAbs(h[(k + 5 * (k
            + 1)) + 2]);
          tst = 0.75F * s + h[k + 5 * k];
          aa = -0.4375F * s;
          htmp1 = s;
          ba = tst;
        } else if (its == 20) {
          s = fAbs(h[i + 5 * (i - 1)]) + fAbs(h[(i + 5 * (i
            - 2)) - 1]);
          tst = 0.75F * s + h[i + 5 * i];
          aa = -0.4375F * s;
          htmp1 = s;
          ba = tst;
        } else {
          tst = h[(i + 5 * (i - 1)) - 1];
          htmp1 = h[i + 5 * (i - 1)];
          aa = h[(i + 5 * i) - 1];
          ba = h[i + 5 * i];
        }

        s = ((fAbs(tst) + fAbs(aa)) + fAbs(htmp1))
          + fAbs(ba);
        if (s == 0.0F) {
          rt1r = 0.0F;
          ab = 0.0F;
          rt2r = 0.0F;
          ba = 0.0F;
        } else {

          s_inv = fDiv(1.0F,s);
          tst   *= s_inv;
          htmp1 *= s_inv;
          aa    *= s_inv;
          ba    *= s_inv;
          ab =  (tst + ba)*0.5F;
          tst = (tst - ab) * (ba - ab) - aa * htmp1;
          htmp1 = fSqrt(fAbs(tst));
          if (tst >= 0.0F) {
            rt1r = ab * s;
            rt2r = rt1r;
            ab = htmp1 * s;
            ba = -ab;
          } else {
            rt1r = ab + htmp1;
            rt2r = ab - htmp1;
            if (fAbs(rt1r - ba) <= fAbs(rt2r - ba)) {
              rt1r *= s;
              rt2r = rt1r;
            } else {
              rt2r *= s;
              rt1r = rt2r;
            }

            ab = 0.0F;
            ba = 0.0F;
          }
        }

        m = i - 1;
        exitg3 = false;
        while ((!exitg3) && (m >= k + 1)) {
          s = (fAbs(h[(m + 5 * (m - 1)) - 1] - rt2r) + fAbs
               (ba)) + fAbs(h[m + 5 * (m - 1)]);

          s_inv = fDiv(1.0F,s);

          tst = h[m + 5 * (m - 1)] * s_inv;
          v[0] = (tst * h[(m + 5 * m) - 1] + (h[(m + 5 * (m - 1)) - 1] - rt1r) *
                  ((h[(m + 5 * (m - 1)) - 1] - rt2r) * s_inv)) - ab * (ba * s_inv);
          v[1] = tst * (((h[(m + 5 * (m - 1)) - 1] + h[m + 5 * m]) - rt1r) -
                        rt2r);
          v[2] = tst * h[(m + 5 * m) + 1];

          s = (fAbs(v[0]) + fAbs(v[1])) + fAbs(v[2]);
          
          s_inv = fDiv(1.0F,s);

          tst = v[0] * s;
          v[0] *= s;
          htmp1 = v[1] * s;
          v[1] *= s;
          aa = v[2] * s;
          v[2] *= s;
          if ((m == k + 1) || (fAbs(h[(m + 5 * (m - 2)) - 1]) *
                               (fAbs(htmp1) + fAbs(aa)) <=
                               1.1920929E-7F * fAbs(tst) * ((fAbs(h[(m + 5 * (m - 2)) - 2]) + fAbs(h[(m + 5 * (m -
                    1)) - 1])) + fAbs(h[m + 5 * m])))) {
            exitg3 = true;
          } else {
            m--;
          }
        }

        for (b_k = m; b_k <= i; b_k++) {
          nr = (i - b_k) + 2;
          if (3 <= nr) {
            nr = 3;
          }

          if (b_k > m) {
            hoffset = b_k + 5 * (b_k - 2);
            for (j = 1; j <= nr; j++) {
              v[j - 1] = h[(j + hoffset) - 2];
            }
          }

          tst = v[0];
          rt2r = eml_matlab_zlarfg(nr, &tst, v);
          v[0] = tst;
          if (b_k > m) {
            h[(b_k + 5 * (b_k - 2)) - 1] = tst;
            h[b_k + 5 * (b_k - 2)] = 0.0F;
            if (b_k < i) {
              h[(b_k + 5 * (b_k - 2)) + 1] = 0.0F;
            }
          } else {
            if (m > k + 1) {
              h[(b_k + 5 * (b_k - 2)) - 1] *= 1.0F - rt2r;
            }
          }

          tst = v[1];
          htmp1 = rt2r * v[1];
          if (nr == 3) {
            ab = v[2];
            ba = rt2r * v[2];
            for (j = b_k - 1; j + 1 < 6; j++) {
              aa = (h[(b_k + 5 * j) - 1] + tst * h[b_k + 5 * j]) + ab * h[(b_k +
                5 * j) + 1];
              h[(b_k + 5 * j) - 1] -= aa * rt2r;
              h[b_k + 5 * j] -= aa * htmp1;
              h[(b_k + 5 * j) + 1] -= aa * ba;
            }

            if (b_k + 3 <= i + 1) {
              nr = b_k;
            } else {
              nr = i - 2;
            }

            for (j = 0; j + 1 <= nr + 3; j++) {
              aa = (h[j + 5 * (b_k - 1)] + tst * h[j + 5 * b_k]) + ab * h[j + 5 *
                (b_k + 1)];
              h[j + 5 * (b_k - 1)] -= aa * rt2r;
              h[j + 5 * b_k] -= aa * htmp1;
              h[j + 5 * (b_k + 1)] -= aa * ba;
            }

            for (j = 0; j < 5; j++) {
              aa = (z[j + 5 * (b_k - 1)] + tst * z[j + 5 * b_k]) + ab * z[j + 5 *
                (b_k + 1)];
              z[j + 5 * (b_k - 1)] -= aa * rt2r;
              z[j + 5 * b_k] -= aa * htmp1;
              z[j + 5 * (b_k + 1)] -= aa * ba;
            }
          } else {
            if (nr == 2) {
              for (j = b_k - 1; j + 1 < 6; j++) {
                aa = h[(b_k + 5 * j) - 1] + tst * h[b_k + 5 * j];
                h[(b_k + 5 * j) - 1] -= aa * rt2r;
                h[b_k + 5 * j] -= aa * htmp1;
              }

              for (j = 0; j + 1 <= i + 1; j++) {
                aa = h[j + 5 * (b_k - 1)] + tst * h[j + 5 * b_k];
                h[j + 5 * (b_k - 1)] -= aa * rt2r;
                h[j + 5 * b_k] -= aa * htmp1;
              }

              for (j = 0; j < 5; j++) {
                aa = z[j + 5 * (b_k - 1)] + tst * z[j + 5 * b_k];
                z[j + 5 * (b_k - 1)] -= aa * rt2r;
                z[j + 5 * b_k] -= aa * htmp1;
              }
            }
          }
        }

        its++;
      }
    }

    if (!goto150) {
      info = i + 1;
      exitg1 = true;
    } else {
      if ((L == i + 1) || (!(L == i))) {
      } else {
        tst = h[(i + 5 * i) - 1];
        htmp1 = h[i + 5 * (i - 1)];
        aa = h[i + 5 * i];
        eml_dlanv2(&h[(i + 5 * (i - 1)) - 1], &tst, &htmp1, &aa, &ab, &ba, &rt2r,
                   &rt1r, &s, &sn);
        h[(i + 5 * i) - 1] = tst;
        h[i + 5 * (i - 1)] = htmp1;
        h[i + 5 * i] = aa;
        if (5 > i + 1) {
          eml_xrot(4 - i, h, i + (i + 1) * 5, (i + (i + 1) * 5) + 1, s, sn);
        }

        b_eml_xrot(i - 1, h, 1 + (i - 1) * 5, 1 + i * 5, s, sn);
        hoffset = (i - 1) * 5;
        nr = i * 5;
        for (k = 0; k < 5; k++) {
          tst = s * z[hoffset] + sn * z[nr];
          z[nr] = s * z[nr] - sn * z[hoffset];
          z[hoffset] = tst;
          nr++;
          hoffset++;
        }
      }

      i = L - 2;
    }
  }

  return info;
}

/*
 * Arguments    : float *a
 *                float *b
 *                float *c
 *                float *d
 *                float *rt1r
 *                float *rt1i
 *                float *rt2r
 *                float *rt2i
 *                float *cs
 *                float *sn
 * Return Type  : void
 */
void eml_dlanv2(float *a, float *b, float *c, float *d, float *rt1r,
  float *rt1i, float *rt2r, float *rt2i, float *cs, float *sn)
{
  float temp;
  float p;
  float z;
  float scale;
  float bcmax;
  float b_z;
  int b_b;
  int b_c;
  float bcmis;
  float b_p;
  float tau;
  int b_scale;
  float scale_inv;
  float tau_inv;

  if (*c == 0.0F) {
    *cs = 1.0F;
    *sn = 0.0F;
  } else if (*b == 0.0F) {
    *cs = 0.0F;
    *sn = 1.0F;
    temp = *d;
    *d = *a;
    *a = temp;
    *b = -*c;
    *c = 0.0F;
  } else if ((*a - *d == 0.0F) && ((*b < 0.0F) != (*c < 0.0F))) {
    *cs = 1.0F;
    *sn = 0.0F;
  } else {
    temp = *a - *d;
    p = 0.5F * temp;
    z = fAbs(*b);
    scale = fAbs(*c);
    if ((z >= scale) || fIsNan(scale)) {
      bcmax = z;
    } else {
      bcmax = scale;
    }

    z = fAbs(*b);
    scale = fAbs(*c);
    if ((z <= scale) || fIsNan(scale)) {
      b_z = z;
    } else {
      b_z = scale;
    }

    if (!(*b < 0.0F)) {
      b_b = 1;
    } else {
      b_b = -1;
    }

    if (!(*c < 0.0F)) {
      b_c = 1;
    } else {
      b_c = -1;
    }

    bcmis = b_z * (float)b_b * (float)b_c;
    z = fAbs(p);
    if ((z >= bcmax) || fIsNan(bcmax)) {
      scale = z;
    } else {
      scale = bcmax;
    }

    scale_inv = fDiv(1.0F,scale);

    z = p *scale_inv * p + bcmax *scale_inv * bcmis;
    if (z >= 8.8817842E-16F) {
      *a = fSqrt(scale) * fSqrt(z);
      if (!(p < 0.0F)) {
        b_p = *a;
      } else {
        b_p = -*a;
      }

      z = p + b_p;
      *a = *d + z;
      *d -= fDiv(bcmax, z) * bcmis;
      tau = rt_hypotf_snf(*c, z);

      tau_inv = fDiv(1.0F, tau);

      *cs = z *tau_inv;
      *sn = *c * tau_inv;
      *b -= *c;
      *c = 0.0F;
    } else {
      scale = *b + *c;
      tau = rt_hypotf_snf(scale, temp);

      tau_inv = fDiv(1.0F, tau);

      *cs = fSqrt(0.5F * (1.0F + fAbs(scale) * tau_inv));
      if (!(scale < 0.0F)) {
        b_scale = 1;
      } else {
        b_scale = -1;
      }

      *sn = -(p * tau_inv * *cs) * (float)b_scale;
      bcmax = *a * *cs + *b * *sn;
      z = -*a * *sn + *b * *cs;
      bcmis = *c * *cs + *d * *sn;
      scale = -*c * *sn + *d * *cs;
      *b = z * *cs + scale * *sn;
      *c = -bcmax * *sn + bcmis * *cs;
      temp = 0.5F * ((bcmax * *cs + bcmis * *sn) + (-z * *sn + scale * *cs));
      *a = temp;
      *d = temp;
      if (*c != 0.0F) {
        if (*b != 0.0F) {
          if ((*b < 0.0F) == (*c < 0.0F)) {
            scale = fSqrt(fAbs(*b));
            bcmis = fSqrt(fAbs(*c));
            *a = scale * bcmis;
            if (!(*c < 0.0F)) {
              p = *a;
            } else {
              p = -*a;
            }

            tau = fDiv(1.0F, fSqrt(fAbs(*b + *c)));
            *a = temp + p;
            *d = temp - p;
            *b -= *c;
            *c = 0.0F;
            bcmax = scale * tau;
            scale = bcmis * tau;
            temp = *cs * bcmax - *sn * scale;
            *sn = *cs * scale + *sn * bcmax;
            *cs = temp;
          }
        } else {
          *b = -*c;
          *c = 0.0F;
          temp = *cs;
          *cs = -*sn;
          *sn = temp;
        }
      }
    }
  }

  *rt1r = *a;
  *rt2r = *d;
  if (*c == 0.0F) {
    *rt1i = 0.0F;
    *rt2i = 0.0F;
  } else {
    *rt1i = fSqrt(fAbs(*b)) * fSqrt(fAbs(*c));
    *rt2i = -*rt1i;
  }
}

/*
 * Arguments    : int n
 *                float *alpha1
 *                float x[3]
 * Return Type  : float
 */
float eml_matlab_zlarfg(int n, float *alpha1, float x[3])
{
  float tau;
  float xnorm;
  int knt;
  int k;

  tau = 0.0F;
  if (n <= 0) {
  } else {
    xnorm = b_eml_xnrm2(n - 1, x);
    if (xnorm != 0.0F) {
      xnorm = rt_hypotf_snf(*alpha1, xnorm);
      if (*alpha1 >= 0.0F) {
        xnorm = -xnorm;
      }

      if (fAbs(xnorm) < 9.86076132E-32F) {
        knt = 0;
        do {
          knt++;
          for (k = 1; k + 1 <= n; k++) {
            x[k] *= 1.01412048E+31F;
          }

          xnorm *= 1.01412048E+31F;
          *alpha1 *= 1.01412048E+31F;
        } while (!(fAbs(xnorm) >= 9.86076132E-32F));

        xnorm = rt_hypotf_snf(*alpha1, b_eml_xnrm2(n - 1, x));
        if (*alpha1 >= 0.0F) {
          xnorm = -xnorm;
        }

        tau = fDiv((xnorm - *alpha1), xnorm);
        *alpha1 = fDiv(1.0F, (*alpha1 - xnorm));
        for (k = 1; k + 1 <= n; k++) {
          x[k] *= *alpha1;
        }

        for (k = 1; k <= knt; k++) {
          xnorm *= 9.86076132E-32F;
        }

        *alpha1 = xnorm;
      } else {
        tau = fDiv((xnorm - *alpha1), xnorm);
        *alpha1 = fDiv(1.0F, (*alpha1 - xnorm));
        for (k = 1; k + 1 <= n; k++) {
          x[k] *= *alpha1;
        }

        *alpha1 = xnorm;
      }
    }
  }

  return tau;
}

/*
 * Arguments    : float a[25]
 *                float tau[4]
 * Return Type  : void
 */
void eml_xgehrd(float a[25], float tau[4])
{
  float work[5];
  int i;
  int im1n;
  int in;
  int b_i;
  int ia0;
  float alpha1;
  float f0;
  float xnorm;
  int knt;
  int i0;
  int k;
  int jy;
  int lastv;
  int lastc;
  boolean_T exitg4;
  int ia;
  int32_T exitg3;
  int ix;
  int i1;
  boolean_T exitg2;
  int32_T exitg1;

  for (i = 0; i < 5; i++) {
    work[i] = 0.0F;
  }

  for (i = 0; i < 4; i++) {
    im1n = i * 5 + 2;
    in = (i + 1) * 5;
    if (i + 3 <= 5) {
      b_i = i + 3;
    } else {
      b_i = 5;
    }

    ia0 = b_i + i * 5;
    alpha1 = a[(i + 5 * i) + 1];
    f0 = 0.0F;
    xnorm = eml_xnrm2(3 - i, a, ia0);
    if (xnorm != 0.0F) {
      xnorm = rt_hypotf_snf(a[(i + 5 * i) + 1], xnorm);
      if (a[(i + 5 * i) + 1] >= 0.0F) {
        xnorm = -xnorm;
      }

      if (fAbs(xnorm) < 9.86076132E-32F) {
        knt = 0;
        do {
          knt++;
          i0 = (ia0 - i) + 2;
          for (k = ia0; k <= i0; k++) {
            a[k - 1] *= 1.01412048E+31F;
          }

          xnorm *= 1.01412048E+31F;
          alpha1 *= 1.01412048E+31F;
        } while (!(fAbs(xnorm) >= 9.86076132E-32F));

        xnorm = rt_hypotf_snf(alpha1, eml_xnrm2(3 - i, a, ia0));
        if (alpha1 >= 0.0F) {
          xnorm = -xnorm;
        }

        f0 = fDiv((xnorm - alpha1), xnorm);
        alpha1 = fDiv(1.0F, (alpha1 - xnorm));
        i0 = (ia0 - i) + 2;
        while (ia0 <= i0) {
          a[ia0 - 1] *= alpha1;
          ia0++;
        }

        for (k = 1; k <= knt; k++) {
          xnorm *= 9.86076132E-32F;
        }

        alpha1 = xnorm;
      } else {
        f0 = fDiv((xnorm - a[(i + 5 * i) + 1]), xnorm);
        alpha1 = fDiv(1.0F, (a[(i + 5 * i) + 1] - xnorm));
        i0 = (ia0 - i) + 2;
        while (ia0 <= i0) {
          a[ia0 - 1] *= alpha1;
          ia0++;
        }

        alpha1 = xnorm;
      }
    }

    tau[i] = f0;
    a[(i + 5 * i) + 1] = 1.0F;
    jy = (i + im1n) - 1;
    if (tau[i] != 0.0F) {
      lastv = 4 - i;
      ia0 = (jy - i) + 3;
      while ((lastv > 0) && (a[ia0] == 0.0F)) {
        lastv--;
        ia0--;
      }

      lastc = 5;
      exitg4 = false;
      while ((!exitg4) && (lastc > 0)) {
        ia0 = in + lastc;
        ia = ia0;
        do {
          exitg3 = 0;
          if (ia <= ia0 + (lastv - 1) * 5) {
            if (a[ia - 1] != 0.0F) {
              exitg3 = 1;
            } else {
              ia += 5;
            }
          } else {
            lastc--;
            exitg3 = 2;
          }
        } while (exitg3 == 0);

        if (exitg3 == 1) {
          exitg4 = true;
        }
      }
    } else {
      lastv = 0;
      lastc = 0;
    }

    if (lastv > 0) {
      if (lastc == 0) {
      } else {
        for (ia0 = 1; ia0 <= lastc; ia0++) {
          work[ia0 - 1] = 0.0F;
        }

        ix = jy;
        i0 = (in + 5 * (lastv - 1)) + 1;
        for (k = in + 1; k <= i0; k += 5) {
          ia0 = 0;
          i1 = (k + lastc) - 1;
          for (ia = k; ia <= i1; ia++) {
	    work[ia0] += a[ia - 1] * a[ix];
            ia0++;
          }

          ix++;
        }
      }

      if (-tau[i] == 0.0F) {
      } else {
        ia0 = in;
        for (knt = 1; knt <= lastv; knt++) {
          if (a[jy] != 0.0F) {
            xnorm = a[jy] * -tau[i];
            ix = 0;
            i0 = lastc + ia0;
            for (k = ia0; k + 1 <= i0; k++) {
              a[k] += work[ix] * xnorm;
	      ix++;
            }
          }

          jy++;
          ia0 += 5;
        }
      }
    }

    jy = i + im1n;
    knt = (i + in) + 2;
    if (tau[i] != 0.0F) {
      lastv = 4 - i;
      ia0 = (jy - i) + 2;
      while ((lastv > 0) && (a[ia0] == 0.0F)) {
        lastv--;
        ia0--;
      }

      lastc = 4 - i;
      exitg2 = false;
      while ((!exitg2) && (lastc > 0)) {
        ia0 = knt + (lastc - 1) * 5;
        ia = ia0;
        do {
          exitg1 = 0;
          if (ia <= (ia0 + lastv) - 1) {
            if (a[ia - 1] != 0.0F) {
              exitg1 = 1;
            } else {
              ia++;
            }
          } else {
            lastc--;
            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = true;
        }
      }
    } else {
      lastv = 0;
      lastc = 0;
    }

    if (lastv > 0) {
      if (lastc == 0) {
      } else {
        for (ia0 = 1; ia0 <= lastc; ia0++) {
          work[ia0 - 1] = 0.0F;
        }

        ia0 = 0;
        i0 = knt + 5 * (lastc - 1);
        for (k = knt; k <= i0; k += 5) {
          ix = jy;
          xnorm = 0.0F;
          i1 = (k + lastv) - 1;
          for (ia = k; ia <= i1; ia++) {
            xnorm += a[ia - 1] * a[ix - 1];
            ix++;
          }

          work[ia0] += xnorm;
          ia0++;
        }
      }

      eml_xgerc(lastv, lastc, -tau[i], jy, work, a, knt);
    }

    a[(i + 5 * i) + 1] = alpha1;
  }
}

/*
 * Arguments    : int m
 *                int n
 *                float alpha1
 *                int ix0
 *                const float y[5]
 *                float A[25]
 *                int ia0
 * Return Type  : void
 */
void eml_xgerc(int m, int n, float alpha1, int ix0, const float y[5],
                      float A[25], int ia0)
{
  int jA;
  int jy;
  int j;
  float temp;
  int ix;
  int i2;
  int ijA;

  if (alpha1 == 0.0F) {
  } else {
    jA = ia0 - 1;
    jy = 0;
    for (j = 1; j <= n; j++) {
      if (y[jy] != 0.0F) {
        temp = y[jy] * alpha1;
        ix = ix0;
        i2 = m + jA;
        for (ijA = jA; ijA + 1 <= i2; ijA++) {
          A[ijA] += A[ix - 1] * temp;
          ix++;
        }
      }

      jy++;
      jA += 5;
    }
  }
}

/*
 * Arguments    : int n
 *                const float x[25]
 *                int ix0
 * Return Type  : float
 */
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

/*
 * Arguments    : int n
 *                float x[25]
 *                int ix0
 *                int iy0
 *                float c
 *                float s
 * Return Type  : void
 */
void eml_xrot(int n, float x[25], int ix0, int iy0, float c, float s)
{
  int ix;
  int iy;
  int k;
  float temp;
  if (n < 1) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 1; k <= n; k++) {
      temp = c * x[ix] + s * x[iy];
      x[iy] = c * x[iy] - s * x[ix];
      x[ix] = temp;
      iy += 5;
      ix += 5;
    }
  }
}

/*
 * Arguments    : int n
 *                int ilo
 *                int ihi
 *                float A[25]
 *                int ia0
 *                const float tau[4]
 *                int itau0
 * Return Type  : void
 */
void eml_xunghr(int n, int ilo, int ihi, float A[25], int ia0, const
  float tau[4], int itau0)
{
  int nh;
  int i;
  int ia;
  int b_i;
  int b_ia;
  int itau;
  float work[5];
  int iaii;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int32_T exitg1;
  int i3;
  int iac;
  int ix;
  float c;
  int i4;

  if (n == 0) {
  } else {
    nh = ihi - ilo;
    for (i = ihi; i >= ilo + 1; i--) {
      ia = (ia0 + (i - 1) * 5) - 2;
      for (b_i = 1; b_i < i; b_i++) {
        A[ia + b_i] = 0.0F;
      }

      for (b_i = i + 1; b_i <= ihi; b_i++) {
        A[ia + b_i] = A[(ia + b_i) - 5];
      }

      for (b_i = ihi + 1; b_i <= n; b_i++) {
        A[ia + b_i] = 0.0F;
      }
    }

    for (i = 0; i + 1 <= ilo; i++) {
      ia = (ia0 + i * 5) - 1;
      for (b_i = 1; b_i <= n; b_i++) {
        A[(ia + b_i) - 1] = 0.0F;
      }

      A[ia + i] = 1.0F;
    }

    for (i = ihi; i + 1 <= n; i++) {
      ia = (ia0 + i * 5) - 1;
      for (b_i = 1; b_i <= n; b_i++) {
        A[(ia + b_i) - 1] = 0.0F;
      }

      A[ia + i] = 1.0F;
    }

    ia = (ia0 + ilo) + ilo * 5;
    if (nh < 1) {
    } else {
      for (i = nh; i < nh; i++) {
        b_ia = ia + i * 5;
        for (b_i = 0; b_i < nh; b_i++) {
          A[(b_ia + b_i) - 1] = 0.0F;
        }

        A[(b_ia + i) - 1] = 1.0F;
      }

      itau = ((itau0 + ilo) + nh) - 3;
      for (b_i = 0; b_i < 5; b_i++) {
        work[b_i] = 0.0F;
      }

      for (b_i = nh; b_i >= 1; b_i--) {
        iaii = ((ia + b_i) + (b_i - 1) * 5) - 1;
        if (b_i < nh) {
          A[iaii - 1] = 1.0F;
          i = nh - b_i;
          if (tau[itau] != 0.0F) {
            lastv = i + 1;
            i += iaii;
            while ((lastv > 0) && (A[i - 1] == 0.0F)) {
              lastv--;
              i--;
            }

            lastc = nh - b_i;
            exitg2 = false;
            while ((!exitg2) && (lastc > 0)) {
              i = (iaii + (lastc - 1) * 5) + 5;
              b_ia = i;
              do {
                exitg1 = 0;
                if (b_ia <= (i + lastv) - 1) {
                  if (A[b_ia - 1] != 0.0F) {
                    exitg1 = 1;
                  } else {
                    b_ia++;
                  }
                } else {
                  lastc--;
                  exitg1 = 2;
                }
              } while (exitg1 == 0);

              if (exitg1 == 1) {
                exitg2 = true;
              }
            }
          } else {
            lastv = 0;
            lastc = 0;
          }

          if (lastv > 0) {
            if (lastc == 0) {
            } else {
              for (i = 1; i <= lastc; i++) {
                work[i - 1] = 0.0F;
              }

              i = 0;
              i3 = (iaii + 5 * (lastc - 1)) + 5;
              for (iac = iaii + 5; iac <= i3; iac += 5) {
                ix = iaii;
                c = 0.0F;
                i4 = (iac + lastv) - 1;
                for (b_ia = iac; b_ia <= i4; b_ia++) {
                  c += A[b_ia - 1] * A[ix - 1];
                  ix++;
                }

                work[i] += c;
                i++;
              }
            }

            eml_xgerc(lastv, lastc, -tau[itau], iaii, work, A, iaii + 5);
          }
        }

        if (b_i < nh) {
          i3 = (iaii + nh) - b_i;
          for (i = iaii; i + 1 <= i3; i++) {
            A[i] *= -tau[itau];
          }
        }

        A[iaii - 1] = 1.0F - tau[itau];
        for (i = 1; i < b_i; i++) {
          A[(iaii - i) - 1] = 0.0F;
        }

        itau--;
      }
    }
  }
}

/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
float rt_hypotf_snf(float u0, float u1)
{
  float y;
  float a;
  float b;
  a = fAbs(u0);
  b = fAbs(u1);
  if (a < b) {
    a = fDiv(a,b);
    y = b * fSqrt(a * a + 1.0F);
  } else if (a > b) {
    b = fDiv(b,a);
    y = a * fSqrt(b * b + 1.0F);
  } else if (fIsNan(b)) {
    y = b;
  } else {
    y = a * 1.41421354F;
  }

  return y;
}


/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  int coreid, k;
  boolean_T pass, flag;
  
  float A[25];
  int i;
  int istart;
  float tau[4];
  float U[25];
  int ix;
  float d[5];
  float y;
  float b_y;
  float xbar;
  float r;
  float c_y;
  float d_y;
  float tmp[4];

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
    // matlab kernel
    for (i = 0; i < 5; i++) {
      for (istart = 0; istart < 5; istart++) {
        A[istart + 5 * i] = fv0[(istart + 5 * i) + 25 * coreid];
      }
    }

    eml_xgehrd(A, tau);
    
    //memcpy(&U[0], &A[0], 25U * sizeof(float));
    for (int indx=0;indx<25;indx++)
      U[indx] = A[indx];
    //    mem_fp_cpy(&U[0], &A[0], 25);
    eml_xunghr(5, 1, 5, U, 1, tau, 1);
    eml_dlahqr(A, U);
    istart = 4;
    for (ix = 0; ix < 2; ix++) {
      for (i = istart; i < 6; i++) {
        A[(i + 5 * ix) - 1] = 0.0F;
      }

      istart++;
    }
  }

  synch_barrier();

  perf_end();

  /////////////////////////////////////////////////////////
  // check results
  /////////////////////////////////////////////////////////

  synch_barrier();

  for (ix = 0; ix < 5; ix++) {
    d[ix] = A[ix * 6];
  }

  y = fAbs(U[0]);
  for (istart = 0; istart < 24; istart++) {
    y += fAbs(U[istart + 1]);
  }

  b_y = d[0];
  for (istart = 0; istart < 4; istart++) {
    b_y += d[istart + 1];
  }

  ix = 0;
  xbar = fAbs(U[0]);
  for (istart = 0; istart < 24; istart++) {
    ix++;
    xbar += fAbs(U[ix]);
  }

  xbar = fDiv(xbar,25.0F);
  ix = 0;
  r = fAbs(U[0]) - xbar;
  c_y = r * r;
  for (istart = 0; istart < 24; istart++) {
    ix++;
    r = fAbs(U[ix]) - xbar;
    c_y += r * r;
  }

  c_y = fDiv(c_y,24.0F);
  ix = 0;
  xbar = d[0];
  for (istart = 0; istart < 4; istart++) {
    ix++;
    xbar += d[ix];
  }

  xbar = fDiv(xbar,5.0F);
  ix = 0;
  r = d[0] - xbar;
  d_y = r * r;
  for (istart = 0; istart < 4; istart++) {
    ix++;
    r = d[ix] - xbar;
    d_y += r * r;
  }

  d_y *= 0.25F;
  tmp[0] = y;
  tmp[1] = b_y;
  tmp[2] = c_y;
  tmp[3] = d_y;
  
  pass   = true;

  for (i = 0; i < 4; i++) {
    flag = true;
    flag = flag && (tmp[i] <= fv1[(0 + (i << 1)) + (coreid << 3)]);
    flag = flag && (tmp[i] >= fv1[(1 + (i << 1)) + (coreid << 3)]);
    printErrors(!flag, i, tmp[i] ,fv1[(0 + (i << 1)) + (coreid << 3)] ,fv1[(1 + (i << 1)) + (coreid << 3)]);
    pass = flag && pass;
  }

  synch_barrier();

  flagPassFail(pass, get_core_id());

/////////////////////////////////////////////////////////
// synchronize and exit
/////////////////////////////////////////////////////////

  return !pass;
}

