#!/bin/tcsh

setenv PATH /usr/scratch/badile-j68-1/msc15f8/custom-riscv-gcc/env-stable/bin:$PATH

./update-ips.py

set OBJDUMP=`which riscv32-unknown-elf-objdump`
set OBJCOPY=`which riscv32-unknown-elf-objcopy`
set TARGET_C_FLAGS="-m32 -O3"
set COMPILER="riscv32-unknown-elf-gcc"
set SIZE=`which riscv32-unknown-elf-size`
set RISCV=1
set RVC=0

set VSIM=`which vsim`

set GIT_DIR=../../
set SIM_DIR="$GIT_DIR/vsim"
set SW_DIR="$GIT_DIR/sw"

mkdir -p ./sw/build
cd sw/build

cmake-3.3.0 "$SW_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPULP_MODELSIM_DIRECTORY="$SIM_DIR" \
    -DVSIM="$VSIM" \
    -DCMAKE_C_COMPILER="$COMPILER" \
    -DCMAKE_C_FLAGS="$TARGET_C_FLAGS" \
    -DRISCV=$RISCV \
    -DRVC=$RVC \
    -DCMAKE_OBJCOPY="$OBJCOPY" \
    -DCMAKE_OBJDUMP="$OBJDUMP" \
    -DCMAKE_SIZE="$SIZE" \
    -G "Ninja"

# Add -G "Ninja" to the cmake call above to use ninja instead of make
# compile RTL
ninja vcompile

# compile SW
ninja
