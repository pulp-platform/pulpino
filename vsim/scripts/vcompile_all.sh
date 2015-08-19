#!/bin/tcsh
clear
source scripts/colors.sh


\rm -rf modelsim_libs
vlib modelsim_libs

\rm -rf work
vlib work

setenv MSIM_LIBS_PATH `pwd`/modelsim_libs
setenv IPS_PATH `pwd`/../ips
setenv RTL_PATH `pwd`/../rtl

echo ""
echo "${Green}--> Compiling PULP Platform... ${NC}"

source ${RTL_PATH}/scripts/vcompile_standalone.sh     || exit 1
source ${IPS_PATH}/scripts/vcompile_or10n.sh          || exit 1
source ${IPS_PATH}/scripts/vcompile_riscv.sh          || exit 1
source ${IPS_PATH}/scripts/vcompile_scm.sh            || exit 1
source ${IPS_PATH}/scripts/vcompile_axi_node.sh       || exit 1
source ${IPS_PATH}/scripts/vcompile_axi2apb.sh        || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_uart.sh       || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_spi_master.sh || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_gpio.sh       || exit 1
source ${IPS_PATH}/scripts/vcompile_common_cells.sh   || exit 1
source ${IPS_PATH}/scripts/vcompile_adv_dbg_if.sh     || exit 1
source ${IPS_PATH}/scripts/vcompile_axi_spi_slave.sh  || exit 1
source ${IPS_PATH}/scripts/vcompile_axi_slice_dc.sh   || exit 1

source ./scripts/vcompile_tb.sh                       || exit 1

echo "${Green}--> PULP platform compilation Complete! ${NC}"
echo ""
