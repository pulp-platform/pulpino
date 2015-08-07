#!/bin/tcsh
source scripts/colors.sh


echo "${Green} -> Compiling PULP Testbench..."

echo "${Green}Compiling component:   ${Brown} work.tb ${NC}"
echo "${Green}library: work ${NC}"
echo "${Red}"

vlog -quiet -sv +incdir+../tb ../tb/uart.sv                  || exit 1
vlog -quiet -sv +incdir+../tb ../tb/tb.sv                    || exit 1



echo "${Cyan} Testbench files have been compiled Succesfully${NC}"

