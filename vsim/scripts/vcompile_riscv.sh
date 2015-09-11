#!/bin/tcsh
source scripts/colors.sh

setenv MSIM_LIBS_PATH `pwd`/modelsim_libs
setenv IPS_PATH `pwd`/../ips

echo ""
echo "${Green}--> Compiling riscv core... ${NC}"

source ${IPS_PATH}/scripts/vcompile_riscv.sh || exit 1

echo "${Green}--> RiscV core compilation Complete! ${NC}"
echo ""
