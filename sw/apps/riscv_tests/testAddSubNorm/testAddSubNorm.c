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
/*
p.addun
p.addn
p.addurn
p.addrn
p.subun
p.subn
p.suburn
p.subrn
p.addunr
p.addnr
p.addurnr
p.addrnr
p.subunr
p.subnr
p.suburnr
p.subrnr
*/
#include <stdio.h>
#include "utils.h"
#include "bench.h"

// both riscv and orion
void check_adduRN    (testresult_t *result, void (*start)(), void (*stop)());
void check_addRN     (testresult_t *result, void (*start)(), void (*stop)());
void check_subRN     (testresult_t *result, void (*start)(), void (*stop)());

//riscv only
void check_subuRN    (testresult_t *result, void (*start)(), void (*stop)());
void check_adduRNr   (testresult_t *result, void (*start)(), void (*stop)());
void check_addRNr    (testresult_t *result, void (*start)(), void (*stop)());
void check_subRNr    (testresult_t *result, void (*start)(), void (*stop)());
void check_subuRNr   (testresult_t *result, void (*start)(), void (*stop)());
//orion only
void check_addNl     (testresult_t *result, void (*start)(), void (*stop)());
void check_subNl     (testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "adduRN"     , .test = check_adduRN      },
  { .name = "addRN"      , .test = check_addRN       },
  { .name = "subRN"      , .test = check_subRN       },
#ifndef __riscv__
  { .name = "addNl"      , .test = check_addNl       },
  { .name = "subNl"      , .test = check_subNl       },
#else
  { .name = "subuRN"     , .test = check_subuRN      },
  { .name = "adduRNr"    , .test = check_adduRNr     },
  { .name = "addRNr"     , .test = check_addRNr      },
  { .name = "subRNr"     , .test = check_subRNr      },
  { .name = "subuRNr"    , .test = check_subuRNr     },
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

#include "testAddSubNorm_stimuli.h"

#ifdef __riscv__
#define ADDU_N      "p.addun"
#define ADD_N       "p.addn"
#define ADDU_RN     "p.addurn"
#define ADD_RN      "p.addrn"
#define SUBU_N      "p.subun"
#define SUB_N       "p.subn"
#define SUBU_RN     "p.suburn"
#define SUB_RN      "p.subrn"
#define ADDU_Nr     "p.addunr"
#define ADD_Nr      "p.addnr"
#define ADDU_RNr    "p.addurnr"
#define ADD_RNr     "p.addrnr"
#define SUBU_Nr     "p.subunr"
#define SUB_Nr      "p.subnr"
#define SUBU_RNr    "p.suburnr"
#define SUB_RNr     "p.subrnr"
#else
#define ADDU_N      "l.addNru"
#define ADD_N       "l.addNr"
#define ADDU_RN     "l.addNrRu"
#define ADD_RN      "l.addNrR"
#define SUB_N       "l.subNr"
#define SUB_RN      "l.subNrR"
#endif

void check_adduRN(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.addu{R}N
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (ADDU_N " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_adduRN_a[i]), [b] "r" (g_adduRN_b[i]), [imm] "i" (g_adduRN_IMM));

    check_uint32(result, "adduN", res,  g_adduN_exp[i]);

    asm volatile (ADDU_RN " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_adduRN_a[i]), [b] "r" (g_adduRN_b[i]), [imm] "i" (g_adduRN_IMM));

    check_uint32(result, "adduRN", res,  g_adduRN_exp[i]);
  }
}

void check_addRN(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.add{R}N
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (ADD_N " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_addRN_a[i]), [b] "r" (g_addRN_b[i]), [imm] "i" (g_addRN_IMM));

    check_uint32(result, "addN", res,  g_addN_exp[i]);

    asm volatile (ADD_RN " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_addRN_a[i]), [b] "r" (g_addRN_b[i]), [imm] "i" (g_addRN_IMM));

    check_uint32(result, "addRN", res,  g_addRN_exp[i]);
  }
}

#ifdef __riscv__

void check_adduRNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.addu{R}Nr
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    res = g_adduRNr_c[i];
    asm volatile (ADDU_Nr " %[c], %[a], %[b]"
                  : [c] "+r" (res)
                  : [a] "r"  (g_adduRNr_a[i]), [b] "r" (g_adduRNr_b[i]));

    check_uint32(result, "adduNr", res,  g_adduNr_exp[i]);
    res = g_adduRNr_c[i];
    asm volatile (ADDU_RNr " %[c], %[a], %[b]"
                  : [c] "+r" (res)
                  : [a] "r"  (g_adduRNr_a[i]), [b] "r" (g_adduRNr_b[i]));

    check_uint32(result, "adduRNr", res,  g_adduRNr_exp[i]);
  }
}

void check_addRNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.add{R}Nr
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    res = g_addRNr_c[i];
    asm volatile (ADD_Nr " %[c], %[a], %[b]"
                  : [c] "+r" (res)
                  : [a] "r"  (g_addRNr_a[i]), [b] "r" (g_addRNr_b[i]));

    check_uint32(result, "addNr", res,  g_addNr_exp[i]);
    res = g_addRNr_c[i];
    asm volatile (ADD_RNr " %[c], %[a], %[b]"
                  : [c] "+r" (res)
                  : [a] "r"  (g_addRNr_a[i]), [b] "r" (g_addRNr_b[i]));

    check_uint32(result, "adduRNr", res,  g_addRNr_exp[i]);
  }
}

void check_subuRN(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.subu{R}N
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (SUBU_N " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_subuRN_a[i]), [b] "r" (g_subuRN_b[i]), [imm] "i" (g_subuRN_IMM));

    check_uint32(result, "subuN", res,  g_subuN_exp[i]);

    asm volatile (SUBU_RN " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_subuRN_a[i]), [b] "r" (g_subuRN_b[i]), [imm] "i" (g_subuRN_IMM));

    check_uint32(result, "subuRN", res,  g_subuRN_exp[i]);
  }
}

void check_subuRNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.subu{R}Nr
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    res = g_subuRNr_c[i];
    asm volatile (SUBU_Nr " %[c], %[a], %[b]"
                  : [c] "+r" (res)
                  : [a] "r"  (g_subuRNr_a[i]), [b] "r" (g_subuRNr_b[i]));

    check_uint32(result, "subuNr", res,  g_subuNr_exp[i]);
    res = g_subuRNr_c[i];
    asm volatile (SUBU_RNr " %[c], %[a], %[b]"
                  : [c] "+r" (res)
                  : [a] "r"  (g_subuRNr_a[i]), [b] "r" (g_subuRNr_b[i]));

    check_uint32(result, "subuRNr", res,  g_subuRNr_exp[i]);
  }
}

void check_subRNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.sub{R}Nr
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    res = g_subRNr_c[i];
    asm volatile (SUB_Nr " %[c], %[a], %[b]"
                  : [c] "+r" (res)
                  : [a] "r"  (g_subRNr_a[i]), [b] "r" (g_subRNr_b[i]));

    check_uint32(result, "subRNr", res,  g_subNr_exp[i]);
    res = g_subRNr_c[i];
    asm volatile (SUB_RNr " %[c], %[a], %[b]"
                  : [c] "+r" (res)
                  : [a] "r"  (g_subRNr_a[i]), [b] "r" (g_subRNr_b[i]));

    check_uint32(result, "subRNr", res,  g_subRNr_exp[i]);
  }
}


#endif

void check_subRN(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.sub{R}N
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (SUB_N " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_subRN_a[i]), [b] "r" (g_subRN_b[i]), [imm] "i" (g_subRN_IMM));

    check_uint32(result, "subN", res,  g_subN_exp[i]);

    asm volatile (SUB_RN " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_subRN_a[i]), [b] "r" (g_subRN_b[i]), [imm] "i" (g_subRN_IMM));

    check_uint32(result, "subRN", res,  g_subRN_exp[i]);
  }
}

#ifndef __riscv__
void check_addNl(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNl
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile ("l.addNl %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_addNl_a[i]), [b] "r" (g_addNl_b[i]), [imm] "i" (g_addNl_IMM));

    check_uint32(result, "addNl", res,  g_addNl_exp[i]);
  }
}

void check_subNl(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.subNl{R}
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile ("l.subNl %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_subNl_a[i]), [b] "r" (g_subNl_b[i]), [imm] "i" (g_subNl_IMM));

    check_uint32(result, "subNl", res,  g_subNl_exp[i]);
  }
}
#endif
