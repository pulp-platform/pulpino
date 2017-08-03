// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

`ifndef CONFIG_SV
`define CONFIG_SV
`define RISCV
// always define ASIC when we do a synthesis run
`ifndef PULP_FPGA_EMUL
`ifdef SYNTHESIS
`define ASIC
`endif
`endif

// data and instruction RAM address and word width
`define ROM_ADDR_WIDTH      12
`define ROM_START_ADDR      32'h8000

// Simulation only stuff
`ifndef SYNTHESIS
//`define DATA_STALL_RANDOM
//`define INSTR_STALL_RANDOM
`endif

`endif
