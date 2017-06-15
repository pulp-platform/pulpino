#!/bin/tcsh

cd ./sw/build
ctest -L sequential_test -j4 --timeout 4000 --output-on-failure
