#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling Advanced Debug Interface... ${NC}"

echo "${Green}library: adv_dbg_if_lib ${NC}"

rm -rf ${MSIM_LIBS_PATH}/adv_dbg_if_lib

vlib ${MSIM_LIBS_PATH}/adv_dbg_if_lib
vmap adv_dbg_if_lib ${MSIM_LIBS_PATH}/adv_dbg_if_lib

echo "${Green}Compiling component:   ${Brown} adv_dbg_if ${NC}"
echo "${Red}"

vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_axi_biu.sv         || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_axi_module.sv      || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_crc32.v            || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_or1k_biu.sv        || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_or1k_module.sv     || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_or1k_status_reg.sv || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_top.sv             || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/bytefifo.v              || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/syncflop.v              || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/syncreg.v               || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_tap_top.v          || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adv_dbg_if.sv           || exit 1
vlog -work adv_dbg_if_lib +incdir+${IPS_PATH}/adv_dbg_if/rtl -quiet -sv ${IPS_PATH}/adv_dbg_if/rtl/adbg_axionly_top.sv     || exit 1

echo "${Cyan}--> Advanced Debug Interface compilation complete! ${NC}"
