#!/bin/tcsh
clear
source scripts/colors.sh


\rm -rf modelsim_libs
vlib modelsim_libs

\rm -rf work
vlib work

setenv MSIM_LIBS_PATH `pwd`/modelsim_libs
setenv IPS_PATH `pwd`/../ips
setenv RTL_PATH `pwd`/../rtl

echo ""
echo "${Green}--> Compiling PULPino Platform for post layout... ${NC}"
echo ""

# Hacky solution: compile imperio_tb before imperio_ps since the testbench gets recompiled with different defines enabled
source ./scripts/vcompile_imperio_tb.sh               || exit 1
source ${IPS_PATH}/scripts/vcompile_fll_umc65.sh      || exit 1
source ./scripts/vcompile_imperio_pl.sh               || exit 1
source ./scripts/vcompile_models.sh                   || exit 1

echo ""
echo "${Green}--> PULPino platform compilation complete! ${NC}"
echo ""
