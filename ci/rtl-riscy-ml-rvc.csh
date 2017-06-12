#!/bin/tcsh

cd ./sw/build-rvc_riscy
ctest -L ml_tests -j4 --timeout 3000 --output-on-failure
