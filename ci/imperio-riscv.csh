#!/bin/tcsh

cd ./sw/build-imperio
ctest -L riscv_test -j4 --timeout 3000 --output-on-failure