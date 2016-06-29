#!/bin/tcsh


wget http://www.veripool.org/ftp/verilator-3.884.tgz
tar -xzvf verilator_3_884.tgz && cd verilator-verilator_3_884
autoconf && ./configure --prefix="${PWD}/build/" && make && make test && make install

