// Copyright 2015 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// This code checks certain aspects of the debug unit
// It should be run together with the tb_top_jtag.sv testbench

#include <stdio.h>
#include "utils.h"
#include "bench.h"

void check_register_read(testresult_t *result, void (*start)(), void (*stop)());
void check_register_write(testresult_t *result, void (*start)(), void (*stop)());
void check_burst_read(testresult_t *result, void (*start)(), void (*stop)());
void check_axi_write(testresult_t *result, void (*start)(), void (*stop)());
void check_axi_read(testresult_t *result, void (*start)(), void (*stop)());
void check_st(testresult_t *result, void (*start)(), void (*stop)());
void get_report(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "reg_read",         .test = check_register_read      },
  { .name = "reg_write",        .test = check_register_write     },
  { .name = "burst_read",       .test = check_burst_read         },
  { .name = "axi_write",        .test = check_axi_write          },
  { .name = "axi_read",         .test = check_axi_read           },
  { .name = "single_st",        .test = check_st                 },
  { .name = "report",           .test = get_report               },
  {0, 0},
};


int main()
{
  if(get_core_id() == 0) {
    run_suite(testcases);
  }

  synch_barrier();

  return 0;
}


// check if we can read GPR from the debug unit
void check_register_read(testresult_t *result, void (*start)(), void (*stop)()) {
  asm volatile ("li x31, 0x0000\n"
                "EBREAK\n"
                "li x31, 0xFFFF0000\n"
                "EBREAK\n" : : : "x31");
}

// check if we can write to GPR from the debug unit
void check_register_write(testresult_t *result, void (*start)(), void (*stop)()) {
  uint32_t a = 0;

  asm volatile ("li x31, 0x0000\n"
                "EBREAK\n"
                "addi %[a], x31, 0\n"
                : [a] "+r" (a) : : "x31");

  check_uint32(result, "Register write has failed", a, 0xABCDABCD);
}

// check if we can read GPR from the debug unit, in BURST mode
void check_burst_read(testresult_t *result, void (*start)(), void (*stop)()) {
  asm volatile ("addi x25, x0, 0x0025\n"
                "addi x27, x0, 0x0027\n"
                "addi x29, x0, 0x0029\n"
                "addi x31, x0, 0x0031\n"
                "EBREAK\n"
                :
                :
                : "x25", "x27", "x29", "x31");
}

void check_axi_write(testresult_t *result, void (*start)(), void (*stop)()) {
  volatile uint32_t* ptr = (uint32_t*)(TCDM_BASE_ADDR);

  unsigned int i;
  for(i = 0; i < 16; i++)
    ptr[i] = 0;

  asm volatile ("EBREAK");

  check_uint32(result, "AXI write has failed", ptr[0], 0x00842100);
  check_uint32(result, "AXI write has failed", ptr[1], 0xABBA1234);
  check_uint32(result, "AXI write has failed", ptr[2], 0x01234567);
  check_uint32(result, "AXI write has failed", ptr[3], 0xBABBA123);
}

void check_axi_read(testresult_t *result, void (*start)(), void (*stop)()) {
  volatile uint32_t* ptr = (uint32_t*)(TCDM_BASE_ADDR);

  ptr[0] = 0x01020304;
  ptr[1] = 0xFEEDBEEF;
  ptr[2] = 0xDEADC0DE;
  ptr[3] = 0x1A2B3C4D;

  asm volatile ("EBREAK");
}

// check if single-stepping mode works
void check_st(testresult_t *result, void (*start)(), void (*stop)()) {
  asm volatile ("EBREAK\n"
                "nop\n"
                "nop\n"
                "nop\n"
                "nop\n"
                "nop\n");
}

void get_report(testresult_t *result, void (*start)(), void (*stop)()) {
  // get test report from jtag testbench
  unsigned int tb_errors;

  asm volatile ("EBREAK\n"
                "addi %[d], x31, 0\n"
                : [d] "=r" (tb_errors) : : "x31");

  if (tb_errors > 0) {
    printf("Errors while executing testbench: %d\n", tb_errors);
    result->errors = tb_errors;
  }
}
