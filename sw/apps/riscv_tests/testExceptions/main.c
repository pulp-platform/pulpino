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

#define NUM_INTERRUPTED  10

void check_illegal(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "illegal",          .test = check_illegal          },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}


unsigned int g_illegal_insn_counter;

void check_illegal(testresult_t *result, void (*start)(), void (*stop)()) {

  unsigned int i;

  //////////////////////////////////////////////////////////////////////////////
  // Check if we can correctly cause an illegal instruction exception and jump
  // over the illegal instruction
  //////////////////////////////////////////////////////////////////////////////


  g_illegal_insn_counter = 0;

  for(i = 0; i < NUM_INTERRUPTED; i++) {
    unsigned int act = 0;
    unsigned int exp = 1+2;
    asm volatile ("addi %[act], %[act], 1\n"
                  ".word 0x90F090F0\n"
                  "addi %[act], %[act], 2\n"
                  : [act] "+r" (act));

    check_uint32(result, "Checking illegal instruction", act, exp);
  }

  check_uint32(result, "Comparing number of illegal instructions found", g_illegal_insn_counter, NUM_INTERRUPTED);

}


// overwrite illegal instruction handler
void illegal_insn_handler_c()
{
  uint32_t exception_address;

  g_illegal_insn_counter += 1;

  // jump over illegal instruction
  asm("csrr %0, 0x341" : "=r" (exception_address) );
  asm("csrw 0x341, %0" : : "r" (exception_address + 4) );
}
