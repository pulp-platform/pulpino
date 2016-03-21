#!/bin/tcsh

if (! $?VSIM_PATH ) then
  setenv VSIM_PATH      `pwd`
endif

if (! $?PULP_PATH ) then
  setenv PULP_PATH      `pwd`/../../
endif

setenv MSIM_LIBS_PATH ${VSIM_PATH}/modelsim_libs

setenv IPS_PATH       ${PULP_PATH}/ips
setenv RTL_PATH       ${PULP_PATH}/rtl
setenv TB_PATH        ${PULP_PATH}/tb

source ${PULP_PATH}/vsim/vcompile/colors.csh
