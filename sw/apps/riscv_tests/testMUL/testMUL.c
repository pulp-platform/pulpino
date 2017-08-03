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
#include "utils.h"
#include "bench.h"

#include "stimuli.h"

void check_mul   (testresult_t *result, void (*start)(), void (*stop)());
void check_mulh  (testresult_t *result, void (*start)(), void (*stop)());
void check_mulhu (testresult_t *result, void (*start)(), void (*stop)());
void check_mulhsu(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "mul",           .test = check_mul           },
  { .name = "mulh",          .test = check_mulh          },
  { .name = "mulhu",         .test = check_mulhu         },
  { .name = "mulhsu",        .test = check_mulhsu        },
  {0, 0}
};

int main()
{
  if(get_core_id() == 0) {
    return run_suite(testcases);
  }

  return 0;
}

#define util_check_mul(result, a_const, b_const, exp) \
    a = a_const; \
    b = b_const; \
    asm volatile ("mul %[c], %[a], %[b];" \
                  : [c] "=r" (act) \
                  : [a] "r"  (a), \
                    [b] "r"  (b)); \
    check_uint32(result, "mul", act, exp);

#define util_check_mulh(result, a_const, b_const, exp) \
    a = a_const; \
    b = b_const; \
    asm volatile ("mulh %[c], %[a], %[b];" \
                  : [c] "=r" (act) \
                  : [a] "r"  (a), \
                    [b] "r"  (b)); \
    check_uint32(result, "mulh", act, exp);

#define util_check_mulhu(result, a_const, b_const, exp) \
    a = a_const; \
    b = b_const; \
    asm volatile ("mulhu %[c], %[a], %[b];" \
                  : [c] "=r" (act) \
                  : [a] "r"  (a), \
                    [b] "r"  (b)); \
    check_uint32(result, "mulhu", act, exp);

#define util_check_mulhsu(result, a_const, b_const, exp) \
    a = a_const; \
    b = b_const; \
    asm volatile ("mulhsu %[c], %[a], %[b];" \
                  : [c] "=r" (act) \
                  : [a] "r"  (a), \
                    [b] "r"  (b)); \
    check_uint32(result, "mulhsu", act, exp);

void check_mul(testresult_t *result, void (*start)(), void (*stop)()) {
  int act;
  int a;
  int b;
  int i;

  util_check_mul(result, 0x0, 0x0, 0x0);
  util_check_mul(result, 3, 5, 15);
  util_check_mul(result, 234, 5, 1170);
  util_check_mul(result, 1170, 1170, 1368900);
  util_check_mul(result, 1368900, 3, 4106700);
  util_check_mul(result, 234, 12345, 2888730);

  for(i = 0; i < n_stimuli; i++) {
    util_check_mul(result, stim_mul_a[i], stim_mul_b[i], stim_mul_exp[i]);
  }
}

void check_mulh(testresult_t *result, void (*start)(), void (*stop)()) {
  int act;
  int a;
  int b;
  int i;

  util_check_mulh(result, 0x0, 0x0, 0x0);
  util_check_mulh(result, 0x00010000, 0x00010000, 0x00000001);
  util_check_mulh(result, 0x00010000, 0x00000000, 0x00000000);
  util_check_mulh(result, 0x00000000, 0x00010000, 0x00000000);
  util_check_mulh(result, 0x00000001, 0x00000000, 0x00000000);
  util_check_mulh(result, 0x00000000, 0x00000001, 0x00000000);
  util_check_mulh(result, 0x10000000, 0x10000000, 0x01000000);
  util_check_mulh(result, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
  util_check_mulh(result, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF);
  util_check_mulh(result, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFF);

  for(i = 0; i < n_stimuli; i++) {
    util_check_mulh(result, stim_mulh_a[i], stim_mulh_b[i], stim_mulh_exp[i]);
  }
}

void check_mulhu(testresult_t *result, void (*start)(), void (*stop)()) {
  int act;
  int a;
  int b;
  int i;

  util_check_mulhu(result, 0x0, 0x0, 0x0);
  util_check_mulhu(result, 0x00010000, 0x00010000, 0x00000001);
  util_check_mulhu(result, 0x00010000, 0x00000000, 0x00000000);
  util_check_mulhu(result, 0x00000000, 0x00010000, 0x00000000);
  util_check_mulhu(result, 0x00000001, 0x00000000, 0x00000000);
  util_check_mulhu(result, 0x00000000, 0x00000001, 0x00000000);
  util_check_mulhu(result, 0x10000000, 0x10000000, 0x01000000);
  util_check_mulhu(result, 0xFFFFFFFF, 0xFFFFFFFF, 0xfffffffe);
  util_check_mulhu(result, 0xFFFFFFFF, 0x00000001, 0x00000000);
  util_check_mulhu(result, 0x00000001, 0xFFFFFFFF, 0x00000000);

  for(i = 0; i < n_stimuli; i++) {
    util_check_mulhu(result, stim_mulhu_a[i], stim_mulhu_b[i], stim_mulhu_exp[i]);
  }
}

void check_mulhsu(testresult_t *result, void (*start)(), void (*stop)()) {
  int act;
  int a;
  int b;
  int i;

  util_check_mulhsu(result, 0x0, 0x0, 0x0);
  util_check_mulhsu(result, 0x00010000, 0x00010000, 0x00000001);
  util_check_mulhsu(result, 0x00010000, 0x00000000, 0x00000000);
  util_check_mulhsu(result, 0x00000000, 0x00010000, 0x00000000);
  util_check_mulhsu(result, 0x00000001, 0x00000000, 0x00000000);
  util_check_mulhsu(result, 0x00000000, 0x00000001, 0x00000000);
  util_check_mulhsu(result, 0x10000000, 0x10000000, 0x01000000);
  util_check_mulhsu(result, 0xFFFFFFFF, 0xFFFFFFFF, 0xffffffff);
  util_check_mulhsu(result, 0xFFFFFFFF, 0x00000001, 0xffffffff);
  util_check_mulhsu(result, 0x00000001, 0xFFFFFFFF, 0x00000000);

  for(i = 0; i < n_stimuli; i++) {
    util_check_mulhsu(result, stim_mulhsu_a[i], stim_mulhsu_b[i], stim_mulhsu_exp[i]);
  }
}