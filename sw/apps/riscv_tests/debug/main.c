// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdio.h>
#include "bench.h"

void test_init(testresult_t *result, void (*start)(), void (*stop)());
void test_finish(testresult_t *result, void (*start)(), void (*stop)());

void test_rw_gpr(testresult_t *result, void (*start)(), void (*stop)());
void test_rw_csr(testresult_t *result, void (*start)(), void (*stop)());
void test_rw_dbg_regs(testresult_t *result, void (*start)(), void (*stop)());
void test_halt_resume(testresult_t *result, void (*start)(), void (*stop)());
void test_ebreak(testresult_t *result, void (*start)(), void (*stop)());
void test_npc_ppc(testresult_t *result, void (*start)(), void (*stop)());
void test_illegal(testresult_t *result, void (*start)(), void (*stop)());
void test_single_step(testresult_t *result, void (*start)(), void (*stop)());
void test_jumps(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "init",                    .test = test_init            },

  { .name = " 2. test_rw_gpr",          .test = test_rw_gpr          },
  { .name = " 3. test_rw_csr",          .test = test_rw_csr          },
  { .name = " 4. test_rw_dbg_regs",     .test = test_rw_dbg_regs     },
  { .name = " 5. test_halt_resume",     .test = test_halt_resume     },
  { .name = " 6. test_ebreak",          .test = test_ebreak          },
  { .name = " 7. test_npc_ppc",         .test = test_npc_ppc         },
  { .name = " 8. test_illegal",         .test = test_illegal         },
  { .name = " 9. test_single_step",     .test = test_single_step     },
  { .name = "10. test_jumps",           .test = test_jumps           },

  { .name = "finish",                  .test = test_finish          },
  {0, 0}
};

volatile uint32_t testcase_current = 0;
volatile uint32_t tb_errors = 0;


int main() {
  return run_suite(testcases);
}

void test_init(testresult_t *result, void (*start)(), void (*stop)()) {
  //----------------------------------------------------------------------------
  // 1. Stop and tell TB about our testcase_current variable
  //----------------------------------------------------------------------------
  asm volatile ("mv x16, %[current];"
                "mv x17, %[tb_errors];"
                "ebreak" :: [current] "r" (&testcase_current), [tb_errors] "r" (&tb_errors));
}

void test_finish(testresult_t *result, void (*start)(), void (*stop)()) {
  testcase_current = 0xFFFFFFFF;
  asm volatile ("ebreak");

  result->errors += tb_errors;
}

//----------------------------------------------------------------------------
// 2. Fill GPRs with well known data
//----------------------------------------------------------------------------
void test_rw_gpr(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t a, b, c;

  testcase_current = 2;

  asm volatile ("li x5, 0x55555555;"
                "li x6, 0x12345666;"
                "li x7, 0x77;"
                "li x18, 0x88;"
                "li x31, 0x31;"
                "ebreak;"
                ::: "x5", "x6", "x7", "x18", "x31");

  // now read back some values written by the debug system
  asm volatile ("ebreak;"
                "mv %[a], x5;"
                "mv %[b], x6;"
                "mv %[c], x31;"
                : [a] "=r" (a), [b] "=r" (b), [c] "=r" (c) :: "x5", "x6", "x31");

  check_uint32(result, "x5",  a, 0x54321089);
  check_uint32(result, "x6",  b, 0x11223344);
  check_uint32(result, "x31", c, 0xFEDCBA09);
}

//----------------------------------------------------------------------------
// 3. Check CSR access
//----------------------------------------------------------------------------
void test_rw_csr(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t a, b;

  testcase_current = 3;

  asm volatile ("csrw 0x300, %[a];"
                "csrw 0x341, %[b];"
                "ebreak;" :: [a] "r" (0x1), [b] "r" (0x12345678));

  // now read back some values written by the debug system
  asm volatile ("ebreak;"
                "csrr %[a], 0x300;"
                "csrr %[b], 0x341;"
                : [a] "=r" (a), [b] "=r" (b));

  check_uint32(result, "mstatus",  a, 0x00000006);
  check_uint32(result, "mepc",     b, 0x87654321);
}

//----------------------------------------------------------------------------
// 4. Check Debug Register access
//----------------------------------------------------------------------------
void test_rw_dbg_regs(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t a, b;

  testcase_current = 4;

  asm volatile ("ebreak");
}

//----------------------------------------------------------------------------
// 5. Halt and resume core a couple of times
//----------------------------------------------------------------------------
void test_halt_resume(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t a, b;
  volatile uint32_t changeme = 1;

  testcase_current = 5;

  asm volatile ("mv x16, %[addr];"
                "ebreak" :: [addr] "r" (&changeme));

  while(changeme);

  asm volatile ("ebreak");
}

//----------------------------------------------------------------------------
// 6. ebreak, ebreak, ebreak
//
// Check if PPC and NPC work correctly
//----------------------------------------------------------------------------
void test_ebreak(testresult_t *result, void (*start)(), void (*stop)()) {
  testcase_current = 6;

  asm volatile ("nop;"
                "ebreak;"
                "ebreak;"
                "ebreak;"
                "ebreak;"
                "ebreak;"
                "ebreak;"
                "ebreak;"
                "ebreak;"
                "ebreak;"
                "ebreak;");
}

//----------------------------------------------------------------------------
// 7. NPC and PPC values
//----------------------------------------------------------------------------
void test_npc_ppc(testresult_t *result, void (*start)(), void (*stop)()) {
  testcase_current = 7;

  asm volatile ("ebreak");

  asm volatile ("la x16, before;"
                "li x17, 0x88;"
                "la x18, after;"
                "        nop;"
                "before: ecall;"
                "after:  nop"
                ::: "x16", "x17", "x18");
}

//----------------------------------------------------------------------------
// 8. Illegal Instruction Exception
//----------------------------------------------------------------------------
void test_illegal(testresult_t *result, void (*start)(), void (*stop)()) {
  testcase_current = 8;

  asm volatile ("ebreak");

  asm volatile ("la x16,     before_ill;"
                "li x17,     0x84;"
                "la x18,     after_ill;"
                "            nop;"
                "before_ill: .word 0xF0F0F0F;"
                "after_ill:  nop"
                ::: "x16", "x17", "x18");
}

//----------------------------------------------------------------------------
// 9. Single Step
//----------------------------------------------------------------------------
void test_single_step(testresult_t *result, void (*start)(), void (*stop)()) {
  volatile uint32_t changeme = 1;
  volatile uint32_t act = 0;
  testcase_current = 9;

  // check single-step with addi
  asm volatile ("ebreak;"
                "addi %[a], %[a], 1;"
                "addi %[a], %[a], 2;"
                "addi %[a], %[a], 4;"
                "addi %[a], %[a], 8;"
                : [a] "+r" (act));

  check_uint32(result, "addi single step", act, 1 + 2 + 4 + 8);

  //----------------------------------------------------------------------------
  // check tight branch loop with load
  //----------------------------------------------------------------------------
  asm volatile ("ebreak;"
                "mv x16, %[addr];"
                "ss_start: lw x17, 0(x16);"
                "          bne x0, x17, ss_start;"
                :: [addr] "r" (&changeme) : "x16", "x17");

  //----------------------------------------------------------------------------
  // check tight branch loop with only bf, so we have to jump out
  //----------------------------------------------------------------------------
  asm volatile ("ebreak;"
                "la x16, tb_jump;"
                "tb_start: beq x0, x0, tb_start;"
                "tb_jump:  nop;");

  //----------------------------------------------------------------------------
  // check tight loop with no exit, so we have to jump out
  //----------------------------------------------------------------------------
  asm volatile ("ebreak;"
                "la x16, tl_jump;"
                "tl_start: j tl_start;"
                "tl_jump:  nop;");
}

//----------------------------------------------------------------------------
// 10. Jumps with NPC setting
//----------------------------------------------------------------------------
void test_jumps(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act = 0;
  testcase_current = 10;

  // check jumps with addi
  asm volatile ("la x16, jmp1;"
                "ebreak;"
                "      addi %[a], %[a], 1;"
                "      addi %[a], %[a], 2;"
                "jmp1: addi %[a], %[a], 4;"
                "      addi %[a], %[a], 8;"
                : [a] "+r" (act)
                :: "x16");

  check_uint32(result, "jmp1", act, 4 + 8);

  // check jumps to ebreak
  asm volatile ("la x16, jmpe;"
                "ebreak;"
                "      j jmpd;"
                "      nop;"
                "jmpe: ebreak;"
                "      nop;"
                "jmpd: nop;"
                ::: "x16");
}
