// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// This test checks if misaligned accesses work

#include <stdio.h>
#include "utils.h"
#include "bar.h"
#include "bench.h"


void check_lw(testresult_t *result, void (*start)(), void (*stop)());
void check_lh(testresult_t *result, void (*start)(), void (*stop)());
void check_sw(testresult_t *result, void (*start)(), void (*stop)());
void check_sh(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "lw",          .test = check_lw          },
  { .name = "lh",          .test = check_lh          },
  { .name = "sw",          .test = check_sw          },
  { .name = "sh",          .test = check_sh          },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}


void check_lw(testresult_t *result, void (*start)(), void (*stop)()) {
  if(get_core_id() == 0) {
    volatile char word[8];

    word[0] = 0x0;
    word[1] = 0x1;
    word[2] = 0x2;
    word[3] = 0x3;
    word[4] = 0x4;
    word[5] = 0x5;
    word[6] = 0x6;
    word[7] = 0x7;

    uint32_t act;

    asm volatile ("lw %[d], 1(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lw misaligned 1", act, 0x04030201);

    asm volatile ("lw %[d], 2(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lw misaligned 2", act, 0x05040302);

    asm volatile ("lw %[d], 3(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lw misaligned 3", act, 0x06050403);
  }
}

void check_lh(testresult_t *result, void (*start)(), void (*stop)()) {
  if(get_core_id() == 0) {
    volatile char word[8];
    uint32_t act;

    // zero extension
    word[0] = 0x0;
    word[1] = 0x1;
    word[2] = 0x2;
    word[3] = 0x3;
    word[4] = 0x4;
    word[5] = 0x5;
    word[6] = 0x6;
    word[7] = 0x7;

    asm volatile ("lhu %[d], 1(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lhu misaligned 1", act, 0x0201);

    asm volatile ("lhu %[d], 2(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lhu misaligned 2", act, 0x0302);

    asm volatile ("lhu %[d], 3(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lhu misaligned 3", act, 0x0403);

    // sign extension
    word[0] = 0x80;
    word[1] = 0x81;
    word[2] = 0x82;
    word[3] = 0x83;
    word[4] = 0x84;
    word[5] = 0x85;
    word[6] = 0x86;
    word[7] = 0x87;

    asm volatile ("lh %[d], 1(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lh misaligned 1", act, 0xFFFF8281);

    asm volatile ("lh %[d], 2(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lh misaligned 2", act, 0xFFFF8382);

    asm volatile ("lh %[d], 3(%[addr])\n"
                  : [d] "+r" (act)
                  : [addr] "r" (word));

    check_uint32(result, "lh misaligned 3", act, 0xFFFF8483);
  }
}

void check_sw(testresult_t *result, void (*start)(), void (*stop)()) {
  if(get_core_id() == 0) {
    volatile char act[8];
    uint32_t word;
    unsigned int i;

    for(i = 0; i < 8; i++) act[i] = 0;
    word = 0x01020304;
    asm volatile ("sw %[a], 1(%[addr])\n"
                  : : [addr] "r" (act), [a] "r" (word));

    check_uint32(result, "sw misaligned 1", *((uint32_t*)&act[1]), word);


    for(i = 0; i < 8; i++) act[i] = 0;
    word = 0x01020304;
    asm volatile ("sw %[a], 2(%[addr])\n"
                  : : [addr] "r" (act), [a] "r" (word));

    check_uint32(result, "sw misaligned 2", *((uint32_t*)&act[2]), word);


    for(i = 0; i < 8; i++) act[i] = 0;
    word = 0x01020304;
    asm volatile ("sw %[a], 3(%[addr])\n"
                  : : [addr] "r" (act), [a] "r" (word));

    check_uint32(result, "sw misaligned 3", *((uint32_t*)&act[3]), word);
  }
}

void check_sh(testresult_t *result, void (*start)(), void (*stop)()) {
  if(get_core_id() == 0) {
    volatile char act[8];
    uint32_t word;
    unsigned int i;

    for(i = 0; i < 8; i++) act[i] = 0;
    word = 0x0304;
    asm volatile ("sh %[a], 1(%[addr])\n"
                  : : [addr] "r" (act), [a] "r" (word));

    check_uint32(result, "sh misaligned 1", *((uint32_t*)&act[1]), word);


    for(i = 0; i < 8; i++) act[i] = 0;
    word = 0x0304;
    asm volatile ("sh %[a], 2(%[addr])\n"
                  : : [addr] "r" (act), [a] "r" (word));

    check_uint32(result, "sh misaligned 2", *((uint32_t*)&act[2]), word);


    for(i = 0; i < 8; i++) act[i] = 0;
    word = 0x0304;
    asm volatile ("sh %[a], 3(%[addr])\n"
                  : : [addr] "r" (act), [a] "r" (word));

    check_uint32(result, "sh misaligned 3", *((uint32_t*)&act[3]), word);
  }
}
