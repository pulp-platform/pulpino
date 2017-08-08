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
// lv.cmp_eq.h
// lv.cmp_eq.h.sc
// lv.cmp_eq.h.sci
// lv.cmp_eq.b
// lv.cmp_eq.b.sc
// lv.cmp_eq.b.sci
//
// lv.cmp_ne.h
// lv.cmp_ne.h.sc
// lv.cmp_ne.h.sci
// lv.cmp_ne.b
// lv.cmp_ne.b.sc
// lv.cmp_ne.b.sci
//
// lv.cmp_le.h
// lv.cmp_le.h.sc
// lv.cmp_le.h.sci
// lv.cmp_le.b
// lv.cmp_le.b.sc
// lv.cmp_le.b.sci
//
// lv.cmp_leu.h
// lv.cmp_leu.h.sc
// lv.cmp_leu.h.sci
// lv.cmp_leu.b
// lv.cmp_leu.b.sc
// lv.cmp_leu.b.sci
//
// lv.cmp_lt.h
// lv.cmp_lt.h.sc
// lv.cmp_lt.h.sci
// lv.cmp_lt.b
// lv.cmp_lt.b.sc
// lv.cmp_lt.b.sci
//
// lv.cmp_ltu.h
// lv.cmp_ltu.h.sc
// lv.cmp_ltu.h.sci
// lv.cmp_ltu.b
// lv.cmp_ltu.b.sc
// lv.cmp_ltu.b.sci
//
// lv.cmp_ge.h
// lv.cmp_ge.h.sc
// lv.cmp_ge.h.sci
// lv.cmp_ge.b
// lv.cmp_ge.b.sc
// lv.cmp_ge.b.sci
//
// lv.cmp_geu.h
// lv.cmp_geu.h.sc
// lv.cmp_geu.h.sci
// lv.cmp_geu.b
// lv.cmp_geu.b.sc
// lv.cmp_geu.b.sci
//
// lv.cmp_gt.h
// lv.cmp_gt.h.sc
// lv.cmp_gt.h.sci
// lv.cmp_gt.b
// lv.cmp_gt.b.sc
// lv.cmp_gt.b.sci
//
// lv.cmp_gtu.h
// lv.cmp_gtu.h.sc
// lv.cmp_gtu.h.sci
// lv.cmp_gtu.b
// lv.cmp_gtu.b.sc
// lv.cmp_gtu.b.sci


#include "utils.h"
#include <stdio.h>
#include "bench.h"

#ifdef __riscv
#define SCI_IMM      "11"
#define CMPEQ_H      "pv.cmpeq.h"
#define CMPEQ_H_SC   "pv.cmpeq.sc.h"
#define CMPEQ_H_SCI  "pv.cmpeq.sci.h"
#define CMPEQ_B      "pv.cmpeq.b"
#define CMPEQ_B_SC   "pv.cmpeq.sc.b"
#define CMPEQ_B_SCI  "pv.cmpeq.sci.b"
#define CMPNE_H      "pv.cmpne.h"
#define CMPNE_H_SC   "pv.cmpne.sc.h"
#define CMPNE_H_SCI  "pv.cmpne.sci.h"
#define CMPNE_B      "pv.cmpne.b"
#define CMPNE_B_SC   "pv.cmpne.sc.b"
#define CMPNE_B_SCI  "pv.cmpne.sci.b"
#define CMPLE_H      "pv.cmple.h"
#define CMPLE_H_SC   "pv.cmple.sc.h"
#define CMPLE_H_SCI  "pv.cmple.sci.h"
#define CMPLE_B      "pv.cmple.b"
#define CMPLE_B_SC   "pv.cmple.sc.b"
#define CMPLE_B_SCI  "pv.cmple.sci.b"
#define CMPLEU_H     "pv.cmpleu.h"
#define CMPLEU_H_SC  "pv.cmpleu.sc.h"
#define CMPLEU_H_SCI "pv.cmpleu.sci.h"
#define CMPLEU_B     "pv.cmpleu.b"
#define CMPLEU_B_SC  "pv.cmpleu.sc.b"
#define CMPLEU_B_SCI "pv.cmpleu.sci.b"
#define CMPLT_H      "pv.cmplt.h"
#define CMPLT_H_SC   "pv.cmplt.sc.h"
#define CMPLT_H_SCI  "pv.cmplt.sci.h"
#define CMPLT_B      "pv.cmplt.b"
#define CMPLT_B_SC   "pv.cmplt.sc.b"
#define CMPLT_B_SCI  "pv.cmplt.sci.b"
#define CMPLTU_H     "pv.cmpltu.h"
#define CMPLTU_H_SC  "pv.cmpltu.sc.h"
#define CMPLTU_H_SCI "pv.cmpltu.sci.h"
#define CMPLTU_B     "pv.cmpltu.b"
#define CMPLTU_B_SC  "pv.cmpltu.sc.b"
#define CMPLTU_B_SCI "pv.cmpltu.sci.b"
#define CMPGE_H      "pv.cmpge.h"
#define CMPGE_H_SC   "pv.cmpge.sc.h"
#define CMPGE_H_SCI  "pv.cmpge.sci.h"
#define CMPGE_B      "pv.cmpge.b"
#define CMPGE_B_SC   "pv.cmpge.sc.b"
#define CMPGE_B_SCI  "pv.cmpge.sci.b"
#define CMPGEU_H     "pv.cmpgeu.h"
#define CMPGEU_H_SC  "pv.cmpgeu.sc.h"
#define CMPGEU_H_SCI "pv.cmpgeu.sci.h"
#define CMPGEU_B     "pv.cmpgeu.b"
#define CMPGEU_B_SC  "pv.cmpgeu.sc.b"
#define CMPGEU_B_SCI "pv.cmpgeu.sci.b"
#define CMPGT_H      "pv.cmpgt.h"
#define CMPGT_H_SC   "pv.cmpgt.sc.h"
#define CMPGT_H_SCI  "pv.cmpgt.sci.h"
#define CMPGT_B      "pv.cmpgt.b"
#define CMPGT_B_SC   "pv.cmpgt.sc.b"
#define CMPGT_B_SCI  "pv.cmpgt.sci.b"
#define CMPGTU_H     "pv.cmpgtu.h"
#define CMPGTU_H_SC  "pv.cmpgtu.sc.h"
#define CMPGTU_H_SCI "pv.cmpgtu.sci.h"
#define CMPGTU_B     "pv.cmpgtu.b"
#define CMPGTU_B_SC  "pv.cmpgtu.sc.b"
#define CMPGTU_B_SCI "pv.cmpgtu.sci.b"
#else
#define SCI_IMM      "111"
#define CMPEQ_H      "lv32.cmp_eq.h"
#define CMPEQ_H_SC   "lv32.cmp_eq.h.sc"
#define CMPEQ_H_SCI  "lv32.cmp_eq.h.sci"
#define CMPEQ_B      "lv32.cmp_eq.b"
#define CMPEQ_B_SC   "lv32.cmp_eq.b.sc"
#define CMPEQ_B_SCI  "lv32.cmp_eq.b.sci"
#define CMPNE_H      "lv32.cmp_ne.h"
#define CMPNE_H_SC   "lv32.cmp_ne.h.sc"
#define CMPNE_H_SCI  "lv32.cmp_ne.h.sci"
#define CMPNE_B      "lv32.cmp_ne.b"
#define CMPNE_B_SC   "lv32.cmp_ne.b.sc"
#define CMPNE_B_SCI  "lv32.cmp_ne.b.sci"
#define CMPLE_H      "lv32.cmp_le.h"
#define CMPLE_H_SC   "lv32.cmp_le.h.sc"
#define CMPLE_H_SCI  "lv32.cmp_le.h.sci"
#define CMPLE_B      "lv32.cmp_le.b"
#define CMPLE_B_SC   "lv32.cmp_le.b.sc"
#define CMPLE_B_SCI  "lv32.cmp_le.b.sci"
#define CMPLEU_H     "lv32.cmp_leu.h"
#define CMPLEU_H_SC  "lv32.cmp_leu.h.sc"
#define CMPLEU_H_SCI "lv32.cmp_leu.h.sci"
#define CMPLEU_B     "lv32.cmp_leu.b"
#define CMPLEU_B_SC  "lv32.cmp_leu.b.sc"
#define CMPLEU_B_SCI "lv32.cmp_leu.b.sci"
#define CMPLT_H      "lv32.cmp_lt.h"
#define CMPLT_H_SC   "lv32.cmp_lt.h.sc"
#define CMPLT_H_SCI  "lv32.cmp_lt.h.sci"
#define CMPLT_B      "lv32.cmp_lt.b"
#define CMPLT_B_SC   "lv32.cmp_lt.b.sc"
#define CMPLT_B_SCI  "lv32.cmp_lt.b.sci"
#define CMPLTU_H     "lv32.cmp_ltu.h"
#define CMPLTU_H_SC  "lv32.cmp_ltu.h.sc"
#define CMPLTU_H_SCI "lv32.cmp_ltu.h.sci"
#define CMPLTU_B     "lv32.cmp_ltu.b"
#define CMPLTU_B_SC  "lv32.cmp_ltu.b.sc"
#define CMPLTU_B_SCI "lv32.cmp_ltu.b.sci"
#define CMPGE_H      "lv32.cmp_ge.h"
#define CMPGE_H_SC   "lv32.cmp_ge.h.sc"
#define CMPGE_H_SCI  "lv32.cmp_ge.h.sci"
#define CMPGE_B      "lv32.cmp_ge.b"
#define CMPGE_B_SC   "lv32.cmp_ge.b.sc"
#define CMPGE_B_SCI  "lv32.cmp_ge.b.sci"
#define CMPGEU_H     "lv32.cmp_geu.h"
#define CMPGEU_H_SC  "lv32.cmp_geu.h.sc"
#define CMPGEU_H_SCI "lv32.cmp_geu.h.sci"
#define CMPGEU_B     "lv32.cmp_geu.b"
#define CMPGEU_B_SC  "lv32.cmp_geu.b.sc"
#define CMPGEU_B_SCI "lv32.cmp_geu.b.sci"
#define CMPGT_H      "lv32.cmp_gt.h"
#define CMPGT_H_SC   "lv32.cmp_gt.h.sc"
#define CMPGT_H_SCI  "lv32.cmp_gt.h.sci"
#define CMPGT_B      "lv32.cmp_gt.b"
#define CMPGT_B_SC   "lv32.cmp_gt.b.sc"
#define CMPGT_B_SCI  "lv32.cmp_gt.b.sci"
#define CMPGTU_H     "lv32.cmp_gtu.h"
#define CMPGTU_H_SC  "lv32.cmp_gtu.h.sc"
#define CMPGTU_H_SCI "lv32.cmp_gtu.h.sci"
#define CMPGTU_B     "lv32.cmp_gtu.b"
#define CMPGTU_B_SC  "lv32.cmp_gtu.b.sc"
#define CMPGTU_B_SCI "lv32.cmp_gtu.b.sci"
#endif

void check_cmp  (testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "veccmp"     , .test = check_cmp      },
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

#ifdef __riscv
#include "testVecCmp_stimuli_riscv.h"
#else
#include "testVecCmp_stimuli.h"
#endif

#define util_check_rr(result, asm_str, i, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], %[b]\n" \
                    : [c] "+r" (prefix ## _act[i]) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r" (prefix ## _b[i])); \
      test_check(result, asm_str, prefix ## _act[i], prefix ## _exp[i]); \
    }

#define util_check_rr_sci(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], " imm "\n" \
                    : [c] "+r" (prefix ## _act[i]) \
                    : [a] "r"  (prefix ## _a[i])); \
      test_check(result, asm_str, prefix ## _act[i], prefix ## _exp[i]); \
    }

#define util_check_rri(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], %[b], " imm "\n" \
                    : [c] "+r" (prefix ## _act[i]) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r" (prefix ## _b[i])); \
      test_check(result, asm_str, prefix ## _act[i], prefix ## _exp[i]); \
    }

#define util_check_ri(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], " imm "\n" \
                    : [c] "+r" (prefix ## _act[i]) \
                    : [a] "r"  (prefix ## _a[i])); \
      test_check(result, asm_str, prefix ## _act[i], prefix ## _exp[i]); \
    }

#define util_check_r(result, asm_str, i, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a]\n" \
                    : [c] "+r" (prefix ## _act[i]) \
                    : [a] "r"  (prefix ## _a[i])); \
      test_check(result, asm_str, prefix ## _act[i], prefix ## _exp[i]); \
    }

#define util_check_cmov_rr(result, asm_str, i, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], %[b]\n" \
                    "l.cmov   %[d], %[e0], %[e1]\n" \
                    : [c] "+r" (prefix ## _act[i]), \
                      [d] "+r" (prefix ## _act2[i]) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r"  (prefix ## _b[i]), \
                      [e0] "r" (0x13371337), \
                      [e1] "r" (0xDEADBEEF)); \
      test_check(result, asm_str, prefix ## _act[i],  prefix ## _exp[i]); \
      test_check(result, asm_str, prefix ## _act2[i], prefix ## _cmov[i]); \
    }

#define util_check_cmov_rr_sci(result, asm_str, i, prefix, imm) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], " imm "\n" \
                    "l.cmov   %[d], %[e0], %[e1]\n" \
                    : [c] "+r" (prefix ## _act[i]), \
                      [d] "+r" (prefix ## _act2[i]) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [e0] "r" (0x13371337), \
                      [e1] "r" (0xDEADBEEF)); \
      test_check(result, asm_str, prefix ## _act[i],  prefix ## _exp[i]); \
      test_check(result, asm_str, prefix ## _act2[i], prefix ## _cmov[i]); \
    }

void check_cmp (testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check lv.cmp_eq
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPEQ_H,     i, g_eq_h);
  util_check_rr    (result, CMPEQ_H_SC,  i, g_eq_h_sc);
  util_check_rr_sci(result, CMPEQ_H_SCI, i, g_eq_h_sci, SCI_IMM);

  util_check_rr    (result, CMPEQ_B,     i, g_eq_b);
  util_check_rr    (result, CMPEQ_B_SC,  i, g_eq_b_sc);
  util_check_rr_sci(result, CMPEQ_B_SCI, i, g_eq_b_sci, SCI_IMM);

  //-----------------------------------------------------------------
  // Check lv32.cmp_ne
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPNE_H,     i, g_ne_h);
  util_check_rr    (result, CMPNE_H_SC,  i, g_ne_h_sc);
  util_check_rr_sci(result, CMPNE_H_SCI, i, g_ne_h_sci, SCI_IMM);

  util_check_rr    (result, CMPNE_B,     i, g_ne_b);
  util_check_rr    (result, CMPNE_B_SC,  i, g_ne_b_sc);
  util_check_rr_sci(result, CMPNE_B_SCI, i, g_ne_b_sci, SCI_IMM);

  //-----------------------------------------------------------------
  // Check lv32.cmp_le
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPLE_H,     i, g_le_h);
  util_check_rr    (result, CMPLE_H_SC,  i, g_le_h_sc);
  util_check_rr_sci(result, CMPLE_H_SCI, i, g_le_h_sci, SCI_IMM);

  util_check_rr    (result, CMPLE_B,     i, g_le_b);
  util_check_rr    (result, CMPLE_B_SC,  i, g_le_b_sc);
  util_check_rr_sci(result, CMPLE_B_SCI, i, g_le_b_sci, SCI_IMM);

  //-----------------------------------------------------------------
  // Check lv32.cmp_leu
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPLEU_H,     i, g_leu_h);
  util_check_rr    (result, CMPLEU_H_SC,  i, g_leu_h_sc);
  util_check_rr_sci(result, CMPLEU_H_SCI, i, g_leu_h_sci, SCI_IMM);

  util_check_rr    (result, CMPLEU_B,     i, g_leu_b);
  util_check_rr    (result, CMPLEU_B_SC,  i, g_leu_b_sc);
  util_check_rr_sci(result, CMPLEU_B_SCI, i, g_leu_b_sci, SCI_IMM);

  //-----------------------------------------------------------------
  // Check lv32.cmp_lt
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPLT_H,     i, g_lt_h);
  util_check_rr    (result, CMPLT_H_SC,  i, g_lt_h_sc);
  util_check_rr_sci(result, CMPLT_H_SCI, i, g_lt_h_sci, SCI_IMM);

  util_check_rr    (result, CMPLT_B,     i, g_lt_b);
  util_check_rr    (result, CMPLT_B_SC,  i, g_lt_b_sc);
  util_check_rr_sci(result, CMPLT_B_SCI, i, g_lt_b_sci, SCI_IMM);

   //-----------------------------------------------------------------
  // Check lv32.cmp_ltu
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPLTU_H,     i, g_ltu_h);
  util_check_rr    (result, CMPLTU_H_SC,  i, g_ltu_h_sc);
  util_check_rr_sci(result, CMPLTU_H_SCI, i, g_ltu_h_sci, SCI_IMM);

  util_check_rr    (result, CMPLTU_B,     i, g_ltu_b);
  util_check_rr    (result, CMPLTU_B_SC,  i, g_ltu_b_sc);
  util_check_rr_sci(result, CMPLTU_B_SCI, i, g_ltu_b_sci, SCI_IMM);


  //-----------------------------------------------------------------
  // Check lv32.cmp_ge
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPGE_H,     i, g_ge_h);
  util_check_rr    (result, CMPGE_H_SC,  i, g_ge_h_sc);
  util_check_rr_sci(result, CMPGE_H_SCI, i, g_ge_h_sci, SCI_IMM);

  util_check_rr    (result, CMPGE_B,     i, g_ge_b);
  util_check_rr    (result, CMPGE_B_SC,  i, g_ge_b_sc);
  util_check_rr_sci(result, CMPGE_B_SCI, i, g_ge_b_sci, SCI_IMM);

  //-----------------------------------------------------------------
  // Check lv32.cmp_geu
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPGEU_H,     i, g_geu_h);
  util_check_rr    (result, CMPGEU_H_SC,  i, g_geu_h_sc);
  util_check_rr_sci(result, CMPGEU_H_SCI, i, g_geu_h_sci, SCI_IMM);

  util_check_rr    (result, CMPGEU_B,     i, g_geu_b);
  util_check_rr    (result, CMPGEU_B_SC,  i, g_geu_b_sc);
  util_check_rr_sci(result, CMPGEU_B_SCI, i, g_geu_b_sci, SCI_IMM);

  //-----------------------------------------------------------------
  // Check lv32.cmp_gt
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPGT_H,     i, g_gt_h);
  util_check_rr    (result, CMPGT_H_SC,  i, g_gt_h_sc);
  util_check_rr_sci(result, CMPGT_H_SCI, i, g_gt_h_sci, SCI_IMM);

  util_check_rr    (result, CMPGT_B,     i, g_gt_b);
  util_check_rr    (result, CMPGT_B_SC,  i, g_gt_b_sc);
  util_check_rr_sci(result, CMPGT_B_SCI, i, g_gt_b_sci, SCI_IMM);

  //-----------------------------------------------------------------
  // Check lv32.cmp_gtu
  //-----------------------------------------------------------------
  util_check_rr    (result, CMPGTU_H,     i, g_gtu_h);
  util_check_rr    (result, CMPGTU_H_SC,  i, g_gtu_h_sc);
  util_check_rr_sci(result, CMPGTU_H_SCI, i, g_gtu_h_sci, SCI_IMM);

  util_check_rr    (result, CMPGTU_B,     i, g_gtu_b);
  util_check_rr    (result, CMPGTU_B_SC,  i, g_gtu_b_sc);
  util_check_rr_sci(result, CMPGTU_B_SCI, i, g_gtu_b_sci, SCI_IMM);
}
