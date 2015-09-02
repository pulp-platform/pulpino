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


## Version Control

PULPino uses git submodules

To initialize all submodules
  git submodule update --init --recurisve

To update to the current versions
  git submodule update --recursive


## Documentation

There is a preliminary datasheet available that includes a block diagram and a memory map of PULPino.
See docs/datasheet/ in this repository.

It is written in LaTeX and there is no pdf included in the repository. Simply type
  make all

To generate the pdf

## Debugging the core

To interactively debug the core via gdb, you need the jtag bridge as well as a
working version of gdb for the ISA you want to debug.

TODO: more information required
