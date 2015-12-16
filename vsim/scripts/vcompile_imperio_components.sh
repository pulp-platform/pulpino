#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling imperio components... ${NC}"

echo "${Green}Compiling component:   ${Brown} Imperio components ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

set LIB=imperio_components_lib

echo "${Green}library: ${LIB} ${NC}"
rm -rf ${MSIM_LIBS_PATH}/${LIB}

vlib ${MSIM_LIBS_PATH}/${LIB}
vmap ${LIB} ${MSIM_LIBS_PATH}/${LIB}

vlog -work ${LIB} -quiet +define+FUNCTIONAL /usr/pack/umc-65-kgf/umc/ll/uk65lscllmvbbl/b03/verilog/uk65lscllmvbbl_sdf30.v        || exit 1
vlog -work ${LIB} -quiet +define+functional   /usr/pack/umc-65-kgf/umc/ll/u065gioll25mvir/b04/verilog/u065gioll25mvir.v          || exit 1
vlog -work ${LIB} -quiet +delay_mode_zero   /usr/pack/umc-65-kgf/faraday/ll/memaker/201301.1.1/verilog.dz/SHKA65_1024X8X4CM8.v   || exit 1


echo "${Cyan}--> Imperio components compilation complete! ${NC}"