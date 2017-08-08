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
#endif

testcase_t testcases[] = {
  { .name = "bset"          , .test = check_bset          },
  { .name = "bclr"          , .test = check_bclr          },
  { .name = "bextract"      , .test = check_bextract      },
  { .name = "bextractu"     , .test = check_bextractu     },
  { .name = "binsert"       , .test = check_binsert       },
#ifdef __riscv
  { .name = "bset_reg"      , .test = check_bset_reg      },
  { .name = "bclr_reg"      , .test = check_bclr_reg      },
  { .name = "bextract_reg"  , .test = check_bextract_reg  },
  { .name = "bextractu_reg" , .test = check_bextractu_reg },
  { .name = "binsert_reg"   , .test = check_binsert_reg   },
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
#endif