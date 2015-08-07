#!/bin/bash 
# \
exec vsim -64 -do "$0"

set TB            tb
set VSIM_FLAGS    ""

source ./tcl_files/vsim.tcl

