#!/bin/bash
# \
exec vsim -64 -do "$0"

set TB            tb
set VSIM_FLAGS    "-gENABLE_VPI=1"
set MEMLOAD       "PRELOAD"

source ./tcl_files/vsim.tcl

