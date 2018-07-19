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


	asm volatile ("li x4, 2339862156;"
	              "li x8, 4;"
	              ".word 0xc5d251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[0]) : "x4", "x3", "x8");

	asm volatile ("li x5, 359971260;"
	              "li x8, 4;"
	              ".word 0xc3e2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[1]) : "x5" , "x8");

	asm volatile ("li x5, 2921749930;"
	              "li x8, 4;"
	              ".word 0xc022d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[2]) : "x5" , "x8");

	asm volatile ("li x7, 633167787;"
	              "li x8, 4;"
	              ".word 0xc063d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[3]) : "x7", "x5", "x8");

	asm volatile ("li x4, 1993209288;"
	              "li x8, 4;"
	              ".word 0xc5125233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[4]) : "x4" , "x8");

	asm volatile ("li x7, 3009217673;"
	              "li x8, 4;"
	              ".word 0xc283d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[5]) : "x7" , "x8");

	asm volatile ("li x5, 745193095;"
	              "li x8, 4;"
	              ".word 0xc282d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[6]) : "x5" , "x8");

	asm volatile ("li x3, 56238837;"
	              "li x8, 4;"
	              ".word 0xc301d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[7]) : "x3", "x5", "x8");

	asm volatile ("li x7, 1362370416;"
	              "li x8, 4;"
	              ".word 0xc1b3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[8]) : "x7", "x5", "x8");

	asm volatile ("li x7, 1243705450;"
	              "li x8, 4;"
	              ".word 0xc5d3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[9]) : "x7", "x5", "x8");

	asm volatile ("li x7, 941549814;"
	              "li x8, 4;"
	              ".word 0xc2c3d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[10]) : "x7" , "x8");

	asm volatile ("li x6, 829150164;"
	              "li x8, 4;"
	              ".word 0xc0e352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[11]) : "x6", "x5", "x8");

	asm volatile ("li x3, 979441302;"
	              "li x8, 4;"
	              ".word 0xc421d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[12]) : "x3", "x7", "x8");

	asm volatile ("li x7, 1956584029;"
	              "li x8, 4;"
	              ".word 0xc153d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[13]) : "x7", "x6", "x8");

	asm volatile ("li x5, 3811214580;"
	              "li x8, 4;"
	              ".word 0xc2a2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[14]) : "x5", "x4", "x8");

	asm volatile ("li x3, 2523641296;"
	              "li x8, 4;"
	              ".word 0xc301d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[15]) : "x3" , "x8");

	asm volatile ("li x7, 2292280107;"
	              "li x8, 4;"
	              ".word 0xc013d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[16]) : "x7", "x6", "x8");

	asm volatile ("li x7, 2315001956;"
	              "li x8, 4;"
	              ".word 0xc513d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[17]) : "x7", "x3", "x8");

	asm volatile ("li x6, 632844330;"
	              "li x8, 4;"
	              ".word 0xc09353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[18]) : "x6", "x7", "x8");

	asm volatile ("li x7, 4071535975;"
	              "li x8, 4;"
	              ".word 0xc2c3d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[19]) : "x7" , "x8");

	asm volatile ("li x5, 1492477818;"
	              "li x8, 4;"
	              ".word 0xc0a2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[20]) : "x5" , "x8");

	asm volatile ("li x4, 679141110;"
	              "li x8, 4;"
	              ".word 0xc1a251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[21]) : "x4", "x3", "x8");

	asm volatile ("li x5, 2753910685;"
	              "li x8, 4;"
	              ".word 0xc132d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[22]) : "x5", "x4", "x8");

	asm volatile ("li x5, 2133330935;"
	              "li x8, 4;"
	              ".word 0xc1e2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[23]) : "x5", "x4", "x8");

	asm volatile ("li x6, 2859307217;"
	              "li x8, 4;"
	              ".word 0xc2035233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[24]) : "x6", "x4", "x8");

	asm volatile ("li x5, 902740678;"
	              "li x8, 4;"
	              ".word 0xc482d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[25]) : "x5", "x4", "x8");

	asm volatile ("li x4, 1524806809;"
	              "li x8, 4;"
	              ".word 0xc48252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[26]) : "x4", "x5", "x8");

	asm volatile ("li x5, 229216645;"
	              "li x8, 4;"
	              ".word 0xc2a2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[27]) : "x5" , "x8");

	asm volatile ("li x4, 4067128851;"
	              "li x8, 4;"
	              ".word 0xc00253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[28]) : "x4", "x7", "x8");

	asm volatile ("li x4, 1354533873;"
	              "li x8, 4;"
	              ".word 0xc1e251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[29]) : "x4", "x3", "x8");

	asm volatile ("li x3, 2442701507;"
	              "li x8, 4;"
	              ".word 0xc361d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[30]) : "x3", "x4", "x8");

	asm volatile ("li x7, 1258783327;"
	              "li x8, 4;"
	              ".word 0xc1c3d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[31]) : "x7" , "x8");

	asm volatile ("li x5, 3217249561;"
	              "li x8, 4;"
	              ".word 0xc5d2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[32]) : "x5" , "x8");

	asm volatile ("li x6, 3125753424;"
	              "li x8, 4;"
	              ".word 0xc5435333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[33]) : "x6" , "x8");

	asm volatile ("li x3, 3076963821;"
	              "li x8, 4;"
	              ".word 0xc051d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[34]) : "x3" , "x8");

	asm volatile ("li x5, 4078371559;"
	              "li x8, 4;"
	              ".word 0xc4e2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[35]) : "x5" , "x8");

	asm volatile ("li x3, 1051890704;"
	              "li x8, 4;"
	              ".word 0xc0b1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[36]) : "x3", "x6", "x8");

	asm volatile ("li x4, 567153197;"
	              "li x8, 4;"
	              ".word 0xc5d25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[37]) : "x4", "x6", "x8");

	asm volatile ("li x5, 420520075;"
	              "li x8, 4;"
	              ".word 0xc202d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[38]) : "x5", "x6", "x8");

	asm volatile ("li x6, 1695617655;"
	              "li x8, 4;"
	              ".word 0xc5735333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[39]) : "x6" , "x8");

	asm volatile ("li x4, 2662015716;"
	              "li x8, 4;"
	              ".word 0xc3a25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[40]) : "x4", "x6", "x8");

	asm volatile ("li x3, 1106697647;"
	              "li x8, 4;"
	              ".word 0xc051d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[41]) : "x3", "x7", "x8");

	asm volatile ("li x5, 1699485387;"
	              "li x8, 4;"
	              ".word 0xc0d2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[42]) : "x5", "x6", "x8");

	asm volatile ("li x7, 3380617044;"
	              "li x8, 4;"
	              ".word 0xc263d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[43]) : "x7", "x6", "x8");

	asm volatile ("li x7, 2473032024;"
	              "li x8, 4;"
	              ".word 0xc0a3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[44]) : "x7", "x5", "x8");

	asm volatile ("li x7, 844821557;"
	              "li x8, 4;"
	              ".word 0xc303d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[45]) : "x7" , "x8");

	asm volatile ("li x4, 449122837;"
	              "li x8, 4;"
	              ".word 0xc4b25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[46]) : "x4", "x6", "x8");

	asm volatile ("li x7, 4046678501;"
	              "li x8, 4;"
	              ".word 0xc173d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[47]) : "x7" , "x8");

	asm volatile ("li x7, 1718674532;"
	              "li x8, 4;"
	              ".word 0xc173d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[48]) : "x7", "x5", "x8");

	asm volatile ("li x4, 4251170976;"
	              "li x8, 4;"
	              ".word 0xc3625233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[49]) : "x4" , "x8");

	asm volatile ("li x3, 3928602178;"
	              "li x8, 4;"
	              ".word 0xc181d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[50]) : "x3", "x5", "x8");

	asm volatile ("li x3, 3474641099;"
	              "li x8, 4;"
	              ".word 0xc1a1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[51]) : "x3", "x6", "x8");

	asm volatile ("li x3, 1338533872;"
	              "li x8, 4;"
	              ".word 0xc201d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[52]) : "x3" , "x8");

	asm volatile ("li x6, 2418678157;"
	              "li x8, 4;"
	              ".word 0xc14351b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[53]) : "x6", "x3", "x8");

	asm volatile ("li x6, 1501565654;"
	              "li x8, 4;"
	              ".word 0xc3a353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[54]) : "x6", "x7", "x8");

	asm volatile ("li x6, 2821635556;"
	              "li x8, 4;"
	              ".word 0xc1b35333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[55]) : "x6" , "x8");

	asm volatile ("li x5, 2690088228;"
	              "li x8, 4;"
	              ".word 0xc182d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[56]) : "x5", "x7", "x8");

	asm volatile ("li x7, 1150307662;"
	              "li x8, 4;"
	              ".word 0xc363d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[57]) : "x7", "x4", "x8");

	asm volatile ("li x4, 830647857;"
	              "li x8, 4;"
	              ".word 0xc06251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[58]) : "x4", "x3", "x8");

	asm volatile ("li x3, 32791590;"
	              "li x8, 4;"
	              ".word 0xc241d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[59]) : "x3" , "x8");

	asm volatile ("li x4, 1405421484;"
	              "li x8, 4;"
	              ".word 0xc0325233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[60]) : "x4" , "x8");

	asm volatile ("li x5, 124651324;"
	              "li x8, 4;"
	              ".word 0xc4e2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[61]) : "x5", "x6", "x8");

	asm volatile ("li x3, 2293264275;"
	              "li x8, 4;"
	              ".word 0xc1c1d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[62]) : "x3", "x5", "x8");

	asm volatile ("li x5, 2059465261;"
	              "li x8, 4;"
	              ".word 0xc1e2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[63]) : "x5" , "x8");

	asm volatile ("li x6, 1703362263;"
	              "li x8, 4;"
	              ".word 0xc4e352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[64]) : "x6", "x5", "x8");

	asm volatile ("li x5, 890702383;"
	              "li x8, 4;"
	              ".word 0xc262d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[65]) : "x5", "x4", "x8");

	asm volatile ("li x4, 1456676383;"
	              "li x8, 4;"
	              ".word 0xc2a25233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[66]) : "x4" , "x8");

	asm volatile ("li x5, 2432403012;"
	              "li x8, 4;"
	              ".word 0xc052d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[67]) : "x5", "x4", "x8");

	asm volatile ("li x3, 2229176692;"
	              "li x8, 4;"
	              ".word 0xc451d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[68]) : "x3", "x5", "x8");

	asm volatile ("li x4, 1341572959;"
	              "li x8, 4;"
	              ".word 0xc57253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[69]) : "x4", "x7", "x8");

	asm volatile ("li x5, 754014179;"
	              "li x8, 4;"
	              ".word 0xc062d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[70]) : "x5" , "x8");

	asm volatile ("li x3, 2706287084;"
	              "li x8, 4;"
	              ".word 0xc511d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[71]) : "x3" , "x8");

	asm volatile ("li x4, 2546735484;"
	              "li x8, 4;"
	              ".word 0xc51252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[72]) : "x4", "x5", "x8");

	asm volatile ("li x4, 3768226042;"
	              "li x8, 4;"
	              ".word 0xc0925233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[73]) : "x4" , "x8");

	asm volatile ("li x7, 3334976289;"
	              "li x8, 4;"
	              ".word 0xc4b3d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[74]) : "x7", "x3", "x8");

	asm volatile ("li x3, 3756272168;"
	              "li x8, 4;"
	              ".word 0xc221d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[75]) : "x3" , "x8");

	asm volatile ("li x5, 4271827347;"
	              "li x8, 4;"
	              ".word 0xc062d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[76]) : "x5" , "x8");

	asm volatile ("li x6, 3494049951;"
	              "li x8, 4;"
	              ".word 0xc0e351b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[77]) : "x6", "x3", "x8");

	asm volatile ("li x5, 2819954966;"
	              "li x8, 4;"
	              ".word 0xc5a2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[78]) : "x5", "x4", "x8");

	asm volatile ("li x3, 2302179624;"
	              "li x8, 4;"
	              ".word 0xc3e1d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[79]) : "x3" , "x8");

	asm volatile ("li x3, 2430695830;"
	              "li x8, 4;"
	              ".word 0xc171d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[80]) : "x3", "x7", "x8");

	asm volatile ("li x4, 2386578910;"
	              "li x8, 4;"
	              ".word 0xc16251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[81]) : "x4", "x3", "x8");

	asm volatile ("li x7, 545071444;"
	              "li x8, 4;"
	              ".word 0xc543d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[82]) : "x7", "x5", "x8");

	asm volatile ("li x5, 1524353563;"
	              "li x8, 4;"
	              ".word 0xc222d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[83]) : "x5", "x3", "x8");

	asm volatile ("li x5, 3797339955;"
	              "li x8, 4;"
	              ".word 0xc082d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[84]) : "x5", "x7", "x8");

	asm volatile ("li x6, 2711631639;"
	              "li x8, 4;"
	              ".word 0xc48353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[85]) : "x6", "x7", "x8");

	asm volatile ("li x7, 2284523471;"
	              "li x8, 4;"
	              ".word 0xc363d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[86]) : "x7" , "x8");

	asm volatile ("li x5, 1588157490;"
	              "li x8, 4;"
	              ".word 0xc022d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[87]) : "x5", "x3", "x8");

	asm volatile ("li x6, 100242699;"
	              "li x8, 4;"
	              ".word 0xc4e353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[88]) : "x6", "x7", "x8");

	asm volatile ("li x7, 2205779439;"
	              "li x8, 4;"
	              ".word 0xc2a3d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[89]) : "x7", "x3", "x8");

	asm volatile ("li x5, 2200646398;"
	              "li x8, 4;"
	              ".word 0xc2e2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[90]) : "x5" , "x8");

	asm volatile ("li x4, 2685284149;"
	              "li x8, 4;"
	              ".word 0xc45253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[91]) : "x4", "x7", "x8");

	asm volatile ("li x6, 2453066342;"
	              "li x8, 4;"
	              ".word 0xc3e352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[92]) : "x6", "x5", "x8");

	asm volatile ("li x4, 63015277;"
	              "li x8, 4;"
	              ".word 0xc45251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[93]) : "x4", "x3", "x8");

	asm volatile ("li x7, 107320171;"
	              "li x8, 4;"
	              ".word 0xc063d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[94]) : "x7", "x3", "x8");

	asm volatile ("li x5, 4221549734;"
	              "li x8, 4;"
	              ".word 0xc0e2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[95]) : "x5", "x3", "x8");

	asm volatile ("li x5, 2791604357;"
	              "li x8, 4;"
	              ".word 0xc342d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[96]) : "x5", "x4", "x8");

	asm volatile ("li x7, 2953118635;"
	              "li x8, 4;"
	              ".word 0xc2c3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[97]) : "x7", "x5", "x8");

	asm volatile ("li x5, 3267137248;"
	              "li x8, 4;"
	              ".word 0xc5d2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[98]) : "x5", "x4", "x8");

	asm volatile ("li x7, 1994093903;"
	              "li x8, 4;"
	              ".word 0xc053d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[99]) : "x7", "x5", "x8");

	asm volatile ("li x3, 3396727517;"
	              "li x8, 4;"
	              ".word 0xc2c1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[100]) : "x3", "x4", "x8");

	asm volatile ("li x3, 2555939341;"
	              "li x8, 4;"
	              ".word 0xc1b1d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[101]) : "x3", "x7", "x8");

	asm volatile ("li x6, 4275334441;"
	              "li x8, 4;"
	              ".word 0xc3835333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[102]) : "x6" , "x8");

	asm volatile ("li x4, 76192204;"
	              "li x8, 4;"
	              ".word 0xc5425233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[103]) : "x4" , "x8");

	asm volatile ("li x5, 3656677021;"
	              "li x8, 4;"
	              ".word 0xc2e2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[104]) : "x5", "x3", "x8");

	asm volatile ("li x5, 3957958623;"
	              "li x8, 4;"
	              ".word 0xc152d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[105]) : "x5", "x7", "x8");

	asm volatile ("li x3, 75200165;"
	              "li x8, 4;"
	              ".word 0xc0b1d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[106]) : "x3", "x5", "x8");

	asm volatile ("li x7, 2693174542;"
	              "li x8, 4;"
	              ".word 0xc0e3d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[107]) : "x7", "x6", "x8");

	asm volatile ("li x6, 894870852;"
	              "li x8, 4;"
	              ".word 0xc0c352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[108]) : "x6", "x5", "x8");

	asm volatile ("li x4, 3163401499;"
	              "li x8, 4;"
	              ".word 0xc0925233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[109]) : "x4" , "x8");

	asm volatile ("li x5, 1999830497;"
	              "li x8, 4;"
	              ".word 0xc242d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[110]) : "x5", "x6", "x8");

	asm volatile ("li x3, 458528535;"
	              "li x8, 4;"
	              ".word 0xc381d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[111]) : "x3", "x7", "x8");

	asm volatile ("li x7, 1850549109;"
	              "li x8, 4;"
	              ".word 0xc203d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[112]) : "x7", "x5", "x8");

	asm volatile ("li x3, 447913243;"
	              "li x8, 4;"
	              ".word 0xc121d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[113]) : "x3" , "x8");

	asm volatile ("li x7, 810394038;"
	              "li x8, 4;"
	              ".word 0xc483d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[114]) : "x7", "x4", "x8");

	asm volatile ("li x7, 1882359881;"
	              "li x8, 4;"
	              ".word 0xc143d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[115]) : "x7", "x3", "x8");

	asm volatile ("li x6, 2049545162;"
	              "li x8, 4;"
	              ".word 0xc42352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[116]) : "x6", "x5", "x8");

	asm volatile ("li x7, 73662968;"
	              "li x8, 4;"
	              ".word 0xc513d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[117]) : "x7", "x6", "x8");

	asm volatile ("li x3, 1816118314;"
	              "li x8, 4;"
	              ".word 0xc511d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[118]) : "x3", "x4", "x8");

	asm volatile ("li x7, 2545955540;"
	              "li x8, 4;"
	              ".word 0xc423d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[119]) : "x7", "x6", "x8");

	asm volatile ("li x7, 1305752975;"
	              "li x8, 4;"
	              ".word 0xc2e3d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[120]) : "x7", "x3", "x8");

	asm volatile ("li x7, 3225835937;"
	              "li x8, 4;"
	              ".word 0xc453d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[121]) : "x7", "x5", "x8");

	asm volatile ("li x7, 1751384456;"
	              "li x8, 4;"
	              ".word 0xc023d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[122]) : "x7" , "x8");

	asm volatile ("li x7, 3711879110;"
	              "li x8, 4;"
	              ".word 0xc483d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[123]) : "x7", "x4", "x8");

	asm volatile ("li x4, 136837932;"
	              "li x8, 4;"
	              ".word 0xc3c253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[124]) : "x4", "x7", "x8");

	asm volatile ("li x4, 3790720875;"
	              "li x8, 4;"
	              ".word 0xc5d25233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[125]) : "x4" , "x8");

	asm volatile ("li x5, 2243543519;"
	              "li x8, 4;"
	              ".word 0xc042d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[126]) : "x5", "x6", "x8");

	asm volatile ("li x3, 1460594247;"
	              "li x8, 4;"
	              ".word 0xc1d1d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[127]) : "x3", "x7", "x8");

	asm volatile ("li x5, 95608196;"
	              "li x8, 4;"
	              ".word 0xc5d2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[128]) : "x5" , "x8");

	asm volatile ("li x3, 3632585166;"
	              "li x8, 4;"
	              ".word 0xc2a1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[129]) : "x3", "x6", "x8");

	asm volatile ("li x5, 1823592865;"
	              "li x8, 4;"
	              ".word 0xc4e2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[130]) : "x5", "x3", "x8");

	asm volatile ("li x5, 3557327520;"
	              "li x8, 4;"
	              ".word 0xc382d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[131]) : "x5", "x3", "x8");

	asm volatile ("li x7, 197189993;"
	              "li x8, 4;"
	              ".word 0xc4b3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[132]) : "x7", "x4", "x8");

	asm volatile ("li x3, 1123249855;"
	              "li x8, 4;"
	              ".word 0xc1e1d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[133]) : "x3", "x5", "x8");

	asm volatile ("li x5, 2092632621;"
	              "li x8, 4;"
	              ".word 0xc262d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[134]) : "x5", "x4", "x8");

	asm volatile ("li x3, 111323075;"
	              "li x8, 4;"
	              ".word 0xc341d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[135]) : "x3", "x7", "x8");

	asm volatile ("li x4, 4256429460;"
	              "li x8, 4;"
	              ".word 0xc22253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[136]) : "x4", "x7", "x8");

	asm volatile ("li x7, 4223812966;"
	              "li x8, 4;"
	              ".word 0xc123d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[137]) : "x7" , "x8");

	asm volatile ("li x7, 970654751;"
	              "li x8, 4;"
	              ".word 0xc113d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[138]) : "x7", "x6", "x8");

	asm volatile ("li x3, 653122057;"
	              "li x8, 4;"
	              ".word 0xc3c1d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[139]) : "x3", "x5", "x8");

	asm volatile ("li x4, 4076083347;"
	              "li x8, 4;"
	              ".word 0xc5d251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[140]) : "x4", "x3", "x8");

	asm volatile ("li x6, 2318290643;"
	              "li x8, 4;"
	              ".word 0xc12353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[141]) : "x6", "x7", "x8");

	asm volatile ("li x4, 2014308549;"
	              "li x8, 4;"
	              ".word 0xc57252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[142]) : "x4", "x5", "x8");

	asm volatile ("li x5, 419312243;"
	              "li x8, 4;"
	              ".word 0xc222d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[143]) : "x5", "x3", "x8");

	asm volatile ("li x3, 1827778540;"
	              "li x8, 4;"
	              ".word 0xc1d1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[144]) : "x3", "x6", "x8");

	asm volatile ("li x3, 2766056633;"
	              "li x8, 4;"
	              ".word 0xc201d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[145]) : "x3", "x5", "x8");

	asm volatile ("li x4, 2373599815;"
	              "li x8, 4;"
	              ".word 0xc24251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[146]) : "x4", "x3", "x8");

	asm volatile ("li x4, 2389168658;"
	              "li x8, 4;"
	              ".word 0xc2e25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[147]) : "x4", "x6", "x8");

	asm volatile ("li x7, 2236651145;"
	              "li x8, 4;"
	              ".word 0xc173d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[148]) : "x7", "x3", "x8");

	asm volatile ("li x3, 3461453142;"
	              "li x8, 4;"
	              ".word 0xc1f1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[149]) : "x3", "x4", "x8");

	asm volatile ("li x7, 3603677217;"
	              "li x8, 4;"
	              ".word 0xc223d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[150]) : "x7", "x6", "x8");

	asm volatile ("li x7, 220207239;"
	              "li x8, 4;"
	              ".word 0xc183d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[151]) : "x7", "x4", "x8");

	asm volatile ("li x7, 4006348745;"
	              "li x8, 4;"
	              ".word 0xc1f3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[152]) : "x7", "x4", "x8");

	asm volatile ("li x3, 1912145424;"
	              "li x8, 4;"
	              ".word 0xc131d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[153]) : "x3" , "x8");

	asm volatile ("li x6, 2486709050;"
	              "li x8, 4;"
	              ".word 0xc36352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[154]) : "x6", "x5", "x8");

	asm volatile ("li x5, 4135337177;"
	              "li x8, 4;"
	              ".word 0xc082d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[155]) : "x5", "x6", "x8");

	asm volatile ("li x7, 2695578584;"
	              "li x8, 4;"
	              ".word 0xc3e3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[156]) : "x7", "x5", "x8");

	asm volatile ("li x4, 2866060270;"
	              "li x8, 4;"
	              ".word 0xc3e251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[157]) : "x4", "x3", "x8");

	asm volatile ("li x6, 3591752141;"
	              "li x8, 4;"
	              ".word 0xc0235333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[158]) : "x6" , "x8");

	asm volatile ("li x3, 1995387246;"
	              "li x8, 4;"
	              ".word 0xc3e1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[159]) : "x3", "x6", "x8");

	asm volatile ("li x7, 3904261101;"
	              "li x8, 4;"
	              ".word 0xc423d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[160]) : "x7", "x4", "x8");

	asm volatile ("li x4, 4119494124;"
	              "li x8, 4;"
	              ".word 0xc54252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[161]) : "x4", "x5", "x8");

	asm volatile ("li x7, 1619397267;"
	              "li x8, 4;"
	              ".word 0xc4e3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[162]) : "x7", "x5", "x8");

	asm volatile ("li x3, 2223633015;"
	              "li x8, 4;"
	              ".word 0xc5a1d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[163]) : "x3" , "x8");

	asm volatile ("li x6, 3232016520;"
	              "li x8, 4;"
	              ".word 0xc18351b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[164]) : "x6", "x3", "x8");

	asm volatile ("li x4, 3945721265;"
	              "li x8, 4;"
	              ".word 0xc42253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[165]) : "x4", "x7", "x8");

	asm volatile ("li x3, 806859747;"
	              "li x8, 4;"
	              ".word 0xc301d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[166]) : "x3", "x7", "x8");

	asm volatile ("li x4, 2706404337;"
	              "li x8, 4;"
	              ".word 0xc3e25233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[167]) : "x4" , "x8");

	asm volatile ("li x6, 1306601610;"
	              "li x8, 4;"
	              ".word 0xc0a353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[168]) : "x6", "x7", "x8");

	asm volatile ("li x4, 3677170756;"
	              "li x8, 4;"
	              ".word 0xc48253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[169]) : "x4", "x7", "x8");

	asm volatile ("li x4, 1032905023;"
	              "li x8, 4;"
	              ".word 0xc1c25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[170]) : "x4", "x6", "x8");

	asm volatile ("li x6, 3077011820;"
	              "li x8, 4;"
	              ".word 0xc22351b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[171]) : "x6", "x3", "x8");

	asm volatile ("li x5, 4017367683;"
	              "li x8, 4;"
	              ".word 0xc362d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[172]) : "x5", "x7", "x8");

	asm volatile ("li x7, 2175715000;"
	              "li x8, 4;"
	              ".word 0xc2e3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[173]) : "x7", "x4", "x8");

	asm volatile ("li x7, 2351275017;"
	              "li x8, 4;"
	              ".word 0xc073d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[174]) : "x7", "x6", "x8");

	asm volatile ("li x6, 481694234;"
	              "li x8, 4;"
	              ".word 0xc5735233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[175]) : "x6", "x4", "x8");

	asm volatile ("li x5, 1314186824;"
	              "li x8, 4;"
	              ".word 0xc362d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[176]) : "x5" , "x8");

	asm volatile ("li x4, 1320597788;"
	              "li x8, 4;"
	              ".word 0xc0b251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[177]) : "x4", "x3", "x8");

	asm volatile ("li x3, 1167172536;"
	              "li x8, 4;"
	              ".word 0xc511d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[178]) : "x3", "x5", "x8");

	asm volatile ("li x5, 145554018;"
	              "li x8, 4;"
	              ".word 0xc2a2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[179]) : "x5", "x4", "x8");

	asm volatile ("li x3, 1702861754;"
	              "li x8, 4;"
	              ".word 0xc091d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[180]) : "x3" , "x8");

	asm volatile ("li x6, 382386539;"
	              "li x8, 4;"
	              ".word 0xc38353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[181]) : "x6", "x7", "x8");

	asm volatile ("li x4, 1219257864;"
	              "li x8, 4;"
	              ".word 0xc5d25233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[182]) : "x4" , "x8");

	asm volatile ("li x3, 3964892165;"
	              "li x8, 4;"
	              ".word 0xc481d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[183]) : "x3", "x4", "x8");

	asm volatile ("li x6, 4245225868;"
	              "li x8, 4;"
	              ".word 0xc08353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[184]) : "x6", "x7", "x8");

	asm volatile ("li x5, 3328342302;"
	              "li x8, 4;"
	              ".word 0xc4e2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[185]) : "x5", "x3", "x8");

	asm volatile ("li x4, 3397902541;"
	              "li x8, 4;"
	              ".word 0xc02252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[186]) : "x4", "x5", "x8");

	asm volatile ("li x7, 4091791438;"
	              "li x8, 4;"
	              ".word 0xc223d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[187]) : "x7", "x3", "x8");

	asm volatile ("li x3, 1919182780;"
	              "li x8, 4;"
	              ".word 0xc091d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[188]) : "x3", "x6", "x8");

	asm volatile ("li x4, 3446996931;"
	              "li x8, 4;"
	              ".word 0xc3c251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[189]) : "x4", "x3", "x8");

	asm volatile ("li x3, 1271583627;"
	              "li x8, 4;"
	              ".word 0xc2a1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[190]) : "x3", "x4", "x8");

	asm volatile ("li x5, 1309642555;"
	              "li x8, 4;"
	              ".word 0xc1d2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[191]) : "x5", "x3", "x8");

	asm volatile ("li x3, 3135154304;"
	              "li x8, 4;"
	              ".word 0xc0b1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[192]) : "x3", "x4", "x8");

	asm volatile ("li x4, 865401873;"
	              "li x8, 4;"
	              ".word 0xc1f252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[193]) : "x4", "x5", "x8");

	asm volatile ("li x3, 811994739;"
	              "li x8, 4;"
	              ".word 0xc4e1d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[194]) : "x3", "x5", "x8");

	asm volatile ("li x7, 3414587392;"
	              "li x8, 4;"
	              ".word 0xc4b3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[195]) : "x7", "x5", "x8");

	asm volatile ("li x5, 868393741;"
	              "li x8, 4;"
	              ".word 0xc5a2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[196]) : "x5", "x6", "x8");

	asm volatile ("li x7, 1650804156;"
	              "li x8, 4;"
	              ".word 0xc323d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[197]) : "x7", "x4", "x8");

	asm volatile ("li x7, 1609487372;"
	              "li x8, 4;"
	              ".word 0xc283d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[198]) : "x7", "x4", "x8");

	asm volatile ("li x4, 2496720896;"
	              "li x8, 4;"
	              ".word 0xc1d251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[199]) : "x4", "x3", "x8");


for(i = 0; i < NumberOfStimuli; i++) {
    	check_uint32(result, "brev", res_vec[i],  res_brev[i]);
    	}

}

#endif
