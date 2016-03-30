# false paths
#source tcl/false_paths.tcl

# clocks
create_clock -period 12.000 -name clk      [get_nets {pulpino_wrap_i/clk}]
create_clock -period 40.000 -name spi_sck  [get_nets {pulpino_wrap_i/spi_clk_i}]
create_clock -period 40.000 -name tck      [get_nets {pulpino_wrap_i/tck_i}]

# define false paths between all clocks
set_clock_groups -asynchronous \
                 -group { clk } \
                 -group { spi_sck } \
                 -group { tck }

# ----------------------------------------------------------------------------
# User LEDs - Bank 33
# ----------------------------------------------------------------------------
set_property PACKAGE_PIN T22 [get_ports {LD_o[0]}];  # "LD0"
set_property PACKAGE_PIN T21 [get_ports {LD_o[1]}];  # "LD1"
set_property PACKAGE_PIN U22 [get_ports {LD_o[2]}];  # "LD2"
set_property PACKAGE_PIN U21 [get_ports {LD_o[3]}];  # "LD3"
set_property PACKAGE_PIN V22 [get_ports {LD_o[4]}];  # "LD4"
set_property PACKAGE_PIN W22 [get_ports {LD_o[5]}];  # "LD5"
set_property PACKAGE_PIN U19 [get_ports {LD_o[6]}];  # "LD6"
set_property PACKAGE_PIN U14 [get_ports {LD_o[7]}];  # "LD7"

# ----------------------------------------------------------------------------
# OLED Display - Bank 13
# ----------------------------------------------------------------------------
set_property PACKAGE_PIN AB12 [get_ports {oled_sclk_io}];  # "OLED-SCLK"
set_property PACKAGE_PIN AA12 [get_ports {oled_sdin_io}];  # "OLED-SCIN"
set_property PACKAGE_PIN U10  [get_ports {oled_dc_o}];     # "OLED-DC"
set_property PACKAGE_PIN U9   [get_ports {oled_res_o}];    # "OLED-RES"
set_property PACKAGE_PIN U11  [get_ports {oled_vbat_o}];   # "OLED-VBAT"
set_property PACKAGE_PIN U12  [get_ports {oled_vdd_o}];    # "OLED-VDD"


# ----------------------------------------------------------------------------
# User Push Buttons - Bank 34
# ----------------------------------------------------------------------------
set_property PACKAGE_PIN P16 [get_ports {btn_i[0]}];  # "BTNC"
set_property PACKAGE_PIN R16 [get_ports {btn_i[1]}];  # "BTND"
set_property PACKAGE_PIN N15 [get_ports {btn_i[2]}];  # "BTNL"
set_property PACKAGE_PIN R18 [get_ports {btn_i[3]}];  # "BTNR"
set_property PACKAGE_PIN T18 [get_ports {btn_i[4]}];  # "BTNU"

# ----------------------------------------------------------------------------
# User DIP Switches - Bank 35
# ----------------------------------------------------------------------------
set_property PACKAGE_PIN F22 [get_ports {sw_i[0]}];  # "SW0"
set_property PACKAGE_PIN G22 [get_ports {sw_i[1]}];  # "SW1"
set_property PACKAGE_PIN H22 [get_ports {sw_i[2]}];  # "SW2"
set_property PACKAGE_PIN F21 [get_ports {sw_i[3]}];  # "SW3"
set_property PACKAGE_PIN H19 [get_ports {sw_i[4]}];  # "SW4"
set_property PACKAGE_PIN H18 [get_ports {sw_i[5]}];  # "SW5"
set_property PACKAGE_PIN H17 [get_ports {sw_i[6]}];  # "SW6"
set_property PACKAGE_PIN M15 [get_ports {sw_i[7]}];  # "SW7"

set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 33]];
set_property IOSTANDARD LVCMOS18 [get_ports -of_objects [get_iobanks 34]];
set_property IOSTANDARD LVCMOS18 [get_ports -of_objects [get_iobanks 35]];
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 13]];



# physical constraints
# source tcl/floorplan.xdc

save_constraints

# set for RuntimeOptimized implementation
# set_property "steps.opt_design.args.directive" "RuntimeOptimized"   [get_runs impl_1]
# set_property "steps.place_design.args.directive" "RuntimeOptimized" [get_runs impl_1]
# set_property "steps.route_design.args.directive" "RuntimeOptimized" [get_runs impl_1]

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

if { [info exists ::env(PROBES)] } {
   # create new design run for probes
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

