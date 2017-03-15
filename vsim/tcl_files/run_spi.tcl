#!/bin/bash
# \
exec vsim -64 -do "$0"

set TB            tb
set TB_TEST $::env(TB_TEST)
set VSIM_FLAGS    "-GTEST=\"$TB_TEST\""
set MEMLOAD       "SPI"

source ./tcl_files/config/vsim.tcl
