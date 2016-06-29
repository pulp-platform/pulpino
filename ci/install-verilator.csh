#!/bin/tcsh

rm -rf ./verilator           || true
rm -rf ./verilator/build     || true

mkdir -p ./verilator/
mkdir -p ./verilator/build

wget https://github.com/diadatp/verilator/archive/verilator_3_882.tar.gz
tar -xzvf verilator_3_882.tar.gz && cd verilator-verilator_3_882
autoconf && ./configure --prefix="${PWD}/build/" && make && make test && make install

