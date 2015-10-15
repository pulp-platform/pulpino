#!/bin/bash

OBJDUMP=`which llvm-objdump`
OBJCOPY=`which or1kle-elf-objcopy`

COMPILER=`which or1kle-elf-clang`
SIZE=`which or1kle-elf-size`
TARGET_C_FLAGS="-O2 -mcpu=pulp3 -g"

VSIM=`which vsim`

PULP_GIT_DIRECTORY=../../
SIM_DIRECTORY="$PULP_GIT_DIRECTORY/vsim"

cmake "$PULP_GIT_DIRECTORY"/sw/ \
    -DCMAKE_BUILD_TYPE=Release \
    -DPULP_MODELSIM_DIRECTORY="$SIM_DIRECTORY" \
    -DCMAKE_C_COMPILER="$COMPILER" \
    -DVSIM="$VSIM" \
    -DCMAKE_C_FLAGS="$TARGET_C_FLAGS" \
    -DCMAKE_OBJCOPY="$OBJCOPY" \
    -DCMAKE_OBJDUMP="$OBJDUMP" \
    -DCMAKE_SIZE="$SIZE"

# Add -G "Ninja" to the cmake call above to use ninja instead of make
