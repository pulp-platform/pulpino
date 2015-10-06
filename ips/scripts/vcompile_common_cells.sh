#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling pulp_common_cells... ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif


echo "${Green}library: pulp_common_cells ${NC}"
rm -rf ${MSIM_LIBS_PATH}/pulp_common_cells_lib

vlib ${MSIM_LIBS_PATH}/pulp_common_cells_lib
vmap pulp_common_cells_lib ${MSIM_LIBS_PATH}/pulp_common_cells_lib

echo "${Green}Compiling component:   ${Brown} pulp_common_cells ${NC}"
echo "${Red}"

vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_buffer.sv      || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_inverter.sv    || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_gating.sv      || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_mux2.sv        || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_xor2.sv        || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_buffer.sv            || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_and2.sv        || exit 1


vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_buffer.sv   || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_inverter.sv || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_gating.sv   || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_mux2.sv     || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_xor2.sv     || exit 1
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_and2.sv     || exit 1

vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/generic_fifo.sv           || exit 1

echo "${Cyan}--> pulp_common_cells compilation complete! ${NC}"
