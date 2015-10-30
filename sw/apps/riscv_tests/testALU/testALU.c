// This testbench checks the basic functionality of:
//
// add
// addi
// sub
// and
// andi   1 test
// or
// ori    1 test
// xor
// xori   1 test
// p.ror
// sll
// slli
// sra
// srai
// srl
// srli
//
// (p.cmov)


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
//void check_cmov(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "add",          .test = check_add          },
  { .name = "sub",          .test = check_sub          },
  { .name = "logic",        .test = check_logic        },
  { .name = "logic_imm",    .test = check_logic_imm    },
  { .name = "shifts",       .test = check_shifts       },
  { .name = "shifts_imm",   .test = check_shifts_imm   },
  //{ .name = "cmov",         .test = check_cmov         },
  {0, 0}
};

int main()
{
  if(get_core_id() == 0) {
    run_suite(testcases);
  }

  synch_barrier();

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

  //-----------------------------------------------------------------
  // Check p.ror
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ror_act)/4); i++) {
    asm volatile ("p.ror %[c], %[a], %[b]\n"
                  : [c] "+r" (g_ror_act[i])
                  : [a] "r"  (g_ror_a[i]), [b] "r" (g_ror_b[i]));

    check_uint32(result, "p.ror", g_ror_act[i],  g_ror_exp[i]);
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

/*

void check_cmov(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Check l.cmov
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_cmov_act)/4); i++) {
    asm volatile ("l.sfgtu      %[a], %[b]\n"
                  "l.cmov %[c], %[a], %[b]\n"
                  : [c] "+r" (g_cmov_act[i])
                  : [a] "r"  (g_cmov_a[i]), [b] "r" (g_cmov_b[i]));

    check_uint32(result, "l.cmov", g_cmov_act[i],  g_cmov_exp[i]);
  }
}

*/

/*

void check_setflags(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;

  unsigned int act;
  unsigned int res_ok;
  unsigned int res_bad;

  res_ok  = 0x1337;
  res_bad = 0xDEAD;

  //-----------------------------------------------------------------
  // Check l.sfltu
  //-----------------------------------------------------------------
  act = 0x0;
  for(i = 0; i < (sizeof(g_sfltu_exp)/4); i++) {
    asm volatile ("l.sfltu      %[a], %[b]\n"
                  "l.cmov %[c], %[d0], %[d1]\n"
                  : [c]  "+r" (act)
                  : [d0] "r"  (res_ok),
                    [d1] "r"  (res_bad),
                    [a]  "r"  (g_sfltu_a[i]),
                    [b]  "r"  (g_sfltu_b[i]));

    check_uint32(result, "l.sfltu", act, g_sfltu_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check l.sfleu
  //-----------------------------------------------------------------
  act = 0x0;
  for(i = 0; i < (sizeof(g_sfleu_exp)/4); i++) {
    asm volatile ("l.sfleu      %[a], %[b]\n"
                  "l.cmov %[c], %[d0], %[d1]\n"
                  : [c]  "+r" (act)
                  : [d0] "r"  (res_ok),
                    [d1] "r"  (res_bad),
                    [a]  "r"  (g_sfleu_a[i]),
                    [b]  "r"  (g_sfleu_b[i]));

    check_uint32(result, "l.sfleu", act, g_sfleu_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check l.sflts
  //-----------------------------------------------------------------
  act = 0x0;
  for(i = 0; i < (sizeof(g_sflts_exp)/4); i++) {
    asm volatile ("l.sflts      %[a], %[b]\n"
                  "l.cmov %[c], %[d0], %[d1]\n"
                  : [c]  "+r" (act)
                  : [d0] "r"  (res_ok),
                    [d1] "r"  (res_bad),
                    [a]  "r"  (g_sflts_a[i]),
                    [b]  "r"  (g_sflts_b[i]));

    check_uint32(result, "l.sflts", act, g_sflts_exp[i]);
  }

  //-----------------------------------------------------------------
  // Check l.sfles
  //-----------------------------------------------------------------
  act = 0x0;
  for(i = 0; i < (sizeof(g_sfles_exp)/4); i++) {
    asm volatile ("l.sfles      %[a], %[b]\n"
                  "l.cmov %[c], %[d0], %[d1]\n"
                  : [c]  "+r" (act)
                  : [d0] "r"  (res_ok),
                    [d1] "r"  (res_bad),
                    [a]  "r"  (g_sfles_a[i]),
                    [b]  "r"  (g_sfles_b[i]));

    check_uint32(result, "l.sfles", act, g_sfles_exp[i]);
  }
}

/*

void check_setflags_imm(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;

  unsigned int act;
  unsigned int exp;
  unsigned int a;

  unsigned int res_ok;
  unsigned int res_bad;

  res_ok  = 0x1337;
  res_bad = 0xDEAD;

  //-----------------------------------------------------------------
  // Check l.sfltui
  //-----------------------------------------------------------------
  a   = 0x100;
  act = 0x0;
  exp = res_bad;
  asm volatile ("l.sfltui      %[a], 0x100\n"
                "l.cmov %[c], %[d0], %[d1]\n"
                : [c]  "+r" (act)
                : [d0] "r"  (res_ok),
                  [d1] "r"  (res_bad),
                  [a]  "r"  (a));

  check_uint32(result, "l.sfltui", act, exp);

  a   = 0x0FF;
  act = 0x0;
  exp = res_ok;
  asm volatile ("l.sfltui      %[a], 0x100\n"
                "l.cmov %[c], %[d0], %[d1]\n"
                : [c]  "+r" (act)
                : [d0] "r"  (res_ok),
                  [d1] "r"  (res_bad),
                  [a]  "r"  (a));

  check_uint32(result, "l.sfltui", act, exp);

  //-----------------------------------------------------------------
  // Check l.sfleui
  //-----------------------------------------------------------------
  a   = 0x100;
  act = 0x0;
  exp = res_ok;
  asm volatile ("l.sfleui      %[a], 0x100\n"
                "l.cmov %[c], %[d0], %[d1]\n"
                : [c]  "+r" (act)
                : [d0] "r"  (res_ok),
                  [d1] "r"  (res_bad),
                  [a]  "r"  (a));

  check_uint32(result, "l.sfleui", act, exp);

  a   = 0x101;
  act = 0x0;
  exp = res_bad;
  asm volatile ("l.sfleui      %[a], 0x100\n"
                "l.cmov %[c], %[d0], %[d1]\n"
                : [c]  "+r" (act)
                : [d0] "r"  (res_ok),
                  [d1] "r"  (res_bad),
                  [a]  "r"  (a));

  check_uint32(result, "l.sfleui", act, exp);

  //-----------------------------------------------------------------
  // Check l.sfltsi
  //-----------------------------------------------------------------
  a   = 0x100;
  act = 0x0;
  exp = res_bad;
  asm volatile ("l.sfltsi      %[a], 0x100\n"
                "l.cmov %[c], %[d0], %[d1]\n"
                : [c]  "+r" (act)
                : [d0] "r"  (res_ok),
                  [d1] "r"  (res_bad),
                  [a]  "r"  (a));

  check_uint32(result, "l.sfltsi", act, exp);

  a   = 0x0FF;
  act = 0x0;
  exp = res_ok;
  asm volatile ("l.sfltsi      %[a], 0x100\n"
                "l.cmov %[c], %[d0], %[d1]\n"
                : [c]  "+r" (act)
                : [d0] "r"  (res_ok),
                  [d1] "r"  (res_bad),
                  [a]  "r"  (a));

  check_uint32(result, "l.sfltsi", act, exp);
}

*/
