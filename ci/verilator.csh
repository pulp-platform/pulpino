#!/bin/tcsh

cd ./sw/build
ctest -L verilator_tests -j4 --timeout 3000 --output-on-failure
