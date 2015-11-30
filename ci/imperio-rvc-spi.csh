#!/bin/tcsh

cd ./sw/build-imperio-rvc-spi
ctest -L imperio_tests -j4 --timeout 3000 --output-on-failure
