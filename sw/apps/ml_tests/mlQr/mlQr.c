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
#include "mlQr.h"
#include "math_fns.h"
#include "bar.h"
#include "utils.h"
#include "bench.h"

/////////////////////////////////////////////////////////
// shared globals
/////////////////////////////////////////////////////////

    __attribute__ ((section(".heapsram"))) static const float fv0[400] = { 0.978359163F, 8.28446293F, 2.74249792F,
    4.44618368F, 4.91883564F, 2.16686702F, 7.44941568F, 8.94630432F, 7.7380228F,
    8.9145689F, 8.13881111F, 6.82247972F, 6.94574881F, 9.52040482F, 4.26812506F,
    0.202749804F, 0.0769735F, 5.23738384F, 1.89208448F, 7.5554533F, 0.962459385F,
    2.57449317F, 5.51448774F, 0.673683822F, 7.48734903F, 8.10885715F,
    5.25850868F, 0.148076028F, 8.12766647F, 9.35018444F, 3.1359787F,
    0.436668903F, 3.13955688F, 9.87244129F, 9.44302177F, 0.267830729F,
    0.237358645F, 8.94860554F, 2.02599F, 1.40612018F, 6.64940262F, 4.49263811F,
    4.45186234F, 6.43537331F, 4.83642435F, 6.16256142F, 6.99662638F, 6.27520514F,
    6.55251F, 0.714409113F, 7.18750381F, 2.96428394F, 8.74764442F, 1.91141582F,
    9.80977345F, 1.07262421F, 2.49536753F, 1.95237458F, 6.60074043F, 5.66710663F,
    9.16386127F, 0.677633703F, 3.20289397F, 2.86197853F, 7.04885244F,
    7.43087196F, 5.57862759F, 0.964980304F, 2.01621914F, 9.84779263F,
    6.89153194F, 7.29993057F, 2.18266129F, 1.24191153F, 6.99823618F, 3.01602292F,
    3.26589131F, 4.65340185F, 4.56596518F, 7.28039694F, 9.88356686F, 3.24945545F,
    1.27619183F, 5.50671673F, 8.87466049F, 4.14843F, 7.32897282F, 4.60600662F,
    6.29835606F, 6.78692245F, 0.370993525F, 2.70403266F, 4.28514338F,
    0.665952444F, 5.89038849F, 5.15289974F, 7.25649452F, 2.30419445F,
    3.35459518F, 2.81692314F, 9.44757366F, 5.70327473F, 9.37386F, 4.79707766F,
    6.64475727F, 3.63449287F, 2.7037313F, 9.49244499F, 0.918644786F, 2.50710988F,
    9.546422F, 3.76888967F, 8.02194595F, 7.77187252F, 6.05623579F, 9.16164112F,
    7.10255766F, 3.46282744F, 8.42330265F, 4.11845779F, 3.99122691F, 8.33287144F,
    0.00536007201F, 5.72695446F, 1.87450373F, 9.48674F, 3.8071394F, 9.63703537F,
    4.38570452F, 4.20815039F, 5.15119267F, 3.93981433F, 5.52339363F, 8.19129467F,
    2.45019865F, 5.97979212F, 8.85469532F, 9.74499607F, 9.55252266F, 2.2377038F,
    6.8190546F, 7.2567873F, 0.410026133F, 7.23058319F, 8.82194614F, 7.64212084F,
    0.291405112F, 7.05209732F, 1.62227893F, 0.897139549F, 4.64760828F,
    3.00095391F, 6.21945953F, 0.606374383F, 9.57504177F, 1.53016424F,
    8.36569118F, 6.46711636F, 5.43956232F, 7.95679665F, 3.748878F, 6.62860823F,
    9.65424156F, 5.79582644F, 9.09280682F, 7.32101345F, 2.38123727F, 8.61099148F,
    6.99228811F, 9.34727192F, 8.55102F, 6.87434435F, 5.18081093F, 8.4190588F,
    8.43286F, 8.75141239F, 1.69889009F, 2.08599591F, 1.7026515F, 5.67082882F,
    3.64612484F, 8.23619938F, 1.7416836F, 0.569309652F, 9.46996593F, 3.45723748F,
    8.41721058F, 6.88166142F, 2.93998957F, 0.497126311F, 2.39243317F,
    2.24116468F, 3.3105092F, 4.65704679F, 1.45813096F, 5.98051071F, 0.361362487F,
    2.259938F, 8.49502373F, 3.86114717F, 7.69291258F, 6.16769409F, 0.740819275F,
    0.300507784F, 2.20782661F, 2.38371634F, 6.41797638F, 3.87059307F,
    3.21528959F, 3.78093767F, 9.59664F, 2.95995593F, 2.31274629F, 0.107653521F,
    6.13940239F, 9.2407608F, 0.794434607F, 8.80411816F, 9.7911129F, 9.46128082F,
    3.76968837F, 5.76319885F, 9.61743641F, 0.948687255F, 7.25171947F,
    8.25321293F, 5.09755087F, 2.66162848F, 9.85395527F, 7.96981192F,
    0.159166336F, 7.27598715F, 6.0311656F, 3.33444023F, 8.4270649F, 2.61638165F,
    3.87007689F, 4.18961096F, 2.49949789F, 1.24123788F, 4.89954185F,
    0.765888572F, 8.64368343F, 7.98042107F, 6.38620186F, 1.49082911F,
    7.57396269F, 3.88219118F, 5.07647896F, 0.579109251F, 0.0481676646F,
    9.21017075F, 8.84767056F, 3.98309255F, 8.19959164F, 9.42276764F, 2.17408371F,
    7.41782236F, 7.85490656F, 1.93362737F, 4.70119476F, 4.81361818F, 4.28209829F,
    9.65259647F, 7.52698755F, 3.80994749F, 9.54101086F, 0.719927311F,
    1.38916743F, 7.98733F, 0.491590858F, 3.49322176F, 8.1767168F, 9.9973278F,
    8.89987755F, 6.68147087F, 4.33519888F, 5.64888859F, 7.11771345F, 6.40900373F,
    1.74552F, 7.81947327F, 2.3796761F, 7.8580451F, 8.51677895F, 5.53961802F,
    5.32382822F, 8.12930202F, 6.91011286F, 4.54992F, 7.14694643F, 2.48260427F,
    3.98396564F, 7.93148661F, 2.87902188F, 0.270747811F, 9.53588104F, 9.79553F,
    9.99705124F, 3.39535451F, 6.50461388F, 3.48513603F, 7.4125123F, 9.82258415F,
    7.04917383F, 4.5748167F, 1.78437936F, 9.2738266F, 7.95705557F, 1.5601207F,
    3.92926192F, 9.0887394F, 8.55145073F, 6.53836393F, 0.529201269F, 3.67245674F,
    2.33352518F, 2.16650033F, 0.31958276F, 4.84085226F, 5.50597763F, 0.26984939F,
    7.84061575F, 7.05313396F, 8.59311867F, 4.40468788F, 0.185447112F,
    7.90666485F, 7.72993088F, 5.20327425F, 0.234597459F, 9.65525818F,
    8.35341644F, 8.60630512F, 0.9235093F, 0.745206416F, 5.24615145F,
    0.180778325F, 0.420307606F, 4.30879593F, 1.73209167F, 3.0514431F,
    3.72460437F, 6.63079119F, 4.167943F, 0.802753091F, 3.62733078F, 5.84344578F,
    5.32984877F, 8.38431931F, 9.17054653F, 7.1862359F, 0.132641703F, 4.06867933F,
    2.27168179F, 0.795718253F, 8.71114159F, 4.74596119F, 7.86214495F,
    6.89485168F, 4.08324575F, 8.43065834F, 1.99066973F, 2.13489771F, 4.87391233F,
    5.16345F, 5.16215801F, 7.15778446F, 1.33737409F, 8.89610863F, 6.29952955F,
    3.13235712F, 4.9834919F, 0.921435773F, 7.94419861F, 7.66502237F, 5.69099379F,
    1.68009794F, 7.90576839F, 7.60283709F, 8.17945576F, 1.28696275F, 5.8151288F,
    6.68394375F, 8.7433691F, 6.56122112F, 2.83550406F, 2.22196674F, 8.33105755F,
    8.53829765F, 6.16953039F, 4.58443594F, 9.38473606F, 9.24851704F, 5.71801F,
    8.39234447F, 6.46240425F, 9.32483578F, 5.18225F, 1.5328095F };


    __attribute__ ((section(".heapsram")))  static const float fv1[32] = { -2.01881242F, -2.01885295F, -136.447723F,
    -136.450439F, 0.100599431F, 0.100597419F, 30.5345783F, 30.533968F,
    -0.270549476F, -0.2705549F, -46.4437981F, -46.4447289F, 0.101003684F,
    0.101001658F, 38.612854F, 38.6120834F, -3.28489876F, -3.28496456F,
    -196.209961F, -196.213898F, 0.0999211147F, 0.099919118F, 34.4129448F,
    34.4122581F, -3.28090048F, -3.28096628F, -182.426758F, -182.43042F,
    0.0999237746F, 0.0999217778F, 32.35112F, 32.3504715F };

/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

float var(const float x[100])
{
  float y;
  int ix;
  float xbar;
  int k;
  float r;
  ix = 0;
  xbar = x[0];
  for (k = 0; k < 99; k++) {
    ix++;
    xbar += x[ix];
  }

  xbar *= 1.0F/100.0F;
  ix = 0;
  r = x[0] - xbar;
  y = r * r;
  for (k = 0; k < 99; k++) {
    ix++;
    r = x[ix] - xbar;
    y += r * r;
  }

  y *= 1.0F/99.0F;
  return y;
}
/*
 * Arguments    : float A[100]
 *                float tau[10]
 * Return Type  : void
 */
void eml_xgeqrf(float A[100], float tau[10])
{
  float work[10];
  int i;
  int i_i;
  float atmp;
  float f0;
  float xnorm;
  int knt;
  int i2;
  int b_i;
  int i_ip1;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int ia;
  int32_T exitg1;
  int ix;
  int j;

  for (i = 0; i < 10; i++) {
    work[i] = 0.0F;
  }

  for (i = 0; i < 10; i++) {
    i_i = i + i * 10;
    if (i + 1 < 10) {
      atmp = A[i_i];
      f0 = 0.0F;
      xnorm = eml_xnrm2(9 - i, A, i_i + 2);
      if (xnorm != 0.0F) {
        xnorm = rt_hypotf_snf(A[i_i], xnorm);
        if (A[i_i] >= 0.0F) {
          xnorm = -xnorm;
        }

        if ((real32_T)fAbs(xnorm) < 9.86076132E-32F) {
          knt = 0;
          do {
            knt++;
            i2 = (i_i - i) + 10;
            for (b_i = i_i + 1; b_i + 1 <= i2; b_i++) {
              A[b_i] *= 1.01412048E+31F;
            }

            xnorm *= 1.01412048E+31F;
            atmp *= 1.01412048E+31F;
          } while (!((real32_T)fAbs(xnorm) >= 9.86076132E-32F));

          xnorm = rt_hypotf_snf(atmp, eml_xnrm2(9 - i, A, i_i + 2));
          if (atmp >= 0.0F ) {
            xnorm = -xnorm;
          }

          f0 = fDiv((xnorm - atmp), xnorm);
          atmp = fDiv(1.0F, (atmp - xnorm));
          i2 = (i_i - i) + 10;
          for (b_i = i_i + 1; b_i + 1 <= i2; b_i++) {
            A[b_i] *= atmp;
          }

          for (b_i = 1; b_i <= knt; b_i++) {
            xnorm *= 9.86076132E-32F;
          }

          atmp = xnorm;
        } else {
          f0 = fDiv((xnorm - A[i_i]), xnorm);
          atmp = fDiv(1.0F, (A[i_i] - xnorm));
          i2 = (i_i - i) + 10;
          for (b_i = i_i + 1; b_i + 1 <= i2; b_i++) {
            A[b_i] *= atmp;
          }

          atmp = xnorm;
        }
      }

      tau[i] = f0;
    } else {
      atmp = A[i_i];
      tau[9] = 0.0F;
    }

    A[i_i] = atmp;
    if (i + 1 < 10) {
      atmp = A[i_i];
      A[i_i] = 1.0F;
      i_ip1 = (i + (i + 1) * 10) + 1;
      if (tau[i] != 0.0F) {
        lastv = 10 - i;
        b_i = (i_i - i) + 9;
        while ((lastv > 0) && (A[b_i] == 0.0F)) {
          lastv--;
          b_i--;
        }

        lastc = 9 - i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          knt = i_ip1 + (lastc - 1) * 10;
          ia = knt;
          do {
            exitg1 = 0;
            if (ia <= (knt + lastv) - 1) {
              if (A[ia - 1] != 0.0F) {
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
          for (knt = 1; knt <= lastc; knt++) {
            work[knt - 1] = 0.0F;
          }

          knt = 0;
          i2 = i_ip1 + 10 * (lastc - 1);
          for (b_i = i_ip1; b_i <= i2; b_i += 10) {
            ix = i_i;
            xnorm = 0.0F;
            j = (b_i + lastv) - 1;
            for (ia = b_i; ia <= j; ia++) {
	      xnorm += A[ia - 1] * A[ix];
              ix++;
            }

            work[knt] += xnorm;
            knt++;
          }
        }

        if (tau[i] == 0.0F) {
        } else {
          knt = i_ip1 - 1;
          b_i = 0;
          for (j = 1; j <= lastc; j++) {
            if (work[b_i] != 0.0F) {
              xnorm = work[b_i] * -tau[i];
              ix = i_i;
              i2 = lastv + knt;
              for (ia = knt; ia + 1 <= i2; ia++) {
                A[ia] += A[ix] * xnorm;
                ix++;
              }
            }

            b_i++;
            knt += 10;
          }
        }
      }

      A[i_i] = atmp;
    }
  }
}

/*
 * Arguments    : int n
 *                const float x[100]
 *                int ix0
 * Return Type  : float
 */
float eml_xnrm2(int n, const float x[100], int ix0)
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
    y = (real32_T)fAbs(x[ix0 - 1]);
  } else {
    scale = 1.17549435E-38F;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = (real32_T)fAbs(x[k - 1]);
      if (absxk > scale) {
        t = fDiv(scale, absxk);
        y = 1.0F + y * t * t;
        scale = absxk;
      } else {
        t = fDiv(absxk, scale);
	y += t * t;
      }
    }

    y = scale * (real32_T)fSqrt(y);
  }

  return y;
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
  a = (real32_T)fAbs(u0);
  b = (real32_T)fAbs(u1);
  if (a < b) {
    a = fDiv(a,b);
    y = b * (real32_T)fSqrt(a * a + 1.0F);
  } else if (a > b) {
    b = fDiv(b,a);
    y = a * (real32_T)fSqrt(b * b + 1.0F);
  } else if (fIsNan(b)) {
    y = b;
  } else {
    y = a * 1.41421354F;
  }

  return y;
}

/*
 * Arguments    : const float A[100]
 *                float Q[100]
 *                float R[100]
 * Return Type  : void
 */
void mlQr(const float A[100], float Q[100], float R[100])
{
  float b_A[100];
  float tau[10];
  int i;
  int b_i;
  int itau;
  float work[10];
  int iaii;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int ia;
  int32_T exitg1;
  int i1;
  int jA;
  int ix;
  float c;
  int jy;

  /*  mlRbf */
  for (int idxk = 0; idxk<100;idxk++)
    b_A[idxk] = A[idxk];

  eml_xgeqrf(b_A, tau);
  for (i = 0; i < 10; i++) {
    for (b_i = 0; b_i + 1 <= i + 1; b_i++) {
      R[b_i + 10 * i] = b_A[b_i + 10 * i];
    }

    for (b_i = i + 1; b_i + 1 < 11; b_i++) {
      R[b_i + 10 * i] = 0.0F;
    }
  }

  itau = 9;
  for (b_i = 0; b_i < 10; b_i++) {
    work[b_i] = 0.0F;
  }

  for (b_i = 9; b_i > -1; b_i += -1) {
    iaii = (b_i + b_i * 10) + 1;
    if (b_i + 1 < 10) {
      b_A[iaii - 1] = 1.0F;
      if (tau[itau] != 0.0F) {
        lastv = 10 - b_i;
        i = (iaii - b_i) + 8;
        while ((lastv > 0) && (b_A[i] == 0.0F)) {
          lastv--;
          i--;
        }

        lastc = 9 - b_i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          i = iaii + (lastc - 1) * 10;
          ia = i;
          do {
            exitg1 = 0;
            if (ia + 10 <= (i + lastv) + 9) {
              if (b_A[ia + 9] != 0.0F) {
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
          for (i = 1; i <= lastc; i++) {
            work[i - 1] = 0.0F;
          }

          i = 0;
          i1 = (iaii + 10 * (lastc - 1)) + 10;
          for (jA = iaii + 10; jA <= i1; jA += 10) {
            ix = iaii;
            c = 0.0F;
            jy = (jA + lastv) - 1;
            for (ia = jA; ia <= jy; ia++) {
              c += b_A[ia - 1] * b_A[ix - 1];
              ix++;
            }

            work[i] += c;
            i++;
          }
        }

        if (tau[itau] == 0.0F) {
        } else {
          jA = iaii + 9;
          jy = 0;
          for (i = 1; i <= lastc; i++) {
            if (work[jy] != 0.0F) {
              c = work[jy] * -tau[itau];
              ix = iaii;
              i1 = lastv + jA;
              for (ia = jA; ia + 1 <= i1; ia++) {
                b_A[ia] += b_A[ix - 1] * c;
                ix++;
              }
            }

            jy++;
            jA += 10;
          }
        }
      }
    }

    if (b_i + 1 < 10) {
      i1 = (iaii - b_i) + 9;
      for (i = iaii; i + 1 <= i1; i++) {
        b_A[i] *= (-tau[itau]);
      }
    }

    b_A[iaii - 1] = 1.0F - tau[itau];
    for (i = 1; i <= b_i; i++) {
      b_A[(iaii - i) - 1] = 0.0F;
    }

    itau--;
  }

  for (i = 0; i < 10; i++) {
    for (b_i = 0; b_i < 10; b_i++) {
      Q[b_i + 10 * i] = b_A[b_i + 10 * i];
    }
  }
}

/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  int coreid;
  int it;

  boolean_T pass, flag;
  float R[100];
  float Q[100];
  float y;
  float b_y;
  int b_k;
  float tmp[4];
  int i0;
  float golden[8];

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

  for(it = 0; it < KERNEL_ITS; it++)
  {
    // matlab kernel
    mlQr(*(float (*)[100])&fv0[100 * coreid], Q, R);

  }  

  synch_barrier();

  perf_end();
  
  synch_barrier();

  /////////////////////////////////////////////////////////
  // check results
  /////////////////////////////////////////////////////////

  y = Q[0];
  b_y = R[0];
  for (b_k = 0; b_k < 99; b_k++) {
    y += Q[b_k + 1];
    b_y += R[b_k + 1];
  }

  tmp[0] = y;
  tmp[1] = b_y;
  tmp[2] = var(Q);
  tmp[3] = var(R);

  pass   = true;
  for (b_k = 0; b_k < 4; b_k++) {
    for (i0 = 0; i0 < 2; i0++) {
      golden[i0 + (b_k << 1)] = fv1[(i0 + (b_k << 1)) + (coreid << 3)];
    }
    flag = true;
    flag = flag && (tmp[b_k] <= golden[b_k << 1]);
    flag = flag && (tmp[b_k] >= golden[1 + (b_k << 1)]);
    printErrors(!flag, b_k, tmp[b_k] ,golden[(b_k << 1)] ,golden[1 + (b_k << 1)]);
    pass = pass && flag;
  }

  flagPassFail(pass, get_core_id());
  
/////////////////////////////////////////////////////////
// synchronize and exit
/////////////////////////////////////////////////////////

  return !pass;
}

