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

if (! ($?VSIM) ) then
  set VSIM=`which vsim`
endif

set GIT_DIR=../../
set SIM_DIR="$GIT_DIR/vsim"
set SW_DIR="$GIT_DIR/sw"

mkdir -p ./sw/build
mkdir -p ./sw/build-rvc
mkdir -p ./sw/build-imperio
mkdir -p ./sw/build-imperio-rvc

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
    -DARG_TB="run.tcl" \
    -G "Ninja"

# compile RTL
ninja vcompile || exit 1

# compile SW
ninja || exit 1

cd ../..

set RVC=1
cd sw/build-rvc

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
    -DARG_TB="run.tcl" \
    -G "Ninja"

# compile RTL
ninja vcompile || exit 1

# compile SW
ninja || exit 1

#############################################
#                                           #
#  Imperio Tests                            #
#                                           #
#############################################

cd ../..

set RVC=0
cd sw/build-imperio

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
    -DARG_TB="run_imperio.tcl" \
    -G "Ninja"

# compile RTL
ninja vcompile || exit 1

# compile SW
ninja || exit 1

cd ../..

set RVC=1
cd sw/build-imperio-rvc

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
    -DARG_TB="run_imperio.tcl" \
    -G "Ninja"

# compile RTL
ninja vcompile || exit 1

# compile SW
ninja || exit 1

# TODO: Add SPI, JTAG