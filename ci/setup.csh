#!/bin/tcsh

setenv PATH /usr/scratch2/larain/haugoug/artefacts/riscv_gcc/2.2.8/bin/:$PATH

./update-ips.py git@iis-git.ee.ethz.ch   || exit 1

set OBJDUMP=`which riscv32-unknown-elf-objdump`
set OBJCOPY=`which riscv32-unknown-elf-objcopy`
set TARGET_C_FLAGS="-m32 -O3"
set COMPILER=`which riscv32-unknown-elf-gcc`
set GCC_ETH=1

set TB="run.tcl"

if (! ($?VSIM) ) then
  set VSIM=`which vsim`
endif

set GIT_DIR=../../
set SIM_DIR="$GIT_DIR/vsim"
set SW_DIR="$GIT_DIR/sw"

rm -rf ./sw/build     || true
rm -rf ./sw/build-rvc || true

mkdir -p ./sw/build
mkdir -p ./sw/build-rvc

cd ./sw/build

cmake-3.3.0 "$SW_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPULP_MODELSIM_DIRECTORY="$SIM_DIR" \
    -DVSIM="$VSIM" \
    -DCMAKE_C_COMPILER="$COMPILER" \
    -DCMAKE_C_FLAGS="$TARGET_C_FLAGS" \
    -DCMAKE_OBJCOPY="$OBJCOPY" \
    -DCMAKE_OBJDUMP="$OBJDUMP" \
    -DARG_TB="$TB" \
    -DGCC_ETH="$GCC_ETH" \
    -G "Ninja"

# compile RTL
ninja vcompile || exit 1

# compile SW
ninja || exit 1

cd ../../

cd ./sw/build-rvc

set TARGET_C_FLAGS="$TARGET_C_FLAGS -mrvc"

cmake-3.3.0 "$SW_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPULP_MODELSIM_DIRECTORY="$SIM_DIR" \
    -DVSIM="$VSIM" \
    -DCMAKE_C_COMPILER="$COMPILER" \
    -DCMAKE_C_FLAGS="$TARGET_C_FLAGS" \
    -DCMAKE_OBJCOPY="$OBJCOPY" \
    -DCMAKE_OBJDUMP="$OBJDUMP" \
    -DARG_TB="$TB" \
    -DGCC_ETH="$GCC_ETH" \
    -G "Ninja"

# compile SW
ninja || exit 1
