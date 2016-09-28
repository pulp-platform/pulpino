if { ![info exists ::env(BOARD) ]} {
  set ::env(BOARD) "zedboard"
}

if { ![info exists ::env(XILINX_PART)] } {
  set ::env(XILINX_PART) "xc7z020clg484-1"
}

if { ![info exists ::env(XILINX_BOARD)] } {
  set ::env(XILINX_BOARD) "em.avnet.com:zynq:zed:c"
}


# create project
create_project pulpino . -part $::env(XILINX_PART)
set_property board $::env(XILINX_BOARD) [current_project]

source tcl/ips_inc_dirs.tcl

# set up meaningful errors
source ../common/messages.tcl

source tcl/ips_src_files.tcl
source tcl/src_files.tcl

# add memory cuts
add_files -norecurse $FPGA_IPS/xilinx_mem_8192x32/ip/xilinx_mem_8192x32.dcp

source ./tcl/ips_add_files.tcl

# add components
add_files -norecurse $SRC_COMPONENTS

# add pulpino
add_files -norecurse $SRC_PULPINO

# set pulpino_top as top
set_property top pulpino [current_fileset]

# needed only if used in batch mode
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

add_files -fileset constrs_1 -norecurse constraints.xdc

# save area
set_property strategy Flow_AreaOptimized_High [get_runs synth_1]

# run synthesis
# first try will fail
catch {synth_design -rtl -name rtl_1 -verilog_define PULP_FPGA_EMUL=1 -verilog_define RISCV -flatten_hierarchy full -gated_clock_conversion on -constrset constrs_1}

update_compile_order -fileset sources_1

synth_design -rtl -name rtl_1 -verilog_define PULP_FPGA_EMUL=1 -verilog_define RISCV -flatten_hierarchy full -gated_clock_conversion on -constrset constrs_1

#set_property STEPS.SYNTH_DESIGN.ARGS.KEEP_EQUIVALENT_REGISTERS true [get_runs synth_1]
#set_property STEPS.SYNTH_DESIGN.ARGS.RESOURCE_SHARING off [get_runs synth_1]
#set_property STEPS.SYNTH_DESIGN.ARGS.NO_LC true [get_runs synth_1]
launch_runs synth_1
wait_on_run synth_1

# create reports
exec mkdir -p reports/
exec rm -rf reports/*
check_timing                                                            -file reports/pulpino.check_timing.rpt 
report_timing -max_paths 100 -nworst 100 -delay_type max -sort_by slack -file reports/pulpino.timing_WORST_100.rpt
report_timing -nworst 1 -delay_type max -sort_by group                  -file reports/pulpino.timing.rpt
report_utilization -hierarchical                                        -file reports/pulpino.utilization.rpt

# save EDIF netlist
open_run synth_1
write_edif -force pulpino.edf
write_verilog -force -mode synth_stub pulpino_stub.v
write_verilog -force -mode funcsim pulpino_funcsim.v
