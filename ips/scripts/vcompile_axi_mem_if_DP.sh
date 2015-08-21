#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling AXI memory interface...${NC}"

echo "${Green}library: axi_mem_if_DP_lib ${NC}"

rm -rf ${MSIM_LIBS_PATH}/axi_mem_if_DP_lib

vlib ${MSIM_LIBS_PATH}/axi_mem_if_DP_lib
vmap axi_mem_if_DP_lib ${MSIM_LIBS_PATH}/axi_mem_if_DP_lib

echo "${Green}Compiling component:   ${Brown} axi_mem_if ----> DUAL PORT <-----  ${NC}"
echo "${Red}"


vlog -work axi_mem_if_DP_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_mem_if_DP/axi_mem_if_SP.sv              || exit 1
vlog -work axi_mem_if_DP_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_mem_if_DP/axi_mem_if_DP.sv              || exit 1
vlog -work axi_mem_if_DP_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_mem_if_DP/axi_read_only_ctrl.sv         || exit 1
vlog -work axi_mem_if_DP_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_mem_if_DP/axi_write_only_ctrl.sv        || exit 1

echo "${Cyan}--> AXI memory interface compilation complete! ${NC}"





