#!/bin/tcsh

cd ./sw/build-imperio-rvc-spi
ctest -L riscv_test -j4 --timeout 3000 --output-on-failure
