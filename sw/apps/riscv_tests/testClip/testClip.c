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

#include <stdio.h>
#include "utils.h"
#include "bench.h"
#include "testClip_stimuli.h"

#ifdef __riscv
#define CLIP   "p.clip"
#define CLIPU  "p.clipu"
#define CLIPR  "p.clipr"
#define CLIPUR "p.clipur"
#else
#define CLIP   "l.clip"
#define CLIPU  "l.clipu"
#endif

void check_clip       (testresult_t *result, void (*start)(), void (*stop)());
void check_clipu      (testresult_t *result, void (*start)(), void (*stop)());
#ifdef __riscv
void check_clip_reg   (testresult_t *result, void (*start)(), void (*stop)());
void check_clipu_reg  (testresult_t *result, void (*start)(), void (*stop)());
#endif

testcase_t testcases[] = {
  { .name = "clip"        , .test = check_clip       },
  { .name = "clipu"       , .test = check_clipu      },
#ifdef __riscv
  { .name = "clipr"       , .test = check_clip_reg   },
  { .name = "clipur"      , .test = check_clipu_reg  },
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
//# T E S T    CLIP
//################################################################################

void check_clip(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.clip
  //-----------------------------------------------------------------
  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[0]), [I] "i" (CLIP_IMM_0));

  check_uint32(result, "clip", res,  res_clip[0]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[1]), [I] "i" (CLIP_IMM_1));

  check_uint32(result, "clip", res,  res_clip[1]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[2]), [I] "i" (CLIP_IMM_2));

  check_uint32(result, "clip", res,  res_clip[2]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[3]), [I] "i" (CLIP_IMM_3));

  check_uint32(result, "clip", res,  res_clip[3]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[4]), [I] "i" (CLIP_IMM_4));

  check_uint32(result, "clip", res,  res_clip[4]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[5]), [I] "i" (CLIP_IMM_5));

  check_uint32(result, "clip", res,  res_clip[5]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[6]), [I] "i" (CLIP_IMM_6));

  check_uint32(result, "clip", res,  res_clip[6]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[7]), [I] "i" (CLIP_IMM_7));

  check_uint32(result, "clip", res,  res_clip[7]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[8]), [I] "i" (CLIP_IMM_8));

  check_uint32(result, "clip", res,  res_clip[8]);

  asm volatile (CLIP " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clip[9]), [I] "i" (CLIP_IMM_9));

  check_uint32(result, "clip", res,  res_clip[9]);
}

void check_clipu(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.clip
  //-----------------------------------------------------------------
  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[0]), [I] "i" (CLIP_IMM_0));

  check_uint32(result, "clipu", res,  res_clipu[0]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[1]), [I] "i" (CLIP_IMM_1));

  check_uint32(result, "clipu", res,  res_clipu[1]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[2]), [I] "i" (CLIP_IMM_2));

  check_uint32(result, "clipu", res,  res_clipu[2]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[3]), [I] "i" (CLIP_IMM_3));

  check_uint32(result, "clipu", res,  res_clipu[3]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[4]), [I] "i" (CLIP_IMM_4));

  check_uint32(result, "clipu", res,  res_clipu[4]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[5]), [I] "i" (CLIP_IMM_5));

  check_uint32(result, "clipu", res,  res_clipu[5]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[6]), [I] "i" (CLIP_IMM_6));

  check_uint32(result, "clipu", res,  res_clipu[6]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[7]), [I] "i" (CLIP_IMM_7));

  check_uint32(result, "clipu", res,  res_clipu[7]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[8]), [I] "i" (CLIP_IMM_8));

  check_uint32(result, "clipu", res,  res_clipu[8]);

  asm volatile (CLIPU " %[c], %[a],%[I]\n"
      : [c] "=r" (res)
      : [a] "r"  (op_a_clipu[9]), [I] "i" (CLIP_IMM_9));

  check_uint32(result, "clipu", res,  res_clipu[9]);
}

#ifdef __riscv
void check_clip_reg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.clipr
  //-----------------------------------------------------------------
  for(i=0;i<NumberOfStimuli;i++) {
    asm volatile (CLIPR " %[c], %[a],%[b]\n"
        : [c] "=r" (res)
        : [a] "r"  (op_a_clip_reg[i]), [b] "r" (op_b_clip_reg[i]));

    check_uint32(result, "clipr", res,  res_clip_reg[i]);
  }
}
void check_clipu_reg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check p.clipur
  //-----------------------------------------------------------------
  for(i=0;i<NumberOfStimuli;i++) {
    asm volatile (CLIPUR " %[c], %[a],%[b]\n"
        : [c] "=r" (res)
        : [a] "r"  (op_a_clipu_reg[i]), [b] "r" (op_b_clipu_reg[i]));

    check_uint32(result, "clipr", res,  res_clipu_reg[i]);
  }
}

#endif