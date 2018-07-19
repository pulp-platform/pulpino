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
//# T E S T    BREVERSE
//################################################################################

void check_breverse(testresult_t *result, void (*start)(), void (*stop)()) {
                  unsigned int i;
                  unsigned int res;
                  volatile unsigned int res_vec[NumberOfStimuli];
                  // Initialize result vector
                  for(i = 0; i < NumberOfStimuli; i++) res_vec[i] = 0;


	asm volatile ("li x6, 161810921;"
	              "li x8, 4;"
	              ".word 0xc1e35233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[0]) : "x6", "x4", "x8");

	asm volatile ("li x5, 720;"
	              "li x8, 4;"
	              ".word 0xc0a2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[1]) : "x5", "x6", "x8");

	asm volatile ("li x5, 14236;"
	              "li x8, 4;"
	              ".word 0xc0e2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[2]) : "x5", "x6", "x8");

	asm volatile ("li x3, 336;"
	              "li x8, 4;"
	              ".word 0xc491d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[3]) : "x3", "x7", "x8");

	asm volatile ("li x3, 518520411;"
	              "li x8, 4;"
	              ".word 0xc1e1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[4]) : "x3", "x4", "x8");

	asm volatile ("li x3, 396624178;"
	              "li x8, 4;"
	              ".word 0xc5e1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[5]) : "x3", "x6", "x8");

	asm volatile ("li x5, 8;"
	              "li x8, 4;"
	              ".word 0xc242d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[6]) : "x5", "x4", "x8");

	asm volatile ("li x3, 3;"
	              "li x8, 4;"
	              ".word 0xc221d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[7]) : "x3", "x7", "x8");

	asm volatile ("li x4, 2156;"
	              "li x8, 4;"
	              ".word 0xc0c253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[8]) : "x4", "x7", "x8");

	asm volatile ("li x5, 60516;"
	              "li x8, 4;"
	              ".word 0xc302d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[9]) : "x5", "x6", "x8");

	asm volatile ("li x3, 201863;"
	              "li x8, 4;"
	              ".word 0xc551d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[10]) : "x3", "x6", "x8");

	asm volatile ("li x7, 29252;"
	              "li x8, 4;"
	              ".word 0xc4f3d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[11]) : "x7" , "x8");

	asm volatile ("li x4, 184424;"
	              "li x8, 4;"
	              ".word 0xc5225233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[12]) : "x4" , "x8");

	asm volatile ("li x3, 3;"
	              "li x8, 4;"
	              ".word 0xc221d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[13]) : "x3", "x7", "x8");

	asm volatile ("li x6, 2146;"
	              "li x8, 4;"
	              ".word 0xc4c351b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[14]) : "x6", "x3", "x8");

	asm volatile ("li x5, 991;"
	              "li x8, 4;"
	              ".word 0xc2a2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[15]) : "x5", "x6", "x8");

	asm volatile ("li x6, 230;"
	              "li x8, 4;"
	              ".word 0xc0c35233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[16]) : "x6", "x4", "x8");

	asm volatile ("li x4, 936;"
	              "li x8, 4;"
	              ".word 0xc4c25233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[17]) : "x4" , "x8");

	asm volatile ("li x7, 483581;"
	              "li x8, 4;"
	              ".word 0xc143d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[18]) : "x7", "x3", "x8");

	asm volatile ("li x4, 511430220;"
	              "li x8, 4;"
	              ".word 0xc1d25233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[19]) : "x4" , "x8");

	asm volatile ("li x5, 15;"
	              "li x8, 4;"
	              ".word 0xc242d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[20]) : "x5" , "x8");

	asm volatile ("li x5, 655;"
	              "li x8, 4;"
	              ".word 0xc2a2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[21]) : "x5", "x6", "x8");

	asm volatile ("li x4, 73754;"
	              "li x8, 4;"
	              ".word 0xc11251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[22]) : "x4", "x3", "x8");

	asm volatile ("li x3, 826616;"
	              "li x8, 4;"
	              ".word 0xc551d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[23]) : "x3", "x4", "x8");

	asm volatile ("li x3, 47313738;"
	              "li x8, 4;"
	              ".word 0xc5b1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[24]) : "x3", "x6", "x8");

	asm volatile ("li x6, 38428;"
	              "li x8, 4;"
	              ".word 0xc3035233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[25]) : "x6", "x4", "x8");

	asm volatile ("li x7, 832730;"
	              "li x8, 4;"
	              ".word 0xc553d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[26]) : "x7" , "x8");

	asm volatile ("li x4, 11;"
	              "li x8, 4;"
	              ".word 0xc24252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[27]) : "x4", "x5", "x8");

	asm volatile ("li x6, 3600;"
	              "li x8, 4;"
	              ".word 0xc2c353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[28]) : "x6", "x7", "x8");

	asm volatile ("li x6, 196273;"
	              "li x8, 4;"
	              ".word 0xc1235233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[29]) : "x6", "x4", "x8");

	asm volatile ("li x4, 63;"
	              "li x8, 4;"
	              ".word 0xc46251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[30]) : "x4", "x3", "x8");

	asm volatile ("li x5, 606694;"
	              "li x8, 4;"
	              ".word 0xc342d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[31]) : "x5", "x6", "x8");

	asm volatile ("li x6, 171596122;"
	              "li x8, 4;"
	              ".word 0xc3c35233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[32]) : "x6", "x4", "x8");

	asm volatile ("li x3, 26724175;"
	              "li x8, 4;"
	              ".word 0xc191d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[33]) : "x3" , "x8");

	asm volatile ("li x3, 3047646;"
	              "li x8, 4;"
	              ".word 0xc161d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[34]) : "x3" , "x8");

	asm volatile ("li x5, 2504088;"
	              "li x8, 4;"
	              ".word 0xc162d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[35]) : "x5", "x6", "x8");

	asm volatile ("li x4, 12;"
	              "li x8, 4;"
	              ".word 0xc4625333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[36]) : "x4", "x6", "x8");

	asm volatile ("li x5, 0;"
	              "li x8, 4;"
	              ".word 0xc022d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[37]) : "x5", "x6", "x8");

	asm volatile ("li x4, 6;"
	              "li x8, 4;"
	              ".word 0xc24253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[38]) : "x4", "x7", "x8");

	asm volatile ("li x3, 156;"
	              "li x8, 4;"
	              ".word 0xc491d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[39]) : "x3", "x7", "x8");

	asm volatile ("li x4, 247782;"
	              "li x8, 4;"
	              ".word 0xc5225233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[40]) : "x4" , "x8");

	asm volatile ("li x7, 182;"
	              "li x8, 4;"
	              ".word 0xc093d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[41]) : "x7", "x3", "x8");

	asm volatile ("li x4, 728323;"
	              "li x8, 4;"
	              ".word 0xc34251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[42]) : "x4", "x3", "x8");

	asm volatile ("li x6, 508911283;"
	              "li x8, 4;"
	              ".word 0xc1f35333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[43]) : "x6" , "x8");

	asm volatile ("li x7, 6;"
	              "li x8, 4;"
	              ".word 0xc433d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[44]) : "x7", "x6", "x8");

	asm volatile ("li x4, 170016911;"
	              "li x8, 4;"
	              ".word 0xc3c252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[45]) : "x4", "x5", "x8");

	asm volatile ("li x3, 39242223;"
	              "li x8, 4;"
	              ".word 0xc1b1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[46]) : "x3", "x6", "x8");

	asm volatile ("li x5, 148716;"
	              "li x8, 4;"
	              ".word 0xc142d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[47]) : "x5" , "x8");

	asm volatile ("li x3, 9;"
	              "li x8, 4;"
	              ".word 0xc061d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[48]) : "x3", "x7", "x8");

	asm volatile ("li x4, 1263755;"
	              "li x8, 4;"
	              ".word 0xc1625333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[49]) : "x4", "x6", "x8");

	asm volatile ("li x5, 49267737;"
	              "li x8, 4;"
	              ".word 0xc1d2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[50]) : "x5", "x3", "x8");

	asm volatile ("li x7, 3581;"
	              "li x8, 4;"
	              ".word 0xc2c3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[51]) : "x7", "x5", "x8");

	asm volatile ("li x7, 3144425;"
	              "li x8, 4;"
	              ".word 0xc193d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[52]) : "x7", "x3", "x8");

	asm volatile ("li x3, 1523881;"
	              "li x8, 4;"
	              ".word 0xc361d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[53]) : "x3", "x7", "x8");

	asm volatile ("li x6, 20;"
	              "li x8, 4;"
	              ".word 0xc2635233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[54]) : "x6", "x4", "x8");

	asm volatile ("li x4, 0;"
	              "li x8, 4;"
	              ".word 0xc4325233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[55]) : "x4" , "x8");

	asm volatile ("li x5, 49;"
	              "li x8, 4;"
	              ".word 0xc072d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[56]) : "x5", "x3", "x8");

	asm volatile ("li x5, 46;"
	              "li x8, 4;"
	              ".word 0xc2a2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[57]) : "x5", "x4", "x8");

	asm volatile ("li x7, 211354714;"
	              "li x8, 4;"
	              ".word 0xc3c3d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[58]) : "x7" , "x8");

	asm volatile ("li x3, 1;"
	              "li x8, 4;"
	              ".word 0xc431d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[59]) : "x3", "x4", "x8");

	asm volatile ("li x7, 61978095;"
	              "li x8, 4;"
	              ".word 0xc3a3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[60]) : "x7", "x4", "x8");

	asm volatile ("li x3, 54;"
	              "li x8, 4;"
	              ".word 0xc461d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[61]) : "x3", "x6", "x8");

	asm volatile ("li x4, 77;"
	              "li x8, 4;"
	              ".word 0xc0b25233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[62]) : "x4" , "x8");

	asm volatile ("li x3, 68;"
	              "li x8, 4;"
	              ".word 0xc281d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[63]) : "x3", "x7", "x8");

	asm volatile ("li x7, 27660954;"
	              "li x8, 4;"
	              ".word 0xc193d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[64]) : "x7" , "x8");

	asm volatile ("li x5, 259;"
	              "li x8, 4;"
	              ".word 0xc492d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[65]) : "x5" , "x8");

	asm volatile ("li x5, 3742;"
	              "li x8, 4;"
	              ".word 0xc4c2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[66]) : "x5", "x4", "x8");

	asm volatile ("li x4, 312268;"
	              "li x8, 4;"
	              ".word 0xc16253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[67]) : "x4", "x7", "x8");

	asm volatile ("li x4, 544;"
	              "li x8, 4;"
	              ".word 0xc0a25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[68]) : "x4", "x6", "x8");

	asm volatile ("li x3, 4;"
	              "li x8, 4;"
	              ".word 0xc241d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[69]) : "x3", "x5", "x8");

	asm volatile ("li x6, 0;"
	              "li x8, 4;"
	              ".word 0xc2435333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[70]) : "x6" , "x8");

	asm volatile ("li x3, 3335;"
	              "li x8, 4;"
	              ".word 0xc4c1d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[71]) : "x3" , "x8");

	asm volatile ("li x5, 1764;"
	              "li x8, 4;"
	              ".word 0xc4c2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[72]) : "x5", "x6", "x8");

	asm volatile ("li x4, 190;"
	              "li x8, 4;"
	              ".word 0xc4c25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[73]) : "x4", "x6", "x8");

	asm volatile ("li x4, 4133;"
	              "li x8, 4;"
	              ".word 0xc0e25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[74]) : "x4", "x6", "x8");

	asm volatile ("li x3, 4;"
	              "li x8, 4;"
	              ".word 0xc431d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[75]) : "x3", "x4", "x8");

	asm volatile ("li x4, 3805407;"
	              "li x8, 4;"
	              ".word 0xc36251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[76]) : "x4", "x3", "x8");

	asm volatile ("li x6, 1619423;"
	              "li x8, 4;"
	              ".word 0xc55352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[77]) : "x6", "x5", "x8");

	asm volatile ("li x6, 2718311;"
	              "li x8, 4;"
	              ".word 0xc36352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[78]) : "x6", "x5", "x8");

	asm volatile ("li x6, 10991603;"
	              "li x8, 4;"
	              ".word 0xc58352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[79]) : "x6", "x5", "x8");

	asm volatile ("li x7, 659;"
	              "li x8, 4;"
	              ".word 0xc0b3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[80]) : "x7", "x4", "x8");

	asm volatile ("li x5, 41;"
	              "li x8, 4;"
	              ".word 0xc072d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[81]) : "x5" , "x8");

	asm volatile ("li x3, 1931857;"
	              "li x8, 4;"
	              ".word 0xc151d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[82]) : "x3", "x6", "x8");

	asm volatile ("li x5, 13859;"
	              "li x8, 4;"
	              ".word 0xc2e2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[83]) : "x5", "x4", "x8");

	asm volatile ("li x4, 176580;"
	              "li x8, 4;"
	              ".word 0xc5225233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[84]) : "x4" , "x8");

	asm volatile ("li x7, 111846;"
	              "li x8, 4;"
	              ".word 0xc343d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[85]) : "x7", "x5", "x8");

	asm volatile ("li x5, 13;"
	              "li x8, 4;"
	              ".word 0xc2a2d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[86]) : "x5", "x7", "x8");

	asm volatile ("li x3, 187088974;"
	              "li x8, 4;"
	              ".word 0xc1c1d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[87]) : "x3", "x5", "x8");

	asm volatile ("li x7, 308;"
	              "li x8, 4;"
	              ".word 0xc2a3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[88]) : "x7", "x4", "x8");

	asm volatile ("li x3, 12623;"
	              "li x8, 4;"
	              ".word 0xc0f1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[89]) : "x3", "x4", "x8");

	asm volatile ("li x3, 217868718;"
	              "li x8, 4;"
	              ".word 0xc3e1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[90]) : "x3", "x4", "x8");

	asm volatile ("li x4, 383;"
	              "li x8, 4;"
	              ".word 0xc49253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[91]) : "x4", "x7", "x8");

	asm volatile ("li x7, 8999;"
	              "li x8, 4;"
	              ".word 0xc2e3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[92]) : "x7", "x5", "x8");

	asm volatile ("li x6, 6;"
	              "li x8, 4;"
	              ".word 0xc43353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[93]) : "x6", "x7", "x8");

	asm volatile ("li x6, 6;"
	              "li x8, 4;"
	              ".word 0xc43352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[94]) : "x6", "x5", "x8");

	asm volatile ("li x3, 1991;"
	              "li x8, 4;"
	              ".word 0xc2c1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[95]) : "x3", "x6", "x8");

	asm volatile ("li x5, 0;"
	              "li x8, 4;"
	              ".word 0xc012d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[96]) : "x5", "x3", "x8");

	asm volatile ("li x5, 214302;"
	              "li x8, 4;"
	              ".word 0xc322d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[97]) : "x5", "x3", "x8");

	asm volatile ("li x3, 72484882;"
	              "li x8, 4;"
	              ".word 0xc5b1d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[98]) : "x3", "x7", "x8");

	asm volatile ("li x4, 548147319;"
	              "li x8, 4;"
	              ".word 0xc3e252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[99]) : "x4", "x5", "x8");

	asm volatile ("li x5, 859;"
	              "li x8, 4;"
	              ".word 0xc0c2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[100]) : "x5", "x3", "x8");

	asm volatile ("li x7, 107409103;"
	              "li x8, 4;"
	              ".word 0xc3e3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[101]) : "x7", "x5", "x8");

	asm volatile ("li x5, 35419822;"
	              "li x8, 4;"
	              ".word 0xc3a2d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[102]) : "x5", "x7", "x8");

	asm volatile ("li x3, 2098;"
	              "li x8, 4;"
	              ".word 0xc4f1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[103]) : "x3", "x4", "x8");

	asm volatile ("li x3, 212148587;"
	              "li x8, 4;"
	              ".word 0xc3c1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[104]) : "x3", "x6", "x8");

	asm volatile ("li x6, 42764;"
	              "li x8, 4;"
	              ".word 0xc52351b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[105]) : "x6", "x3", "x8");

	asm volatile ("li x6, 4039;"
	              "li x8, 4;"
	              ".word 0xc10353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[106]) : "x6", "x7", "x8");

	asm volatile ("li x4, 151351629;"
	              "li x8, 4;"
	              ".word 0xc5e252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[107]) : "x4", "x5", "x8");

	asm volatile ("li x4, 209030814;"
	              "li x8, 4;"
	              ".word 0xc3e25333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[108]) : "x4", "x6", "x8");

	asm volatile ("li x6, 509553641;"
	              "li x8, 4;"
	              ".word 0xc3e35233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[109]) : "x6", "x4", "x8");

	asm volatile ("li x3, 227155;"
	              "li x8, 4;"
	              ".word 0xc141d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[110]) : "x3", "x7", "x8");

	asm volatile ("li x5, 4;"
	              "li x8, 4;"
	              ".word 0xc432d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[111]) : "x5", "x3", "x8");

	asm volatile ("li x7, 12851;"
	              "li x8, 4;"
	              ".word 0xc4f3d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[112]) : "x7", "x6", "x8");

	asm volatile ("li x5, 222;"
	              "li x8, 4;"
	              ".word 0xc492d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[113]) : "x5" , "x8");

	asm volatile ("li x7, 817119111;"
	              "li x8, 4;"
	              ".word 0xc3e3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[114]) : "x7", "x4", "x8");

	asm volatile ("li x5, 803;"
	              "li x8, 4;"
	              ".word 0xc2a2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[115]) : "x5", "x3", "x8");

	asm volatile ("li x3, 63972080;"
	              "li x8, 4;"
	              ".word 0xc3c1d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[116]) : "x3", "x4", "x8");

	asm volatile ("li x3, 515051046;"
	              "li x8, 4;"
	              ".word 0xc1d1d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[117]) : "x3", "x7", "x8");

	asm volatile ("li x4, 1230;"
	              "li x8, 4;"
	              ".word 0xc0b252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[118]) : "x4", "x5", "x8");

	asm volatile ("li x7, 59423;"
	              "li x8, 4;"
	              ".word 0xc103d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[119]) : "x7", "x6", "x8");

	asm volatile ("li x7, 964536;"
	              "li x8, 4;"
	              ".word 0xc553d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[120]) : "x7", "x6", "x8");

	asm volatile ("li x4, 4;"
	              "li x8, 4;"
	              ".word 0xc24253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[121]) : "x4", "x7", "x8");

	asm volatile ("li x7, 274767;"
	              "li x8, 4;"
	              ".word 0xc363d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[122]) : "x7", "x6", "x8");

	asm volatile ("li x3, 27;"
	              "li x8, 4;"
	              ".word 0xc261d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[123]) : "x3", "x7", "x8");

	asm volatile ("li x7, 39627040;"
	              "li x8, 4;"
	              ".word 0xc5b3d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[124]) : "x7", "x6", "x8");

	asm volatile ("li x3, 463039;"
	              "li x8, 4;"
	              ".word 0xc131d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[125]) : "x3", "x5", "x8");

	asm volatile ("li x5, 418;"
	              "li x8, 4;"
	              ".word 0xc092d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[126]) : "x5", "x3", "x8");

	asm volatile ("li x4, 3;"
	              "li x8, 4;"
	              ".word 0xc02253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[127]) : "x4", "x7", "x8");

	asm volatile ("li x4, 23;"
	              "li x8, 4;"
	              ".word 0xc4625233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[128]) : "x4" , "x8");

	asm volatile ("li x7, 4662;"
	              "li x8, 4;"
	              ".word 0xc0d3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[129]) : "x7", "x5", "x8");

	asm volatile ("li x4, 1;"
	              "li x8, 4;"
	              ".word 0xc04253b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[130]) : "x4", "x7", "x8");

	asm volatile ("li x5, 7107;"
	              "li x8, 4;"
	              ".word 0xc0d2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[131]) : "x5", "x3", "x8");

	asm volatile ("li x4, 3;"
	              "li x8, 4;"
	              ".word 0xc03252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[132]) : "x4", "x5", "x8");

	asm volatile ("li x5, 24780714;"
	              "li x8, 4;"
	              ".word 0xc192d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[133]) : "x5" , "x8");

	asm volatile ("li x7, 99851;"
	              "li x8, 4;"
	              ".word 0xc343d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[134]) : "x7", "x3", "x8");

	asm volatile ("li x6, 5565;"
	              "li x8, 4;"
	              ".word 0xc2e352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[135]) : "x6", "x5", "x8");

	asm volatile ("li x5, 97;"
	              "li x8, 4;"
	              ".word 0xc2a2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[136]) : "x5" , "x8");

	asm volatile ("li x5, 28279;"
	              "li x8, 4;"
	              ".word 0xc0f2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[137]) : "x5", "x3", "x8");

	asm volatile ("li x3, 47214873;"
	              "li x8, 4;"
	              ".word 0xc3a1d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[138]) : "x3", "x7", "x8");

	asm volatile ("li x7, 4;"
	              "li x8, 4;"
	              ".word 0xc463d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[139]) : "x7", "x3", "x8");

	asm volatile ("li x5, 1981;"
	              "li x8, 4;"
	              ".word 0xc2c2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[140]) : "x5", "x3", "x8");

	asm volatile ("li x3, 5;"
	              "li x8, 4;"
	              ".word 0xc041d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[141]) : "x3", "x6", "x8");

	asm volatile ("li x3, 1;"
	              "li x8, 4;"
	              ".word 0xc011d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[142]) : "x3" , "x8");

	asm volatile ("li x6, 15;"
	              "li x8, 4;"
	              ".word 0xc46351b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[143]) : "x6", "x3", "x8");

	asm volatile ("li x3, 3083;"
	              "li x8, 4;"
	              ".word 0xc4c1d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[144]) : "x3", "x6", "x8");

	asm volatile ("li x3, 7901;"
	              "li x8, 4;"
	              ".word 0xc301d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[145]) : "x3", "x6", "x8");

	asm volatile ("li x5, 5;"
	              "li x8, 4;"
	              ".word 0xc042d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[146]) : "x5", "x3", "x8");

	asm volatile ("li x5, 27;"
	              "li x8, 4;"
	              ".word 0xc052d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[147]) : "x5" , "x8");

	asm volatile ("li x6, 91624;"
	              "li x8, 4;"
	              ".word 0xc58352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[148]) : "x6", "x5", "x8");

	asm volatile ("li x7, 419411760;"
	              "li x8, 4;"
	              ".word 0xc5e3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[149]) : "x7", "x4", "x8");

	asm volatile ("li x5, 93084;"
	              "li x8, 4;"
	              ".word 0xc522d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[150]) : "x5", "x3", "x8");

	asm volatile ("li x5, 7717;"
	              "li x8, 4;"
	              ".word 0xc4f2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[151]) : "x5", "x3", "x8");

	asm volatile ("li x5, 4055297;"
	              "li x8, 4;"
	              ".word 0xc362d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[152]) : "x5", "x4", "x8");

	asm volatile ("li x7, 4;"
	              "li x8, 4;"
	              ".word 0xc033d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[153]) : "x7" , "x8");

	asm volatile ("li x6, 11;"
	              "li x8, 4;"
	              ".word 0xc06353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[154]) : "x6", "x7", "x8");

	asm volatile ("li x7, 1399994;"
	              "li x8, 4;"
	              ".word 0xc553d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[155]) : "x7" , "x8");

	asm volatile ("li x6, 13;"
	              "li x8, 4;"
	              ".word 0xc04353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[156]) : "x6", "x7", "x8");

	asm volatile ("li x5, 119036465;"
	              "li x8, 4;"
	              ".word 0xc5b2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[157]) : "x5", "x6", "x8");

	asm volatile ("li x7, 1540978158;"
	              "li x8, 4;"
	              ".word 0xc1f3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[158]) : "x7", "x5", "x8");

	asm volatile ("li x7, 13;"
	              "li x8, 4;"
	              ".word 0xc043d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[159]) : "x7", "x5", "x8");

	asm volatile ("li x7, 2304;"
	              "li x8, 4;"
	              ".word 0xc0e3d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[160]) : "x7", "x5", "x8");

	asm volatile ("li x7, 2135053;"
	              "li x8, 4;"
	              ".word 0xc363d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[161]) : "x7" , "x8");

	asm volatile ("li x7, 1;"
	              "li x8, 4;"
	              ".word 0xc033d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[162]) : "x7" , "x8");

	asm volatile ("li x5, 41411;"
	              "li x8, 4;"
	              ".word 0xc132d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[163]) : "x5", "x6", "x8");

	asm volatile ("li x4, 19528;"
	              "li x8, 4;"
	              ".word 0xc0f252b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[164]) : "x4", "x5", "x8");

	asm volatile ("li x5, 12;"
	              "li x8, 4;"
	              ".word 0xc242d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[165]) : "x5", "x3", "x8");

	asm volatile ("li x5, 5584062;"
	              "li x8, 4;"
	              ".word 0xc1b2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[166]) : "x5", "x4", "x8");

	asm volatile ("li x5, 54400081;"
	              "li x8, 4;"
	              ".word 0xc3a2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[167]) : "x5", "x6", "x8");

	asm volatile ("li x6, 65;"
	              "li x8, 4;"
	              ".word 0xc28353b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[168]) : "x6", "x7", "x8");

	asm volatile ("li x6, 615924;"
	              "li x8, 4;"
	              ".word 0xc5535233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[169]) : "x6", "x4", "x8");

	asm volatile ("li x7, 578988765;"
	              "li x8, 4;"
	              ".word 0xc3e3d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[170]) : "x7", "x6", "x8");

	asm volatile ("li x3, 9126739;"
	              "li x8, 4;"
	              ".word 0xc581d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[171]) : "x3", "x5", "x8");

	asm volatile ("li x4, 16168;"
	              "li x8, 4;"
	              ".word 0xc4f251b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[172]) : "x4", "x3", "x8");

	asm volatile ("li x5, 229459;"
	              "li x8, 4;"
	              ".word 0xc322d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[173]) : "x5", "x6", "x8");

	asm volatile ("li x7, 27383;"
	              "li x8, 4;"
	              ".word 0xc303d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[174]) : "x7" , "x8");

	asm volatile ("li x4, 4497453;"
	              "li x8, 4;"
	              ".word 0xc3825333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[175]) : "x4", "x6", "x8");

	asm volatile ("li x6, 66626;"
	              "li x8, 4;"
	              ".word 0xc5535233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[176]) : "x6", "x4", "x8");

	asm volatile ("li x7, 90718;"
	              "li x8, 4;"
	              ".word 0xc323d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[177]) : "x7", "x3", "x8");

	asm volatile ("li x5, 3540007;"
	              "li x8, 4;"
	              ".word 0xc3e2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[178]) : "x5" , "x8");

	asm volatile ("li x5, 1053625992;"
	              "li x8, 4;"
	              ".word 0xc5e2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[179]) : "x5", "x4", "x8");

	asm volatile ("li x3, 54;"
	              "li x8, 4;"
	              ".word 0xc261d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[180]) : "x3", "x6", "x8");

	asm volatile ("li x3, 352121082;"
	              "li x8, 4;"
	              ".word 0xc1e1d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[181]) : "x3", "x7", "x8");

	asm volatile ("li x6, 4518;"
	              "li x8, 4;"
	              ".word 0xc30352b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[182]) : "x6", "x5", "x8");

	asm volatile ("li x7, 40855;"
	              "li x8, 4;"
	              ".word 0xc303d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[183]) : "x7", "x6", "x8");

	asm volatile ("li x5, 486020218;"
	              "li x8, 4;"
	              ".word 0xc3e2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[184]) : "x5" , "x8");

	asm volatile ("li x7, 5813846;"
	              "li x8, 4;"
	              ".word 0xc583d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[185]) : "x7", "x4", "x8");

	asm volatile ("li x5, 1854255150;"
	              "li x8, 4;"
	              ".word 0xc1f2d1b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[186]) : "x5", "x3", "x8");

	asm volatile ("li x4, 23;"
	              "li x8, 4;"
	              ".word 0xc4625333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[187]) : "x4", "x6", "x8");

	asm volatile ("li x7, 133563;"
	              "li x8, 4;"
	              ".word 0xc553d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[188]) : "x7", "x4", "x8");

	asm volatile ("li x5, 177;"
	              "li x8, 4;"
	              ".word 0xc2a2d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[189]) : "x5" , "x8");

	asm volatile ("li x5, 1061543;"
	              "li x8, 4;"
	              ".word 0xc552d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[190]) : "x5", "x7", "x8");

	asm volatile ("li x6, 4109195;"
	              "li x8, 4;"
	              ".word 0xc1635333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[191]) : "x6" , "x8");

	asm volatile ("li x3, 23;"
	              "li x8, 4;"
	              ".word 0xc071d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[192]) : "x3", "x4", "x8");

	asm volatile ("li x5, 16867;"
	              "li x8, 4;"
	              ".word 0xc4f2d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[193]) : "x5", "x6", "x8");

	asm volatile ("li x5, 489378634;"
	              "li x8, 4;"
	              ".word 0xc3e2d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[194]) : "x5", "x4", "x8");

	asm volatile ("li x6, 1559;"
	              "li x8, 4;"
	              ".word 0xc4c351b3;"
	              "divu x8, x3, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[195]) : "x6", "x3", "x8");

	asm volatile ("li x5, 3;"
	              "li x8, 4;"
	              ".word 0xc222d333;"
	              "divu x8, x6, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[196]) : "x5", "x6", "x8");

	asm volatile ("li x7, 1958;"
	              "li x8, 4;"
	              ".word 0xc0b3d233;"
	              "divu x8, x4, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[197]) : "x7", "x4", "x8");

	asm volatile ("li x5, 9040525;"
	              "li x8, 4;"
	              ".word 0xc582d2b3;"
	              "divu x8, x5, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[198]) : "x5" , "x8");

	asm volatile ("li x7, 44030;"
	              "li x8, 4;"
	              ".word 0xc303d3b3;"
	              "divu x8, x7, x8;"
	              "sw x8, 0(%[addr]);"
	              : : [addr] "r" (&res_vec[199]) : "x7" , "x8");


for(i = 0; i < NumberOfStimuli; i++) {
    	check_uint32(result, "brev", res_vec[i],  res_brev[i]);
    	}

}

#endif
