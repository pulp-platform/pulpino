set TB            imperio_tb
set MEMLOAD       "SPI"
set VSIM_FLAGS    "-gDUMP_VCD=5000"

source ./tcl_files/vsim_pl.tcl

# do a vcd dump
vcd file ./pulpino_chip.vcd
vcd add -r imperio_tb/top_i/*
vcd off