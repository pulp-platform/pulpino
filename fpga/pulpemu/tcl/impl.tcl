# false paths
#source tcl/false_paths.tcl

# clocks
#create_clock -period 20.000 -name ref_clk_i  -waveform { 0.0 10.0 } [get_nets {ref_clk_i}]
#create_clock -period 10.000 -name ps7_clk -waveform { 0.0 5.0 } [get_pins {ps7_wrapper_i/ps7_i/processing_system7_0/inst/PS7_i/FCLKCLK[0]}]
#create_clock -period 10.000 -name ulpsoc_clk -waveform { 0.0 5.0 } [get_nets {ulpsoc_i/i_clk_rst_gen/clk_manager_i/clk_o}]
#create_clock -period 100 -name spi_clk -waveform { 0.0 50.0 } [ get_nets {spi_slave_clk_r}]

# pins
set_property package_pin H14     [get_ports PULP_SPI_fetch_en]
set_property is_loc_fixed true   [get_ports [list  PULP_SPI_fetch_en]]
set_property IOSTANDARD LVCMOS15 [get_ports PULP_SPI_fetch_en]
set_property PULLTYPE PULLUP     [get_ports PULP_SPI_fetch_en]
set_property package_pin J15     [get_ports PULP_SPI_eoc]
set_property is_loc_fixed true   [get_ports [list  PULP_SPI_eoc]]
set_property IOSTANDARD LVCMOS15 [get_ports PULP_SPI_eoc]
set_property PULLTYPE PULLDOWN   [get_ports PULP_SPI_eoc]
set_property package_pin AK13    [get_ports PULP_SPI_clk]
set_property is_loc_fixed true   [get_ports [list  PULP_SPI_clk]]
set_property IOSTANDARD LVCMOS25 [get_ports PULP_SPI_clk]
set_property PULLTYPE PULLUP     [get_ports PULP_SPI_clk]
set_property package_pin AK12    [get_ports PULP_SPI_cs]
set_property is_loc_fixed true   [get_ports [list  PULP_SPI_cs]]
set_property IOSTANDARD LVCMOS25 [get_ports PULP_SPI_cs]
set_property PULLTYPE PULLUP     [get_ports PULP_SPI_cs]
set_property package_pin AA13    [get_ports PULP_SPI_sdo]
set_property is_loc_fixed true   [get_ports [list  PULP_SPI_sdo]]
set_property IOSTANDARD LVCMOS25 [get_ports PULP_SPI_sdo]
set_property PULLTYPE PULLUP     [get_ports PULP_SPI_sdo]
set_property package_pin AH18    [get_ports PULP_SPI_sdi]
set_property is_loc_fixed true   [get_ports [list  PULP_SPI_sdi]]
set_property IOSTANDARD LVCMOS25 [get_ports PULP_SPI_sdi]
set_property PULLTYPE PULLUP     [get_ports PULP_SPI_sdi]
set_property package_pin Y20     [get_ports PULP_SPI_mode_1]
set_property is_loc_fixed true   [get_ports [list  PULP_SPI_mode_1]]
set_property IOSTANDARD LVCMOS25 [get_ports PULP_SPI_mode_1]
set_property PULLTYPE PULLUP     [get_ports PULP_SPI_mode_1]
set_property package_pin AA20    [get_ports PULP_SPI_mode_0]
set_property is_loc_fixed true   [get_ports [list  PULP_SPI_mode_0]]
set_property IOSTANDARD LVCMOS25 [get_ports PULP_SPI_mode_0]
set_property PULLTYPE PULLUP     [get_ports PULP_SPI_mode_0]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets PULP_SPI_clk_IBUF]

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
report_utilization -hierarchical -hierarchical_depth 2 -cells ulpsoc_i -file ulpsoc.txt
report_utilization -hierarchical -hierarchical_depth 2 -cells ulpsoc_i/cluster_i/ulpcluster_i -file ulpcluster.txt

# output Verilog netlist + SDC for timing simulation
write_verilog -force -mode timesim -cell ulpsoc_i ../simu/ulpsoc_impl.v
write_sdf     -force -cell ulpsoc_i ../simu/ulpsoc_impl.sdf
write_verilog -force -mode timesim -cell ulpsoc_i/cluster_i/ulpcluster_i ../simu/ulpcluster_impl.v
write_sdf     -force -cell ulpsoc_i/cluster_i/ulpcluster_i ../simu/ulpcluster_impl.sdf

if { $::env(PROBES) } {
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

