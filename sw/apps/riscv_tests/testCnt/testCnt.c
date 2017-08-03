// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

////////////////////////////////////////////////////////////////////////////////
// Company:        IIS @ ETHZ - Federal Institute of Technology               //
//                                                                            //
// Engineer:       Andreas Traber - atraber@student.ethz.ch                   //
//                                                                            //
// Additional contributions by:                                               //
//                 Sven Stucki - svstucki@student.ethz.ch                     //
//                                                                            //
// Description: This testbench checks the basic functionality of:             //
// - p.ff1                                                                    //
// - p.fl1                                                                    //
// - p.cnt                                                                    //
// - p.clb                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "utils.h"
#include "bench.h"


void check_ff1(testresult_t *result, void (*start)(), void (*stop)());
void check_fl1(testresult_t *result, void (*start)(), void (*stop)());
void check_cnt(testresult_t *result, void (*start)(), void (*stop)());
void check_clb(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "ff1",        .test = check_ff1          },
  { .name = "fl1",        .test = check_fl1          },
  { .name = "cnt",        .test = check_cnt          },
  { .name = "clb",        .test = check_clb          },
  {0, 0}
};

int main()
{
  if(get_core_id() == 0) {
    return run_suite(testcases);
  }

  return 0;
}

void check_ff1(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act;
  uint32_t exp;
  uint32_t a;

  a = 0x110110;
  asm volatile ("p.ff1 %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.ff1", act,  4);

  a = 0x110100;
  asm volatile ("p.ff1 %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.ff1", act,  8);

  a = 0x00000000;
  asm volatile ("p.ff1 %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.ff1", act,  32);

  a = 0x80000000;
  asm volatile ("p.ff1 %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.ff1", act,  31);

  unsigned int i;
  for(i = 0; i < 32; i++) {
    a = 1 << i;

    asm volatile ("p.ff1 %[d], %[a]"
                  : [d] "=r" (act)
                  : [a] "r"  (a));

    exp = i;

    check_uint32(result, "p.ff1", act,  exp);
  }

  for(i = 0; i < 32; i++) {
    a = (1 << 31) | (1 << i);

    asm volatile ("p.ff1 %[d], %[a]"
                  : [d] "=r" (act)
                  : [a] "r"  (a));

    exp = i;

    check_uint32(result, "p.ff1", act,  exp);
  }
}


void check_fl1(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act;
  uint32_t exp;
  uint32_t a;

  a = 0x4FFFFFFF;
  asm volatile ("p.fl1 %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.fl1", act,  30);

  a = 0x00000001;
  asm volatile ("p.fl1 %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.fl1", act,  0);

  unsigned int i;
  for(i = 0; i < 32; i++) {
    a = 1 << i;

    asm volatile ("p.fl1 %[d], %[a]"
                  : [d] "=r" (act)
                  : [a] "r"  (a));

    exp = i;

    check_uint32(result, "p.fl1", act,  exp);
  }

  for(i = 0; i < 32; i++) {
    a = 1 | (1 << i);

    asm volatile ("p.fl1 %[d], %[a]"
                  : [d] "+r" (act)
                  : [a] "r"  (a));

    exp = i;

    check_uint32(result, "p.fl1", act,  exp);
  }
}

void check_cnt(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act;
  uint32_t exp;
  uint32_t a;

  a = 0x10101010;
  asm volatile ("p.cnt %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.cnt", act,  4);

  a = 0xFFFFFFFF;
  asm volatile ("p.cnt %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.cnt", act,  32);

  a = 0x00000000;
  asm volatile ("p.cnt %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.cnt", act,  0);

  unsigned int i, j;
  for(i = 0; i < 32; i++) {
    a = 0;
    for(j = 0; j <= i; j++)
      a |= 1 << j;

    asm volatile ("p.cnt %[d], %[a]"
                  : [d] "=r" (act)
                  : [a] "r"  (a));

    exp = i + 1;

    check_uint32(result, "p.cnt", act,  exp);
  }
}


unsigned int clb_golden(int32_t Op1) {
  int A;

  if (Op1 == 0) A = 0;
  else if (Op1 == (int) 0xFFFFFFFFL) A = 31;
  else {
          if ((int) Op1 < (int) 0) Op1 = ~Op1;
          for (A=0; (int) Op1 < (int) 0x40000000L; A++) Op1 = Op1 << 1;
  }
  return A;
}

void check_clb(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act;
  uint32_t exp;
  uint32_t a;

  a = 0x7FFFFFFF;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  clb_golden(a));

  a = 0x00000001;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  clb_golden(a));

  a = 0xF0000000;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  clb_golden(a));

  a = 0xFFFFFFFE;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  clb_golden(a));

  a = 0xFFFFFFFF;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  clb_golden(a));

  a = 0x00000000;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "=r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  clb_golden(a));

  unsigned int i;
  for(i = 0; i < 32; i++) {
    a = 1 << i;

    asm volatile ("p.clb %[d], %[a]"
                  : [d] "=r" (act)
                  : [a] "r"  (a));

    check_uint32(result, "p.clb", act,  clb_golden(a));
  }
}
