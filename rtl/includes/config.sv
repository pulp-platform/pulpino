/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

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
