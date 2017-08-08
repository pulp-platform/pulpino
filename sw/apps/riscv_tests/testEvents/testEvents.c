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
#include "pulpino.h"
#include "bench.h"
#include "int.h"

#define SR_OVERFLOW     0x800
#define SR_CARRY        0x400

#define IT_ADD             10
#define HWLOOP_IT          10

void check_flush(testresult_t *result, void (*start)(), void (*stop)());
void check_sleep_irq(testresult_t *result, void (*start)(), void (*stop)());

volatile unsigned int killme=0;
volatile unsigned int global_counter_branch=0;
testcase_t testcases[] = {
  { .name = "flush",        .test = check_flush        },
  { .name = "sleep_irq",    .test = check_sleep_irq    },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}

void check_flush(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  int act;
  int exp;

  //-----------------------------------------------------------------
  // a very long one
  //-----------------------------------------------------------------
  act = 0;
  exp = (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10);
  asm volatile ("addi %[act], %[act], 1;"
                "addi %[act], %[act], 2;"
                "addi %[act], %[act], 3;"
                "addi %[act], %[act], 4;"
                "addi %[act], %[act], 5;"
                "wfi;"
                "addi %[act], %[act], 6;"
                "addi %[act], %[act], 7;"
                "addi %[act], %[act], 8;"
                "addi %[act], %[act], 9;"
                "addi %[act], %[act], 10;"
                : [act] "+r" (act));

  if(act != exp) {
    result->errors++;
    printf("Testing long sequence with wfi: %X, expected %X\n", act, exp);
  }
}

int g_sleep_irq_global;

void check_sleep_irq(testresult_t *result, void (*start)(), void (*stop)()) {
  int act;
  int exp;

  g_sleep_irq_global = 0;
  ECP = 0xFFFFFFFF;
  IER = 1 << 29;
  int_enable();

  // enable timer and wait for 2000 cycles before triggering
  TPRA  = 0x0;
  TIRA  = 0x0;
  TOCRA = 2000;

  TPRA  = 0x1;


  act = 0;
  exp = (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10);
  asm volatile ("sw %[SLEEP_VALUE], 0(%[SLEEP_ADDR]);"
                "addi %[act], %[act], 1;"
                "addi %[act], %[act], 2;"
                "addi %[act], %[act], 3;"
                "addi %[act], %[act], 4;"
                "addi %[act], %[act], 5;"
                "wfi;"
                "addi %[act], %[act], 6;"
                "addi %[act], %[act], 7;"
                "addi %[act], %[act], 8;"
                "addi %[act], %[act], 9;"
                "addi %[act], %[act], 10;"
                : [act] "+r" (act)
                : [SLEEP_VALUE] "r" (0x1),
                  [SLEEP_ADDR] "r"  (EVENT_UNIT_BASE_ADDR + REG_SLEEP_CTRL));

  // disable timer
  TPRA = 0x0;

  if(act != exp) {
    result->errors++;
    printf("Timer with wfi: %X, expected %X\n", act, exp);
  }

  if(g_sleep_irq_global != 2) {
    result->errors++;
    printf("Timer Interrupt was not triggered at the correct time\n");
  }

  if((EPR & 0x1) != 0x1) {
    result->errors++;
    printf("Event was not triggered\n");
  }

  ECP = 0x1;
  int_disable();
}

void ISR_TA_CMP(void) {
  ICP = (1 << 29);

  global_counter_branch++;
  switch (g_sleep_irq_global) {
    case 0:
      g_sleep_irq_global = 1;

      // re-arm timer
      TIRA = 0x0;
      break;

    case 1:
      g_sleep_irq_global = 2;
      ESP = 0x1;
      break;
  }
}
