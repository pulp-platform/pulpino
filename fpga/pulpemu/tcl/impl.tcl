# false paths
#source tcl/false_paths.tcl

# clocks
create_clock -period 20.000 -name ref_clk_i  -waveform { 0.0 10.0 } [get_nets {ref_clk_i}]
#create_clock -period 10.000 -name ps7_clk -waveform { 0.0 5.0 } [get_pins {ps7_wrapper_i/ps7_i/processing_system7_0/inst/PS7_i/FCLKCLK[0]}]

set_property PACKAGE_PIN T22 [get_ports {LD0}];  # "LD0"
set_property PACKAGE_PIN T21 [get_ports {LD1}];  # "LD1"
set_property PACKAGE_PIN U22 [get_ports {LD2}];  # "LD2"
set_property PACKAGE_PIN U21 [get_ports {LD3}];  # "LD3"
set_property PACKAGE_PIN V22 [get_ports {LD4}];  # "LD4"
set_property PACKAGE_PIN W22 [get_ports {LD5}];  # "LD5"
set_property PACKAGE_PIN U19 [get_ports {LD6}];  # "LD6"
set_property PACKAGE_PIN U14 [get_ports {LD7}];  # "LD7"

set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 33]];

# physical constraints
# source tcl/floorplan.xdc

save_constraints

# set for RuntimeOptimized implementation
set_property "steps.opt_design.args.directive" "RuntimeOptimized" [get_runs impl_1]
set_property "steps.place_design.args.directive" "RuntimeOptimized" [get_runs impl_1]
set_property "steps.route_design.args.directive" "RuntimeOptimized" [get_runs impl_1]

launch_runs impl_1
wait_on_run impl_1
launch_runs impl_1 -to_step write_bitstream
wait_on_run impl_1

# report area utilization
report_utilization -hierarchical -hierarchical_depth 1 -file pulpemu.txt
report_utilization -hierarchical -hierarchical_depth 2 -cells pulpino_wrap_i -file pulpino.txt

# output Verilog netlist + SDC for timing simulation
write_verilog -force -mode timesim -cell pulpino_wrap_i ../simu/pulpino_impl.v
write_sdf     -force -cell pulpino_wrap_i ../simu/pulpino_impl.sdf

if { [info exists $::env(PROBES)] } {
   # create new design run for probes
   #create_run impl_2 -flow {Vivado Implementation 2014}
   create_run impl_2 -parent_run synth_1 -flow {Vivado Implementation 2014}
   current_run [get_runs impl_2]
   set_property incremental_checkpoint pulpemu.runs/impl_1/pulpemu_top_routed.dcp [get_runs impl_2]
   set_property strategy Flow_RuntimeOptimized [get_runs impl_2]
   open_run synth_1
   #link_design -name netlist_1
   source tcl/probes.tcl
   save_constraints
   reset_run impl_2

   # set for RuntimeOptimized implementation
   set_property "steps.opt_design.args.directive" "RuntimeOptimized" [get_runs impl_2]
   set_property "steps.place_design.args.directive" "RuntimeOptimized" [get_runs impl_2]
   set_property "steps.route_design.args.directive" "RuntimeOptimized" [get_runs impl_2]

   launch_runs impl_2 -to_step write_bitstream
   wait_on_run impl_2
}

