#!/bin/tcsh

if (! $?VSIM_PATH ) then
  setenv VSIM_PATH      `pwd`
endif

if (! $?PULP_PATH ) then
  setenv PULP_PATH      `pwd`/../
endif

setenv MSIM_LIBS_PATH ${VSIM_PATH}/modelsim_libs

setenv IPS_PATH       ${PULP_PATH}/ips
setenv RTL_PATH       ${PULP_PATH}/rtl
setenv TB_PATH        ${PULP_PATH}/tb

clear
source ${PULP_PATH}/vsim/vcompile/colors.csh

rm -rf modelsim_libs
vlib modelsim_libs

rm -rf work
vlib work

echo ""
echo "${Green}--> Compiling PULPino Platform... ${NC}"
echo ""

# IP blocks
source ${PULP_PATH}/vsim/vcompile/vcompile_ips.csh  || exit 1

source ${PULP_PATH}/vsim/vcompile/rtl/vcompile_pulpino.sh  || exit 1
source ${PULP_PATH}/vsim/vcompile/rtl/vcompile_tb.sh       || exit 1

echo ""
echo "${Green}--> PULPino platform compilation complete! ${NC}"
echo ""
