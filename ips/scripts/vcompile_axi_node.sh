#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling AXI node... ${NC}"

echo "${Green}library: axi_node_lib ${NC}"

rm -rf ${MSIM_LIBS_PATH}/axi_node_lib

vlib ${MSIM_LIBS_PATH}/axi_node_lib
vmap axi_node_lib ${MSIM_LIBS_PATH}/axi_node_lib

echo "${Green}Compiling component:   ${Brown} axi_node ${NC}"
echo "${Red}"

vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_address_decoder_AR.sv  || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_address_decoder_AW.sv  || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_address_decoder_BR.sv  || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_address_decoder_BW.sv  || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_address_decoder_DW.sv  || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_AR_allocator.sv        || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_ArbitrationTree.sv     || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_AW_allocator.sv        || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_BR_allocator.sv        || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_BW_allocator.sv        || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_DW_allocator.sv        || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_FanInPrimitive_Req.sv  || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_multiplexer.sv         || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_node.sv                || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_regs_top.sv             || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_request_block.sv       || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_response_block.sv      || exit 1
vlog -work axi_node_lib -quiet -sv +incdir+${IPS_PATH}/axi/axi_node ${IPS_PATH}/axi/axi_node/axi_RR_Flag_Req.sv         || exit 1

echo "${Cyan}--> AXI node compilation complete! ${NC}"
