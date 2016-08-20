#!/bin/bash
# \
exec vsim -64 -do "$0"

set TB_TEST $::env(TB_TEST)

set TB            tb
set VSIM_FLAGS    "-GTEST=\"$TB_TEST\""
set MEMLOAD       "PRELOAD"

source ./tcl_files/config/vsim.tcl
