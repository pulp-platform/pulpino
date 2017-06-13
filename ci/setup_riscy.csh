#!/bin/tcsh

setenv PATH /usr/scratch2/larain/jenkins/artefacts/riscvv2_gcc/2.3.7/bin/:${PWD}/verilator-3.884/build/bin:$PATH

./update-ips.py git@iis-git.ee.ethz.ch

set OBJDUMP=`which riscv32-unknown-elf-objdump`
set OBJCOPY=`which riscv32-unknown-elf-objcopy`
set TARGET_C_FLAGS="-m32 -O3"
set COMPILER=`which riscv32-unknown-elf-gcc`
set GCC_MARCH="IMXpulpv2"

set RVC=0
set USE_RISCY=1
set RISCY_RV32F=0
set USE_ZERO_RISCY=0
set ZERO_RV32M=0
set ZERO_RV32E=0
set ARDUINO_LIB=1

set TB="run.tcl"

if (! ($?VSIM) ) then
  set VSIM=`which vsim`
endif

set GIT_DIR=../../
set SIM_DIR="$GIT_DIR/vsim"
set SW_DIR="$GIT_DIR/sw"


rm -rf ./sw/build || true
rm -rf ./sw/build-rvc || true

mkdir -p ./sw/build
mkdir -p ./sw/build-rvc


cd ./sw/build

cmake-3.3.0 "$SW_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPULP_MODELSIM_DIRECTORY="$SIM_DIR" \
    -DVSIM="$VSIM" \
    -DRVC="$RVC" \
    -DUSE_RISCY="$USE_RISCY" \
    -DRISCY_RV32F="$RISCY_RV32F" \
    -DUSE_ZERO_RISCY="$USE_ZERO_RISCY" \
    -DZERO_RV32M="$ZERO_RV32M" \
    -DZERO_RV32E="$ZERO_RV32E" \
    -DGCC_MARCH="$GCC_MARCH" \
    -DARDUINO_LIB="$ARDUINO_LIB" \
    -DCMAKE_C_COMPILER="$COMPILER" \
    -DCMAKE_C_FLAGS="$TARGET_C_FLAGS" \
    -DCMAKE_OBJCOPY="$OBJCOPY" \
    -DCMAKE_OBJDUMP="$OBJDUMP" \
    -DARG_TB="$TB" \
    -G "Ninja"

# compile RTL
ninja vcompile || exit 1
# compile verilator
#ninja vericompile || exit 1

# compile SW
ninja || exit 1

cd ../../

cd ./sw/build-rvc

set RVC=1

cmake-3.3.0 "$SW_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPULP_MODELSIM_DIRECTORY="$SIM_DIR" \
    -DVSIM="$VSIM" \
    -DRVC="$RVC" \
    -DUSE_RISCY="$USE_RISCY" \
    -DRISCY_RV32F="$RISCY_RV32F" \
    -DUSE_ZERO_RISCY="$USE_ZERO_RISCY" \
    -DZERO_RV32M="$ZERO_RV32M" \
    -DZERO_RV32E="$ZERO_RV32E" \
    -DGCC_MARCH="$GCC_MARCH" \
    -DARDUINO_LIB="$ARDUINO_LIB" \
    -DCMAKE_C_COMPILER="$COMPILER" \
    -DCMAKE_C_FLAGS="$TARGET_C_FLAGS" \
    -DCMAKE_OBJCOPY="$OBJCOPY" \
    -DCMAKE_OBJDUMP="$OBJDUMP" \
    -DARG_TB="$TB" \
    -G "Ninja"

# compile SW
ninja || exit 1

