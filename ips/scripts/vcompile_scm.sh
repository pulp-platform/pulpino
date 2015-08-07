#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling CEA common_cells... ${NC}"

rm -rf ${MSIM_LIBS_PATH}/scm_lib

vlib ${MSIM_LIBS_PATH}/scm_lib
vmap scm_lib ${MSIM_LIBS_PATH}/scm_lib

echo "${Green}Compiling component:   ${Brown} scm ${NC}"
echo "${Red}"

vcom -work scm_lib -quiet ${IPS_PATH}/scm/scm128x32/VHDLTools.vhd || exit 1
vcom -work scm_lib -quiet ${IPS_PATH}/scm/scm128x32/SCM_pkg.vhd   || exit 1
vcom -work scm_lib -quiet ${IPS_PATH}/scm/scm128x32/MUX_opt.vhd   || exit 1
vcom -work scm_lib -quiet ${IPS_PATH}/scm/scm128x32/MUXTop.vhd    || exit 1
vcom -work scm_lib -quiet ${IPS_PATH}/scm/scm128x32/mcg.vhd       || exit 1
vcom -work scm_lib -quiet ${IPS_PATH}/scm/scm128x32/mcg_be.vhd    || exit 1

vlog -work scm_lib -quiet -sv ${IPS_PATH}/scm/generic_scm/register_file_1r_1w_be.sv  || exit 1
vlog -work scm_lib -quiet -sv ${IPS_PATH}/scm/generic_scm/register_file_1r_1w_all.sv  || exit 1
vlog -work scm_lib -quiet -sv ${IPS_PATH}/scm/generic_scm/register_file_1r_1w.sv     || exit 1
vlog -work scm_lib -quiet -sv ${IPS_PATH}/scm/generic_scm/register_file_2r_2w.sv     || exit 1
vlog -work scm_lib -quiet -sv ${IPS_PATH}/scm/generic_scm/register_file_2r_1w_asymm.sv     || exit 1
vlog -work scm_lib -quiet -sv ${IPS_PATH}/scm/generic_scm/register_file_3r_2w.sv     || exit 1

echo "${Cyan}--> scm compilation complete! ${NC}"
