#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling AXI dual clock FIFO... ${NC}"

echo "${Green}library: axi2per_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/axi_slice_dc_lib

vlib ${MSIM_LIBS_PATH}/axi_slice_dc_lib
vmap axi_slice_dc_lib ${MSIM_LIBS_PATH}/axi_slice_dc_lib

echo "${Green}Compiling component:   ${Brown} axi_slice_dc_master ${NC}"
echo "${Red}"

vlog -work axi_slice_dc_lib -quiet -sv ${IPS_PATH}/axi/axi_slice_dc/dc_data_buffer.v            || exit 1
vlog -work axi_slice_dc_lib -quiet -sv ${IPS_PATH}/axi/axi_slice_dc/dc_full_detector.v          || exit 1
vlog -work axi_slice_dc_lib -quiet -sv ${IPS_PATH}/axi/axi_slice_dc/dc_synchronizer.v           || exit 1
vlog -work axi_slice_dc_lib -quiet -sv ${IPS_PATH}/axi/axi_slice_dc/dc_token_ring_fifo_din.v    || exit 1
vlog -work axi_slice_dc_lib -quiet -sv ${IPS_PATH}/axi/axi_slice_dc/dc_token_ring_fifo_dout.v   || exit 1
vlog -work axi_slice_dc_lib -quiet -sv ${IPS_PATH}/axi/axi_slice_dc/dc_token_ring.v             || exit 1
vlog -work axi_slice_dc_lib -quiet -sv ${IPS_PATH}/axi/axi_slice_dc/axi_slice_dc_master.sv      || exit 1

echo "${Green}Compiling component:   ${Brown} axi_slice_dc_slave ${NC}"
echo "${Red}"

vlog -work axi_slice_dc_lib -quiet -sv ${IPS_PATH}/axi/axi_slice_dc/axi_slice_dc_slave.sv       || exit 1

echo "${Cyan}--> AXI dual clock FIFO compilation complete! ${NC}"
