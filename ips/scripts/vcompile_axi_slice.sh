#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling AXI register slice... ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif

echo "${Green}library: axi_slice_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/axi_slice_lib

vlib ${MSIM_LIBS_PATH}/axi_slice_lib
vmap axi_slice_lib ${MSIM_LIBS_PATH}/axi_slice_lib

echo "${Green}Compiling component:   ${Brown} axi_slice ${NC}"
echo "${Red}"

vlog -work axi_slice_lib -quiet -sv ${IPS_PATH}/axi/axi_slice/axi_buffer.sv      || exit 1
vlog -work axi_slice_lib -quiet -sv ${IPS_PATH}/axi/axi_slice/axi_ar_buffer.sv   || exit 1
vlog -work axi_slice_lib -quiet -sv ${IPS_PATH}/axi/axi_slice/axi_aw_buffer.sv   || exit 1
vlog -work axi_slice_lib -quiet -sv ${IPS_PATH}/axi/axi_slice/axi_b_buffer.sv    || exit 1
vlog -work axi_slice_lib -quiet -sv ${IPS_PATH}/axi/axi_slice/axi_r_buffer.sv    || exit 1
vlog -work axi_slice_lib -quiet -sv ${IPS_PATH}/axi/axi_slice/axi_w_buffer.sv    || exit 1
vlog -work axi_slice_lib -quiet -sv ${IPS_PATH}/axi/axi_slice/axi_slice.sv       || exit 1

echo "${Cyan}--> AXI register slice compilation complete! ${NC}"
