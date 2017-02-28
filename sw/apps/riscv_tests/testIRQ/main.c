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
#include "pulpino.h"
#include "int.h"

void test_infinite_jmp_loop(testresult_t *result, void (*start)(), void (*stop)());
void test_infinite_branch_loop(testresult_t *result, void (*start)(), void (*stop)());
void test_infinite_hw_loop(testresult_t *result, void (*start)(), void (*stop)());
void test_infinite_jmp_mulh(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = " 1. test_infinite_jmp_loop",     .test = test_infinite_jmp_loop     },
  { .name = " 2. test_infinite_branch_loop",  .test = test_infinite_branch_loop  },
#if defined(GCC_ETH) && !defined(USE_ZERO_RISCY)
  { .name = " 3. test_infinite_hw_loop",      .test = test_infinite_hw_loop      },
#endif
#ifndef NO_NUM
  { .name = " 4. test_infinite_jmp_mulh",     .test = test_infinite_jmp_mulh     },
#endif
  {0, 0}
};

volatile uint32_t testcase_current = 0;
volatile uint32_t irq_trig = 0;
volatile uint32_t tb_errors = 0;
volatile int mepc_jmp;
volatile int mepc_branch;
volatile int mepc_mulh;

int main() {
  return run_suite(testcases);
}

//----------------------------------------------------------------------------
// 1. while(1)
//----------------------------------------------------------------------------
void test_infinite_jmp_loop(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0;
  testcase_current = 1;
  irq_trig = 0;

  ECP = 0xFFFFFFFF;
  IER = 1 << 29;
  int_enable();

  // enable timer and wait for 3500 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 3500;
  TPRA  = 0x1;

  asm volatile ("la x16, jmp1;"
                "sw x16, 0(%[newmepc_jmp]);"
                "lblj1: j lblj1;"
                "jmp1: addi %[a], %[a], 4;"
                : [a] "+r" (act)
                : [newmepc_jmp] "r" (&mepc_jmp)
                : "x16", "x17");

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;
  int_disable();


  check_uint32(result, "while(1) jmp",   act, 4);

  irq_trig = 0;

}
//----------------------------------------------------------------------------
// 2. while(1) with branch
//----------------------------------------------------------------------------
void test_infinite_branch_loop(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0, abba = 0;
  testcase_current = 2;
  irq_trig = 0;

  ECP = 0xFFFFFFFF;
  IER = 1 << 29;
  int_enable();

  // enable timer and wait for 1000 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 1000;
  TPRA  = 0x1;

  asm volatile ("la x16, branch1;"
                "sw x16, 0(%[newmepc_branch]);"
                "addi x16, x0, 0x10;"
                "lblb1: bne x16, x0, lblb1;"
                "branch1: addi x16, x16, 0x4;"
                "mv %[a], x16;"
                : [a] "+r" (act)
                : [newmepc_branch] "r" (&mepc_branch)
                : "x16", "x17");

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;
  int_disable();

  check_uint32(result, "while(1) branch", act, 20);

  irq_trig = 0;

}
#if defined(GCC_ETH) && !defined(USE_ZERO_RISCY)
//----------------------------------------------------------------------------
// 3. while(i < N) with hw loop
//----------------------------------------------------------------------------
void test_infinite_hw_loop(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0, abba = 0;
  testcase_current = 3;
  irq_trig = 0;

  ECP = 0xFFFFFFFF;
  IER = 1 << 29;
  int_enable();

  // enable timer and wait for 1000 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 1000;
  TPRA  = 0x1;

  asm volatile ("lp.counti x1, %[N];"
                "add %[a], x0, x0;"
                "lp.starti x1, lbl_start_hw1;"
                "lp.endi   x1, lbl_end_hw1;"
                "lbl_start_hw1: addi %[a], %[a], 1;"
                "lbl_end_hw1: nop;"
                : [a] "+r" (act)
                : [N] "i"  (0xFFF));

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;
  int_disable();

  check_uint32(result, "while(1) hw loop",   act, 0xFFF);

  irq_trig = 0;

}
#endif
#ifndef NO_NUM
//----------------------------------------------------------------------------
// 4. while(1) mulh
//----------------------------------------------------------------------------
void test_infinite_jmp_mulh(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0;
  volatile int32_t res_mulh = 0;
  testcase_current = 4;
  irq_trig = 0;

  ECP = 0xFFFFFFFF;
  IER = 1 << 29;
  int_enable();

  // enable timer and wait for 3500 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 3500;
  TPRA  = 0x1;

  asm volatile ("la x16, jmp2;"
                "sw x16, 0(%[newmepc_mulh]);"
                "lbl_mulh: mulh %[res], %[m], %[m];"
                "j lbl_mulh;"
                "jmp2: addi %[a], %[a], 4;"
                : [a] "+r" (act),
                  [res] "+r" (res_mulh)
                : [newmepc_mulh] "r" (&mepc_mulh),
                  [m] "r" (0x000FFFFF)
                : "x16", "x17");

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;
  int_disable();

  check_uint32(result, "mulh result",   res_mulh, 0x000000FF);
  check_uint32(result, "while(1) mulh",  act, 4);

  irq_trig = 0;

}
#endif
void ISR_TA_CMP(void) {
  ICP = (1 << 29);
  int dest_mepc, source_mepc;
  source_mepc = 0;
  irq_trig++;
  if(irq_trig > 10) {
    asm volatile ("csrrw %[d], mepc, %[s]"
      : [d] "=r" (dest_mepc)
      : [s] "r"  (source_mepc));
    if(testcase_current == 1)
      dest_mepc = mepc_jmp; //jump
    else if (testcase_current == 2)
      dest_mepc = mepc_branch; //branch
    else if (testcase_current == 4)
      dest_mepc = mepc_mulh; //mulh
    asm volatile ("csrrw %[d], mepc, %[s]"
      : [d] "=r" (source_mepc)
      : [s] "r"  (dest_mepc));
  }
  // re-arm timer
  TIRA = 0x0;

}