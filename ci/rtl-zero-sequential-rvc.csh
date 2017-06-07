#!/bin/tcsh

cd ./sw/build-rvc_zero
ctest -L sequential_test -j4 --timeout 300000 --output-on-failure
