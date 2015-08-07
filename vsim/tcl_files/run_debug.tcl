#!/bin/bash 
# \
exec vsim -64 -do "$0"

set TB            tb
set VSIM_FLAGS    "-gENABLE_VPI=1 -pli ./work/jtag_vpi.so"

source ./tcl_files/vsim.tcl

