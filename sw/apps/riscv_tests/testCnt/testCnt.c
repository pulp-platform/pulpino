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
    run_suite(testcases);
  }

  synch_barrier();

  return 0;
}

void check_ff1(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act;
  uint32_t exp;
  uint32_t a;

  a = 0x110110;
  asm volatile ("p.ff1 %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.ff1", act,  5);

  a = 0x110100;
  asm volatile ("p.ff1 %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.ff1", act,  9);

  unsigned int i;
  for(i = 0; i < 32; i++) {
    a = 1 << i;

    asm volatile ("p.ff1 %[d], %[a]"
                  : [d] "+r" (act)
                  : [a] "r"  (a));

    exp = i + 1;

    check_uint32(result, "p.ff1", act,  exp);
  }

  for(i = 0; i < 32; i++) {
    a = (1 << 31) | (1 << i);

    asm volatile ("p.ff1 %[d], %[a]"
                  : [d] "+r" (act)
                  : [a] "r"  (a));

    exp = i + 1;

    check_uint32(result, "p.ff1", act,  exp);
  }
}


void check_fl1(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act;
  uint32_t exp;
  uint32_t a;

  a = 0x4FFFFFFF;
  asm volatile ("p.fl1 %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.fl1", act,  31);

  a = 0x00000001;
  asm volatile ("p.fl1 %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.fl1", act,  1);

  unsigned int i;
  for(i = 0; i < 32; i++) {
    a = 1 << i;

    asm volatile ("p.fl1 %[d], %[a]"
                  : [d] "+r" (act)
                  : [a] "r"  (a));

    exp = i + 1;

    check_uint32(result, "p.fl1", act,  exp);
  }

  for(i = 0; i < 32; i++) {
    a = 1 | (1 << i);

    asm volatile ("p.fl1 %[d], %[a]"
                  : [d] "+r" (act)
                  : [a] "r"  (a));

    exp = i + 1;

    check_uint32(result, "p.fl1", act,  exp);
  }
}

void check_cnt(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act;
  uint32_t exp;
  uint32_t a;

  a = 0x10101010;
  asm volatile ("p.cnt %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.cnt", act,  4);

  a = 0xFFFFFFFF;
  asm volatile ("p.cnt %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.cnt", act,  32);

  a = 0x00000000;
  asm volatile ("p.cnt %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.cnt", act,  0);

  unsigned int i, j;
  for(i = 0; i < 32; i++) {
    a = 0;
    for(j = 0; j <= i; j++)
      a |= 1 << j;

    asm volatile ("p.cnt %[d], %[a]"
                  : [d] "+r" (act)
                  : [a] "r"  (a));

    exp = i + 1;

    check_uint32(result, "p.cnt", act,  exp);
  }
}


unsigned int clb_golden(int32_t val) {
  unsigned int var_out;

  if (val == 0)
    var_out = 0;
  else if (val == (int32_t)0xffffffff)
    var_out = 31;
  else {
    if (val < 0)
      val = ~val;

    for(var_out = 0; val < (int32_t)0x40000000; var_out++)
      val <<= 1;
  }

  return var_out;
}

void check_clb(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t act;
  uint32_t exp;
  uint32_t a;

  a = 0x7FFFFFFF;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  0);

  a = 0x00000001;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  30);

  a = 0xF0000000;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  3);

  a = 0xFFFFFFFE;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  30);

  a = 0x00000000;
  asm volatile ("p.clb %[d], %[a]"
                : [d] "+r" (act)
                : [a] "r"  (a));

  check_uint32(result, "p.clb", act,  0);

  unsigned int i;
  for(i = 0; i < 32; i++) {
    a = 1 << i;

    asm volatile ("p.clb %[d], %[a]"
                  : [d] "+r" (act)
                  : [a] "r"  (a));

    exp = clb_golden(a);

    check_uint32(result, "p.clb", act,  exp);
  }
}
