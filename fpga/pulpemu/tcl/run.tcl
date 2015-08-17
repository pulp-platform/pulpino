set RTL ../../rtl
set IPS ../../ips
set FPGA_IPS ../ips
set FPGA_RTL ../rtl
set FPGA_PULPINO ../pulpino

# create project
create_project pulpemu . -part $::env(XILINX_PART)
set_property board $::env(XILINX_BOARD) [current_project]

# set up meaningful errors
source ../common/messages.tcl

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
add_files -norecurse ../pulpino/pulpino.edf \
                     ../pulpino/pulpino_stub.v \
                     ../ips/xilinx_mem_32768x32_dp/ip/xilinx_mem_32768x32_dp.dcp \
                     ../ips/xilinx_clock_manager/ip/xilinx_clock_manager.dcp

update_compile_order -fileset sources_1

# synthesize
synth_design -rtl -name rtl_1

# create_clock -period 4.000 -name ref_clk_i -waveform {0.000 2.000} [get_nets {ref_clk_i}]
launch_runs synth_1
wait_on_run synth_1
open_run synth_1 -name netlist_1
# write_edif pulpemu.edf

# export hardware design for sdk
write_hwdef -force -file ./pulpemu.hwdef

# run implementation
source tcl/impl.tcl
