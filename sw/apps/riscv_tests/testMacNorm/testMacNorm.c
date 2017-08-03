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

//l.mulsNl
//l.macsNl
//l.muluNl
//l.macuNl
//l.mulusNl
//l.macusNl
//l.mulsNr
//l.macsNr
//l.muluNr
//l.macuNr
//l.mulusNr
//l.macusNr
//l.mulsNrR
//l.macsNrR
//l.muluNrR
//l.macuNrR
//l.mulusNrR
//l.macusNrR

#include <stdio.h>
#include "utils.h"
#include "bench.h"

#ifdef __riscv__
#define MULSNR   "p.mulsN"
#define MULUNR   "p.muluN"
#define MULSNRR  "p.mulsRN"
#define MULUNRR  "p.muluRN"
#define MACSNR   "p.macsN"
#define MACUNR   "p.macuN"
#define MACSNRR  "p.macsRN"
#define MACUNRR  "p.macuRN"
#else
#define MULSNL   "l.mulsNl"
#define MULUNL   "l.muluNl"
#define MULUSNL  "l.mulusNl"
#define MULSNR   "l.mulsNr"
#define MULUNR   "l.muluNr"
#define MULUSNR  "l.mulusNr"
#define MULSNRR  "l.mulsNrR"
#define MULUNRR  "l.muluNrR"
#define MULUSNRR "l.mulusNrR"
#define MACSNL   "l.macsNl"
#define MACUNL   "l.macuNl"
#define MACUSNL  "l.macusNl"
#define MACSNR   "l.macsNr"
#define MACUNR   "l.macuNr"
#define MACUSNR  "l.macusNr"
#define MACSNRR  "l.macsNrR"
#define MACUNRR  "l.macuNrR"
#define MACUSNRR "l.macusNrR"
#endif

void check_mulsNl  (testresult_t *result, void (*start)(), void (*stop)());
void check_muluNl  (testresult_t *result, void (*start)(), void (*stop)());
void check_mulusNl (testresult_t *result, void (*start)(), void (*stop)());
void check_mulsNr  (testresult_t *result, void (*start)(), void (*stop)());
void check_muluNr  (testresult_t *result, void (*start)(), void (*stop)());
void check_mulusNr (testresult_t *result, void (*start)(), void (*stop)());
void check_mulsNrR (testresult_t *result, void (*start)(), void (*stop)());
void check_muluNrR (testresult_t *result, void (*start)(), void (*stop)());
void check_mulusNrR(testresult_t *result, void (*start)(), void (*stop)());
void check_macsNl  (testresult_t *result, void (*start)(), void (*stop)());
void check_macuNl  (testresult_t *result, void (*start)(), void (*stop)());
void check_macusNl (testresult_t *result, void (*start)(), void (*stop)());
void check_macsNr  (testresult_t *result, void (*start)(), void (*stop)());
void check_macuNr  (testresult_t *result, void (*start)(), void (*stop)());
void check_macusNr (testresult_t *result, void (*start)(), void (*stop)());
void check_macsNrR (testresult_t *result, void (*start)(), void (*stop)());
void check_macuNrR (testresult_t *result, void (*start)(), void (*stop)());
void check_macusNrR(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "mulsNl"    , .test = check_mulsNl     },
  { .name = "macsNl"    , .test = check_macsNl     },
  { .name = "muluNl"    , .test = check_muluNl     },
  { .name = "macuNl"    , .test = check_macuNl     },
  { .name = "mulusNl"   , .test = check_mulusNl    },
  { .name = "macusNl"   , .test = check_macusNl    },
  { .name = "mulsNr"    , .test = check_mulsNr     },
  { .name = "macsNr"    , .test = check_macsNr     },
  { .name = "muluNr"    , .test = check_muluNr     },
  { .name = "macuNr"    , .test = check_macuNr     },
  { .name = "mulusNr"   , .test = check_mulusNr    },
  { .name = "macusNr"   , .test = check_macusNr    },
  { .name = "mulsNrR"   , .test = check_mulsNrR    },
  { .name = "macsNrR"   , .test = check_macsNrR    },
  { .name = "muluNrR"   , .test = check_muluNrR    },
  { .name = "macuNrR"   , .test = check_macuNrR    },
  { .name = "mulusNrR"  , .test = check_mulusNrR   },
  { .name = "macusNrR"  , .test = check_macusNrR   },
  {0, 0}
};

int main()
{
  if(get_core_id() == 0) {
    return run_suite(testcases);
  }

  return 0;
}

#include "testMacNorm_stimuli.h"

//################################################################################
//# T E S T    M U L {S, U, US} Nl
//################################################################################

void check_mulsNl(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MULSNL
    asm volatile (MULSNL " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_mulsNl_a[i]), [b] "r" (g_mulsNl_b[i]), [imm] "i" (g_mulsNl_IMM));

    check_uint32(result, "mulsNl", res,  g_mulsNl_exp[i]);
#endif
  }

}


void check_muluNl(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MULUNL
    asm volatile (MULUNL " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_muluNl_a[i]), [b] "r" (g_muluNl_b[i]), [imm] "i" (g_muluNl_IMM));

    check_uint32(result, "muluNl", res,  g_muluNl_exp[i]);
#endif

  }

}

void check_mulusNl(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MULUSNL
    asm volatile (MULUSNL " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_mulusNl_a[i]), [b] "r" (g_mulusNl_b[i]), [imm] "i" (g_mulusNl_IMM));

    check_uint32(result, "mulusNl", res,  g_mulusNl_exp[i]);
#endif
  }

}

//################################################################################
//# T E S T    M U L {S, U, US} Nr
//################################################################################

void check_mulsNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (MULSNR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_mulsNr_a[i]), [b] "r" (g_mulsNr_b[i]), [imm] "i" (g_mulsNr_IMM));

    check_uint32(result, "mulsNr", res,  g_mulsNr_exp[i]);

  }

}


void check_muluNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (MULUNR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_muluNr_a[i]), [b] "r" (g_muluNr_b[i]), [imm] "i" (g_muluNr_IMM));

    check_uint32(result, "muluNr", res,  g_muluNr_exp[i]);
  }

}

void check_mulusNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MULUSNR
    asm volatile (MULUSNR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_mulusNr_a[i]), [b] "r" (g_mulusNr_b[i]), [imm] "i" (g_mulusNr_IMM));

    check_uint32(result, "mulusNr", res,  g_mulusNr_exp[i]);
#endif
  }

}

//################################################################################
//# T E S T    M U L {S, U, US} NrR
//################################################################################

void check_mulsNrR(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNrR{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (MULSNRR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_mulsNrR_a[i]), [b] "r" (g_mulsNrR_b[i]), [imm] "i" (g_mulsNrR_IMM));

    check_uint32(result, "mulsNrR", res,  g_mulsNrR_exp[i]);


  }

}


void check_muluNrR(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNrR{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (MULUNRR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_muluNrR_a[i]), [b] "r" (g_muluNrR_b[i]), [imm] "i" (g_muluNrR_IMM));

    check_uint32(result, "muluNrR", res,  g_muluNrR_exp[i]);

  }

}

void check_mulusNrR(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNrR{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MULUSNRR
    asm volatile (MULUSNRR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (g_mulusNrR_a[i]), [b] "r" (g_mulusNrR_b[i]), [imm] "i" (g_mulusNrR_IMM));

    check_uint32(result, "mulusNrR", res,  g_mulusNrR_exp[i]);
#endif
  }

}

//################################################################################
//# T E S T    M A C {S, U, US} Nl
//################################################################################

void check_macsNl(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MACSNL
    asm volatile (MACSNL " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macsNl_c[i])
                  : [a] "r"  (g_macsNl_a[i]), [b] "r" (g_macsNl_b[i]), [imm] "i" (g_macsNl_IMM));

    check_uint32(result, "macsNl", g_macsNl_c[i],  g_macsNl_exp[i]);
#endif
  }
}


void check_macuNl(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MACUNL
    asm volatile (MACUNL " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macuNl_c[i])
                  : [a] "r"  (g_macuNl_a[i]), [b] "r" (g_macuNl_b[i]), [imm] "i" (g_macuNl_IMM));

    check_uint32(result, "macuNl", g_macuNl_c[i],  g_macuNl_exp[i]);
#endif

  }

}

void check_macusNl(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MACUSNL
    asm volatile (MACUSNL " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macusNl_c[i])
                  : [a] "r"  (g_macusNl_a[i]), [b] "r" (g_macusNl_b[i]), [imm] "i" (g_macusNl_IMM));

    check_uint32(result, "macusNl", g_macusNl_c[i],  g_macusNl_exp[i]);
#endif
  }

}

//################################################################################
//# T E S T    M A C {S, U, US} Nr
//################################################################################

void check_macsNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (MACSNR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macsNr_c[i])
                  : [a] "r"  (g_macsNr_a[i]), [b] "r" (g_macsNr_b[i]), [imm] "i" (g_macsNr_IMM));

    check_uint32(result, "macsNr", g_macsNr_c[i],  g_macsNr_exp[i]);


  }
}

void check_macuNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (MACUNR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macuNr_c[i])
                  : [a] "r"  (g_macuNr_a[i]), [b] "r" (g_macuNr_b[i]), [imm] "i" (g_macuNr_IMM));

    check_uint32(result, "macuNr", g_macuNr_c[i],  g_macuNr_exp[i]);


  }

}

void check_macusNr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNr{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MACUSNR
    asm volatile (MACUSNR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macusNr_c[i])
                  : [a] "r"  (g_macusNr_a[i]), [b] "r" (g_macusNr_b[i]), [imm] "i" (g_macusNr_IMM));

    check_uint32(result, "macusNr", g_macusNr_c[i],  g_macusNr_exp[i]);
#endif

  }

}

//################################################################################
//# T E S T    M A C {S, U, US} NrR
//################################################################################

void check_macsNrR(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNrR{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (MACSNRR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macsNrR_c[i])
                  : [a] "r"  (g_macsNrR_a[i]), [b] "r" (g_macsNrR_b[i]), [imm] "i" (g_macsNrR_IMM));

    check_uint32(result, "macsNrR", g_macsNrR_c[i],  g_macsNrR_exp[i]);


  }
}

void check_macuNrR(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNrR{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (MACUNRR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macuNrR_c[i])
                  : [a] "r"  (g_macuNrR_a[i]), [b] "r" (g_macuNrR_b[i]), [imm] "i" (g_macuNrR_IMM));

    check_uint32(result, "macuNrR", g_macuNrR_c[i],  g_macuNrR_exp[i]);

  }

}

void check_macusNrR(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.addNrR{R}u
  //-----------------------------------------------------------------
  for(i = 0; i < NumberOfStimuli; i++) {
#ifdef MACUSNRR
    asm volatile (MACUSNRR " %[c], %[a], %[b],%[imm]\n"
                  : [c] "+r" (g_macusNrR_c[i])
                  : [a] "r"  (g_macusNrR_a[i]), [b] "r" (g_macusNrR_b[i]), [imm] "i" (g_macusNrR_IMM));

    check_uint32(result, "macusNrR", g_macusNrR_c[i],  g_macusNrR_exp[i]);
#endif
  }

}
