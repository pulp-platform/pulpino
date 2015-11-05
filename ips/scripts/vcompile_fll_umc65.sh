#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling FLL... ${NC}"

echo "${Green}library: fll_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/fll_lib

vlib ${MSIM_LIBS_PATH}/fll_lib
vmap fll_lib ${MSIM_LIBS_PATH}/fll_lib

echo "${Green}Compiling component:   ${Brown} fll_top ${NC}"
echo "${Red}"

vcom -work fll_lib -quiet ${IPS_PATH}/fll/VHDLTools.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/RealARITH.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/reset_synchronizer.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/synchronizer.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/umcL65_LL_DCO_model.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/umcL65_LL_FLL.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/umcL65_LL_FLL_clk_period_quantizer.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/umcL65_LL_FLL_digital.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/umcL65_LL_FLL_dither_pattern_gen.vhd
vcom -work fll_lib -quiet ${IPS_PATH}/fll/umcL65_LL_FLL_loop_filter_and_control.vhd

echo "${Cyan}--> FLL compilation complete! ${NC}"