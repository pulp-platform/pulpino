// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// This testbench checks the basic functionality of:
//
// lv32.add.h
// lv32.add.h.sc
// lv32.add.h.sci
// lv32.add.b
// lv32.add.b.sc
// lv32.add.b.sci
//
// lv32.sub.h
// lv32.sub.h.sc
// lv32.sub.h.sci
// lv32.sub.b
// lv32.sub.b.sc
// lv32.sub.b.sci
//
// lv32.avg.h
// lv32.avg.h.sc
// lv32.avg.h.sci
// lv32.avg.b
// lv32.avg.b.sc
// lv32.avg.b.sci
//
//
// lv32.avgu.h
// lv32.avgu.h.sc
// lv32.avgu.h.sci
// lv32.avgu.b
// lv32.avgu.b.sc
// lv32.avgu.b.sci
//
// lv32.abs.h
// lv32.abs.b
//
// lv32.ext.h
// lv32.ext.h.sc
// lv32.ext.h.sci
// lv32.ext.b
// lv32.ext.b.sc
// lv32.ext.b.sci

// lv32.extu.h
// lv32.extu.h.sc
// lv32.extu.h.sci
// lv32.extu.b
// lv32.extu.b.sc
// lv32.extu.b.sci


#include "utils.h"
#include <stdio.h>
#include "bench.h"

#ifdef __riscv
#define SCI_IMM "11"
#define ADD           "ADD"
#define LV_ADD_H      "pv.add.h"
#define LV_ADD_H_SC   "pv.add.sc.h"
#define LV_ADD_H_SCI  "pv.add.sci.h"
#define LV_ADD_B      "pv.add.b"
#define LV_ADD_B_SC   "pv.add.sc.b"
#define LV_ADD_B_SCI  "pv.add.sci.b"
#define SUB           "SUB"
#define LV_SUB_H      "pv.sub.h"
#define LV_SUB_H_SC   "pv.sub.sc.h"
#define LV_SUB_H_SCI  "pv.sub.sci.h"
#define LV_SUB_B      "pv.sub.b"
#define LV_SUB_B_SC   "pv.sub.sc.b"
#define LV_SUB_B_SCI  "pv.sub.sci.b"
#define AVG           "p.avg"
#define LV_AVG_H      "pv.avg.h"
#define LV_AVG_H_SC   "pv.avg.sc.h"
#define LV_AVG_H_SCI  "pv.avg.sci.h"
#define LV_AVG_B      "pv.avg.b"
#define LV_AVG_B_SC   "pv.avg.sc.b"
#define LV_AVG_B_SCI  "pv.avg.sci.b"
#define AVGU          "p.avgu"
#define LV_AVGU_H     "pv.avgu.h"
#define LV_AVGU_H_SC  "pv.avgu.sc.h"
#define LV_AVGU_H_SCI "pv.avgu.sci.h"
#define LV_AVGU_B     "pv.avgu.b"
#define LV_AVGU_B_SC  "pv.avgu.sc.b"
#define LV_AVGU_B_SCI "pv.avgu.sci.b"
#define ABS           "p.abs"
#define LV_ABS_H      "pv.abs.h"
#define LV_ABS_B      "pv.abs.b"
#define LV_EXT_H      "pv.extract.h"
#define LV_EXT_B      "pv.extract.b"
#define LV_EXTU_H     "pv.extractu.h"
#define LV_EXTU_B     "pv.extractu.b"
#define LV_INS_H      "pv.insert.h"
#define LV_INS_B      "pv.insert.b"
#else
#define SCI_IMM "111"
#define ADD           "l.add"
#define LV_ADD_H      "lv32.add.h"
#define LV_ADD_H_SC   "lv32.add.h.sc"
#define LV_ADD_H_SCI  "lv32.add.h.sci"
#define LV_ADD_B      "lv32.add.b"
#define LV_ADD_B_SC   "lv32.add.b.sc"
#define LV_ADD_B_SCI  "lv32.add.b.sci"
#define SUB           "l.sub"
#define LV_SUB_H      "lv32.sub.h"
#define LV_SUB_H_SC   "lv32.sub.h.sc"
#define LV_SUB_H_SCI  "lv32.sub.h.sci"
#define LV_SUB_B      "lv32.sub.b"
#define LV_SUB_B_SC   "lv32.sub.b.sc"
#define LV_SUB_B_SCI  "lv32.sub.b.sci"
#define AVG           "l.avg"
#define LV_AVG_H      "lv32.avg.h"
#define LV_AVG_H_SC   "lv32.avg.h.sc"
#define LV_AVG_H_SCI  "lv32.avg.h.sci"
#define LV_AVG_B      "lv32.avg.b"
#define LV_AVG_B_SC   "lv32.avg.b.sc"
#define LV_AVG_B_SCI  "lv32.avg.b.sci"
#define AVGU           "l.avgu"
#define LV_AVGU_H      "lv32.avgu.h"
#define LV_AVGU_H_SC   "lv32.avgu.h.sc"
#define LV_AVGU_H_SCI  "lv32.avgu.h.sci"
#define LV_AVGU_B      "lv32.avgu.b"
#define LV_AVGU_B_SC   "lv32.avgu.b.sc"
#define LV_AVGU_B_SCI  "lv32.avgu.b.sci"
#define ABS            "l.abs"
#define LV_ABS_H       "lv32.abs.h"
#define LV_ABS_B       "lv32.abs.b"
#define LV_EXT_H       "lv32.ext.h"
#define LV_EXT_B       "lv32.ext.b"
#define LV_EXTU_H       "lv32.extu.h"
#define LV_EXTU_B       "lv32.extu.b"
#endif

#ifdef __riscv
#include "testVecArith_stimuli_riscv.h"
#else
#include "testVecArith_stimuli.h"
#endif

void check_add  (testresult_t *result, void (*start)(), void (*stop)());
void check_sub  (testresult_t *result, void (*start)(), void (*stop)());
void check_avg  (testresult_t *result, void (*start)(), void (*stop)());
void check_abs  (testresult_t *result, void (*start)(), void (*stop)());
void check_ext  (testresult_t *result, void (*start)(), void (*stop)());
void check_extu (testresult_t *result, void (*start)(), void (*stop)());
void check_ins  (testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "add"        , .test = check_add      },
  { .name = "sub"        , .test = check_sub      },
  { .name = "avg"        , .test = check_avg      },
  { .name = "abs"        , .test = check_abs      },
  { .name = "ext"        , .test = check_ext      },
  { .name = "extu"       , .test = check_extu     },
#ifdef __riscv
  { .name = "ins"        , .test = check_ins      },
#endif
  {0, 0}
};

int main()
{
  if(get_core_id() == 0) {
    return run_suite(testcases);
  }

  return 0;
}

void test_check(testresult_t *result, const char* asm_str, uint32_t act, uint32_t exp) {
  if (act != exp) {
    result->errors++;
    printf("%s: Actual %08x, expected %08x\n", asm_str, act, exp);
  }
}

#define util_check_rrr(result, asm_str, i, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      act = prefix ## _d[i]; \
      asm volatile (asm_str " %[c], %[a], %[b]\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r" (prefix ## _b[i])); \
      test_check(result, asm_str, act, prefix ## _exp[i]); \
    }

#define util_check_rrr_sci(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      act = prefix ## _d[i]; \
      asm volatile (asm_str " %[c], %[a], " #imm "\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i])); \
      test_check(result, asm_str, act, prefix ## _exp[i]); \
    }

#define util_check_rrri(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      act = prefix ## _d[i]; \
      asm volatile (asm_str " %[c], %[a], %[b], " #imm "\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r" (prefix ## _b[i])); \
      test_check(result, asm_str, act, prefix ## _exp[i]); \
    }

#define util_check_rr(result, asm_str, i, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], %[b]\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r" (prefix ## _b[i])); \
      test_check(result, asm_str, act, prefix ## _exp[i]); \
    }

#define util_check_rr_sci(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], " imm "\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i])); \
      test_check(result, asm_str, act, prefix ## _exp[i]); \
    }

#define util_check_rri(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], %[b], " #imm "\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r" (prefix ## _b[i])); \
      test_check(result, asm_str, act, prefix ## _exp[i]); \
    }

#define util_check_ri(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], " #imm "\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i])); \
      test_check(result, asm_str, act, prefix ## _exp[i]); \
    }

#define util_check_r(result, asm_str, i, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a]\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i])); \
      test_check(result, asm_str, act, prefix ## _exp[i]); \
    }

void check_add(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  //-----------------------------------------------------------------
  // Check l.add (standard addition)
  //-----------------------------------------------------------------
  util_check_rr(result, ADD, i, g_add);

  //-----------------------------------------------------------------
  // Check lv32.add
  //-----------------------------------------------------------------
  util_check_rr    (result, LV_ADD_H,     i, g_add_h);
  util_check_rr    (result, LV_ADD_H_SC,  i, g_add_h_sc);
  util_check_rr_sci(result, LV_ADD_H_SCI, i, g_add_h_sci, SCI_IMM);

  util_check_rr    (result, LV_ADD_B,     i, g_add_b);
  util_check_rr    (result, LV_ADD_B_SC,  i, g_add_b_sc);
  util_check_rr_sci(result, LV_ADD_B_SCI, i, g_add_b_sci, SCI_IMM);
}

void check_sub(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  //-----------------------------------------------------------------
  // Check l.sub (standard subtraction)
  //-----------------------------------------------------------------
  util_check_rr(result, SUB, i, g_sub);

  //-----------------------------------------------------------------
  // Check lv32.sub
  //-----------------------------------------------------------------
  util_check_rr    (result, LV_SUB_H,     i, g_sub_h);
  util_check_rr    (result, LV_SUB_H_SC,  i, g_sub_h_sc);
  util_check_rr_sci(result, LV_SUB_H_SCI, i, g_sub_h_sci, SCI_IMM);

  util_check_rr    (result, LV_SUB_B,     i, g_sub_b);
  util_check_rr    (result, LV_SUB_B_SC,  i, g_sub_b_sc);
  util_check_rr_sci(result, LV_SUB_B_SCI, i, g_sub_b_sci, SCI_IMM);
}

void check_avg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  // replaced by addN
  // util_check_rr(result, AVG,  i, g_avg);
  // util_check_rr(result, AVGU, i, g_avgu);

  //-----------------------------------------------------------------
  // Check lv32.avg.h
  //-----------------------------------------------------------------
  util_check_rr    (result, LV_AVG_H,     i, g_avg_h);
  util_check_rr    (result, LV_AVG_H_SC,  i, g_avg_h_sc);
  util_check_rr_sci(result, LV_AVG_H_SCI, i, g_avg_h_sci, SCI_IMM);

  //-----------------------------------------------------------------
  // Check lv32.avgu.h
  //-----------------------------------------------------------------
  util_check_rr    (result, LV_AVGU_H,     i, g_avgu_h);
  util_check_rr    (result, LV_AVGU_H_SC,  i, g_avgu_h_sc);
  util_check_rr_sci(result, LV_AVGU_H_SCI, i, g_avgu_h_sci, SCI_IMM);


  //-----------------------------------------------------------------
  // Check lv32.avg.b
  //-----------------------------------------------------------------
  util_check_rr    (result, LV_AVG_B,     i, g_avg_b);
  util_check_rr    (result, LV_AVG_B_SC,  i, g_avg_b_sc);
  util_check_rr_sci(result, LV_AVG_B_SCI, i, g_avg_b_sci, SCI_IMM);


  //-----------------------------------------------------------------
  // Check lv32.avgu.b
  //-----------------------------------------------------------------
  util_check_rr    (result, LV_AVGU_B,     i, g_avgu_b);
  util_check_rr    (result, LV_AVGU_B_SC,  i, g_avgu_b_sc);
  util_check_rr_sci(result, LV_AVGU_B_SCI, i, g_avgu_b_sci, SCI_IMM);
}

void check_abs(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  util_check_r(result, ABS,  i, g_abs);

  //-----------------------------------------------------------------
  // Check lv32.abs.h
  //-----------------------------------------------------------------
  util_check_r(result, LV_ABS_H, i, g_abs_h);

  //-----------------------------------------------------------------
  // Check lv32.abs.b
  //-----------------------------------------------------------------
  util_check_r(result, LV_ABS_B, i, g_abs_b);
}

void check_ext(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  //-----------------------------------------------------------------
  // Check lv32.ext
  //-----------------------------------------------------------------
  util_check_ri(result, LV_EXT_H, i, g_ext_h0, 0);
  util_check_ri(result, LV_EXT_H, i, g_ext_h1, 1);

  util_check_ri(result, LV_EXT_B, i, g_ext_b0, 0);
  util_check_ri(result, LV_EXT_B, i, g_ext_b1, 1);
  util_check_ri(result, LV_EXT_B, i, g_ext_b2, 2);
  util_check_ri(result, LV_EXT_B, i, g_ext_b3, 3);
}

void check_extu(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  //-----------------------------------------------------------------
  // Check lv32.ext
  //-----------------------------------------------------------------
  util_check_ri(result, LV_EXTU_H, i, g_extu_h0, 0);
  util_check_ri(result, LV_EXTU_H, i, g_extu_h1, 1);

  util_check_ri(result, LV_EXTU_B, i, g_extu_b0, 0);
  util_check_ri(result, LV_EXTU_B, i, g_extu_b1, 1);
  util_check_ri(result, LV_EXTU_B, i, g_extu_b2, 2);
  util_check_ri(result, LV_EXTU_B, i, g_extu_b3, 3);
}

void check_ins(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;
#ifdef __riscv
  //-----------------------------------------------------------------
  // Check pv.insert
  //-----------------------------------------------------------------
  util_check_rrr_sci(result, LV_INS_H, i, g_ins_h0, 0);
  util_check_rrr_sci(result, LV_INS_H, i, g_ins_h1, 1);

  util_check_rrr_sci(result, LV_INS_B, i, g_ins_b0, 0);
  util_check_rrr_sci(result, LV_INS_B, i, g_ins_b1, 1);
  util_check_rrr_sci(result, LV_INS_B, i, g_ins_b2, 2);
  util_check_rrr_sci(result, LV_INS_B, i, g_ins_b3, 3);
#endif
}