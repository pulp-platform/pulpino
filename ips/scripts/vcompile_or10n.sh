#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling or10n processor core... ${NC}"

rm -rf ${MSIM_LIBS_PATH}/or10n_lib

vlib ${MSIM_LIBS_PATH}/or10n_lib
vmap or10n_lib ${MSIM_LIBS_PATH}/or10n_lib

echo "${Green}Compiling component:   ${Brown} or10n_core ${NC}"
echo "${Red}"


vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/id_stage.sv             || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/load_store_unit.sv      || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/wb_stage.sv             || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/ex_stage.sv             || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/if_stage.sv             || exit 1

vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/controller.sv           || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/exc_controller.sv       || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/alu.sv                  || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/mult.sv                 || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/hwloop_controller.sv    || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/hwloop_regs.sv          || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/sp_registers.sv         || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/or10n_core.sv           || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/debug_unit.sv           || exit 1
vlog -quiet -sv -work or10n_lib +incdir+${IPS_PATH}/or10n/include ${IPS_PATH}/or10n/instr_core_interface.sv || exit 1

echo "${Cyan}--> or10n processor core compilation complete! ${NC}"
