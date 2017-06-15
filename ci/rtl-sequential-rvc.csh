#!/bin/tcsh

cd ./sw/build-rvc
ctest -L sequential_test -j4 --timeout 4000 --output-on-failure
