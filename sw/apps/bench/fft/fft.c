// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#include "common.h"

static short wprBase[] __sram = {
  32767,  32758,  32729,  32679,  32610,  32522,  32413,  32286,
  32138,  31972,  31786,  31581,  31357,  31114,  30853,  30572,
  30274,  29957,  29622,  29269,  28899,  28511,  28106,  27684,
  27246,  26791,  26320,  25833,  25330,  24812,  24279,  23732,
  23170,  22595,  22006,  21403,  20788,  20160,  19520,  18868,
  18205,  17531,  16846,  16151,  15447,  14733,  14010,  13279,
  12540,  11793,  11039,  10279,   9512,   8740,   7962,   7180,
   6393,   5602,   4808,   4011,   3212,   2411,   1608,    804,
      0,   -804,  -1608,  -2411,  -3212,  -4011,  -4808,  -5602,
  -6393,  -7180,  -7962,  -8740,  -9512, -10279, -11039, -11793,
 -12540, -13279, -14010, -14733, -15447, -16151, -16846, -17531,
 -18205, -18868, -19520, -20160, -20788, -21403, -22006, -22595,
 -23170, -23732, -24279, -24812, -25330, -25833, -26320, -26791,
 -27246, -27684, -28106, -28511, -28899, -29269, -29622, -29957,
 -30274, -30572, -30853, -31114, -31357, -31581, -31786, -31972,
 -32138, -32286, -32413, -32522, -32610, -32679, -32729, -32758,
};

static short wpiBase[] __sram = {
      0,    804,   1608,   2411,   3212,   4011,   4808,   5602,
   6393,   7180,   7962,   8740,   9512,  10279,  11039,  11793,
  12540,  13279,  14010,  14733,  15447,  16151,  16846,  17531,
  18205,  18868,  19520,  20160,  20788,  21403,  22006,  22595,
  23170,  23732,  24279,  24812,  25330,  25833,  26320,  26791,
  27246,  27684,  28106,  28511,  28899,  29269,  29622,  29957,
  30274,  30572,  30853,  31114,  31357,  31581,  31786,  31972,
  32138,  32286,  32413,  32522,  32610,  32679,  32729,  32758,
  32767,  32758,  32729,  32679,  32610,  32522,  32413,  32286,
  32138,  31972,  31786,  31581,  31357,  31114,  30853,  30572,
  30274,  29957,  29622,  29269,  28899,  28511,  28106,  27684,
  27246,  26791,  26320,  25833,  25330,  24812,  24279,  23732,
  23170,  22595,  22006,  21403,  20788,  20160,  19520,  18868,
  18205,  17531,  16846,  16151,  15447,  14733,  14010,  13279,
  12540,  11793,  11039,  10279,   9512,   8740,   7962,   7180,
   6393,   5602,   4808,   4011,   3212,   2411,   1608,    804,
};

void fft(short *data, int len) {

  int max = len;
  len <<= 1;
  int wstep = 1;
  while (max > 2) {
    short *wpr = wprBase;
    short *wpi = wpiBase;

    for (int m = 0; m < max; m +=2) {
      short wr = *wpr;
      short wi = *wpi;
      wpr+= wstep;
      wpi+= wstep;

      int step = max << 1;

      for (int i = m; i < len; i += step) {
        int j = i + max;

        short tr = data[i] - data[j];
        short ti = data[i+1] - data[j+1];

        data[i] += data[j];
        data[i+1] += data[j+1];

        int xr = ((wr * tr + wi * ti) << 1) + 0x8000;
        int xi = ((wr * ti - wi * tr) << 1) + 0x8000;

        data[j] = xr >> 16;
        data[j+1] = xi >> 16;
      }
    }
    max >>= 1;
    wstep <<= 1;
  }

  {
    int step = max << 1;

    for (int i = 0; i < len; i += step) {
      int j = i + max;

      short tr = data[i] - data[j];
      short ti = data[i+1] - data[j+1];

      data[i] += data[j];
      data[i+1] += data[j+1];


      data[j] = tr;
      data[j+1] = ti;
    }
  }


  #define SWAP(a, b) tmp=(a); (a)=(b); (b)=tmp

  data--;
  int j = 1;
  for (int i = 1; i < len; i += 2) {
    if(j > i) {
      short tmp;
      SWAP(data[j], data[i]);
      SWAP(data[j+1], data[i+1]);
    }
    int m = len>> 1;
    while (m >= 2 && j >m) {
      j -= m;
      m >>= 1;
    }
    j += m;
  }
}
