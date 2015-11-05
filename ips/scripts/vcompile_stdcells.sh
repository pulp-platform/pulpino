#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling Standard cells... ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif


echo "${Green}library: std_cells_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/std_cells_lib

vlib ${MSIM_LIBS_PATH}/std_cells_lib
vmap std_cells_lib ${MSIM_LIBS_PATH}/std_cells_lib

echo "${Green}Compiling component:   ${Brown} Std cells ${NC}"
echo "${Red}"

vlog -work std_cells_lib +define+FUNCTIONAL -quiet /usr/pack/umc-65-kgf/umc/ll/uk65lscllmvbbl/b03/verilog/uk65lscllmvbbl_sdf30.v || exit 1

echo "${Cyan}--> Standard cells compilation complete! ${NC}"

