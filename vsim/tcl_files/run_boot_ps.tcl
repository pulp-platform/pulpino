#!/bin/bash
# \
exec vsim -64 -do "$0"

set TB            imperio_tb
set VSIM_FLAGS    ""
set MEMLOAD       "STANDALONE"

source ./tcl_files/vsim_ps.tcl

