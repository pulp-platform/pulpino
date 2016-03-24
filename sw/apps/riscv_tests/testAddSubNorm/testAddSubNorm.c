
// This testbench checks the basic functionality of:
//

//l.addNr
//l.addNru
//l.addNl
//l.addNrR
//l.addNrRu
//
//l.subNr
//l.subNru
//l.subNl
//l.subNrR
//l.subNrRu

#include <stdio.h>
#include "utils.h"
#include "bench.h"

void check_addNru (testresult_t *result, void (*start)(), void (*stop)());
void check_addNr  (testresult_t *result, void (*start)(), void (*stop)());
void check_subNr  (testresult_t *result, void (*start)(), void (*stop)());
void check_addNl  (testresult_t *result, void (*start)(), void (*stop)());
void check_subNl  (testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "addNru"   , .test = check_addNru    },
  { .name = "addNr"    , .test = check_addNr     },
  { .name = "subNr"    , .test = check_subNr     },
#ifndef __riscv__
  { .name = "addNl"    , .test = check_addNl     },
  { .name = "subNl"    , .test = check_subNl     },
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
#define ADD_NRU  "p.addun"
#define ADD_NRRU "p.adduRN"
#define ADD_NR   "p.addN"
#define ADD_NRR  "p.addRN"
#define SUB_NR   "p.subN"
#define SUB_NRR  "p.subRN"
#else
#define ADD_NRU  "l.addNru"
#define ADD_NRRU "l.addNrRu"
#define ADD_NR   "l.addNr"
#define ADD_NRR  "l.addNrR"
#define SUB_NR   "l.subNr"
#define SUB_NRR  "l.subNrR"
#endif

void check_addNru(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (ADD_NRU " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_addNru_a[i]), [b] "r" (g_addNru_b[i]), [imm] "i" (g_addNru_IMM));

    check_uint32(result, "addNru", res,  g_addNru_exp[i]);

    asm volatile (ADD_NRRU " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_addNru_a[i]), [b] "r" (g_addNru_b[i]), [imm] "i" (g_addNru_IMM));

    check_uint32(result, "addNru", res,  g_addNrRu_exp[i]);
  }

}

void check_addNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (ADD_NR " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_addNr_a[i]), [b] "r" (g_addNr_b[i]), [imm] "i" (g_addNr_IMM));

    check_uint32(result, "addNr", res,  g_addNr_exp[i]);

    asm volatile (ADD_NRR " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_addNr_a[i]), [b] "r" (g_addNr_b[i]), [imm] "i" (g_addNr_IMM));

    check_uint32(result, "addNr", res,  g_addNrR_exp[i]);

  }

}

void check_subNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.subNr{R}
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
    asm volatile (SUB_NR " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_subNr_a[i]), [b] "r" (g_subNr_b[i]), [imm] "i" (g_subNr_IMM));

    check_uint32(result, "subNr", res,  g_subNr_exp[i]);

    asm volatile (SUB_NRR " %[c], %[a], %[b],%[imm]"
                  : [c] "=r" (res)
                  : [a] "r"  (g_subNr_a[i]), [b] "r" (g_subNr_b[i]), [imm] "i" (g_subNr_IMM));

    check_uint32(result, "subNr", res,  g_subNrR_exp[i]);

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
