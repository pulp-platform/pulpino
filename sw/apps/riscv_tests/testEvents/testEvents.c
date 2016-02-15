// Copyright 2015 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdio.h>
#include "pulp.h"
#include "events.h"
#include "events_ids.h"
#include "utils.h"
#include "bar.h"
#include "bench.h"

#define SR_OVERFLOW     0x800
#define SR_CARRY        0x400

#define IT_ADD             10
#define HWLOOP_IT          10

void check_add(testresult_t *result, void (*start)(), void (*stop)());
void check_hwloop(testresult_t *result, void (*start)(), void (*stop)());
void check_hwloop_sleep(testresult_t *result, void (*start)(), void (*stop)());
void check_flush(testresult_t *result, void (*start)(), void (*stop)());

// TODO: hwloops are not yet supported and thus commented
testcase_t testcases[] = {
  { .name = "add",          .test = check_add          },
//  { .name = "hwloop",       .test = check_hwloop       },
//  { .name = "hwloop_sleep", .test = check_hwloop_sleep },
  { .name = "flush",        .test = check_flush        },
  {0, 0}
};

int main()
{
  run_suite(testcases);


  synch_barrier();

  return 0;
}



void check_add(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i, j;

  if(get_core_id() == 0) {
    for(j = 0; j < 400; j++) asm volatile ("nop\n");

    for(i = 0; i < IT_ADD; i++) {
      for(j = 0; j < i*5+100; j++) asm volatile ("nop\n");

      trigg_GP(0xE,GP1);
    }
  } else {
    int act = 0;
    int exp;

    set_evnt_mask_low(get_core_id(), (1 << GP1) | (1 << BARRIER));

    //-----------------------------------------------------------------
    // Basic Check
    //-----------------------------------------------------------------
    exp = 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256;
    for(i = 0; i < IT_ADD; i++) {
      asm volatile ("addi %[act], x0, 1\n"
                    "addi %[act], %[act], 2\n"
                    "addi %[act], %[act], 4\n"
                    "sw %[one], 0x0(%[clkgate_addr])\n"
                    "wfi\n"
                    "sw %[mask], 0x0(%[clr_buf])\n"
                    "addi %[act], %[act], 8\n"
                    "addi %[act], %[act], 16\n"
                    "addi %[act], %[act], 32\n"
                    "addi %[act], %[act], 64\n"
                    "addi %[act], %[act], 128\n"
                    "addi %[act], %[act], 256\n"
                    : [act] "+r" (act)
                    : [clkgate_addr] "r" (CORE_CLKGATE),
                      [one]          "r" (0x1),
                      [clr_buf]      "r" (EV_BUFF_CLEAR),
                      [mask]         "r" (1 << GP1));

      if(act != exp) {
        result->errors++;
        printf("Testing add before and after events: %X, expected %X\n", act, exp);
      }
    }
  }

  synch_barrier();
}

// void check_hwloop(testresult_t *result, void (*start)(), void (*stop)()) {
//   unsigned int i, j;
// 
//   if(get_core_id() == 0) {
//     for(j = 0; j < 400; j++) asm volatile ("l.nop\n");
//     for(i = 0; i < HWLOOP_IT; i++) {
//       for(j = 0; j < i*5+100; j++) asm volatile ("l.nop\n");
// 
//       trigg_GP(0xE,GP1);
//     }
// 
//     synch_barrier();
// 
//     for(j = 0; j < 400; j++) asm volatile ("l.nop\n");
//     for(i = 0; i < HWLOOP_IT; i++) {
//       for(j = 0; j < i*5+100; j++) asm volatile ("l.nop\n");
// 
//       trigg_GP(0xE,GP1);
//     }
// 
//     synch_barrier();
// 
//     for(j = 0; j < 400; j++) asm volatile ("l.nop\n");
//     for(i = 0; i < HWLOOP_IT; i++) {
//       for(j = 0; j < i*5+100; j++) asm volatile ("l.nop\n");
// 
//       trigg_GP(0xE,GP1);
//     }
// 
//     synch_barrier();
// 
//     for(j = 0; j < 400; j++) asm volatile ("l.nop\n");
//     for(i = 0; i < HWLOOP_IT; i++) {
//       for(j = 0; j < i*5+100; j++) asm volatile ("l.nop\n");
// 
//       trigg_GP(0xE,GP1);
//     }
//   } else {
//     int act;
//     int act2;
//     int exp;
// 
//     set_evnt_mask_low(get_core_id(), (1 << GP1) | (1 << BARRIER));
// 
//     //-----------------------------------------------------------------
//     // Basic Check
//     //-----------------------------------------------------------------
//     act = 0;
//     act2 = 0;
//     exp = 10*(1 + 2 + 3 + 4 + 5 + 6);
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 36\n"
//                   "l.addi %[act], %[act], 1\n"
//                   "l.addi %[act], %[act], 2\n"
//                   "l.addi %[act], %[act], 3\n"
//                   "l.sw 0x0(%[clkgate_addr]), %[one]\n"
//                   "l.psync\n"
//                   "l.sw 0x0(%[clr_buf]), %[mask]\n"
//                   "l.addi %[act], %[act], 4\n"
//                   "l.addi %[act], %[act], 5\n"
//                   "l.addi %[act], %[act], 6\n"
//                   : [act] "+r" (act)
//                   : [clkgate_addr] "r" (CORE_CLKGATE),
//                     [one]          "r" (0x1),
//                     [clr_buf]      "r" (EV_BUFF_CLEAR),
//                     [mask]         "r" (1 << GP1));
// 
//     // check if hwloops are okay
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 24\n"
//                   "l.addi %0, %0, 1\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   "l.addi %0, %0, 4\n"
//                   "l.addi %0, %0, 5\n"
//                   "l.addi %0, %0, 6\n"
//                   : "+r" (act2));
// 
//     if(act != exp) {
//       result->errors++;
//       printf("Testing hwloop with clock gating, 3,3: %X, expected %X\n", act, exp);
//     }
// 
//     if(act != act2) {
//       result->errors++;
//       printf("Testing hwloop with clock gating vs. normal, 3,3: %X, expected %X\n", act, act2);
//     }
// 
//     synch_barrier();
//     set_evnt_mask_low(get_core_id(), (1 << GP1) | (1 << BARRIER));
// 
//     //-----------------------------------------------------------------
//     // 3 before, 2 after
//     //-----------------------------------------------------------------
//     act = 0;
//     act2 = 0;
//     exp = 10*(1 + 2 + 3 + 4 + 5);
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 32\n"
//                   "l.addi %0, %0, 1\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   "l.sw 0x0(%[clkgate_addr]), %[one]\n"
//                   "l.psync\n"
//                   "l.sw 0x0(%[clr_buf]), %[mask]\n"
//                   "l.addi %0, %0, 4\n"
//                   "l.addi %0, %0, 5\n"
//                   : [act] "+r" (act)
//                   : [clkgate_addr] "r" (CORE_CLKGATE),
//                     [one]          "r" (0x1),
//                     [clr_buf]      "r" (EV_BUFF_CLEAR),
//                     [mask]         "r" (1 << GP1));
// 
//     // check if hwloops are okay
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 20\n"
//                   "l.addi %0, %0, 1\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   "l.addi %0, %0, 4\n"
//                   "l.addi %0, %0, 5\n"
//                   : "+r" (act2));
// 
//     if(act != exp) {
//       result->errors++;
//       printf("Testing hwloop with clock gating, 3, 2: %X, expected %X\n", act, exp);
//     }
// 
//     if(act != act2) {
//       result->errors++;
//       printf("Testing hwloop with clock gating vs. normal, 3, 2: %X, expected %X\n", act, act2);
//     }
// 
//     synch_barrier();
//     set_evnt_mask_low(get_core_id(), (1 << GP1) | (1 << BARRIER));
// 
//     //-----------------------------------------------------------------
//     // 2 before, 3 after
//     //-----------------------------------------------------------------
//     act = 0;
//     act2 = 0;
//     exp = 10*(2 + 3 + 4 + 5 + 6);
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 32\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   "l.sw 0x0(%[clkgate_addr]), %[one]\n"
//                   "l.psync\n"
//                   "l.sw 0x0(%[clr_buf]), %[mask]\n"
//                   "l.addi %0, %0, 4\n"
//                   "l.addi %0, %0, 5\n"
//                   "l.addi %0, %0, 6\n"
//                   : [act] "+r" (act)
//                   : [clkgate_addr] "r" (CORE_CLKGATE),
//                     [one]          "r" (0x1),
//                     [clr_buf]      "r" (EV_BUFF_CLEAR),
//                     [mask]         "r" (1 << GP1));
// 
//     // check if hwloops are okay
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 20\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   "l.addi %0, %0, 4\n"
//                   "l.addi %0, %0, 5\n"
//                   "l.addi %0, %0, 6\n"
//                   : "+r" (act2));
// 
//     if(act != exp) {
//       result->errors++;
//       printf("Testing hwloop with clock gating, 2, 3: %X, expected %X\n", act, exp);
//     }
// 
//     if(act != act2) {
//       result->errors++;
//       printf("Testing hwloop with clock gating vs. normal, 2, 3: %X, expected %X\n", act, act2);
//     }
// 
//     synch_barrier();
//     set_evnt_mask_low(get_core_id(), (1 << GP1) | (1 << BARRIER));
// 
//     //-----------------------------------------------------------------
//     // 2 before, 2 after
//     //-----------------------------------------------------------------
//     act = 0;
//     act2 = 0;
//     exp = 10*(2 + 3 + 4 + 5);
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 28\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   "l.sw 0x0(%[clkgate_addr]), %[one]\n"
//                   "l.psync\n"
//                   "l.sw 0x0(%[clr_buf]), %[mask]\n"
//                   "l.addi %0, %0, 4\n"
//                   "l.addi %0, %0, 5\n"
//                   : [act] "+r" (act)
//                   : [clkgate_addr] "r" (CORE_CLKGATE),
//                     [one]          "r" (0x1),
//                     [clr_buf]      "r" (EV_BUFF_CLEAR),
//                     [mask]         "r" (1 << GP1));
// 
//     // check if hwloops are okay
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 16\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   "l.addi %0, %0, 4\n"
//                   "l.addi %0, %0, 5\n"
//                   : "+r" (act2));
// 
//     if(act != exp) {
//       result->errors++;
//       printf("Testing hwloop with clock gating, 2, 2: %X, expected %X\n", act, exp);
//     }
// 
//     if(act != act2) {
//       result->errors++;
//       printf("Testing hwloop with clock gating vs. normal, 2, 2: %X, expected %X\n", act, act2);
//     }
//   }
// 
//   synch_barrier();
// }


void check_flush(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  int act;
  int exp;

  // //-----------------------------------------------------------------
  // // Basic Check
  // //-----------------------------------------------------------------
  // act = 0;
  // exp = 10*(1 + 2 + 3 + 4 + 5 + 6);
  // asm volatile ("lp.counti L0, 10\n"
  //               "lp.start L0, 8\n"
  //               "lp.end L0, 28\n"
  //               "l.addi %0, %0, 1\n"
  //               "l.addi %0, %0, 2\n"
  //               "l.addi %0, %0, 3\n"
  //               "l.psync\n"
  //               "l.addi %0, %0, 4\n"
  //               "l.addi %0, %0, 5\n"
  //               "l.addi %0, %0, 6\n"
  //               : "+r" (act));

  // if(act != exp) {
  //   result->errors++;
  //   printf("Testing hwloop with l.psync, 3,3: %X, expected %X\n", act, exp);
  // }

  // //-----------------------------------------------------------------
  // // 3 before, 2 after
  // //-----------------------------------------------------------------
  // act = 0;
  // exp = 10*(1 + 2 + 3 + 4 + 5);
  // asm volatile ("lp.counti L0, 10\n"
  //               "lp.start L0, 8\n"
  //               "lp.end L0, 24\n"
  //               "l.addi %0, %0, 1\n"
  //               "l.addi %0, %0, 2\n"
  //               "l.addi %0, %0, 3\n"
  //               "l.psync\n"
  //               "l.addi %0, %0, 4\n"
  //               "l.addi %0, %0, 5\n"
  //               : "+r" (act));

  // if(act != exp) {
  //   result->errors++;
  //   printf("Testing hwloop with l.psync, 3, 2: %X, expected %X\n", act, exp);
  // }

  // //-----------------------------------------------------------------
  // // 2 before, 3 after
  // //-----------------------------------------------------------------
  // act = 0;
  // exp = 10*(2 + 3 + 4 + 5 + 6);
  // asm volatile ("lp.counti L0, 10\n"
  //               "lp.start L0, 8\n"
  //               "lp.end L0, 24\n"
  //               "l.addi %0, %0, 2\n"
  //               "l.addi %0, %0, 3\n"
  //               "l.psync\n"
  //               "l.addi %0, %0, 4\n"
  //               "l.addi %0, %0, 5\n"
  //               "l.addi %0, %0, 6\n"
  //               : "+r" (act));

  // if(act != exp) {
  //   result->errors++;
  //   printf("Testing hwloop with l.psync, 2, 3: %X, expected %X\n", act, exp);
  // }

  // //-----------------------------------------------------------------
  // // 2 before, 2 after
  // //-----------------------------------------------------------------
  // act = 0;
  // exp = 10*(2 + 3 + 4 + 5);
  // asm volatile ("lp.counti L0, 10\n"
  //               "lp.start L0, 8\n"
  //               "lp.end L0, 20\n"
  //               "l.addi %0, %0, 2\n"
  //               "l.addi %0, %0, 3\n"
  //               "l.psync\n"
  //               "l.addi %0, %0, 4\n"
  //               "l.addi %0, %0, 5\n"
  //               : "+r" (act));

  // if(act != exp) {
  //   result->errors++;
  //   printf("Testing hwloop with l.psync, 2, 2: %X, expected %X\n", act, exp);
  // }

  //-----------------------------------------------------------------
  // a very long one
  //-----------------------------------------------------------------
  act = 0;
  exp = (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10);
  asm volatile ("addi %0, %0, 1\n"
                "addi %0, %0, 2\n"
                "addi %0, %0, 3\n"
                "addi %0, %0, 4\n"
                "addi %0, %0, 5\n"
                "wfi\n"
                "addi %0, %0, 6\n"
                "addi %0, %0, 7\n"
                "addi %0, %0, 8\n"
                "addi %0, %0, 9\n"
                "addi %0, %0, 10\n"
                : "+r" (act));

  if(act != exp) {
    result->errors++;
    printf("Testing long sequence with wfi: %X, expected %X\n", act, exp);
  }
}

// void check_hwloop_sleep(testresult_t *result, void (*start)(), void (*stop)()) {
//   unsigned int i, j;
// 
//   if(get_core_id() == 0) {
//     for(j = 0; j < 400; j++) asm volatile ("l.nop\n");
//     for(i = 0; i < HWLOOP_IT; i++) {
//       for(j = 0; j < i*5+100; j++) asm volatile ("l.nop\n");
// 
//       trigg_GP(0xE,GP1);
//     }
//   } else {
//     int act;
//     int act2;
//     int exp;
// 
//     set_evnt_mask_low(get_core_id(), 1 << GP1);
// 
//     act = 0;
//     act2 = 0;
//     exp = 10*(1 + 2 + 3);
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 24\n"
//                   "l.sw 0x0(%[clr_buf]), %[mask]\n"
//                   "l.addi %0, %0, 1\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   "l.sw 0x0(%1), %2\n"
//                   "l.psync\n"
//                   : [act] "+r" (act)
//                   : [clr_buf]      "r" (EV_BUFF_CLEAR),
//                     [mask]         "r" (1 << GP1));
// 
//     // check if hwloops are okay
//     asm volatile ("lp.counti L0, 10\n"
//                   "lp.start L0, 8\n"
//                   "lp.end L0, 12\n"
//                   "l.addi %0, %0, 1\n"
//                   "l.addi %0, %0, 2\n"
//                   "l.addi %0, %0, 3\n"
//                   : "+r" (act2));
// 
//     if(act != exp) {
//       result->errors++;
//       printf("Testing hwloop with clock gating: %X, expected %X\n", act, exp);
//     }
// 
//     if(act != act2) {
//       result->errors++;
//       printf("Testing hwloop with clock gating vs. normal: %X, expected %X\n", act, act2);
//     }
//   }
// 
//   set_evnt_mask_low(get_core_id(), 1 << BARRIER);
// 
//   synch_barrier();
// }
