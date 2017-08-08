#!/bin/tcsh


wget http://www.veripool.org/ftp/verilator-3.884.tgz
tar -xzvf verilator-3.884.tgz && cd verilator-3.884/  && mkdir build
autoconf && ./configure --prefix="${PWD}/build/" && make && make test && make install

