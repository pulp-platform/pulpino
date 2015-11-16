#!/bin/tcsh
clear
source scripts/colors.sh

# decide if we want to build for riscv or or1k
if ( ! $?PULP_CORE) then
  set PULP_CORE="riscv"
endif

\rm -rf modelsim_libs
vlib modelsim_libs

\rm -rf work
vlib work

setenv MSIM_LIBS_PATH `pwd`/modelsim_libs
setenv IPS_PATH `pwd`/../ips
setenv RTL_PATH `pwd`/../rtl

echo ""
echo "${Green}--> Compiling PULPino Platform... ${NC}"
echo ""

source ${RTL_PATH}/scripts/vcompile_standalone.sh     || exit 1

if ( $PULP_CORE == "riscv" ) then
  source ${IPS_PATH}/scripts/vcompile_riscv.sh        || exit 1
else
  source ${IPS_PATH}/scripts/vcompile_or10n.sh        || exit 1
endif

# IP blocks
source ${IPS_PATH}/scripts/vcompile_axi_node.sh       || exit 1
source ${IPS_PATH}/scripts/vcompile_axi2apb.sh        || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_uart.sh       || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_spi_master.sh || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_gpio.sh       || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_event_unit.sh || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_timer.sh      || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_i2c.sh        || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_fll_if.sh     || exit 1
source ${IPS_PATH}/scripts/vcompile_apb_pulpino.sh    || exit 1
source ${IPS_PATH}/scripts/vcompile_adv_dbg_if.sh     || exit 1
source ${IPS_PATH}/scripts/vcompile_axi_spi_slave.sh  || exit 1
source ${IPS_PATH}/scripts/vcompile_axi_slice.sh      || exit 1
source ${IPS_PATH}/scripts/vcompile_axi_slice_dc.sh   || exit 1
source ${IPS_PATH}/scripts/vcompile_axi_mem_if_DP.sh  || exit 1

# ASIC specific
source ${IPS_PATH}/scripts/vcompile_fll_umc65.sh      || exit 1
source ./scripts/vcompile_imperio.sh                  || exit 1

source ./scripts/vcompile_tb.sh                       || exit 1

echo ""
echo "${Green}--> PULPino platform compilation complete! ${NC}"
echo ""
