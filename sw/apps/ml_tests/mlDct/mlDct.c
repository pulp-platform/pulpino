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
#include "mlDct.h"
#include "math_fns.h"
#include "bar.h"
#include "utils.h"
#include "bench.h"

/////////////////////////////////////////////////////////
// shared globals
/////////////////////////////////////////////////////////

  __attribute__ ((section(".heapsram"))) static const float dv0[128] = { 5.0863591120923317F, 2.1030842213096088F,
    9.9845298288829589F, 9.6991060573651851F, 6.731481705711321F,
    0.76505598359816962F, 2.1169347595066745F, 0.457577485728925F,
    5.0571044697357639F, 9.5015951533885641F, 5.7756112667333879F,
    4.394463117525742F, 6.0200409039664917F, 2.1454644040150583F,
    6.7256687057818665F, 3.2385797525095592F, 6.341428060560693F,
    3.5558071227557053F, 1.5258202228808504F, 0.84485700474387637F,
    7.3879948265632311F, 8.37413006096115F, 3.6747402557995734F, 1.2704808754954966F,
    0.733284011407932F, 8.9221733337333369F, 2.34806257611924F, 7.6720009027357818F,
    4.8706153822743445F, 7.9642780557012944F, 3.0860461623585653F,
    5.0859342720032394F, 8.0712249837547727F, 7.5031312154255234F,
    8.6175077387681682F, 5.5329518626143495F, 7.9564251965347923F,
    9.498605081606259F, 7.4583750020097748F, 0.61982991036180213F,
    1.9035159041735339F, 7.4645062754924583F, 1.0922473105139241F,
    9.7673850377606257F, 7.73325541597088F, 9.9429901005876626F, 3.1727194970255788F,
    7.3397414022471708F, 8.1552044047853638F, 4.1541190171134943F,
    4.4664847891687351F, 5.7617695223246947F, 7.6267548459940171F,
    6.1893992696994724F, 3.1478271203981256F, 8.0159434321400216F,
    7.6322717246370928F, 2.4573465230489813F, 5.0747344476133351F,
    1.4701930089396464F, 1.2969866545802533F, 6.6065461687495217F,
    0.73231204046801279F, 2.0156413967811462F, 8.5074725551830674F,
    5.957954736035318F, 6.7731219273958718F, 9.6495454210297886F, 8.054418769399712F,
    7.5105704406252976F, 7.984396698441528F, 2.7387327382079807F,
    4.1287392074921421F, 2.7342248028692318F, 2.1485433953890354F,
    2.8461086237939059F, 6.84535163002128F, 4.3476064224007205F, 5.5976207075698543F,
    2.0073153875080854F, 6.09905824110137F, 7.8869529492556918F,
    0.61429550642829067F, 9.8898426752028517F, 6.2153383240236684F,
    9.7438340990363219F, 8.54679699791956F, 0.39348463669488742F,
    4.4547578911800922F, 3.2321577056354602F, 1.5495966948276862F,
    8.7399321638476266F, 3.5213574241836154F, 3.9979861155737471F,
    9.111254085824104F, 3.9817812344011125F, 4.7493312437729642F,
    8.9412095434067123F, 6.875104876187109F, 6.59992612221552F, 7.7474288818392232F,
    8.0854976470280011F, 3.1796863114390526F, 0.0954553215680054F, 9.14243391675322F,
    0.97525984745820749F, 8.6431789995310364F, 9.5779735286063143F,
    8.6501405630682839F, 6.7389742077892087F, 5.5616399374938883F,
    4.513543894173818F, 3.2994277287263793F, 0.87931171817962617F,
    3.3157650092967872F, 8.4795597109349217F, 0.81606454321387467F,
    1.3991563027423493F, 2.87279137737404F, 8.28023861478086F, 5.5479216481567679F,
    3.2037284920419973F, 7.7967647350488525F, 8.7742041631186467F,
    5.7081768471271292F, 5.5948121246493345F, 7.1070291217003847F,
    3.3212638936568686F };

  __attribute__ ((section(".heapsram"))) static const float fv0[16] = { 38.3423653F, 38.3416F, 30.8577538F, 30.8571358F,
    42.1025734F, 42.1017303F, 38.9903336F, 38.9895515F, 46.2867393F, 46.2858162F,
    37.1457062F, 37.1449623F, 25.0504303F, 25.0499287F, 39.0325203F, 39.0317383F
  };

/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

/*
 * Arguments    : float y[]
 *                const int N
 *                const int stride
 *                const int startIdx
 * Return Type  : void
 */
void MWDSPCG_BitReverseData_YD(float y[], const int N, const int stride, const int startIdx)
{
  int j;
  int iIdx;
  int i;
  int jIdx;
  float t;
  j = 0;
  iIdx = startIdx;
  for (i = 0; i < N - 1; i++) {
    /* swap bit-reversed pairs - do not float swap */
    if (i < j) {
      /* swap when i<j */
      jIdx = startIdx + j * stride;
      t = y[iIdx];
      y[iIdx] = y[jIdx];
      y[jIdx] = t;
    }

    jIdx = N >> 1;
    j ^= jIdx;
    while (!((j & jIdx) != 0)) {
      jIdx >>= 1;
      j ^= jIdx;
    }

    iIdx += stride;
  }
}

/*
 * Arguments    : float y[]
 *                const int elem[]
 *                int N
 * Return Type  : void
 */
void MWDSPCG_Dct16Plus_YD_Trig(float y[], const int elem[], int N)
{
  int M;
  int i;
  float acc;
  float t;
  int w;
  int start;
  int end;
  int j;
  int a;
  int b;
  float angle;
  float c;
  if (N == 8) {
    /* terminate recursion here */
    MWDSPCG_Dct8_YD_Trig((float *)&y[0], (int *)&elem[0]);
  } else {
    /* else recurse top half and work on bottom half */
    M = N >> 1;

    /* first trivial butterfly stage */
    for (i = 0; i < M; i++) {
      /* butterfly */
      acc = y[elem[i]];
      acc += y[elem[(N - i) - 1]];
      t = acc;
      acc = y[elem[i]];
      acc -= y[elem[(N - i) - 1]];
      y[elem[(N - i) - 1]] = acc;
      y[elem[i]] = t;
    }

    /* call recursively for an N/2 point DCT */
    MWDSPCG_Dct16Plus_YD_Trig((float *)&y[0], (int *)&elem[0], M);

    /* Work on bottom half now */
    /* First c4 butterfly */
    /* cos(pi/4) */
    for (i = N >> 3; i < (N >> 2); i++) {
      /* butterfly */
      acc = y[elem[(N - i) - 1]];
      acc -= y[elem[M + i]];
      t = acc * 0.70710678118654757F;
      acc = y[elem[M + i]];
      acc += y[elem[(N - i) - 1]];
      y[elem[(N - i) - 1]] = acc * 0.70710678118654757F;
      y[elem[M + i]] = t;
    }

    i = 1;
    while (i < (M >> 2)) {
      /* for each stage */
      //      w = div_s32_floor(M, i << 2);
      w = M/(i<<2);
      start = 0;
      end = (w << 1) - 1;
      for (j = 0; j < i; j++) {
        /* Type 4 Matrices */
        a = 0;
        for (b = start; b < start + w; b++) {
          /* butterfly */
          acc = y[elem[M + b]];
          acc += y[elem[(M + end) - a]];
          t = acc;
          acc = y[elem[M + b]];
          acc -= y[elem[(M + end) - a]];
          y[elem[(M + end) - a]] = acc;
          y[elem[M + b]] = t;
          a++;
        }

        start += w << 1;
        end += w << 1;
        a = 0;
        for (b = start; b < start + w; b++) {
          /* butterfly */
          acc = y[elem[(M + end) - a]];
          acc -= y[elem[M + b]];
          t = acc;
          acc = y[elem[(M + end) - a]];
          acc += y[elem[M + b]];
          y[elem[(M + end) - a]] = acc;
          y[elem[M + b]] = t;
          a++;
        }

        start += w << 1;
        end += w << 1;
      }
      //      w = div_s32_floor(M, i << 3);
      w = M/(i<<3);
      start = w;
      end = w + w;
      for (j = 0; j < i; j++) {
        /* Type 3 matrices  */
        b = (i << 1) + j;
        a = 0;
        while (b != 0) {
          a <<= 1;
          a |= b & 1;
          b >>= 1;
        }

        angle = fDiv((float)a * 0.39269908169872414F,  (float)i);
        c     = fCos(angle);
        angle = fSin(angle);
        for (b = start; b < end; b++) {
          /* butterfly */
          acc = y[elem[M + b]] * -c;
          acc += y[elem[(N - b) - 1]] * angle;
          t = acc;
          acc = y[elem[(N - b) - 1]] * c;
          acc += y[elem[M + b]] * angle;
          y[elem[(N - b) - 1]] = acc;
          y[elem[M + b]] = t;
        }

        start += w;
        end += w;
        for (b = start; b < end; b++) {
          /* butterfly */
          acc = y[elem[M + b]] * -angle;
	  y[elem[(N - b) - 1]];
	  acc += -y[elem[(N - b) - 1]] * c;
	  t = acc;
          
          acc = y[elem[(N - b) - 1]] * angle;
	  acc += y[elem[M + b]] * -c;
	  y[elem[(N - b) - 1]] = acc;
	  y[elem[M + b]] = t;
        }

        start += 3 * w;
        end += 3 * w;
      }

      i <<= 1;
    }

    /* last but one stage 2x2 trivial butterflies! */
    /* process two butterflies each in this loop */
    for (i = 0; i < M; i += 4) {
      /* butterfly */
      acc = y[elem[M + i]];
      acc += y[elem[(M + i) + 1]];
      t = acc;
      acc = y[elem[M + i]];
      acc -= y[elem[(M + i) + 1]];
      y[elem[(M + i) + 1]] = acc;
      y[elem[M + i]] = t;

      /* butterfly */
      acc = y[elem[(M + i) + 3]];
      acc -= y[elem[(M + i) + 2]];
      t = acc;
      acc = y[elem[(M + i) + 3]];
      acc += y[elem[(M + i) + 2]];
      y[elem[(M + i) + 3]] = acc;
      y[elem[(M + i) + 2]] = t;
    }

    /* last stage */
    for (i = 0; i < (M >> 1); i++) {
      /* butterfly */
      b = M + i;
      a = 0;
      while (b != 0) {
        a <<= 1;
        a |= b & 1;
        b >>= 1;
      }
      angle = fDiv((float)a * 0.78539816339744828F, (float)M);
      c = fCos(angle);
      angle = fSin(angle);

      /* butterfly */
      acc = y[elem[M + i]] * angle;
      acc += y[elem[(N - i) - 1]] * c;
      t = acc;
      acc = y[elem[(N - i) - 1]] * angle;
      acc += y[elem[M + i]] * -c;
      y[elem[(N - i) - 1]] = acc;
      y[elem[M + i]] = t;
    }
  }
}

/*
 * Arguments    : float y[]
 *                const int elem[]
 * Return Type  : void
 */
void MWDSPCG_Dct8_YD_Trig(float y[], const int elem[])
{
  int i;
  float acc;
  float t;

  /* Stage 1 */
  for (i = 0; i < 4; i++) {
    /* butterfly */
    acc = y[elem[i]];
    acc += y[elem[7 - i]];
    t = acc;
    acc = y[elem[i]];
    acc -= y[elem[7 - i]];
    y[elem[7 - i]] = acc;
    y[elem[i]] = t;
  }

  /* Stage 2 */
  for (i = 0; i < 2; i++) {
    /* butterfly */
    acc = y[elem[i]];
    acc += y[elem[3 - i]];
    t = acc;
    acc = y[elem[i]];
    acc -= y[elem[3 - i]];
    y[elem[3 - i]] = acc;
    y[elem[i]] = t;
  }

  /* cos(pi/4) */
  /* butterfly */
  acc = y[elem[6]];
  acc -= y[elem[5]];
  t = acc * 0.70710678118654757F;
  acc = y[elem[6]];
  acc += y[elem[5]];
  y[elem[6]] = acc * 0.70710678118654757F;
  y[elem[5]] = t;

  /* Stage 3 */
  /* butterfly */
  acc = y[elem[0]];
  acc += y[elem[1]];
  t = acc * 0.70710678118654757F;
  acc = y[elem[0]];
  acc -= y[elem[1]];
  y[elem[1]] = acc * 0.70710678118654757F;
  y[elem[0]] = t;

  /* cos(pi/8) */
  /* sin(pi/8) */
  /* butterfly */
  acc = y[elem[2]] * 0.38268343236508978F;
  acc += y[elem[3]] * 0.92387953251128674F;
  t = acc;
  acc = y[elem[3]] * 0.38268343236508978F;
  acc += y[elem[2]] * -0.92387953251128674F;
  y[elem[3]] = acc;
  y[elem[2]] = t;

  /* butterfly */
  acc = y[elem[4]];
  acc += y[elem[5]];
  t = acc;
  acc = y[elem[4]];
  acc -= y[elem[5]];
  y[elem[5]] = acc;
  y[elem[4]] = t;

  /* butterfly */
  acc = y[elem[7]];
  acc -= y[elem[6]];
  t = acc;
  acc = y[elem[7]];
  acc += y[elem[6]];
  y[elem[7]] = acc;
  y[elem[6]] = t;

  /* Stage 4 */
  /* cos(pi/16) */
  /* sin(pi/16) */
  /* butterfly */
  acc = y[elem[4]] * 0.19509032201612825F;
  acc += y[elem[7]] * 0.98078528040323043F;
  t = acc;
  acc = y[elem[7]] * 0.19509032201612825F;
  acc += y[elem[4]] * -0.98078528040323043F;
  y[elem[7]] = acc;
  y[elem[4]] = t;

  /* cos(3*pi/16) */
  /* sin(3*pi/16) */
  /* butterfly */
  acc = y[elem[5]] * 0.83146961230254524F;
  acc += y[elem[6]] * 0.55557023301960218F;
  t = acc;
  acc = y[elem[6]] * 0.83146961230254524F;
  acc += y[elem[5]] * -0.55557023301960218F;
  y[elem[6]] = acc;
  y[elem[5]] = t;
}

/*
 * Arguments    : int elem[]
 *                const int N
 *                const int stride
 *                int elemIdx
 * Return Type  : void
 */
void MWDSPCG_IntroduceStride(int elem[], const int N, const int stride,
  int elemIdx)
{
  int i;
  for (i = 0; i < N; i++) {
    /* step by stride starting from elemIdx passed in */
    elem[i] = elemIdx;
    elemIdx += stride;
  }
}

/*
 * Arguments    : int numerator
 *                int denominator
 * Return Type  : int
 */
int div_s32_floor(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  unsigned int tempAbsQuotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator >= 0) {
      absNumerator = (unsigned int)numerator;
    } else {
      absNumerator = (unsigned int)-numerator;
    }

    if (denominator >= 0) {
      absDenominator = (unsigned int)denominator;
    } else {
      absDenominator = (unsigned int)-denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0) {
        tempAbsQuotient++;
      }
    }

    if (quotientNeedsNegation) {
      quotient = -(int)tempAbsQuotient;
    } else {
      quotient = (int)tempAbsQuotient;
    }
  }

  return quotient;
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
  dspcodegen_DCT s;
  dspcodegen_DCT *obj;
  dsp_DCT_1 *b_obj;
  float f[32];  
  float x[32];
  int ix;
  float b_f;
  float y;
  float xbar;
  int b_k;
  float r;
  float b_y;
  float tmp[2];  
  float golden[4];

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
    
    /*  mlDct */
    obj = &s;
    s.isInitialized = 0;

    /* System object Constructor function: dsp.DCT */
    obj->cSFunObject.P0_ScaleFactor = 0.25F;
    obj = &s;
    if (s.isInitialized != 1) {
      s.isInitialized = 1;
    }

    b_obj = &s.cSFunObject;

    /* System object Outputs function: dsp.DCT */
    memcpy(&f[0], &dv0[coreid << 5], sizeof(float) << 5);
    
    MWDSPCG_IntroduceStride((int *)&b_obj->W0_IDXWKSPACE[0], 32, 1, 0);
    MWDSPCG_Dct16Plus_YD_Trig((float *)&f[0], (int *)&b_obj->W0_IDXWKSPACE[0], 32);
    MWDSPCG_BitReverseData_YD((float *)&f[0], 32, 1, 0);
    
    for (ix = 0; ix < 32; ix++) {
      b_f = f[ix] * obj->cSFunObject.P0_ScaleFactor;
      x[ix] = b_f;
      f[ix] = b_f;
    }

  }  


  synch_barrier();
  
  perf_end();
  
  synch_barrier();
  
  /////////////////////////////////////////////////////////
  // check results
  /////////////////////////////////////////////////////////

  y = x[0];
  ix = 0;
  xbar = f[0];
  for (b_k = 0; b_k < 31; b_k++) {
    y += x[b_k + 1];
    ix++;
    xbar += f[ix];
  }

  xbar *= 1.0F/32.0F;
  ix = 0;
  r = f[0] - xbar;
  b_y = r * r;
  for (b_k = 0; b_k < 31; b_k++) {
    ix++;
    r = f[ix] - xbar;
    b_y += r * r;
  }

  b_y *= 1.0F/31.0F;
  tmp[0] = y;
  tmp[1] = b_y;

  pass = true;
  for (ix = 0; ix < 2; ix++) {
    for (b_k = 0; b_k < 2; b_k++) {
      golden[b_k + (ix << 1)] = fv0[(b_k + (ix << 1)) + (coreid << 2)];
    }
    flag = true;
    flag = flag && (tmp[ix] <= golden[ix << 1]);
    flag = flag && (tmp[ix] >= golden[1 + (ix << 1)]);
    printErrors(!flag, ix, tmp[ix] ,golden[(ix << 1)] ,golden[1 + (ix << 1)]);
    pass = pass && flag;
  }

  flagPassFail(pass, get_core_id());

/////////////////////////////////////////////////////////
// synchronize and exit
/////////////////////////////////////////////////////////

  return !pass;
}

