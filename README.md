# PULPino

PULPino uses IPs from the PULP project to build a small-scale single-core SoC.
PULPino aims to be simple and lightweight, but still includes some advanced
features like debugging the core via JTAG.

It is currently compatible RTL simulation only, but it is planned to be
supported in FPGA and ASIC synthesis flows eventually.


## Requirements

PULPino has the following requirements

- ModelSim in reasonably recent version (we tested it with versions >= 10.2c)
- CMake >= 2.8.0, versions greater than 3.1.0 recommended due to support for ninja
- riscv-toolchain, specifically you need riscv32-unknown-elf-gcc and friends

## Editions

There are two PULPino editions available, one for OR1K based on the OR10N core
and one for RISCV based on the RI5CY core.
The software included in this repository is compatible with both ISAs and
automatically targets the correct ISA based on the compiler used.

The HDL must be explicitely told which edition you want to build.

- (Un)comment "`define RISCV" in rtl/includes/config.sv to build for RISCV
- Change or10n_lib to riscv_lib or vice-versa in vsim/tcl_scripts/vsim.tcl to let modelsim know which version you like to simulate


## Debugging the core

To interactively debug the core via gdb, you need the jtag bridge.

TODO: more information required
