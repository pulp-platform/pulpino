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

void test_init(testresult_t *result, void (*start)(), void (*stop)());
void test_finish(testresult_t *result, void (*start)(), void (*stop)());

void test_infinite_loop(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "init",                           .test = test_init                  },
  { .name = " 2. test_infinite_loop",         .test = test_infinite_loop         },
  { .name = "finish",                         .test = test_finish                },
  {0, 0}
};

volatile uint32_t testcase_current = 0;
volatile uint32_t irq_trig = 0;
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
                "mv x18, %[irq_trig];"
                "ebreak" :: [current] "r" (&testcase_current), [tb_errors] "r" (&tb_errors), [irq_trig] "r" (&irq_trig) : "x16", "x17", "x18");
}

void test_finish(testresult_t *result, void (*start)(), void (*stop)()) {
  testcase_current = 0xFFFFFFFF;
  asm volatile ("ebreak");

  result->errors += tb_errors;
}

//----------------------------------------------------------------------------
// 2. while(1)
//----------------------------------------------------------------------------
void test_infinite_loop(testresult_t *result, void (*start)(), void (*stop)()) {

  volatile uint32_t act = 0, abba = 0;
  testcase_current = 2;
  irq_trig = 0;

  ECP = 0xFFFFFFFF;
  IER = 1 << 29;
  int_enable();

  // enable timer and wait for 2000 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 2000;
  TPRA  = 0x1;

  printf("Running while(1) test\n");

  asm volatile ("la x16, jmp1;"
                "la x17, lblj1;"
                "ebreak;"
                "nop;"
                "lblj1: j lblj1;"
                "jmp1: addi %[a], %[a], 4;"
                : [a] "+r" (act)
                :: "x16", "x17");

  // disable timer
  TPRA = 0x0;
  ECP = 0x1;
  int_disable();

  // now read back some values written by the debug system
  asm volatile ("ebreak;"
                "mv %[a], x18;"
                : [a] "=r" (abba) :: "x18");

  check_uint32(result, "while(1)",   act, 4);
  check_uint32(result, "while(1)",  abba, 0xABBAABBA);

  irq_trig = 0;

}

void ISR_TA_CMP(void) {
  ICP = (1 << 29);

  irq_trig++;
  // re-arm timer
  TIRA = 0x0;

  //let the debugger read irq_trig
  asm volatile ("ebreak;");


}