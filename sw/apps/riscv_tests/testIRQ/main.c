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
#include "bench.h"
#include "pulpino.h"
#include "int.h"

void test_infinite_jmp_loop(testresult_t *result, void (*start)(), void (*stop)());
void test_infinite_branch_loop(testresult_t *result, void (*start)(), void (*stop)());
void test_interrupt_loop(testresult_t *result, void (*start)(), void (*stop)());
void test_hazard_interrupt(testresult_t *result, void (*start)(), void (*stop)());
void test_infinite_jmp_mulh(testresult_t *result, void (*start)(), void (*stop)());
void test_infinite_hw_loop(testresult_t *result, void (*start)(), void (*stop)());

void __attribute__ ((noinline)) isr_infinite_jmp_loop();
void __attribute__ ((noinline)) isr_infinite_branch_loop();
void __attribute__ ((noinline)) isr_interrupt_loop();
void __attribute__ ((noinline)) isr_hazard_interrupt();
void __attribute__ ((noinline)) isr_infinite_jmp_mulh();
void __attribute__ ((noinline)) isr_infinite_hw_loop();


#define change_mepc(dest_mepc)     asm volatile ("csrrw x0, mepc, %[s]" :  : [s] "r"  (dest_mepc))
#define disable_mpie()             asm volatile ("csrrw x0, mstatus, %[s]": : [s] "r" (0x1800))
#define get_mstatus(mstatus)       asm volatile ("csrrs %[d], mstatus, x0" : [d] "=r" (mstatus))

testcase_t testcases[] = {
  { .name = " 1. test_infinite_jmp_loop",     .test = test_infinite_jmp_loop     },
  { .name = " 2. test_infinite_branch_loop",  .test = test_infinite_branch_loop  },
  { .name = " 3. test_interrupt_loop",        .test = test_interrupt_loop        },
  { .name = " 4. test_hazard_interrupt",      .test = test_hazard_interrupt      },
#ifdef USE_MUL
  { .name = " 5. test_infinite_jmp_mulh",     .test = test_infinite_jmp_mulh     },
#endif
#ifdef PULP_EXT
  { .name = " 6. test_infinite_hw_loop",      .test = test_infinite_hw_loop      },
#endif
  {0, 0}
};

volatile uint32_t testcase_current = 0;
volatile uint32_t irq_trig = 0;
volatile uint32_t irq_hazard = 0;
volatile uint32_t tb_errors = 0;
volatile int mepc_jmp;
volatile int mepc_branch;
volatile int mepc_loop;
volatile int mepc_mulh;
volatile int mepc_hazard;

int main() {
  return run_suite(testcases);
}

//----------------------------------------------------------------------------
// 1. while(1)
//----------------------------------------------------------------------------
void test_infinite_jmp_loop(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0;
  volatile uint32_t label = 0;
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

  asm volatile ("la %[lbl], jmp1;"
                "sw %[lbl], 0(%[newmepc_jmp]);"
                "lblj1: j lblj1;"
                "jmp1: addi %[a], %[a], 4;"
                : [a] "+r" (act),
                  [lbl] "+r" (label)
                : [newmepc_jmp] "r" (&mepc_jmp)
                );

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;
  int_disable();

  check_uint32(result, "while(1) jmp",   irq_trig > 10, 1);
  check_uint32(result, "while(1) jmp",   act, 4);

  irq_trig = 0;

}
//----------------------------------------------------------------------------
// 2. while(1) with branch
//----------------------------------------------------------------------------
void test_infinite_branch_loop(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0;
  volatile uint32_t label = 0;
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

  asm volatile ("la %[lbl], branch1;"
                "sw %[lbl], 0(%[newmepc_branch]);"
                "addi %[lbl], x0, 0x10;"
                "lblb1: bne %[lbl], x0, lblb1;"
                "branch1: addi %[lbl], %[lbl], 0x4;"
                "mv %[a], %[lbl];"
                : [a] "+r" (act),
                  [lbl] "+r" (label)
                : [newmepc_branch] "r" (&mepc_branch)
                );

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;
  int_disable();
  check_uint32(result, "while(1) jmp",   irq_trig > 10, 1);
  check_uint32(result, "while(1) branch", act, 20);

  irq_trig = 0;

}
//----------------------------------------------------------------------------
// 3. for loop interrupted
//----------------------------------------------------------------------------
void test_interrupt_loop(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0;
  volatile uint32_t label = 0;
  volatile uint32_t cnt = 0;
  volatile uint32_t ctr = 0;
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

  asm volatile ("la %[lbl], after_loop;"
                "sw %[lbl], 0(%[newmepc_loop]);"
                "mv  %[act], x0;"
                "mv  %[ctr], x0;"
                "for_loop: addi %[act], %[act], 0x01;"
                "addi %[ctr], %[ctr], 0x01;"
                "beq %[ctr], %[act], for_loop;"
                "after_loop: csrrw x0, mstatus, %[m];"
                : [act] "+r" (act),
                  [ctr] "+r" (ctr),
                  [lbl] "+r" (label)
                : [newmepc_loop] "r" (&mepc_loop),
                  [m] "r" ((0x1800))
                );

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;

  check_uint32(result, "interrupt loop", irq_trig, 10);

  irq_trig = 0;

}


//----------------------------------------------------------------------------
// 4. hazard interrupted
//----------------------------------------------------------------------------
void test_hazard_interrupt(testresult_t *result, void (*start)(), void (*stop)()) {

  testcase_current = 4;
    volatile uint32_t label = 0;
  irq_trig = 0;

  ECP = 0xFFFFFFFF;
  IER = 1 << 29;

  // enable timer and wait for 10 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 40;
  TPRA  = 0x1;

  asm volatile ("la %[lbl], after_jump;"
                "sw %[lbl], 0(%[newmepc_hazard]);"
                "haz_loop: csrrw x0, mstatus, %[dis];"
                "csrrw x0, mstatus, %[en];"
                "j haz_loop;"
                "after_jump: nop;"
                : [lbl] "+r" (label)
                : [newmepc_hazard] "r" (&mepc_hazard),
                  [dis]            "r" (0x1800),
                  [en]             "r" (0x1808)
                );

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;

  check_uint32(result, "interrupt hazard", irq_trig, 30);
  check_uint32(result, "interrupt hazard", irq_hazard, 0);

  irq_trig = 0;

}

#ifdef USE_MUL
//----------------------------------------------------------------------------
// 5. while(1) mulh
//----------------------------------------------------------------------------
void test_infinite_jmp_mulh(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0;
  volatile uint32_t label = 0;
  volatile int32_t res_mulh = 0;
  testcase_current = 5;
  irq_trig = 0;

  ECP = 0xFFFFFFFF;
  IER = 1 << 29;
  int_enable();

  // enable timer and wait for 3500 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 3500;
  TPRA  = 0x1;

  asm volatile ("la %[lbl], jmp2;"
                "sw %[lbl], 0(%[newmepc_mulh]);"
                "lbl_mulh: mulh %[res], %[m], %[m];"
                "j lbl_mulh;"
                "jmp2: addi %[a], %[a], 4;"
                : [a] "+r" (act),
                  [res] "+r" (res_mulh),
                  [lbl] "+r" (label)
                : [newmepc_mulh] "r" (&mepc_mulh),
                  [m] "r" (0x000FFFFF)
                );

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;
  int_disable();

  check_uint32(result, "mulh result",   res_mulh, 0x000000FF);
  check_uint32(result, "while(1) mulh",  act, 4);

  irq_trig = 0;

}
#endif

#ifdef PULP_EXT
//----------------------------------------------------------------------------
// 6. while(i < N) with hw loop
//----------------------------------------------------------------------------
void test_infinite_hw_loop(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0;
  testcase_current = 6;
  irq_trig = 0;

  ECP = 0xFFFFFFFF;
  IER = 1 << 29;
  int_enable();

  // enable timer and wait for 1000 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 999;
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


void ISR_TA_CMP(void) {

  ICP = (1 << 29);

  switch (testcase_current) {
  case 1:
     isr_infinite_jmp_loop();
     break;
  case 2:
     isr_infinite_branch_loop();
     break;
  case 3:
     isr_interrupt_loop();
     break;
  case 4:
     isr_hazard_interrupt();
     break;
  case 5:
     isr_infinite_jmp_mulh();
     break;
  case 6:
     isr_infinite_hw_loop();
     break;
  default:
     exit(1);
     break;
  }
}

/*
  -- ISR 1
*/

void __attribute__ ((noinline)) isr_infinite_jmp_loop()
{
  int dest_mepc;
  irq_trig++;

  if(irq_trig > 10) {
    dest_mepc = mepc_jmp;
    change_mepc(dest_mepc);
  }
}

/*
  -- ISR 2
*/

void __attribute__ ((noinline)) isr_infinite_branch_loop()
{
  int dest_mepc;
  irq_trig++;

  if(irq_trig > 10) {
    dest_mepc = mepc_branch;
    change_mepc(dest_mepc);
  }
}

/*
  -- ISR 3
*/

void __attribute__ ((noinline)) isr_interrupt_loop()
{
  int dest_mepc;
  irq_trig++;

  if(irq_trig == 10) {
    dest_mepc = mepc_loop;
    change_mepc(dest_mepc);
    disable_mpie();
  }

  TIRA = 0x0;
  TOCRA = TOCRA + 999*irq_trig;
}

/*
  -- ISR 4
*/

void __attribute__ ((noinline)) isr_hazard_interrupt()
{
  int dest_mepc;
  volatile int mstatus;
  irq_trig++;

  // disable timer
  TPRA = 0x0;
  ECP  = 0x1;

  get_mstatus(mstatus);
  //check right contex
  if(mstatus!=0x1880)
    irq_hazard++;

  if(irq_trig == 30) {
    dest_mepc = mepc_hazard;
    change_mepc(dest_mepc);
    disable_mpie();
  }
  // re-arm timer
  TIRA  = 0x0;
  TOCRA = TOCRA + 1;
  TPRA  = 0x1;

}

/*
  -- ISR 5
*/

void __attribute__ ((noinline)) isr_infinite_jmp_mulh()
{
  int dest_mepc;
  irq_trig++;

  if(irq_trig > 10) {
    dest_mepc = mepc_mulh;
    change_mepc(dest_mepc);
  }
}

/*
  -- ISR 6
*/

void __attribute__ ((noinline)) isr_infinite_hw_loop()
{
}
