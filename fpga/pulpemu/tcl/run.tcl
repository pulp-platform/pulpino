source ./tcl/common.tcl

# create project
create_project pulpemu . -part $::env(XILINX_PART)

if { [info exists ::env(XILINX_BOARD) ] } {
  set_property board_part $::env(XILINX_BOARD) [current_project]
}

# create block design
source tcl/ps7_bd.tcl

# validate
validate_bd_design

# generate
generate_target all [get_files ./pulpemu.srcs/sources_1/bd/ps7/ps7.bd]
make_wrapper -files [get_files ./pulpemu.srcs/sources_1/bd/ps7/ps7.bd] -top
add_files -norecurse ./pulpemu.srcs/sources_1/bd/ps7/hdl/ps7_wrapper.v

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

# add pulpemu_top
add_files -norecurse ../rtl/clk_rst_gen.v
add_files -norecurse ../rtl/pulpemu_top.v
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

# add pulpino
if { $::env(USE_ZERO_RISCY)==0 & $::env(RISCY_RV32F)==1 } {
    add_files -norecurse ../pulpino/pulpino.edn \
	../pulpino/pulpino_stub.v \
	../ips/xilinx_fp_fma/ip/xilinx_fp_fma_stub.vhdl \
	../ips/xilinx_fp_fma/ip/xilinx_fp_fma_stub.v \
	../pulpino/xilinx_fp_fma_floating_point_v7_0_viv.edn \
	../pulpino/xilinx_fp_fma_mult_gen_v12_0_viv.edn \
	../ips/xilinx_clock_manager/ip/xilinx_clock_manager.dcp
} else {
    add_files -norecurse ../pulpino/pulpino.edn \
	../pulpino/pulpino_stub.v \
	../ips/xilinx_clock_manager/ip/xilinx_clock_manager.dcp
}

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1
get_property source_mgmt_mode [current_project]
set_property source_mgmt_mode DisplayOnly [current_project]
get_property source_mgmt_mode [current_project]

# Synthesis strategy: save area
set_property strategy Flow_AreaOptimized_High [get_runs synth_1]

# elaborate
synth_design -rtl -name rtl_1

# Create constraints file to avoid "outdated" synthesis and implementation runs later
save_constraints

launch_runs synth_1 -jobs $CPUS
wait_on_run synth_1
open_run synth_1 -name netlist_1
# write_edif pulpemu.edf

# export hardware design for sdk
write_hwdef -force -file ./pulpemu.hwdef

# run implementation
source tcl/impl.tcl
