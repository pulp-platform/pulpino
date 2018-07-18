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
void check_breverse_c    (testresult_t *result, void (*start)(), void (*stop)());
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
 { .name = "breverse"      , .test = check_breverse      },
 // { .name = "breverse_c"    , .test = check_breverse_c    },
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
//# TEST BREVERSE
//################################################################################
void check_breverse(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;
  volatile unsigned int res_vec[NumberOfStimuli];

  // Initialize result vector
  for(i = 0; i < NumberOfStimuli; i++) res_vec[i] = 0;

  // asm volatile ("li x7, 4212865489;"
  //               ".word 0xc423d333;"
  //               "sw x6, 0(%[addr])\n"
  //               : : [addr] "r" (res_vec[i]) : "x7", "x6");

  // uint32_t word;
  // unsigned int i;

  // word = 0x01020304;

  // asm volatile ("sw %[a], 1(%[addr])\n"
  //               : : [addr] "r" (act), [a] "r" (word));



  // GENERATED BY gen_stimuli.py

	asm volatile ("li x6, 832063083;"
	              ".word 0xc2a352b3;"
	              "sw x5, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[0]) : "x6", "x5");

	asm volatile ("li x8, 3604779826;"
	              ".word 0xc3a453b3;"
	              "sw x7, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[1]) : "x8", "x7");

	asm volatile ("li x5, 2637280025;"
	              ".word 0xc422d2b3;"
	              "sw x5, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[2]) : "x5");

	asm volatile ("li x5, 83807409;"
	              ".word 0xc572d3b3;"
	              "sw x7, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[3]) : "x5", "x7");

	asm volatile ("li x10, 1694490646;"
	              ".word 0xc4555433;"
	              "sw x8, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[4]) : "x10", "x8");

	asm volatile ("li x10, 1126319756;"
	              ".word 0xc5d554b3;"
	              "sw x9, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[5]) : "x10", "x9");

	asm volatile ("li x9, 537689;"
	              ".word 0xc4b4d3b3;"
	              "sw x7, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[6]) : "x9", "x7");

	asm volatile ("li x10, 2342081978;"
	              ".word 0xc42554b3;"
	              "sw x9, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[7]) : "x10", "x9");

	asm volatile ("li x5, 832926572;"
	              ".word 0xc2a2d3b3;"
	              "sw x7, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[8]) : "x5", "x7");

	asm volatile ("li x8, 1504965531;"
	              ".word 0xc1845333;"
	              "sw x6, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[9]) : "x8", "x6");

	asm volatile ("li x5, 852395962;"
	              ".word 0xc172d3b3;"
	              "sw x7, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[10]) : "x5", "x7");

	asm volatile ("li x9, 2020514067;"
	              ".word 0xc364d3b3;"
	              "sw x7, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[11]) : "x9", "x7");

	asm volatile ("li x6, 2015005361;"
	              ".word 0xc51354b3;"
	              "sw x9, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[12]) : "x6", "x9");

	asm volatile ("li x8, 1317161970;"
	              ".word 0xc4e45533;"
	              "sw x10, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[13]) : "x8", "x10");

	asm volatile ("li x6, 40573308;"
	              ".word 0xc45354b3;"
	              "sw x9, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[14]) : "x6", "x9");

	asm volatile ("li x8, 3897699825;"
	              ".word 0xc19452b3;"
	              "sw x5, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[15]) : "x8", "x5");

	asm volatile ("li x6, 2498971800;"
	              ".word 0xc5735533;"
	              "sw x10, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[16]) : "x6", "x10");

	asm volatile ("li x10, 299088352;"
	              ".word 0xc4e55433;"
	              "sw x8, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[17]) : "x10", "x8");

	asm volatile ("li x6, 804065364;"
	              ".word 0xc13354b3;"
	              "sw x9, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[18]) : "x6", "x9");

	asm volatile ("li x8, 154046276;"
	              ".word 0xc1b45433;"
	              "sw x8, 0(%[addr])\n"
	              : : [addr] "r" (&res_vec[19]) : "x8");

  // STOP GENERATED BY gen_stimuli.py

  for(i = 0; i < NumberOfStimuli; i++) {
    check_uint32(result, "brev", res_vec[i],  res_brev[i]);
  }
    
}

//################################################################################
//# TEST BREVERSE CORNER CASES
//################################################################################
void check_breverse_c(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  volatile unsigned int res[NumberOfStimuli];

  for(i = 0; i < 8; i++) res[i] = 0;

  i = 0;

  asm volatile ("li x7, 4212865489;"
                ".word 0xc423d333;"
                "sw x6, 0(%[addr])\n"
                : : [addr] "r" (res[i]) : "x7", "x6");

  // uint32_t word;
  // unsigned int i;

  // word = 0x01020304;

  // asm volatile ("sw %[a], 1(%[addr])\n"
  //               : : [addr] "r" (act), [a] "r" (word));

  for(i = 0; i < 8; i++) {
    check_uint32(result, "brev", res[i],  res_brev[i]);
  }
/*

  asm volatile ("li x7, 4023847170;"
                ".word 0xc073d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x7", "x8");

  check_uint32(result, "brev", res,  res_brev[1]);


  asm volatile ("li x9, 2452156024;"
                ".word 0xc1d4d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x9");

  check_uint32(result, "brev", res,  res_brev[2]);


  asm volatile ("li x5, 798943044;"
                ".word 0xc5a2d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x5", "x9");

  check_uint32(result, "brev", res,  res_brev[3]);


  asm volatile ("li x7, 453416748;"
                ".word 0xc1b3d3b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x7");

  check_uint32(result, "brev", res,  res_brev[4]);


  asm volatile ("li x6, 2023491961;"
                ".word 0xc34354b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x6", "x9");

  check_uint32(result, "brev", res,  res_brev[5]);


  asm volatile ("li x6, 3221304725;"
                ".word 0xc4535533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x6", "x10");

  check_uint32(result, "brev", res,  res_brev[6]);


  asm volatile ("li x6, 4043918901;"
                ".word 0xc48354b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x6", "x9");

  check_uint32(result, "brev", res,  res_brev[7]);


  asm volatile ("li x5, 2928916659;"
                ".word 0xc422d2b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x5");

  check_uint32(result, "brev", res,  res_brev[8]);


  asm volatile ("li x10, 2262331859;"
                ".word 0xc1d554b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x10", "x9");

  check_uint32(result, "brev", res,  res_brev[9]);


  asm volatile ("li x6, 3329321177;"
                ".word 0xc42353b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x6", "x7");

  check_uint32(result, "brev", res,  res_brev[10]);


  asm volatile ("li x9, 615339248;"
                ".word 0xc0f4d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x9", "x10");

  check_uint32(result, "brev", res,  res_brev[11]);


  asm volatile ("li x7, 1254183564;"
                ".word 0xc2e3d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x7", "x8");

  check_uint32(result, "brev", res,  res_brev[12]);


  asm volatile ("li x5, 2735248490;"
                ".word 0xc3c2d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x5", "x8");

  check_uint32(result, "brev", res,  res_brev[13]);


  asm volatile ("li x10, 2073781199;"
                ".word 0xc5755333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x10", "x6");

  check_uint32(result, "brev", res,  res_brev[14]);


  asm volatile ("li x6, 3578244023;"
                ".word 0xc0435333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x6");

  check_uint32(result, "brev", res,  res_brev[15]);


  asm volatile ("li x9, 3203071994;"
                ".word 0xc1b4d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x9", "x8");

  check_uint32(result, "brev", res,  res_brev[16]);


  asm volatile ("li x9, 74169776;"
                ".word 0xc574d2b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x9", "x5");

  check_uint32(result, "brev", res,  res_brev[17]);


  asm volatile ("li x9, 680382142;"
                ".word 0xc124d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x9", "x10");

  check_uint32(result, "brev", res,  res_brev[18]);


  asm volatile ("li x6, 2964192276;"
                ".word 0xc1835333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x6");

  check_uint32(result, "brev", res,  res_brev[19]);


  asm volatile ("li x5, 2825696391;"
                ".word 0xc5d2d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x5", "x8");

  check_uint32(result, "brev", res,  res_brev[20]);


  asm volatile ("li x5, 3743883601;"
                ".word 0xc362d2b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x5");

  check_uint32(result, "brev", res,  res_brev[21]);


  asm volatile ("li x6, 793123190;"
                ".word 0xc5135433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x6", "x8");

  check_uint32(result, "brev", res,  res_brev[22]);


  asm volatile ("li x9, 3614901930;"
                ".word 0xc204d333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x9", "x6");

  check_uint32(result, "brev", res,  res_brev[23]);


  asm volatile ("li x8, 863501599;"
                ".word 0xc26454b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x8", "x9");

  check_uint32(result, "brev", res,  res_brev[24]);


  asm volatile ("li x7, 2095632847;"
                ".word 0xc0c3d3b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x7");

  check_uint32(result, "brev", res,  res_brev[25]);


  asm volatile ("li x8, 3430804814;"
                ".word 0xc54453b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x8", "x7");

  check_uint32(result, "brev", res,  res_brev[26]);


  asm volatile ("li x7, 3328900390;"
                ".word 0xc263d2b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x7", "x5");

  check_uint32(result, "brev", res,  res_brev[27]);


  asm volatile ("li x5, 1931920109;"
                ".word 0xc002d333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x5", "x6");

  check_uint32(result, "brev", res,  res_brev[28]);


  asm volatile ("li x6, 98569989;"
                ".word 0xc0f35433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x6", "x8");

  check_uint32(result, "brev", res,  res_brev[29]);


  asm volatile ("li x7, 1165774793;"
                ".word 0xc513d3b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x7");

  check_uint32(result, "brev", res,  res_brev[30]);


  asm volatile ("li x8, 2691358373;"
                ".word 0xc5145533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x8", "x10");

  check_uint32(result, "brev", res,  res_brev[31]);


  asm volatile ("li x7, 2915463736;"
                ".word 0xc043d333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x7", "x6");

  check_uint32(result, "brev", res,  res_brev[32]);


  asm volatile ("li x8, 3769363685;"
                ".word 0xc2e45533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x8", "x10");

  check_uint32(result, "brev", res,  res_brev[33]);


  asm volatile ("li x8, 1140914099;"
                ".word 0xc4245533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x8", "x10");

  check_uint32(result, "brev", res,  res_brev[34]);


  asm volatile ("li x7, 3565073006;"
                ".word 0xc303d2b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x7", "x5");

  check_uint32(result, "brev", res,  res_brev[35]);


  asm volatile ("li x9, 233170636;"
                ".word 0xc5d4d3b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x9", "x7");

  check_uint32(result, "brev", res,  res_brev[36]);


  asm volatile ("li x6, 3617122935;"
                ".word 0xc0735533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x6", "x10");

  check_uint32(result, "brev", res,  res_brev[37]);


  asm volatile ("li x8, 2563124151;"
                ".word 0xc12452b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x8", "x5");

  check_uint32(result, "brev", res,  res_brev[38]);


  asm volatile ("li x9, 3292712373;"
                ".word 0xc064d333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x9", "x6");

  check_uint32(result, "brev", res,  res_brev[39]);


  asm volatile ("li x10, 3951644837;"
                ".word 0xc38552b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x10", "x5");

  check_uint32(result, "brev", res,  res_brev[40]);


  asm volatile ("li x5, 3101358828;"
                ".word 0xc072d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x5", "x8");

  check_uint32(result, "brev", res,  res_brev[41]);


  asm volatile ("li x7, 1069020378;"
                ".word 0xc423d333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x7", "x6");

  check_uint32(result, "brev", res,  res_brev[42]);


  asm volatile ("li x7, 601151312;"
                ".word 0xc453d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x7", "x8");

  check_uint32(result, "brev", res,  res_brev[43]);


  asm volatile ("li x5, 2650458753;"
                ".word 0xc482d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x5", "x9");

  check_uint32(result, "brev", res,  res_brev[44]);


  asm volatile ("li x7, 235669553;"
                ".word 0xc243d333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x7", "x6");

  check_uint32(result, "brev", res,  res_brev[45]);


  asm volatile ("li x10, 3695962485;"
                ".word 0xc0755433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x10", "x8");

  check_uint32(result, "brev", res,  res_brev[46]);


  asm volatile ("li x9, 2119113767;"
                ".word 0xc264d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x9");

  check_uint32(result, "brev", res,  res_brev[47]);


  asm volatile ("li x9, 159278806;"
                ".word 0xc364d2b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x9", "x5");

  check_uint32(result, "brev", res,  res_brev[48]);


  asm volatile ("li x8, 4191670986;"
                ".word 0xc3e45433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x8");

  check_uint32(result, "brev", res,  res_brev[49]);


  asm volatile ("li x8, 239081213;"
                ".word 0xc1245333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x8", "x6");

  check_uint32(result, "brev", res,  res_brev[50]);


  asm volatile ("li x6, 962912701;"
                ".word 0xc11353b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x6", "x7");

  check_uint32(result, "brev", res,  res_brev[51]);


  asm volatile ("li x5, 3789554506;"
                ".word 0xc342d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x5", "x9");

  check_uint32(result, "brev", res,  res_brev[52]);


  asm volatile ("li x10, 2356258551;"
                ".word 0xc0a55533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x10");

  check_uint32(result, "brev", res,  res_brev[53]);


  asm volatile ("li x10, 1903565148;"
                ".word 0xc5755533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x10");

  check_uint32(result, "brev", res,  res_brev[54]);


  asm volatile ("li x5, 1506470395;"
                ".word 0xc4b2d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x5", "x10");

  check_uint32(result, "brev", res,  res_brev[55]);


  asm volatile ("li x7, 894762601;"
                ".word 0xc083d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x7", "x10");

  check_uint32(result, "brev", res,  res_brev[56]);


  asm volatile ("li x10, 501123672;"
                ".word 0xc1755333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x10", "x6");

  check_uint32(result, "brev", res,  res_brev[57]);


  asm volatile ("li x9, 978045866;"
                ".word 0xc384d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x9", "x8");

  check_uint32(result, "brev", res,  res_brev[58]);


  asm volatile ("li x8, 1727931938;"
                ".word 0xc51453b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x8", "x7");

  check_uint32(result, "brev", res,  res_brev[59]);


  asm volatile ("li x9, 3264059991;"
                ".word 0xc1a4d333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x9", "x6");

  check_uint32(result, "brev", res,  res_brev[60]);


  asm volatile ("li x8, 814160941;"
                ".word 0xc36454b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x8", "x9");

  check_uint32(result, "brev", res,  res_brev[61]);


  asm volatile ("li x10, 17278959;"
                ".word 0xc45554b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x10", "x9");

  check_uint32(result, "brev", res,  res_brev[62]);


  asm volatile ("li x9, 2045245766;"
                ".word 0xc284d2b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x9", "x5");

  check_uint32(result, "brev", res,  res_brev[63]);


  asm volatile ("li x10, 2905088946;"
                ".word 0xc5455433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x10", "x8");

  check_uint32(result, "brev", res,  res_brev[64]);


  asm volatile ("li x10, 3046450448;"
                ".word 0xc38553b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x10", "x7");

  check_uint32(result, "brev", res,  res_brev[65]);


  asm volatile ("li x5, 2877622897;"
                ".word 0xc102d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x5", "x9");

  check_uint32(result, "brev", res,  res_brev[66]);


  asm volatile ("li x7, 982818655;"
                ".word 0xc303d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x7", "x8");

  check_uint32(result, "brev", res,  res_brev[67]);


  asm volatile ("li x8, 2765504900;"
                ".word 0xc42452b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x8", "x5");

  check_uint32(result, "brev", res,  res_brev[68]);


  asm volatile ("li x6, 818425878;"
                ".word 0xc5435433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x6", "x8");

  check_uint32(result, "brev", res,  res_brev[69]);


  asm volatile ("li x9, 1028960932;"
                ".word 0xc424d433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x9", "x8");

  check_uint32(result, "brev", res,  res_brev[70]);


  asm volatile ("li x8, 3866626971;"
                ".word 0xc2045333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x8", "x6");

  check_uint32(result, "brev", res,  res_brev[71]);


  asm volatile ("li x7, 1774885295;"
                ".word 0xc123d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x7", "x10");

  check_uint32(result, "brev", res,  res_brev[72]);


  asm volatile ("li x10, 3284664261;"
                ".word 0xc08554b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x10", "x9");

  check_uint32(result, "brev", res,  res_brev[73]);


  asm volatile ("li x7, 3245470981;"
                ".word 0xc0e3d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x7", "x9");

  check_uint32(result, "brev", res,  res_brev[74]);


  asm volatile ("li x6, 2132607880;"
                ".word 0xc0635333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x6");

  check_uint32(result, "brev", res,  res_brev[75]);


  asm volatile ("li x8, 3440612973;"
                ".word 0xc4b45333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x8", "x6");

  check_uint32(result, "brev", res,  res_brev[76]);


  asm volatile ("li x8, 2583720081;"
                ".word 0xc45454b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x8", "x9");

  check_uint32(result, "brev", res,  res_brev[77]);


  asm volatile ("li x5, 3015944086;"
                ".word 0xc482d3b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x5", "x7");

  check_uint32(result, "brev", res,  res_brev[78]);


  asm volatile ("li x6, 2692859386;"
                ".word 0xc0535333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x6");

  check_uint32(result, "brev", res,  res_brev[79]);


  asm volatile ("li x9, 4041582714;"
                ".word 0xc244d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x9", "x10");

  check_uint32(result, "brev", res,  res_brev[80]);


  asm volatile ("li x6, 2090505644;"
                ".word 0xc1735533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x6", "x10");

  check_uint32(result, "brev", res,  res_brev[81]);


  asm volatile ("li x5, 28968877;"
                ".word 0xc172d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x5", "x10");

  check_uint32(result, "brev", res,  res_brev[82]);


  asm volatile ("li x7, 3280807935;"
                ".word 0xc5d3d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x7", "x9");

  check_uint32(result, "brev", res,  res_brev[83]);


  asm volatile ("li x6, 500647450;"
                ".word 0xc0a35433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x6", "x8");

  check_uint32(result, "brev", res,  res_brev[84]);


  asm volatile ("li x8, 1183759362;"
                ".word 0xc3a454b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x8", "x9");

  check_uint32(result, "brev", res,  res_brev[85]);


  asm volatile ("li x6, 878710054;"
                ".word 0xc2a352b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x6", "x5");

  check_uint32(result, "brev", res,  res_brev[86]);


  asm volatile ("li x8, 2341301919;"
                ".word 0xc38454b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x8", "x9");

  check_uint32(result, "brev", res,  res_brev[87]);


  asm volatile ("li x7, 3428564006;"
                ".word 0xc0a3d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x7", "x9");

  check_uint32(result, "brev", res,  res_brev[88]);


  asm volatile ("li x10, 1868382068;"
                ".word 0xc2a55533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x10");

  check_uint32(result, "brev", res,  res_brev[89]);


  asm volatile ("li x5, 3111553826;"
                ".word 0xc022d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x5", "x10");

  check_uint32(result, "brev", res,  res_brev[90]);


  asm volatile ("li x10, 959645428;"
                ".word 0xc3e55533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x10");

  check_uint32(result, "brev", res,  res_brev[91]);


  asm volatile ("li x6, 4095144545;"
                ".word 0xc06353b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x6", "x7");

  check_uint32(result, "brev", res,  res_brev[92]);


  asm volatile ("li x5, 3786145572;"
                ".word 0xc422d3b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x5", "x7");

  check_uint32(result, "brev", res,  res_brev[93]);


  asm volatile ("li x6, 788025470;"
                ".word 0xc4835433;"
                "mv %[c], x8 \n" : [c] "=r" (res) : : "x6", "x8");

  check_uint32(result, "brev", res,  res_brev[94]);


  asm volatile ("li x5, 1740062968;"
                ".word 0xc362d4b3;"
                "mv %[c], x9 \n" : [c] "=r" (res) : : "x5", "x9");

  check_uint32(result, "brev", res,  res_brev[95]);


  asm volatile ("li x8, 4065423662;"
                ".word 0xc14453b3;"
                "mv %[c], x7 \n" : [c] "=r" (res) : : "x8", "x7");

  check_uint32(result, "brev", res,  res_brev[96]);


  asm volatile ("li x5, 1901098917;"
                ".word 0xc482d2b3;"
                "mv %[c], x5 \n" : [c] "=r" (res) : : "x5");

  check_uint32(result, "brev", res,  res_brev[97]);


  asm volatile ("li x6, 2244104220;"
                ".word 0xc4235333;"
                "mv %[c], x6 \n" : [c] "=r" (res) : : "x6");

  check_uint32(result, "brev", res,  res_brev[98]);


  asm volatile ("li x5, 3081422378;"
                ".word 0xc032d533;"
                "mv %[c], x10 \n" : [c] "=r" (res) : : "x5", "x10");

  check_uint32(result, "brev", res,  res_brev[99]);
*/
}

#endif
