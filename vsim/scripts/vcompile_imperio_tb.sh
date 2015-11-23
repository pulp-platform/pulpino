#!/bin/tcsh
source scripts/colors.sh


echo "${Green} -> Compiling Imperio Testbench..."

echo "${Green}Compiling component:   ${Brown} work.imperio_tb ${NC}"
echo "${Green}library: work ${NC}"
echo "${Red}"

vlog -quiet -sv +incdir+../tb                            								  ../tb/uart.sv                    || exit 1
vlog -quiet -sv +incdir+../imperio/sourcecode/tb +incdir+../tb +incdir+../rtl/include/    ../imperio/sourcecode/tb/tb.sv   || exit 1
vlog -quiet -sv                                                        ../imperio/sourcecode/tb/spi_master_padframe.sv     || exit 1

vlog -quiet -sv +incdir+../tb -dpiheader        ../tb/jtag_dpi/dpiheader.h ../tb/jtag_dpi.sv       || exit 1
vlog -quiet -64 -ccflags "-I../tb/jtag_dpi/"    ../tb/jtag_dpi/jtag_dpi.c                          || exit 1

echo "${Cyan} Testbench files have been compiled Succesfully${NC}"

