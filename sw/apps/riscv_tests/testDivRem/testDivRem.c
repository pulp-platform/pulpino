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

void check_div(testresult_t *result, void (*start)(), void (*stop)());
void check_divu(testresult_t *result, void (*start)(), void (*stop)());
void check_rem(testresult_t *result, void (*start)(), void (*stop)());
void check_remu(testresult_t *result, void (*start)(), void (*stop)());
void check_div_random(testresult_t *result, void (*start)(), void (*stop)());
void check_divu_random(testresult_t *result, void (*start)(), void (*stop)());
void check_rem_random(testresult_t *result, void (*start)(), void (*stop)());
void check_remu_random(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "div",           .test = check_div           },
  { .name = "divu",          .test = check_divu          },
  { .name = "rem",           .test = check_rem           },
  { .name = "remu",          .test = check_remu          },
  { .name = "div_random",    .test = check_div_random    },
  { .name = "divu_random",   .test = check_divu_random   },
  { .name = "rem_random",    .test = check_rem_random    },
  { .name = "remu_random",   .test = check_remu_random   },
  {0, 0}
};

int main()
{
  if(get_core_id() == 0)
    return run_suite(testcases);

  return 0;
}

void do_rem(testresult_t* result, int a, int b, int exp) {
  int act;

  asm volatile ("rem %[result], %[a], %[b]" : [result] "=r" (act) : [a] "r" (a), [b] "r" (b));

  if (act != exp) {
    result->errors++;
    printf ("rem %d, %d: actual %d, expected %d\n", a, b, act, exp);
  }
}

void do_remu(testresult_t* result, unsigned int a, unsigned int b, unsigned int exp) {
  unsigned int act;

  asm volatile ("remu %[result], %[a], %[b]" : [result] "=r" (act) : [a] "r" (a), [b] "r" (b));

  if (act != exp) {
    result->errors++;
    printf ("remu %u, %u: actual %u, expected %u\n", a, b, act, exp);
  }
}

void do_div(testresult_t* result, int a, int b, int exp) {
  int act;

  asm volatile ("div %[result], %[a], %[b]" : [result] "=r" (act) : [a] "r" (a), [b] "r" (b));

  if (act != exp) {
    result->errors++;
    printf ("div %d, %d: actual %d, expected %d\n", a, b, act, exp);
  }
}

void do_divu(testresult_t* result, unsigned int a, unsigned int b, unsigned int exp) {
  unsigned int act;

  asm volatile ("divu %[result], %[a], %[b]" : [result] "=r" (act) : [a] "r" (a), [b] "r" (b));

  if (act != exp) {
    result->errors++;
    printf ("divu %u, %u: actual %u, expected %u\n", a, b, act, exp);
  }
}

void check_divu(testresult_t *result, void (*start)(), void (*stop)()) {
  //-----------------------------------------------------------------
  // Check divu
  //-----------------------------------------------------------------
  do_divu(result, 0x80000000, 0x80000000,          1);
  do_divu(result, 0xFFFFFFFF, 0xFFFFFFFF,          1);

  do_divu(result, 0xFFFFFFFF, 0xFFFFFFFF,          1);
  do_divu(result, 0xFFFFFFFE, 0xFFFFFFFF,          0);
  do_divu(result, 0xFFFFFFFE, 0xFFFFFFFE,          1);
  do_divu(result, 0xFFFFFFFF, 0xFFFFFFFE,          1);
  do_divu(result,          1,          1,          1);
  do_divu(result,          0,          1,          0);
  do_divu(result,          0,        100,          0);
  do_divu(result,          0, 0xFFFFFFFE, 0);
  do_divu(result,          0,          0, 0xFFFFFFFF);
  do_divu(result,          1,          0, 0xFFFFFFFF);

  do_divu(result,          5,          0, 0xFFFFFFFF);
  do_divu(result,          5,          1,          5);
  do_divu(result,          5,          2,          2);
  do_divu(result,          5,          3,          1);
  do_divu(result,          5,          4,          1);
  do_divu(result,          5,          5,          1);
  do_divu(result,          5,          6,          0);

  do_divu(result,          1,          0, 0xFFFFFFFF);
  do_divu(result,          2,          0, 0xFFFFFFFF);
  do_divu(result,          3,          0, 0xFFFFFFFF);
  do_divu(result,          4,          0, 0xFFFFFFFF);
  do_divu(result,          5,          0, 0xFFFFFFFF);
  do_divu(result,          6,          0, 0xFFFFFFFF);
  do_divu(result,          7,          0, 0xFFFFFFFF);

  do_divu(result, 0,          0x80000000,          0);
  do_divu(result, 0x80000000,          0, 0xFFFFFFFF);
}

void check_remu(testresult_t *result, void (*start)(), void (*stop)()) {
  //-----------------------------------------------------------------
  // Check remu
  //-----------------------------------------------------------------
  do_remu(result, 0x80000000, 0x80000000,          0);
  do_remu(result, 0xFFFFFFFF, 0xFFFFFFFF,          0);

  do_remu(result, 0xFFFFFFFF, 0xFFFFFFFF,          0);
  do_remu(result, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFE);
  do_remu(result, 0xFFFFFFFE, 0xFFFFFFFE,          0);
  do_remu(result, 0xFFFFFFFF, 0xFFFFFFFE,          1);
  do_remu(result,          1,          1,          0);
  do_remu(result,          0,          1,          0);
  do_remu(result,          0,        100,          0);
  do_remu(result,          0, 0xFFFFFFFE,          0);
  do_remu(result,          0,          0,          0);
  do_remu(result,          1,          0,          1);

  do_remu(result,          5,          0,          5);
  do_remu(result,          5,          1,          0);
  do_remu(result,          5,          2,          1);
  do_remu(result,          5,          3,          2);
  do_remu(result,          5,          4,          1);
  do_remu(result,          5,          5,          0);
  do_remu(result,          5,          6,          5);

  do_remu(result,          1,          0,          1);
  do_remu(result,          2,          0,          2);
  do_remu(result,          3,          0,          3);
  do_remu(result,          4,          0,          4);
  do_remu(result,          5,          0,          5);
  do_remu(result,          6,          0,          6);
  do_remu(result,          7,          0,          7);

  do_remu(result, 0,          0x80000000,          0);
  do_remu(result, 0x80000000,          0, 0x80000000);
}

void check_div(testresult_t *result, void (*start)(), void (*stop)()) {
  //-----------------------------------------------------------------
  // Check div
  //-----------------------------------------------------------------
  do_div(result, 0x80000000, 0x80000000,          1);
  do_div(result, 0xFFFFFFFF, 0xFFFFFFFF,          1);

  do_div(result, 0xFFFFFFFF, 0xFFFFFFFF,          1);
  do_div(result, 0xFFFFFFFE, 0xFFFFFFFF,          2);
  do_div(result, 0xFFFFFFFE, 0xFFFFFFFE,          1);
  do_div(result, 0xFFFFFFFF, 0xFFFFFFFE,          0);
  do_div(result,         -1,         -1,          1);
  do_div(result,         -1,          1,         -1);
  do_div(result,          1,          1,          1);
  do_div(result,          1,         -1,         -1);
  do_div(result,          0,         -1,          0);
  do_div(result,          0,          1,          0);
  do_div(result,          0,        100,          0);
  do_div(result,          0, 0xFFFFFFFE,          0);
  do_div(result,          0,          0,         -1);
  do_div(result,          1,          0,         -1);
  do_div(result,         -1,          0,         -1);

  do_div(result,          5,          0,         -1);
  do_div(result,          5,          1,          5);
  do_div(result,          5,          2,          2);
  do_div(result,          5,          3,          1);
  do_div(result,          5,          4,          1);
  do_div(result,          5,          5,          1);
  do_div(result,          5,          6,          0);

  do_div(result,          1,          0,         -1);
  do_div(result,          2,          0,         -1);
  do_div(result,          3,          0,         -1);
  do_div(result,          4,          0,         -1);
  do_div(result,          5,          0,         -1);
  do_div(result,          6,          0,         -1);
  do_div(result,          7,          0,         -1);

  do_div(result, 0x80000000,         -1, 0x80000000);
  do_div(result, 0,          0x80000000,          0);
  do_div(result, 0x80000000,          0,         -1);
}

void check_rem(testresult_t *result, void (*start)(), void (*stop)()) {
  //-----------------------------------------------------------------
  // Check remu
  //-----------------------------------------------------------------
  do_rem(result, 0x80000000, 0x80000000,          0);
  do_rem(result, 0xFFFFFFFF, 0xFFFFFFFF,          0);

  do_rem(result, 0xFFFFFFFF, 0xFFFFFFFF,          0);
  do_rem(result, 0xFFFFFFFE, 0xFFFFFFFF,          0);
  do_rem(result, 0xFFFFFFFE, 0xFFFFFFFE,          0);
  do_rem(result, 0xFFFFFFFF, 0xFFFFFFFE,         -1);
  do_rem(result,         -1,         -1,          0);
  do_rem(result,         -1,          1,          0);
  do_rem(result,          1,          1,          0);
  do_rem(result,          1,         -1,          0);
  do_rem(result,          0,         -1,          0);
  do_rem(result,          0,          1,          0);
  do_rem(result,          0,        100,          0);
  do_rem(result,          0, 0xFFFFFFFE,          0);
  do_rem(result,          0,          0,          0);
  do_rem(result,          1,          0,          1);
  do_rem(result,         -1,          0,         -1);

  do_rem(result,          5,          0,          5);
  do_rem(result,          5,          1,          0);
  do_rem(result,          5,          2,          1);
  do_rem(result,          5,          3,          2);
  do_rem(result,          5,          4,          1);
  do_rem(result,          5,          5,          0);
  do_rem(result,          5,          6,          5);

  do_rem(result,          1,          0,          1);
  do_rem(result,          2,          0,          2);
  do_rem(result,          3,          0,          3);
  do_rem(result,          4,          0,          4);
  do_rem(result,          5,          0,          5);
  do_rem(result,          6,          0,          6);
  do_rem(result,          7,          0,          7);

  do_rem(result, 0x80000000,         -1,          0);
  do_rem(result, 0,          0x80000000,          0);
  do_rem(result, 0x80000000,          0, 0x80000000);
}

void check_div_random(testresult_t *result, void (*start)(), void (*stop)()) {
  int i;

  for (i = 0; i < DIV_STIMS; i++) {
    do_div(result, div_stim[i].a, div_stim[i].b, div_stim[i].exp);
  }
}

void check_divu_random(testresult_t *result, void (*start)(), void (*stop)()) {
  int i;

  for (i = 0; i < DIVU_STIMS; i++) {
    do_divu(result, divu_stim[i].a, divu_stim[i].b, divu_stim[i].exp);
  }
}

void check_rem_random(testresult_t *result, void (*start)(), void (*stop)()) {
  int i;

  for (i = 0; i < REM_STIMS; i++) {
    do_rem(result, rem_stim[i].a, rem_stim[i].b, rem_stim[i].exp);
  }
}

void check_remu_random(testresult_t *result, void (*start)(), void (*stop)()) {
  int i;

  for (i = 0; i < REMU_STIMS; i++) {
    do_remu(result, remu_stim[i].a, remu_stim[i].b, remu_stim[i].exp);
  }
}
