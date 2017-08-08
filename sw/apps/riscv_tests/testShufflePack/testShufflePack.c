// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdio.h>
#include "utils.h"
#include "bench.h"

#ifdef __riscv
#include "testShufflePack_stimuli_riscv.h"
#else
#include "testShufflePack_stimuli.h"
#endif

#ifdef __riscv
#define SHUFFLE_H        "pv.shuffle.h"
#define SHUFFLE_H_SCI    "pv.shuffle.sci.h"
#define SHUFFLE_B        "pv.shuffle.b"
#define SHUFFLEI0_B_SCI  "pv.shuffleI0.sci.b"
#define SHUFFLEI1_B_SCI  "pv.shuffleI1.sci.b"
#define SHUFFLEI2_B_SCI  "pv.shuffleI2.sci.b"
#define SHUFFLEI3_B_SCI  "pv.shuffleI3.sci.b"
#define SHUFFLE2_H       "pv.shuffle2.h"
#define SHUFFLE2_B       "pv.shuffle2.b"
#define PACK_H_SC        "pv.pack.h"
#define PACKHI_B_SC      "pv.packhi.b"
#define PACKLO_B_SC      "pv.packlo.b"
#else
#define SHUFFLE_H     "lv32.shuffle.h"
#define SHUFFLE_H_SCI "lv32.shuffle.h.sci"
#define SHUFFLE_B     "lv32.shuffle.b"
#define SHUFFLE_B_SCI "lv32.shuffle.b.sci"
#define SHUFFLE2_H    "lv32.shuffle2.h"
#define SHUFFLE2_B    "lv32.shuffle2.b"
#define PACK_H_SC     "lv32.pack.h.sc"
#define PACKHI_B_SC   "lv32.packhi.b.sc"
#define PACKLO_B_SC   "lv32.packlo.b.sc"
#endif

void check_shuffle_h        (testresult_t *result, void (*start)(), void (*stop) () );
void check_shuffle_b        (testresult_t *result, void (*start)(), void (*stop) () );
void check_shuffle2_h       (testresult_t *result, void (*start)(), void (*stop) () );
void check_shuffle2_b       (testresult_t *result, void (*start)(), void (*stop) () );
void check_pack_h           (testresult_t *result, void (*start)(), void (*stop) () );
void check_pack_hi_b        (testresult_t *result, void (*start)(), void (*stop) () );
void check_pack_lo_b        (testresult_t *result, void (*start)(), void (*stop) () );

testcase_t testcases[] = {
  { .name = "shuffle_h"       , .test = check_shuffle_h       },
  { .name = "shuffle_b"       , .test = check_shuffle_b       },
  { .name = "shuffle2_h"      , .test = check_shuffle2_h      },
  { .name = "shuffle2_b"      , .test = check_shuffle2_b      },
  { .name = "pack_h"          , .test = check_pack_h          },
  { .name = "pack_hi_b"       , .test = check_pack_hi_b       },
  { .name = "pack_lo_b"       , .test = check_pack_lo_b       },
  {0, 0}
};

#ifdef __riscv
#define shuffle_b_sci(arg_a, arg_b, arg_exp) \
  if ((arg_b >> 6) == 0) { \
    asm volatile (SHUFFLEI0_B_SCI " %[c], %[a], %[imm]\n" \
      : [c] "+r" (res) \
      : [a] "r"  (arg_a), [imm] "i" (arg_b)); \
    check_uint32(result, "shuffle_b", res, arg_exp); \
  } else if ((arg_b >> 6) == 1) { \
    asm volatile (SHUFFLEI1_B_SCI " %[c], %[a], %[imm]\n" \
      : [c] "+r" (res) \
      : [a] "r"  (arg_a), [imm] "i" (arg_b-64)); \
    check_uint32(result, "shuffle_b", res, arg_exp); \
  } else if ((arg_b >> 6) == 2) { \
    asm volatile (SHUFFLEI2_B_SCI " %[c], %[a], %[imm]\n" \
      : [c] "+r" (res) \
      : [a] "r"  (arg_a), [imm] "i" (arg_b-128)); \
    check_uint32(result, "shuffle_b", res, arg_exp); \
  } else { \
    asm volatile (SHUFFLEI3_B_SCI " %[c], %[a], %[imm]\n" \
      : [c] "+r" (res) \
      : [a] "r"  (arg_a), [imm] "i" (arg_b-192)); \
    check_uint32(result, "shuffle_b", res, arg_exp); \
  }

#else
#define shuffle_b_sci(arg_a, arg_b, arg_exp) \
  asm volatile (SHUFFLE_B_SCI " %[c], %[a], %[imm]\n" \
    : [c] "+r" (res) \
    : [a] "r"  (arg_a), [imm] "i" (arg_b)); \
  check_uint32(result, "shuffle_b", res, arg_exp);
#endif

int main()
{
  int retval = 0;

  if(get_core_id() == 0) {
    return run_suite(testcases);
  }

  return 0;
}


//################################################################################
//#  test lv32.shuffle{ .sci}
//################################################################################

void check_shuffle_h(testresult_t *result, void (*start)(), void (*stop) ()) {
  unsigned int res = 0, i;

  //-----------------------------------------------------------------
  // Check lv32.shuffle.h
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (SHUFFLE_H " %[c], %[a], %[b]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_shuffle_h_a[i]), [b] "r" (g_shuffle_h_b[i]));

    check_uint32(result, "shuffle_h", res, g_shuffle_h_exp[i]);
  }

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
             : [c] "+r" (res)
             : [a] "r"  (g_shuffle_sci_h_a[0]), [imm] "i" (g_shuffle_sci_h_0));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[0]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[1]), [imm] "i" (g_shuffle_sci_h_1));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[1]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[2]), [imm] "i" (g_shuffle_sci_h_2));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[2]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[3]), [imm] "i" (g_shuffle_sci_h_3));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[3]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[4]), [imm] "i" (g_shuffle_sci_h_4));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[4]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[5]), [imm] "i" (g_shuffle_sci_h_5));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[5]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[6]), [imm] "i" (g_shuffle_sci_h_6));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[6]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[7]), [imm] "i" (g_shuffle_sci_h_7));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[7]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[8]), [imm] "i" (g_shuffle_sci_h_8));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[8]);

  asm volatile (SHUFFLE_H_SCI " %[c], %[a], %[imm]\n"
              : [c] "+r" (res)
              : [a] "r"  (g_shuffle_sci_h_a[9]), [imm] "i" (g_shuffle_sci_h_9));

  check_uint32(result, "shuffle_h_sci", res, g_shuffle_sci_h_exp[9]);
}

void check_shuffle_b(testresult_t *result, void (*start)(), void (*stop) ()) {
  unsigned int res=0,i;

  //-----------------------------------------------------------------
  // Check lv32.dotp_h
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (SHUFFLE_B " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_shuffle_b_a[i]), [b] "r" (g_shuffle_b_b[i]));

    check_uint32(result, "shuffle_b", res, g_shuffle_b_exp[i]);
  }

  shuffle_b_sci(g_shuffle_sci_b_a[0], g_shuffle_sci_b_0, g_shuffle_sci_b_exp[0])
  shuffle_b_sci(g_shuffle_sci_b_a[1], g_shuffle_sci_b_1, g_shuffle_sci_b_exp[1])
  shuffle_b_sci(g_shuffle_sci_b_a[2], g_shuffle_sci_b_2, g_shuffle_sci_b_exp[2])
  shuffle_b_sci(g_shuffle_sci_b_a[3], g_shuffle_sci_b_3, g_shuffle_sci_b_exp[3])
  shuffle_b_sci(g_shuffle_sci_b_a[4], g_shuffle_sci_b_4, g_shuffle_sci_b_exp[4])
  shuffle_b_sci(g_shuffle_sci_b_a[5], g_shuffle_sci_b_5, g_shuffle_sci_b_exp[5])
  shuffle_b_sci(g_shuffle_sci_b_a[6], g_shuffle_sci_b_6, g_shuffle_sci_b_exp[6])
  shuffle_b_sci(g_shuffle_sci_b_a[7], g_shuffle_sci_b_7, g_shuffle_sci_b_exp[7])
  shuffle_b_sci(g_shuffle_sci_b_a[8], g_shuffle_sci_b_8, g_shuffle_sci_b_exp[8])
  shuffle_b_sci(g_shuffle_sci_b_a[9], g_shuffle_sci_b_9, g_shuffle_sci_b_exp[9])
  shuffle_b_sci(g_shuffle_sci_b_a[10], g_shuffle_sci_b_10, g_shuffle_sci_b_exp[10])
  shuffle_b_sci(g_shuffle_sci_b_a[11], g_shuffle_sci_b_11, g_shuffle_sci_b_exp[11])
  shuffle_b_sci(g_shuffle_sci_b_a[12], g_shuffle_sci_b_12, g_shuffle_sci_b_exp[12])
  shuffle_b_sci(g_shuffle_sci_b_a[13], g_shuffle_sci_b_13, g_shuffle_sci_b_exp[13])
  shuffle_b_sci(g_shuffle_sci_b_a[14], g_shuffle_sci_b_14, g_shuffle_sci_b_exp[14])
  shuffle_b_sci(g_shuffle_sci_b_a[15], g_shuffle_sci_b_15, g_shuffle_sci_b_exp[15])
  shuffle_b_sci(g_shuffle_sci_b_a[16], g_shuffle_sci_b_16, g_shuffle_sci_b_exp[16])
  shuffle_b_sci(g_shuffle_sci_b_a[17], g_shuffle_sci_b_17, g_shuffle_sci_b_exp[17])
  shuffle_b_sci(g_shuffle_sci_b_a[18], g_shuffle_sci_b_18, g_shuffle_sci_b_exp[18])
  shuffle_b_sci(g_shuffle_sci_b_a[19], g_shuffle_sci_b_19, g_shuffle_sci_b_exp[19])
  shuffle_b_sci(g_shuffle_sci_b_a[20], g_shuffle_sci_b_20, g_shuffle_sci_b_exp[20])
  shuffle_b_sci(g_shuffle_sci_b_a[21], g_shuffle_sci_b_21, g_shuffle_sci_b_exp[21])
  shuffle_b_sci(g_shuffle_sci_b_a[22], g_shuffle_sci_b_22, g_shuffle_sci_b_exp[22])
  shuffle_b_sci(g_shuffle_sci_b_a[23], g_shuffle_sci_b_23, g_shuffle_sci_b_exp[23])
  shuffle_b_sci(g_shuffle_sci_b_a[24], g_shuffle_sci_b_24, g_shuffle_sci_b_exp[24])
  shuffle_b_sci(g_shuffle_sci_b_a[25], g_shuffle_sci_b_25, g_shuffle_sci_b_exp[25])
  shuffle_b_sci(g_shuffle_sci_b_a[26], g_shuffle_sci_b_26, g_shuffle_sci_b_exp[26])
  shuffle_b_sci(g_shuffle_sci_b_a[27], g_shuffle_sci_b_27, g_shuffle_sci_b_exp[27])
  shuffle_b_sci(g_shuffle_sci_b_a[28], g_shuffle_sci_b_28, g_shuffle_sci_b_exp[28])
  shuffle_b_sci(g_shuffle_sci_b_a[29], g_shuffle_sci_b_29, g_shuffle_sci_b_exp[29])
  shuffle_b_sci(g_shuffle_sci_b_a[30], g_shuffle_sci_b_30, g_shuffle_sci_b_exp[30])
  shuffle_b_sci(g_shuffle_sci_b_a[31], g_shuffle_sci_b_31, g_shuffle_sci_b_exp[31])
  shuffle_b_sci(g_shuffle_sci_b_a[32], g_shuffle_sci_b_32, g_shuffle_sci_b_exp[32])
  shuffle_b_sci(g_shuffle_sci_b_a[33], g_shuffle_sci_b_33, g_shuffle_sci_b_exp[33])
  shuffle_b_sci(g_shuffle_sci_b_a[34], g_shuffle_sci_b_34, g_shuffle_sci_b_exp[34])
  shuffle_b_sci(g_shuffle_sci_b_a[35], g_shuffle_sci_b_35, g_shuffle_sci_b_exp[35])
  shuffle_b_sci(g_shuffle_sci_b_a[36], g_shuffle_sci_b_36, g_shuffle_sci_b_exp[36])
  shuffle_b_sci(g_shuffle_sci_b_a[37], g_shuffle_sci_b_37, g_shuffle_sci_b_exp[37])
  shuffle_b_sci(g_shuffle_sci_b_a[38], g_shuffle_sci_b_38, g_shuffle_sci_b_exp[38])
  shuffle_b_sci(g_shuffle_sci_b_a[39], g_shuffle_sci_b_39, g_shuffle_sci_b_exp[39])
}

//################################################################################
//#  test lv32.shuffle2
//################################################################################

void check_shuffle2_h(testresult_t *result, void (*start)(), void (*stop) ()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check lv32.shuffle2.h
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (SHUFFLE2_H " %[c], %[a], %[b]\n"
                  : [c] "+r" (g_shuffle2_h_c[i])
                  : [a] "r"  (g_shuffle2_h_a[i]), [b] "r" (g_shuffle2_h_b[i]));

    check_uint32(result, "shuffle2_h", g_shuffle2_h_c[i], g_shuffle2_h_exp[i]);
  }
}

void check_shuffle2_b(testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int i;

  //-----------------------------------------------------------------
  // Check lv32.shuffle2.b
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (SHUFFLE2_B " %[c], %[a], %[b]\n"
                  : [c] "+r" (g_shuffle2_b_c[i])
                  : [a] "r"  (g_shuffle2_b_a[i]), [b] "r" (g_shuffle2_b_b[i]));

    check_uint32(result, "shuffle2_b", g_shuffle2_b_c[i], g_shuffle2_b_exp[i]);
 }
}

void check_pack_h(testresult_t *result, void (*start)(), void (*stop) ()) {
  unsigned int i,res = 0;

  //-----------------------------------------------------------------
  // Check lv32.pack.h
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (PACK_H_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_pack_h_a[i]), [b] "r" (g_pack_h_b[i]));

    check_uint32(result, "pack_h", res, g_pack_h_exp[i]);
  }
}

void check_pack_hi_b(testresult_t *result, void (*start)(), void (*stop) ()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check lv32.packhi
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (PACKHI_B_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (g_pack_hi_b_c[i])
                  : [a] "r"  (g_pack_hi_b_a[i]), [b] "r" (g_pack_hi_b_b[i]));

    check_uint32(result, "pack_hi_b", g_pack_hi_b_c[i], g_pack_hi_b_exp[i]);
  }
}

void check_pack_lo_b(testresult_t *result, void (*start)(), void (*stop) ()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check lv32.packlo
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (PACKLO_B_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (g_pack_lo_b_c[i])
                  : [a] "r"  (g_pack_lo_b_a[i]), [b] "r" (g_pack_lo_b_b[i]));

    check_uint32(result, "pack_lo_b", g_pack_lo_b_c[i], g_pack_lo_b_exp[i]);
  }
}
