#!/bin/tcsh

setenv PATH /usr/scratch/badile-j68-1/msc15f8/custom-riscv-gcc/env-stable/bin:$PATH

./update-ips.py

set TARGET_C_FLAGS="-m32 -O3"
set COMPILER=gcc
set RISCV=1
set RVC=0
set SIZE=`which riscv32-unknown-elf-size`
set VSIM=`which vsim-10.4c`

PULP_GIT_DIRECTORY=../../
SIM_DIRECTORY="$PULP_GIT_DIRECTORY/vsim"

mkdir -p ./sw/build
cd ./sw/build

set GIT_DIR=../../
set SIM_DIR="$GIT_DIR/fe/sim"
set SW_DIR="$GIT_DIR/sw"

set PULP_PIPELINE_PATH=$SW_DIR/sdk/

cmake-3.3.0 "$SW_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPULP_MODELSIM_DIRECTORY="$SIM_DIR" \
    -DPULP_PIPELINE_PATH="$PULP_PIPELINE_PATH" \
    -DVSIM="$VSIM" \
    -DCOMPILER=$COMPILER \
    -DCMAKE_C_FLAGS="$TARGET_C_FLAGS" \
    -DRISCV=$RISCV \
    -DRVC=$RVC \
    -DCMAKE_SIZE="$SIZE" \
    -G "Ninja"

# compile RTL
ninja vcompile

# compile SW
ninja
