// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// Basic test program that checks the following instructions:
// - csrrw
// - csrrs
// - csrrc
//
// The program also checks if the correct bits of the following CSR are read-
// and writeable:
// - mepc
// - mstatus


#include <stdio.h>
#include "utils.h"
#include "bench.h"

#include "testCSR_stimuli.h"


void check_csr_op(testresult_t *result, void (*start)(), void (*stop)());
void check_csr_rw(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "csr_op", .test = check_csr_op },
  { .name = "csr_rw", .test = check_csr_rw },
  {0, 0}
};


int main()
{
  if(get_core_id() == 0) {
    return run_suite(testcases);
  }

  return 0;
}


#define util_csrrw(csr_str, source, dest) \
  asm volatile ("csrrw %[d], " csr_str ", %[s]" \
      : [d] "=r" (dest) \
      : [s] "r"  (source) \
  );

#define util_csrrs(csr_str, source, dest) \
  asm volatile ("csrrs %[d], " csr_str ", %[s]" \
      : [d] "=r" (dest) \
      : [s] "r"  (source) \
  );

#define util_csrrc(csr_str, source, dest) \
  asm volatile ("csrrc %[d], " csr_str ", %[s]" \
      : [d] "=r" (dest) \
      : [s] "r"  (source) \
  );

#define util_csrr(csr_str, dest) \
  asm volatile ("csrr %[d], " csr_str \
      : [d] "=r" (dest) \
      : \
  );


void check_csr_op(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int act = 0;
  unsigned int last = 0;
  unsigned int i;

  //-----------------------------------------------------------------
  // Check CSRRW
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_csrrw_a)/4); i++) {
    // update and read CSR
    util_csrrw("mepc", g_csrrw_a[i], last);
    util_csrr ("mepc", act);

    // check write
    check_uint32(result, "csrrw", act, g_csrrw_exp[i]);
    // check previous content
    if (i != 0)
      check_uint32(result, "csrrw_prev", last, g_csrrw_exp[i-1]);
  }

  //-----------------------------------------------------------------
  // Check CSRRS
  //-----------------------------------------------------------------
  util_csrrw("mepc", 0, last);

  for(i = 0; i < (sizeof(g_csrrs_a)/4); i++) {
    // update and read CSR
    util_csrrs("mepc", g_csrrs_a[i], last);
    util_csrr ("mepc", act);

    // check write
    check_uint32(result, "csrrs", act, g_csrrs_exp[i]);
    // check previous content
    if (i != 0)
      check_uint32(result, "csrrs_prev", last, g_csrrs_exp[i-1]);
  }

  //-----------------------------------------------------------------
  // Check CSRRC
  //-----------------------------------------------------------------
  util_csrrw("mepc", 0xffffffff, last);

  for(i = 0; i < (sizeof(g_csrrc_a)/4); i++) {
    // update and read CSR
    util_csrrc("mepc", g_csrrc_a[i], last);
    util_csrr ("mepc", act);

    // check write
    check_uint32(result, "csrrc", act, g_csrrc_exp[i]);
    // check previous content
    if (i != 0)
      check_uint32(result, "csrrc_prev", last, g_csrrc_exp[i-1]);
  }
}


#define util_check_rw(csr, addr, i, act, last) \
  for (i = 0; i < (sizeof(g_ ## csr ## _a)/4); i++) { \
    util_csrrw(addr, g_ ## csr ## _a[i], last); \
    util_csrr (addr, act); \
    check_uint32(result, "csrrw_" #csr, act, g_ ## csr ## _exp[i]); \
    if (i != 0) \
      check_uint32(result, "csrrw_prev_" #csr, last, g_ ## csr ## _exp[i-1]); \
  }

void check_csr_rw(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int act = 0;
  unsigned int last = 0;
  unsigned int i;

  // 32-bit registers
  util_check_rw(mepc, "mepc", i, act, last);

  // single bit
  util_check_rw(mstatus, "mstatus", i, act, last);

  // constants
  util_check_rw(mimpid,  "0xF13", i, act, last);
  util_check_rw(mhartid, "0xF14", i, act, last);
}

