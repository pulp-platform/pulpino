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
#include "testBitManipulation_stimuli_riscv.h"
#else
#include "testBitManipulation_stimuli.h"
#endif

#ifdef __riscv
#define BSET       "p.bset"
#define BCLR       "p.bclr"
#define BEXTRACT   "p.extract"
#define BEXTRACTU  "p.extractu"
#define BINSERT    "p.insert"
#define BSETR      "p.bsetr"
#define BCLRR      "p.bclrr"
#define BEXTRACTR  "p.extractr"
#define BEXTRACTUR "p.extractur"
#define BINSERTR   "p.insertr"
#else
#define BSET       "l.bset"
#define BCLR       "l.bclr"
#define BEXTRACT   "l.bextract"
#define BEXTRACTU  "l.bextractu"
#define BINSERT    "l.binsert"
#endif

void check_bset          (testresult_t *result, void (*start)(), void (*stop)());
void check_bclr          (testresult_t *result, void (*start)(), void (*stop)());
void check_bextract      (testresult_t *result, void (*start)(), void (*stop)());
void check_bextractu     (testresult_t *result, void (*start)(), void (*stop)());
void check_binsert       (testresult_t *result, void (*start)(), void (*stop)());

#ifdef __riscv
void check_bset_reg      (testresult_t *result, void (*start)(), void (*stop)());
void check_bclr_reg      (testresult_t *result, void (*start)(), void (*stop)());
void check_bextract_reg  (testresult_t *result, void (*start)(), void (*stop)());
void check_bextractu_reg (testresult_t *result, void (*start)(), void (*stop)());
void check_binsert_reg   (testresult_t *result, void (*start)(), void (*stop)());
void check_breverse      (testresult_t *result, void (*start)(), void (*stop)());
#endif

testcase_t testcases[] = {
 // { .name = "bset"          , .test = check_bset          },
 // { .name = "bclr"          , .test = check_bclr          },
 // { .name = "bextract"      , .test = check_bextract      },
 // { .name = "bextractu"     , .test = check_bextractu     },
 // { .name = "binsert"       , .test = check_binsert       },
#ifdef __riscv
 // { .name = "bset_reg"      , .test = check_bset_reg      },
 // { .name = "bclr_reg"      , .test = check_bclr_reg      },
 // { .name = "bextract_reg"  , .test = check_bextract_reg  },
 // { .name = "bextractu_reg" , .test = check_bextractu_reg },
 // { .name = "binsert_reg"   , .test = check_binsert_reg   },
 { .name = "breverse"      , .test = check_breverse   },
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

//################################################################################
//# T E S T    BSET
//################################################################################

void check_bset(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.bset
  //-----------------------------------------------------------------

  asm volatile ("addi x5, x0, 5;"
                // ".word 0xc262d333;"
                ".word 0xC5A2D333;"
                "nop" : : : "x5", "x6");

  /*
    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[0]), [L] "i" (LEN_0), [I] "i" (IMM_0));

    check_uint32(result, "bset", res,  res_bset[0]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[1]), [L] "i" (LEN_1), [I] "i" (IMM_1));

    check_uint32(result, "bset", res,  res_bset[1]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[2]), [L] "i" (LEN_2), [I] "i" (IMM_2));

    check_uint32(result, "bset", res,  res_bset[2]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[3]), [L] "i" (LEN_3), [I] "i" (IMM_3));

    check_uint32(result, "bset", res,  res_bset[3]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[4]), [L] "i" (LEN_4), [I] "i" (IMM_4));

    check_uint32(result, "bset", res,  res_bset[4]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[5]), [L] "i" (LEN_5), [I] "i" (IMM_5));

    check_uint32(result, "bset", res,  res_bset[5]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[6]), [L] "i" (LEN_6), [I] "i" (IMM_6));

    check_uint32(result, "bset", res,  res_bset[6]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[7]), [L] "i" (LEN_7), [I] "i" (IMM_7));

    check_uint32(result, "bset", res,  res_bset[7]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[8]), [L] "i" (LEN_8), [I] "i" (IMM_8));

    check_uint32(result, "bset", res,  res_bset[8]);

    asm volatile (BSET " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[9]), [L] "i" (LEN_9), [I] "i" (IMM_9));

    check_uint32(result, "bset", res,  res_bset[9]);
    */
}

void check_bclr(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.bclr
  //-----------------------------------------------------------------
    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[0]), [L] "i" (LEN_0), [I] "i" (IMM_0));

    check_uint32(result, "bclr", res,  res_bclr[0]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[1]), [L] "i" (LEN_1), [I] "i" (IMM_1));

    check_uint32(result, "bclr", res,  res_bclr[1]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[2]), [L] "i" (LEN_2), [I] "i" (IMM_2));

    check_uint32(result, "bclr", res,  res_bclr[2]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[3]), [L] "i" (LEN_3), [I] "i" (IMM_3));

    check_uint32(result, "bclr", res,  res_bclr[3]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[4]), [L] "i" (LEN_4), [I] "i" (IMM_4));

    check_uint32(result, "bclr", res,  res_bclr[4]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[5]), [L] "i" (LEN_5), [I] "i" (IMM_5));

    check_uint32(result, "bclr", res,  res_bclr[5]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[6]), [L] "i" (LEN_6), [I] "i" (IMM_6));

    check_uint32(result, "bclr", res,  res_bclr[6]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[7]), [L] "i" (LEN_7), [I] "i" (IMM_7));

    check_uint32(result, "bclr", res,  res_bclr[7]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[8]), [L] "i" (LEN_8), [I] "i" (IMM_8));

    check_uint32(result, "bclr", res,  res_bclr[8]);

    asm volatile (BCLR " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[9]), [L] "i" (LEN_9), [I] "i" (IMM_9));

    check_uint32(result, "bclr", res,  res_bclr[9]);
}

void check_bextract(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

    //-----------------------------------------------------------------
  // Check l.bextract
  //-----------------------------------------------------------------
    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[0]), [L] "i" (LEN_0), [I] "i" (IMM_0));

    check_uint32(result, "bextract", res,  res_bextract[0]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[1]), [L] "i" (LEN_1), [I] "i" (IMM_1));

    check_uint32(result, "bextract", res,  res_bextract[1]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[2]), [L] "i" (LEN_2), [I] "i" (IMM_2));

    check_uint32(result, "bextract", res,  res_bextract[2]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[3]), [L] "i" (LEN_3), [I] "i" (IMM_3));

    check_uint32(result, "bextract", res,  res_bextract[3]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[4]), [L] "i" (LEN_4), [I] "i" (IMM_4));

    check_uint32(result, "bextract", res,  res_bextract[4]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[5]), [L] "i" (LEN_5), [I] "i" (IMM_5));

    check_uint32(result, "bextract", res,  res_bextract[5]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[6]), [L] "i" (LEN_6), [I] "i" (IMM_6));

    check_uint32(result, "bextract", res,  res_bextract[6]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[7]), [L] "i" (LEN_7), [I] "i" (IMM_7));

    check_uint32(result, "bextract", res,  res_bextract[7]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[8]), [L] "i" (LEN_8), [I] "i" (IMM_8));

    check_uint32(result, "bextract", res,  res_bextract[8]);

    asm volatile (BEXTRACT " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[9]), [L] "i" (LEN_9), [I] "i" (IMM_9));

    check_uint32(result, "bextract", res,  res_bextract[9]);
}

void check_bextractu(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.bextractu
  //-----------------------------------------------------------------
    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[0]), [L] "i" (LEN_0), [I] "i" (IMM_0));

    check_uint32(result, "bextractu", res,  res_bextractu[0]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[1]), [L] "i" (LEN_1), [I] "i" (IMM_1));

    check_uint32(result, "bextractu", res,  res_bextractu[1]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[2]), [L] "i" (LEN_2), [I] "i" (IMM_2));

    check_uint32(result, "bextractu", res,  res_bextractu[2]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[3]), [L] "i" (LEN_3), [I] "i" (IMM_3));

    check_uint32(result, "bextractu", res,  res_bextractu[3]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[4]), [L] "i" (LEN_4), [I] "i" (IMM_4));

    check_uint32(result, "bextractu", res,  res_bextractu[4]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[5]), [L] "i" (LEN_5), [I] "i" (IMM_5));

    check_uint32(result, "bextractu", res,  res_bextractu[5]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[6]), [L] "i" (LEN_6), [I] "i" (IMM_6));

    check_uint32(result, "bextractu", res,  res_bextractu[6]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[7]), [L] "i" (LEN_7), [I] "i" (IMM_7));

    check_uint32(result, "bextractu", res,  res_bextractu[7]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[8]), [L] "i" (LEN_8), [I] "i" (IMM_8));

    check_uint32(result, "bextractu", res,  res_bextractu[8]);

    asm volatile (BEXTRACTU " %[c], %[a], %[L],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a[9]), [L] "i" (LEN_9), [I] "i" (IMM_9));

    check_uint32(result, "bextractu", res,  res_bextractu[9]);
}


void check_binsert(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.binsert
  //-----------------------------------------------------------------
    res = op_c[0];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[0]), [L] "i" (LEN_0), [I] "i" (IMM_0));

    check_uint32(result, "binsert", res,  res_binsert[0]);

    res = op_c[1];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[1]), [L] "i" (LEN_1), [I] "i" (IMM_1));

    check_uint32(result, "binsert", res,  res_binsert[1]);

    res = op_c[2];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[2]), [L] "i" (LEN_2), [I] "i" (IMM_2));

    check_uint32(result, "binsert", res,  res_binsert[2]);

    res = op_c[3];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[3]), [L] "i" (LEN_3), [I] "i" (IMM_3));

    check_uint32(result, "binsert", res,  res_binsert[3]);

    res = op_c[4];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[4]), [L] "i" (LEN_4), [I] "i" (IMM_4));

    check_uint32(result, "binsert", res,  res_binsert[4]);

    res = op_c[5];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[5]), [L] "i" (LEN_5), [I] "i" (IMM_5));

    check_uint32(result, "binsert", res,  res_binsert[5]);

    res = op_c[6];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[6]), [L] "i" (LEN_6), [I] "i" (IMM_6));

    check_uint32(result, "binsert", res,  res_binsert[6]);

    res = op_c[7];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[7]), [L] "i" (LEN_7), [I] "i" (IMM_7));

    check_uint32(result, "binsert", res,  res_binsert[7]);

    res = op_c[8];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[8]), [L] "i" (LEN_8), [I] "i" (IMM_8));

    check_uint32(result, "binsert", res,  res_binsert[8]);

    res = op_c[9];
    asm volatile (BINSERT " %[c], %[a], %[L],%[I]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (op_a[9]), [L] "i" (LEN_9), [I] "i" (IMM_9));

    check_uint32(result, "binsert", res,  res_binsert[9]);
}


#ifdef __riscv

void check_bset_reg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.bsetr
  //-----------------------------------------------------------------
    for(i=0;i<NumberOfStimuli;i++){
      asm volatile (BSETR " %[c], %[a], %[b]\n"
                    : [c] "=r" (res)
                    : [a] "r"  (op_a_reg[i]), [b] "r" (op_b_reg[i]));

      check_uint32(result, "bsetr", res,  res_bset_reg[i]);
    }
}

void check_bclr_reg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.bclrr
  //-----------------------------------------------------------------
    for(i=0;i<NumberOfStimuli;i++){
      asm volatile (BCLRR " %[c], %[a], %[b]\n"
                    : [c] "=r" (res)
                    : [a] "r"  (op_a_reg[i]), [b] "r" (op_b_reg[i]));

      check_uint32(result, "bclrr", res,  res_bclr_reg[i]);
    }
}

void check_bextract_reg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.bextractr
  //-----------------------------------------------------------------
    for(i=0;i<NumberOfStimuli;i++){
      asm volatile (BEXTRACTR " %[c], %[a], %[b]\n"
                    : [c] "=r" (res)
                    : [a] "r"  (op_a_reg[i]), [b] "r" (op_b_reg[i]));

      check_uint32(result, "bextractr", res,  res_bextract_reg[i]);
    }
}
void check_bextractu_reg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.bextractr
  //-----------------------------------------------------------------
    for(i=0;i<NumberOfStimuli;i++){
      asm volatile (BEXTRACTUR " %[c], %[a], %[b]\n"
                    : [c] "=r" (res)
                    : [a] "r"  (op_a_reg[i]), [b] "r" (op_b_reg[i]));

      check_uint32(result, "bextractr", res,  res_bextractu_reg[i]);
    }
}

void check_binsert_reg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.binsertr
  //-----------------------------------------------------------------
    for(i=0;i<NumberOfStimuli;i++){
      res = op_c_reg[i];
      asm volatile (BINSERTR " %[c], %[a], %[b]\n"
                    : [c] "+r" (res)
                    : [a] "r"  (op_a_reg[i]), [b] "r" (op_b_reg[i]));

      check_uint32(result, "binsertr", res,  res_binsert_reg[i]);
    }
}
//################################################################################
//# T E S T    BREVERSE
//################################################################################

void check_breverse(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.brev
  //-----------------------------------------------------------------

  // asm volatile ("addi x5, x0, 5;"
  //               // ".word 0xc262d333;"
  //               ".word 0xC5A2D333;"
  //               "nop" : : : "x5", "x6");

  asm volatile ("li x31, 505076710;"
  ".word 0xc51fdfb3;"
  "nop" : : : "x31", "x31");

  asm volatile ("addi %[c], x31, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[0]);


  asm volatile ("li x17, 2531716564;"
  ".word 0xc2c8d8b3;"
  "nop" : : : "x17", "x17");

  asm volatile ("addi %[c], x17, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[1]);


  asm volatile ("li x20, 1268042776;"
  ".word 0xc2ea5a33;"
  "nop" : : : "x20", "x20");

  asm volatile ("addi %[c], x20, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[2]);


  asm volatile ("li x24, 2133586273;"
  ".word 0xc3ac5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[3]);


  asm volatile ("li x25, 1228509782;"
  ".word 0xc16cdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[4]);


  asm volatile ("li x1, 1068661380;"
  ".word 0xc220d0b3;"
  "nop" : : : "x1", "x1");

  asm volatile ("addi %[c], x1, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[5]);


  asm volatile ("li x23, 1030729186;"
  ".word 0xc2ebdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[6]);


  asm volatile ("li x29, 2152931320;"
  ".word 0xc0fedeb3;"
  "nop" : : : "x29", "x29");

  asm volatile ("addi %[c], x29, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[7]);


  asm volatile ("li x28, 3332349450;"
  ".word 0xc48e5e33;"
  "nop" : : : "x28", "x28");

  asm volatile ("addi %[c], x28, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[8]);


  asm volatile ("li x5, 2044036743;"
  ".word 0xc282d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[9]);


  asm volatile ("li x4, 740252217;"
  ".word 0xc5d25233;"
  "nop" : : : "x4", "x4");

  asm volatile ("addi %[c], x4, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[10]);


  asm volatile ("li x26, 3468936861;"
  ".word 0xc05d5d33;"
  "nop" : : : "x26", "x26");

  asm volatile ("addi %[c], x26, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[11]);


  asm volatile ("li x23, 2186861676;"
  ".word 0xc22bdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[12]);


  asm volatile ("li x11, 1068482009;"
  ".word 0xc065d5b3;"
  "nop" : : : "x11", "x11");

  asm volatile ("addi %[c], x11, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[13]);


  asm volatile ("li x7, 4031211966;"
  ".word 0xc4e3d3b3;"
  "nop" : : : "x7", "x7");

  asm volatile ("addi %[c], x7, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[14]);


  asm volatile ("li x4, 2142224965;"
  ".word 0xc0025233;"
  "nop" : : : "x4", "x4");

  asm volatile ("addi %[c], x4, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[15]);


  asm volatile ("li x25, 2113191407;"
  ".word 0xc19cdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[16]);


  asm volatile ("li x21, 1964718651;"
  ".word 0xc3cadab3;"
  "nop" : : : "x21", "x21");

  asm volatile ("addi %[c], x21, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[17]);


  asm volatile ("li x23, 2289315732;"
  ".word 0xc48bdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[18]);


  asm volatile ("li x28, 2179723189;"
  ".word 0xc3ee5e33;"
  "nop" : : : "x28", "x28");

  asm volatile ("addi %[c], x28, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[19]);


  asm volatile ("li x30, 3883086649;"
  ".word 0xc10f5f33;"
  "nop" : : : "x30", "x30");

  asm volatile ("addi %[c], x30, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[20]);


  asm volatile ("li x3, 3988744341;"
  ".word 0xc5d1d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[21]);


  asm volatile ("li x3, 2099488196;"
  ".word 0xc221d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[22]);


  asm volatile ("li x17, 1528771551;"
  ".word 0xc578d8b3;"
  "nop" : : : "x17", "x17");

  asm volatile ("addi %[c], x17, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[23]);


  asm volatile ("li x19, 2104541344;"
  ".word 0xc1d9d9b3;"
  "nop" : : : "x19", "x19");

  asm volatile ("addi %[c], x19, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[24]);


  asm volatile ("li x27, 1475555;"
  ".word 0xc4bdddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[25]);


  asm volatile ("li x21, 3934695034;"
  ".word 0xc5dadab3;"
  "nop" : : : "x21", "x21");

  asm volatile ("addi %[c], x21, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[26]);


  asm volatile ("li x30, 3327500878;"
  ".word 0xc24f5f33;"
  "nop" : : : "x30", "x30");

  asm volatile ("addi %[c], x30, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[27]);


  asm volatile ("li x28, 1027912347;"
  ".word 0xc45e5e33;"
  "nop" : : : "x28", "x28");

  asm volatile ("addi %[c], x28, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[28]);


  asm volatile ("li x27, 913925789;"
  ".word 0xc3adddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[29]);


  asm volatile ("li x10, 1960109797;"
  ".word 0xc5755533;"
  "nop" : : : "x10", "x10");

  asm volatile ("addi %[c], x10, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[30]);


  asm volatile ("li x6, 1341573171;"
  ".word 0xc0435333;"
  "nop" : : : "x6", "x6");

  asm volatile ("addi %[c], x6, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[31]);


  asm volatile ("li x28, 3495743032;"
  ".word 0xc19e5e33;"
  "nop" : : : "x28", "x28");

  asm volatile ("addi %[c], x28, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[32]);


  asm volatile ("li x5, 2843979163;"
  ".word 0xc142d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[33]);


  asm volatile ("li x30, 631791428;"
  ".word 0xc22f5f33;"
  "nop" : : : "x30", "x30");

  asm volatile ("addi %[c], x30, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[34]);


  asm volatile ("li x19, 1867902699;"
  ".word 0xc549d9b3;"
  "nop" : : : "x19", "x19");

  asm volatile ("addi %[c], x19, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[35]);


  asm volatile ("li x12, 2060096815;"
  ".word 0xc0565633;"
  "nop" : : : "x12", "x12");

  asm volatile ("addi %[c], x12, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[36]);


  asm volatile ("li x25, 528451429;"
  ".word 0xc1dcdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[37]);


  asm volatile ("li x25, 3592476355;"
  ".word 0xc2acdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[38]);


  asm volatile ("li x10, 4016963218;"
  ".word 0xc2e55533;"
  "nop" : : : "x10", "x10");

  asm volatile ("addi %[c], x10, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[39]);


  asm volatile ("li x15, 1123185046;"
  ".word 0xc117d7b3;"
  "nop" : : : "x15", "x15");

  asm volatile ("addi %[c], x15, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[40]);


  asm volatile ("li x22, 1553358914;"
  ".word 0xc48b5b33;"
  "nop" : : : "x22", "x22");

  asm volatile ("addi %[c], x22, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[41]);


  asm volatile ("li x8, 3116394984;"
  ".word 0xc3e45433;"
  "nop" : : : "x8", "x8");

  asm volatile ("addi %[c], x8, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[42]);


  asm volatile ("li x12, 1407363476;"
  ".word 0xc1b65633;"
  "nop" : : : "x12", "x12");

  asm volatile ("addi %[c], x12, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[43]);


  asm volatile ("li x28, 2199496149;"
  ".word 0xc5ae5e33;"
  "nop" : : : "x28", "x28");

  asm volatile ("addi %[c], x28, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[44]);


  asm volatile ("li x26, 3838023747;"
  ".word 0xc42d5d33;"
  "nop" : : : "x26", "x26");

  asm volatile ("addi %[c], x26, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[45]);


  asm volatile ("li x17, 3491124360;"
  ".word 0xc388d8b3;"
  "nop" : : : "x17", "x17");

  asm volatile ("addi %[c], x17, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[46]);


  asm volatile ("li x18, 572844049;"
  ".word 0xc2495933;"
  "nop" : : : "x18", "x18");

  asm volatile ("addi %[c], x18, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[47]);


  asm volatile ("li x23, 3708247748;"
  ".word 0xc3abdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[48]);


  asm volatile ("li x30, 3421720931;"
  ".word 0xc5af5f33;"
  "nop" : : : "x30", "x30");

  asm volatile ("addi %[c], x30, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[49]);


  asm volatile ("li x8, 730188755;"
  ".word 0xc1745433;"
  "nop" : : : "x8", "x8");

  asm volatile ("addi %[c], x8, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[50]);


  asm volatile ("li x9, 3586557750;"
  ".word 0xc124d4b3;"
  "nop" : : : "x9", "x9");

  asm volatile ("addi %[c], x9, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[51]);


  asm volatile ("li x24, 1991023203;"
  ".word 0xc15c5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[52]);


  asm volatile ("li x20, 3342539526;"
  ".word 0xc00a5a33;"
  "nop" : : : "x20", "x20");

  asm volatile ("addi %[c], x20, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[53]);


  asm volatile ("li x3, 348522133;"
  ".word 0xc541d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[54]);


  asm volatile ("li x23, 1261912159;"
  ".word 0xc1fbdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[55]);


  asm volatile ("li x17, 2061800957;"
  ".word 0xc578d8b3;"
  "nop" : : : "x17", "x17");

  asm volatile ("addi %[c], x17, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[56]);


  asm volatile ("li x20, 805601911;"
  ".word 0xc11a5a33;"
  "nop" : : : "x20", "x20");

  asm volatile ("addi %[c], x20, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[57]);


  asm volatile ("li x11, 1542937937;"
  ".word 0xc5a5d5b3;"
  "nop" : : : "x11", "x11");

  asm volatile ("addi %[c], x11, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[58]);


  asm volatile ("li x22, 2813715417;"
  ".word 0xc42b5b33;"
  "nop" : : : "x22", "x22");

  asm volatile ("addi %[c], x22, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[59]);


  asm volatile ("li x28, 2528836128;"
  ".word 0xc28e5e33;"
  "nop" : : : "x28", "x28");

  asm volatile ("addi %[c], x28, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[60]);


  asm volatile ("li x5, 1646702054;"
  ".word 0xc032d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[61]);


  asm volatile ("li x16, 2921780507;"
  ".word 0xc5d85833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[62]);


  asm volatile ("li x2, 1353945831;"
  ".word 0xc3215133;"
  "nop" : : : "x2", "x2");

  asm volatile ("addi %[c], x2, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[63]);


  asm volatile ("li x29, 312051943;"
  ".word 0xc03edeb3;"
  "nop" : : : "x29", "x29");

  asm volatile ("addi %[c], x29, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[64]);


  asm volatile ("li x25, 1390547282;"
  ".word 0xc57cdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[65]);


  asm volatile ("li x14, 3554121181;"
  ".word 0xc2275733;"
  "nop" : : : "x14", "x14");

  asm volatile ("addi %[c], x14, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[66]);


  asm volatile ("li x15, 3939018192;"
  ".word 0xc2c7d7b3;"
  "nop" : : : "x15", "x15");

  asm volatile ("addi %[c], x15, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[67]);


  asm volatile ("li x25, 3218532326;"
  ".word 0xc36cdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[68]);


  asm volatile ("li x15, 1368103813;"
  ".word 0xc547d7b3;"
  "nop" : : : "x15", "x15");

  asm volatile ("addi %[c], x15, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[69]);


  asm volatile ("li x25, 3253976751;"
  ".word 0xc48cdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[70]);


  asm volatile ("li x21, 355856929;"
  ".word 0xc32adab3;"
  "nop" : : : "x21", "x21");

  asm volatile ("addi %[c], x21, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[71]);


  asm volatile ("li x15, 598814884;"
  ".word 0xc0e7d7b3;"
  "nop" : : : "x15", "x15");

  asm volatile ("addi %[c], x15, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[72]);


  asm volatile ("li x26, 1795102739;"
  ".word 0xc38d5d33;"
  "nop" : : : "x26", "x26");

  asm volatile ("addi %[c], x26, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[73]);


  asm volatile ("li x18, 942075360;"
  ".word 0xc5d95933;"
  "nop" : : : "x18", "x18");

  asm volatile ("addi %[c], x18, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[74]);


  asm volatile ("li x26, 4014656882;"
  ".word 0xc48d5d33;"
  "nop" : : : "x26", "x26");

  asm volatile ("addi %[c], x26, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[75]);


  asm volatile ("li x29, 693429229;"
  ".word 0xc2aedeb3;"
  "nop" : : : "x29", "x29");

  asm volatile ("addi %[c], x29, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[76]);


  asm volatile ("li x27, 1353225621;"
  ".word 0xc01dddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[77]);


  asm volatile ("li x14, 3138146195;"
  ".word 0xc2675733;"
  "nop" : : : "x14", "x14");

  asm volatile ("addi %[c], x14, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[78]);


  asm volatile ("li x21, 4129120067;"
  ".word 0xc4eadab3;"
  "nop" : : : "x21", "x21");

  asm volatile ("addi %[c], x21, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[79]);


  asm volatile ("li x2, 2829216112;"
  ".word 0xc2015133;"
  "nop" : : : "x2", "x2");

  asm volatile ("addi %[c], x2, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[80]);


  asm volatile ("li x1, 751504861;"
  ".word 0xc010d0b3;"
  "nop" : : : "x1", "x1");

  asm volatile ("addi %[c], x1, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[81]);


  asm volatile ("li x3, 3606901681;"
  ".word 0xc001d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[82]);


  asm volatile ("li x27, 436062718;"
  ".word 0xc01dddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[83]);


  asm volatile ("li x2, 4104728049;"
  ".word 0xc3215133;"
  "nop" : : : "x2", "x2");

  asm volatile ("addi %[c], x2, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[84]);


  asm volatile ("li x16, 1438835330;"
  ".word 0xc2285833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[85]);


  asm volatile ("li x3, 2098514497;"
  ".word 0xc0c1d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[86]);


  asm volatile ("li x3, 3052283127;"
  ".word 0xc171d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[87]);


  asm volatile ("li x29, 3300771945;"
  ".word 0xc1dedeb3;"
  "nop" : : : "x29", "x29");

  asm volatile ("addi %[c], x29, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[88]);


  asm volatile ("li x8, 3719565618;"
  ".word 0xc1645433;"
  "nop" : : : "x8", "x8");

  asm volatile ("addi %[c], x8, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[89]);


  asm volatile ("li x27, 794613970;"
  ".word 0xc57dddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[90]);


  asm volatile ("li x25, 1865990755;"
  ".word 0xc1fcdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[91]);


  asm volatile ("li x30, 226114047;"
  ".word 0xc45f5f33;"
  "nop" : : : "x30", "x30");

  asm volatile ("addi %[c], x30, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[92]);


  asm volatile ("li x14, 2240960127;"
  ".word 0xc0d75733;"
  "nop" : : : "x14", "x14");

  asm volatile ("addi %[c], x14, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[93]);


  asm volatile ("li x18, 4255488970;"
  ".word 0xc3e95933;"
  "nop" : : : "x18", "x18");

  asm volatile ("addi %[c], x18, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[94]);


  asm volatile ("li x18, 215837608;"
  ".word 0xc4b95933;"
  "nop" : : : "x18", "x18");

  asm volatile ("addi %[c], x18, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[95]);


  asm volatile ("li x23, 4198087507;"
  ".word 0xc1fbdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[96]);


  asm volatile ("li x21, 3142183879;"
  ".word 0xc1cadab3;"
  "nop" : : : "x21", "x21");

  asm volatile ("addi %[c], x21, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[97]);


  asm volatile ("li x20, 4111048717;"
  ".word 0xc0aa5a33;"
  "nop" : : : "x20", "x20");

  asm volatile ("addi %[c], x20, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[98]);


  asm volatile ("li x19, 1875990392;"
  ".word 0xc3c9d9b3;"
  "nop" : : : "x19", "x19");

  asm volatile ("addi %[c], x19, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[99]);


  asm volatile ("li x5, 4279208231;"
  ".word 0xc262d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[100]);


  asm volatile ("li x27, 777676145;"
  ".word 0xc11dddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[101]);


  asm volatile ("li x24, 4222714784;"
  ".word 0xc26c5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[102]);


  asm volatile ("li x6, 2605363492;"
  ".word 0xc0e35333;"
  "nop" : : : "x6", "x6");

  asm volatile ("addi %[c], x6, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[103]);


  asm volatile ("li x27, 2463761078;"
  ".word 0xc4bdddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[104]);


  asm volatile ("li x27, 1272904312;"
  ".word 0xc3adddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[105]);


  asm volatile ("li x17, 587066078;"
  ".word 0xc1c8d8b3;"
  "nop" : : : "x17", "x17");

  asm volatile ("addi %[c], x17, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[106]);


  asm volatile ("li x10, 3003111284;"
  ".word 0xc4b55533;"
  "nop" : : : "x10", "x10");

  asm volatile ("addi %[c], x10, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[107]);


  asm volatile ("li x22, 2710605902;"
  ".word 0xc14b5b33;"
  "nop" : : : "x22", "x22");

  asm volatile ("addi %[c], x22, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[108]);


  asm volatile ("li x22, 103520630;"
  ".word 0xc20b5b33;"
  "nop" : : : "x22", "x22");

  asm volatile ("addi %[c], x22, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[109]);


  asm volatile ("li x9, 1538699172;"
  ".word 0xc0c4d4b3;"
  "nop" : : : "x9", "x9");

  asm volatile ("addi %[c], x9, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[110]);


  asm volatile ("li x24, 823409048;"
  ".word 0xc14c5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[111]);


  asm volatile ("li x18, 1612323579;"
  ".word 0xc1c95933;"
  "nop" : : : "x18", "x18");

  asm volatile ("addi %[c], x18, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[112]);


  asm volatile ("li x27, 57476356;"
  ".word 0xc10dddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[113]);


  asm volatile ("li x4, 4078200349;"
  ".word 0xc1525233;"
  "nop" : : : "x4", "x4");

  asm volatile ("addi %[c], x4, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[114]);


  asm volatile ("li x16, 1538107487;"
  ".word 0xc0085833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[115]);


  asm volatile ("li x16, 1146510251;"
  ".word 0xc5785833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[116]);


  asm volatile ("li x25, 3098093812;"
  ".word 0xc30cdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[117]);


  asm volatile ("li x27, 356065028;"
  ".word 0xc5adddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[118]);


  asm volatile ("li x16, 3908623234;"
  ".word 0xc1285833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[119]);


  asm volatile ("li x1, 154515724;"
  ".word 0xc4e0d0b3;"
  "nop" : : : "x1", "x1");

  asm volatile ("addi %[c], x1, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[120]);


  asm volatile ("li x31, 3014385788;"
  ".word 0xc30fdfb3;"
  "nop" : : : "x31", "x31");

  asm volatile ("addi %[c], x31, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[121]);


  asm volatile ("li x16, 2665911345;"
  ".word 0xc0485833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[122]);


  asm volatile ("li x27, 1480887892;"
  ".word 0xc57dddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[123]);


  asm volatile ("li x26, 2958501942;"
  ".word 0xc2ad5d33;"
  "nop" : : : "x26", "x26");

  asm volatile ("addi %[c], x26, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[124]);


  asm volatile ("li x16, 267970469;"
  ".word 0xc3085833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[125]);


  asm volatile ("li x20, 3559143871;"
  ".word 0xc51a5a33;"
  "nop" : : : "x20", "x20");

  asm volatile ("addi %[c], x20, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[126]);


  asm volatile ("li x9, 152035607;"
  ".word 0xc514d4b3;"
  "nop" : : : "x9", "x9");

  asm volatile ("addi %[c], x9, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[127]);


  asm volatile ("li x6, 4072749625;"
  ".word 0xc4e35333;"
  "nop" : : : "x6", "x6");

  asm volatile ("addi %[c], x6, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[128]);


  asm volatile ("li x9, 1821239522;"
  ".word 0xc4e4d4b3;"
  "nop" : : : "x9", "x9");

  asm volatile ("addi %[c], x9, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[129]);


  asm volatile ("li x4, 1820515103;"
  ".word 0xc2625233;"
  "nop" : : : "x4", "x4");

  asm volatile ("addi %[c], x4, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[130]);


  asm volatile ("li x16, 1599140825;"
  ".word 0xc4e85833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[131]);


  asm volatile ("li x31, 650703376;"
  ".word 0xc3afdfb3;"
  "nop" : : : "x31", "x31");

  asm volatile ("addi %[c], x31, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[132]);


  asm volatile ("li x23, 2039213540;"
  ".word 0xc5abdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[133]);


  asm volatile ("li x30, 2174004432;"
  ".word 0xc3ef5f33;"
  "nop" : : : "x30", "x30");

  asm volatile ("addi %[c], x30, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[134]);


  asm volatile ("li x5, 1845960687;"
  ".word 0xc222d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[135]);


  asm volatile ("li x24, 3251103402;"
  ".word 0xc57c5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[136]);


  asm volatile ("li x15, 2877019354;"
  ".word 0xc5d7d7b3;"
  "nop" : : : "x15", "x15");

  asm volatile ("addi %[c], x15, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[137]);


  asm volatile ("li x16, 3608266320;"
  ".word 0xc5785833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[138]);


  asm volatile ("li x10, 390344852;"
  ".word 0xc3c55533;"
  "nop" : : : "x10", "x10");

  asm volatile ("addi %[c], x10, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[139]);


  asm volatile ("li x12, 3363470861;"
  ".word 0xc5a65633;"
  "nop" : : : "x12", "x12");

  asm volatile ("addi %[c], x12, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[140]);


  asm volatile ("li x17, 2124708753;"
  ".word 0xc548d8b3;"
  "nop" : : : "x17", "x17");

  asm volatile ("addi %[c], x17, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[141]);


  asm volatile ("li x5, 1182405857;"
  ".word 0xc4b2d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[142]);


  asm volatile ("li x2, 1844850204;"
  ".word 0xc1115133;"
  "nop" : : : "x2", "x2");

  asm volatile ("addi %[c], x2, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[143]);


  asm volatile ("li x12, 2804758606;"
  ".word 0xc1c65633;"
  "nop" : : : "x12", "x12");

  asm volatile ("addi %[c], x12, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[144]);


  asm volatile ("li x7, 3923244468;"
  ".word 0xc383d3b3;"
  "nop" : : : "x7", "x7");

  asm volatile ("addi %[c], x7, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[145]);


  asm volatile ("li x27, 2648063273;"
  ".word 0xc51dddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[146]);


  asm volatile ("li x12, 1125812849;"
  ".word 0xc5465633;"
  "nop" : : : "x12", "x12");

  asm volatile ("addi %[c], x12, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[147]);


  asm volatile ("li x1, 1240415092;"
  ".word 0xc300d0b3;"
  "nop" : : : "x1", "x1");

  asm volatile ("addi %[c], x1, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[148]);


  asm volatile ("li x5, 231675949;"
  ".word 0xc4e2d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[149]);


  asm volatile ("li x26, 3855391185;"
  ".word 0xc48d5d33;"
  "nop" : : : "x26", "x26");

  asm volatile ("addi %[c], x26, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[150]);


  asm volatile ("li x22, 2188887407;"
  ".word 0xc30b5b33;"
  "nop" : : : "x22", "x22");

  asm volatile ("addi %[c], x22, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[151]);


  asm volatile ("li x11, 3733891286;"
  ".word 0xc0f5d5b3;"
  "nop" : : : "x11", "x11");

  asm volatile ("addi %[c], x11, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[152]);


  asm volatile ("li x13, 3099676079;"
  ".word 0xc186d6b3;"
  "nop" : : : "x13", "x13");

  asm volatile ("addi %[c], x13, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[153]);


  asm volatile ("li x25, 1615647184;"
  ".word 0xc16cdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[154]);


  asm volatile ("li x23, 241754414;"
  ".word 0xc3cbdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[155]);


  asm volatile ("li x6, 1496965200;"
  ".word 0xc0c35333;"
  "nop" : : : "x6", "x6");

  asm volatile ("addi %[c], x6, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[156]);


  asm volatile ("li x25, 138562785;"
  ".word 0xc48cdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[157]);


  asm volatile ("li x4, 2937311404;"
  ".word 0xc4225233;"
  "nop" : : : "x4", "x4");

  asm volatile ("addi %[c], x4, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[158]);


  asm volatile ("li x25, 83907901;"
  ".word 0xc5dcdcb3;"
  "nop" : : : "x25", "x25");

  asm volatile ("addi %[c], x25, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[159]);


  asm volatile ("li x3, 2810440846;"
  ".word 0xc541d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[160]);


  asm volatile ("li x3, 3512655704;"
  ".word 0xc4b1d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[161]);


  asm volatile ("li x18, 4027413550;"
  ".word 0xc2695933;"
  "nop" : : : "x18", "x18");

  asm volatile ("addi %[c], x18, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[162]);


  asm volatile ("li x2, 3886864649;"
  ".word 0xc1e15133;"
  "nop" : : : "x2", "x2");

  asm volatile ("addi %[c], x2, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[163]);


  asm volatile ("li x7, 3415998525;"
  ".word 0xc453d3b3;"
  "nop" : : : "x7", "x7");

  asm volatile ("addi %[c], x7, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[164]);


  asm volatile ("li x31, 2526209563;"
  ".word 0xc07fdfb3;"
  "nop" : : : "x31", "x31");

  asm volatile ("addi %[c], x31, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[165]);


  asm volatile ("li x16, 4196800755;"
  ".word 0xc3885833;"
  "nop" : : : "x16", "x16");

  asm volatile ("addi %[c], x16, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[166]);


  asm volatile ("li x3, 3683841079;"
  ".word 0xc5a1d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[167]);


  asm volatile ("li x7, 4227895781;"
  ".word 0xc123d3b3;"
  "nop" : : : "x7", "x7");

  asm volatile ("addi %[c], x7, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[168]);


  asm volatile ("li x31, 1826111154;"
  ".word 0xc3efdfb3;"
  "nop" : : : "x31", "x31");

  asm volatile ("addi %[c], x31, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[169]);


  asm volatile ("li x3, 2400479053;"
  ".word 0xc0c1d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[170]);


  asm volatile ("li x20, 35301788;"
  ".word 0xc48a5a33;"
  "nop" : : : "x20", "x20");

  asm volatile ("addi %[c], x20, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[171]);


  asm volatile ("li x21, 3107587037;"
  ".word 0xc04adab3;"
  "nop" : : : "x21", "x21");

  asm volatile ("addi %[c], x21, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[172]);


  asm volatile ("li x5, 788573025;"
  ".word 0xc242d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[173]);


  asm volatile ("li x14, 454482553;"
  ".word 0xc0975733;"
  "nop" : : : "x14", "x14");

  asm volatile ("addi %[c], x14, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[174]);


  asm volatile ("li x10, 3217638535;"
  ".word 0xc3655533;"
  "nop" : : : "x10", "x10");

  asm volatile ("addi %[c], x10, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[175]);


  asm volatile ("li x8, 4253524373;"
  ".word 0xc3245433;"
  "nop" : : : "x8", "x8");

  asm volatile ("addi %[c], x8, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[176]);


  asm volatile ("li x18, 4024178405;"
  ".word 0xc0b95933;"
  "nop" : : : "x18", "x18");

  asm volatile ("addi %[c], x18, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[177]);


  asm volatile ("li x26, 3144877409;"
  ".word 0xc19d5d33;"
  "nop" : : : "x26", "x26");

  asm volatile ("addi %[c], x26, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[178]);


  asm volatile ("li x5, 1373072768;"
  ".word 0xc042d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[179]);


  asm volatile ("li x28, 2295467544;"
  ".word 0xc2ae5e33;"
  "nop" : : : "x28", "x28");

  asm volatile ("addi %[c], x28, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[180]);


  asm volatile ("li x20, 2138093056;"
  ".word 0xc0ba5a33;"
  "nop" : : : "x20", "x20");

  asm volatile ("addi %[c], x20, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[181]);


  asm volatile ("li x24, 2323329189;"
  ".word 0xc11c5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[182]);


  asm volatile ("li x17, 2335798105;"
  ".word 0xc2a8d8b3;"
  "nop" : : : "x17", "x17");

  asm volatile ("addi %[c], x17, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[183]);


  asm volatile ("li x24, 2496941044;"
  ".word 0xc32c5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[184]);


  asm volatile ("li x23, 1146668;"
  ".word 0xc02bdbb3;"
  "nop" : : : "x23", "x23");

  asm volatile ("addi %[c], x23, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[185]);


  asm volatile ("li x14, 3934852106;"
  ".word 0xc5175733;"
  "nop" : : : "x14", "x14");

  asm volatile ("addi %[c], x14, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[186]);


  asm volatile ("li x27, 523305717;"
  ".word 0xc0adddb3;"
  "nop" : : : "x27", "x27");

  asm volatile ("addi %[c], x27, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[187]);


  asm volatile ("li x12, 4099587593;"
  ".word 0xc5765633;"
  "nop" : : : "x12", "x12");

  asm volatile ("addi %[c], x12, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[188]);


  asm volatile ("li x7, 118084559;"
  ".word 0xc113d3b3;"
  "nop" : : : "x7", "x7");

  asm volatile ("addi %[c], x7, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[189]);


  asm volatile ("li x5, 2115078819;"
  ".word 0xc152d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[190]);


  asm volatile ("li x5, 2029223436;"
  ".word 0xc382d2b3;"
  "nop" : : : "x5", "x5");

  asm volatile ("addi %[c], x5, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[191]);


  asm volatile ("li x31, 2644151732;"
  ".word 0xc06fdfb3;"
  "nop" : : : "x31", "x31");

  asm volatile ("addi %[c], x31, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[192]);


  asm volatile ("li x12, 1244825225;"
  ".word 0xc5165633;"
  "nop" : : : "x12", "x12");

  asm volatile ("addi %[c], x12, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[193]);


  asm volatile ("li x18, 1050175438;"
  ".word 0xc4b95933;"
  "nop" : : : "x18", "x18");

  asm volatile ("addi %[c], x18, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[194]);


  asm volatile ("li x26, 1941549787;"
  ".word 0xc51d5d33;"
  "nop" : : : "x26", "x26");

  asm volatile ("addi %[c], x26, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[195]);


  asm volatile ("li x3, 2647812177;"
  ".word 0xc511d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[196]);


  asm volatile ("li x24, 2647207701;"
  ".word 0xc10c5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[197]);


  asm volatile ("li x3, 3674360369;"
  ".word 0xc021d1b3;"
  "nop" : : : "x3", "x3");

  asm volatile ("addi %[c], x3, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[198]);


  asm volatile ("li x24, 2647111585;"
  ".word 0xc20c5c33;"
  "nop" : : : "x24", "x24");

  asm volatile ("addi %[c], x24, 0\n": [c] "=r" (res));

  check_uint32(result, "brev", res,  res_brev[199]);


}
#endif