#!/bin/tcsh

cd ./sw/build-rvc
ctest -L ml_tests -j4 --timeout 3000 --output-on-failure
