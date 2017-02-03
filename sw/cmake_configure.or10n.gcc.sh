#!/bin/bash

OBJDUMP=`which or1kle-elf-objdump`
OBJCOPY=`which or1kle-elf-objcopy`

COMPILER=`which or1kle-elf-clang`
COMPILER=`which or1kle-elf-gcc`
TARGET_C_FLAGS="-O3"
GCC_ETH=1

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
    -DGCC_ETH="$GCC_ETH"

# Add -G "Ninja" to the cmake call above to use ninja instead of make
