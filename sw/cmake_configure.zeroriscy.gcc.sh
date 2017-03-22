#!/bin/bash

OBJDUMP=`which riscv32-unknown-elf-objdump`
OBJCOPY=`which riscv32-unknown-elf-objcopy`

COMPILER=`which riscv32-unknown-elf-gcc`
RANLIB=`which riscv32-unknown-elf-ranlib`

TARGET_C_FLAGS="-O3 -m32 -g"

# if you want to have compressed instructions, set this to 1
RVC=0

# if you are using the ETH gcc compiler that supports the extended instruction
# of RI5CY, set this to 1
# Otherwise it is assumed that the official riscv compiler is used and no special
# instructions are inserted
GCC_ETH=0
# if you are using riscy, set this to 1
USE_RISCY=0
# if you are using zero-riscy, set this to 1
USE_ZERO_RISCY=1

# if you are using ZERO-RISCY and you are using gcc_slim compiler,
# you can use only 16 registers by enabling USE_RV32E
USE_RV32E=0

PULP_GIT_DIRECTORY=../../
SIM_DIRECTORY="$PULP_GIT_DIRECTORY/vsim"

cmake-3.3.0 "$PULP_GIT_DIRECTORY"/sw/ \
    -DPULP_MODELSIM_DIRECTORY="$SIM_DIRECTORY" \
    -DCMAKE_C_COMPILER="$COMPILER" \
    -DVSIM="$VSIM" \
    -DRVC="$RVC" \
    -DGCC_ETH="$GCC_ETH" \
    -DUSE_ZERO_RISCY="$USE_ZERO_RISCY" \
    -DUSE_RV32E="$USE_RV32E" \
    -DUSE_RISCY="$USE_RISCY" \
    -DCMAKE_C_FLAGS="$TARGET_C_FLAGS" \
    -DCMAKE_OBJCOPY="$OBJCOPY" \
    -DCMAKE_OBJDUMP="$OBJDUMP"

# Add -G "Ninja" to the cmake call above to use ninja instead of make
