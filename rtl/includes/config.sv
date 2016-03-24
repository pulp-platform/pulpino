`ifndef CONFIG_SV
`define CONFIG_SV


// always define ASIC when we do a synthesis run
`ifndef PULP_FPGA_EMUL
`ifdef SYNTHESIS
`define ASIC
`endif
`endif

// data and instruction RAM address and word width
`define ROM_ADDR_WIDTH      12
`define ROM_START_ADDR      32'h8000

`ifdef RISCV
`define CORE2AXI_REG_GRANT "FALSE"
`else
`define CORE2AXI_REG_GRANT "TRUE"
`endif

// Simulation only stuff
`ifndef SYNTHESIS
//`define DATA_STALL_RANDOM
//`define INSTR_STALL_RANDOM
`endif

`endif
