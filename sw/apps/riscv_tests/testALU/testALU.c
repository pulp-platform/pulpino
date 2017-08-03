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
// add
// addi
// sub
// and
// andi
// or
// ori
// xor
// xori
// sll
// slli
// sra
// srai
// srl
// srli


#include <stdio.h>
#include "utils.h"
#include "bench.h"

#include "testALU_stimuli.h"

void check_add(testresult_t *result, void (*start)(), void (*stop)());
void check_sub(testresult_t *result, void (*start)(), void (*stop)());
void check_logic(testresult_t *result, void (*start)(), void (*stop)());
void check_logic_imm(testresult_t *result, void (*start)(), void (*stop)());
void check_shifts(testresult_t *result, void (*start)(), void (*stop)());
void check_shifts_imm(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "add",          .test = check_add          },
  { .name = "sub",          .test = check_sub          },
  { .name = "logic",        .test = check_logic        },
  { .name = "logic_imm",    .test = check_logic_imm    },
  { .name = "shifts",       .test = check_shifts       },
  { .name = "shifts_imm",   .test = check_shifts_imm   },
  {0, 0}
};

int main()
{
  if(get_core_id() == 0) {
    return run_suite(testcases);
  }

  return 0;
}

void check_add(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check add
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_add_act)/4); i++) {
    asm volatile ("add %[c], %[a], %[b]\n"
                  : [c] "+r" (g_add_act[i])
                  : [a] "r"  (g_add_a[i]), [b] "r" (g_add_b[i]));

    check_uint32(result, "add", g_add_act[i],  g_add_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check addi
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_addi_act)/4); i++) {
    asm volatile ("addi %[c], %[a], 100\n"
                  : [c] "+r" (g_addi_act[i])
                  : [a] "r"  (g_addi_a[i]));

    check_uint32(result, "addi", g_addi_act[i],  g_addi_exp[i]);
  }
}

void check_sub(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check sub
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_sub_act)/4); i++) {
    asm volatile ("sub %[c], %[a], %[b]\n"
                  : [c] "+r" (g_sub_act[i])
                  : [a] "r"  (g_sub_a[i]), [b] "r" (g_sub_b[i]));

    check_uint32(result, "sub", g_sub_act[i],  g_sub_exp[i]);
  }
}


void check_logic(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check and
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_and_act)/4); i++) {
    asm volatile ("and %[c], %[a], %[b]\n"
                  : [c] "+r" (g_and_act[i])
                  : [a] "r"  (g_and_a[i]), [b] "r" (g_and_b[i]));

    check_uint32(result, "and", g_and_act[i],  g_and_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check or
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_or_act)/4); i++) {
    asm volatile ("or %[c], %[a], %[b]\n"
                  : [c] "+r" (g_or_act[i])
                  : [a] "r"  (g_or_a[i]), [b] "r" (g_or_b[i]));

    check_uint32(result, "or", g_or_act[i],  g_or_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check xor
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_xor_act)/4); i++) {
    asm volatile ("xor %[c], %[a], %[b]\n"
                  : [c] "+r" (g_xor_act[i])
                  : [a] "r"  (g_xor_a[i]), [b] "r" (g_xor_b[i]));

    check_uint32(result, "xor", g_xor_act[i],  g_xor_exp[i]);
  }
}

void check_logic_imm(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  //-----------------------------------------------------------------
  // Check andi
  //-----------------------------------------------------------------
  asm volatile ("andi %[c], %[a], 0x06D6\n"
                : [c] "=r" (act)
                : [a] "r"  (0xABCDABCD));

  check_uint32(result, "andi", act,  0x000002C4);

  //-----------------------------------------------------------------
  // Check ori
  //-----------------------------------------------------------------
  asm volatile ("ori %[c], %[a], 0x06D6\n"
                : [c] "=r" (act)
                : [a] "r"  (0xABCD0142));

  check_uint32(result, "ori", act,  0xABCD07D6);

  //-----------------------------------------------------------------
  // Check xori
  //-----------------------------------------------------------------
  asm volatile ("xori %[c], %[a], 0x0624\n"
                : [c] "=r" (act)
                : [a] "r"  (0x01020304));

  check_uint32(result, "xori", act,  0x01020520);
}

void check_shifts(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  uint32_t act;

  //-----------------------------------------------------------------
  // Check sll
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_sll_act)/4); i++) {
    asm volatile ("sll %[c], %[a], %[b]\n"
                  : [c] "+r" (g_sll_act[i])
                  : [a] "r"  (g_sll_a[i]), [b] "r" (g_sll_b[i]));

    check_uint32(result, "sll", g_sll_act[i],  g_sll_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check srl
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_srl_act)/4); i++) {
    asm volatile ("srl %[c], %[a], %[b]\n"
                  : [c] "+r" (g_srl_act[i])
                  : [a] "r"  (g_srl_a[i]), [b] "r" (g_srl_b[i]));

    check_uint32(result, "srl", g_srl_act[i],  g_srl_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check sra
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_sra_act)/4); i++) {
    asm volatile ("sra %[c], %[a], %[b]\n"
                  : [c] "+r" (g_sra_act[i])
                  : [a] "r"  (g_sra_a[i]), [b] "r" (g_sra_b[i]));

    check_uint32(result, "sra", g_sra_act[i],  g_sra_exp[i]);
  }
}

void check_shifts_imm(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check slli
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_slli_act)/4); i++) {
    asm volatile ("slli %[c], %[a], 10\n"
                  : [c] "+r" (g_slli_act[i])
                  : [a] "r"  (g_slli_a[i]));

    check_uint32(result, "slli", g_slli_act[i],  g_slli_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check srli
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_srli_act)/4); i++) {
    asm volatile ("srli %[c], %[a], 9\n"
                  : [c] "+r" (g_srli_act[i])
                  : [a] "r"  (g_srli_a[i]));

    check_uint32(result, "srli", g_srli_act[i],  g_srli_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check srai
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_srai_act)/4); i++) {
    asm volatile ("srai %[c], %[a], 21\n"
                  : [c] "+r" (g_srai_act[i])
                  : [a] "r"  (g_srai_a[i]));

    check_uint32(result, "srai", g_srai_act[i],  g_srai_exp[i]);
  }
}
