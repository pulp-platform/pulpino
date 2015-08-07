#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling pulp_common_cells... ${NC}"

echo "${Green}library: pulp_common_cells ${NC}"
rm -rf ${MSIM_LIBS_PATH}/pulp_common_cells_lib

vlib ${MSIM_LIBS_PATH}/pulp_common_cells_lib
vmap pulp_common_cells_lib ${MSIM_LIBS_PATH}/pulp_common_cells_lib

echo "${Green}Compiling component:   ${Brown} pulp_common_cells ${NC}"
echo "${Red}"

vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_buffer.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_inverter.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_gating.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_mux2.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_xor2.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_buffer.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_level_shifter_in.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_level_shifter_in_clamp.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_level_shifter_out.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_level_shifter_out_clamp.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_and2.sv

# //vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/pulp_clock_buffer_cmos028fdsoi.sv
# //vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/pulp_clock_inverter_cmos028fdsoi.sv
# //vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/pulp_clock_gating_cmos028fdsoi.sv
# //vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/pulp_clock_mux2_cmos028fdsoi.sv
# //vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/pulp_clock_xor2_cmos028fdsoi.sv
# //vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/pulp_level_shifter_in_cmos028fdsoi.sv
# //vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/pulp_level_shifter_out_cmos028fdsoi.sv

vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_buffer.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_inverter.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_gating.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_mux2.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_xor2.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_level_shifter_in.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_level_shifter_in_clamp.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_level_shifter_out.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_level_shifter_out_clamp.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/cluster_clock_and2.sv

#//vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/cluster_clock_buffer_cmos028fdsoi.sv
#//vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/cluster_clock_inverter_cmos028fdsoi.sv
#//vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/cluster_clock_gating_cmos028fdsoi.sv
#//vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/cluster_clock_mux2_cmos028fdsoi.sv
#//vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/cluster_clock_xor2_cmos028fdsoi.sv
#//vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/cluster_level_shifter_in_cmos028fdsoi.sv
#//vlog -sv -work pulp_common_cells_lib +incdir+../rtl/includes -quiet ${IPS_PATH}/common_cells/cluster_level_shifter_out_cmos028fdsoi.sv

vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/edge_propagator_tx.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/edge_propagator_rx.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/edge_propagator.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_sync_wedge.sv

vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_clock_gating_async.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/pulp_sync_wedge.sv
vlog -sv -work pulp_common_cells_lib -quiet ${IPS_PATH}/common_cells/rstgen.sv

echo "${Cyan}--> pulp_common_cells compilation complete! ${NC}"
