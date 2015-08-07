#!/bin/tcsh
source scripts/colors.sh

echo "${Green}---> Compiling AXI2APB...${NC}"

echo "${Green}library: axi2apb_lib ${NC}"

rm -rf ${MSIM_LIBS_PATH}/axi2apb_lib

vlib ${MSIM_LIBS_PATH}/axi2apb_lib
vmap axi2apb_lib ${MSIM_LIBS_PATH}/axi2apb_lib

echo "${Green}Compiling component:   ${Brown} axi2apb ${NC}"
echo "${Red}"

vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi_slice/axi_buffer.sv   || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi_slice/axi_r_buffer.sv   || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi_slice/axi_ar_buffer.sv   || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi_slice/axi_aw_buffer.sv   || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi_slice/axi_w_buffer.sv   || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi_slice/axi_b_buffer.sv   || exit 1

vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi2apb/axi2apb_cmd.sv   || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi2apb/axi2apb_ctrl.sv  || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi2apb/axi2apb_mux.sv   || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi2apb/axi2apb_rd.sv    || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi2apb/axi2apb_wr.sv    || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi2apb/prgen_fifo.sv    || exit 1
vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi2apb/axi2apb.sv       || exit 1

vlog -quiet -work axi2apb_lib -sv +incdir+${IPS_PATH}/axi/axi2apb ${IPS_PATH}/axi/axi2apb/AXI_2_APB.sv       || exit 1

echo "${Cyan}---> AXI2APB compilation complete! ${NC}"
