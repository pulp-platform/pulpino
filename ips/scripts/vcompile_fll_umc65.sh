#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling FLL... ${NC}"

echo "${Green}library: fll_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/fll_lib

vlib ${MSIM_LIBS_PATH}/fll_lib
vmap fll_lib ${MSIM_LIBS_PATH}/fll_lib

echo "${Green}Compiling component:   ${Brown} fll_top ${NC}"
echo "${Red}"

vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/VHDLTools.vhd                             || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/RealARITH.vhd                             || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/reset_synchronizer.vhd                    || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/synchronizer.vhd                          || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/umcL65_LL_FLL_settling_monitor.vhd        || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/behavioral/umcL65_LL_DCO_model.vhd                              || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/umcL65_LL_FLL_model.vhd                   || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/umcL65_LL_FLL_clk_period_quantizer.vhd    || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/umcL65_LL_FLL_digital.vhd                 || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/umcL65_LL_FLL_dither_pattern_gen.vhd      || exit 1
vcom -work fll_lib -quiet /usr/pack/umc-65-kgf/eth/umcL65_LL_FLL_v2.0_20151123/RTL/umcL65_LL_FLL_digital/umcL65_LL_FLL_loop_filter_and_control.vhd || exit 1

echo "${Cyan}--> FLL compilation complete! ${NC}"3a