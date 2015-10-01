
if { ![info exists ::env(XILINX_PART)] } {
  set ::env(XILINX_PART) "xc7z020clg484-1"
}

if { ![info exists ::env(XILINX_BOARD)] } {
  set ::env(XILINX_BOARD) "em.avnet.com:zynq:zed:c"
}

set partNumber $::env(XILINX_PART)
set boardName  $::env(XILINX_BOARD)

set ila_name xilinx_clock_manager

create_project $ila_name . -part $partNumber
set_property board $boardName [current_project]

create_ip -name clk_wiz -vendor xilinx.com -library ip -module_name $ila_name

#set_property -dict [list CONFIG.PRIM_IN_FREQ {50.000} CONFIG.CLKOUT2_USED {true} CONFIG.CLKOUT3_USED {true} CONFIG.CLKOUT4_USED {true} CONFIG.CLKOUT5_USED {true} CONFIG.PRIMARY_PORT {clk50_i} CONFIG.CLK_OUT1_PORT {clk100_o} CONFIG.CLK_OUT2_PORT {clk50_o} CONFIG.CLK_OUT3_PORT {clk25_o} CONFIG.CLK_OUT4_PORT {clk10_o} CONFIG.CLK_OUT5_PORT {clk5_o} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {100.000} CONFIG.CLKOUT2_REQUESTED_OUT_FREQ {50.000} CONFIG.CLKOUT3_REQUESTED_OUT_FREQ {25.000} CONFIG.CLKOUT4_REQUESTED_OUT_FREQ {10.000} CONFIG.CLKOUT5_REQUESTED_OUT_FREQ {5.000} CONFIG.LOCKED_PORT {rst_no} CONFIG.RESET_TYPE {ACTIVE_LOW} CONFIG.RESET_PORT {rst_ni}] [get_ips xilinx_clock_manager]

set_property -dict [list CONFIG.INTERFACE_SELECTION {Enable_AXI} CONFIG.USE_DYN_RECONFIG {true} CONFIG.PRIM_IN_FREQ {50.000} CONFIG.PRIMARY_PORT {clk50_i} CONFIG.CLK_OUT1_PORT {clk_o} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {5.000} CONFIG.LOCKED_PORT {rst_no} CONFIG.RESET_TYPE {ACTIVE_LOW} CONFIG.RESET_PORT {rst_ni}] [get_ips xilinx_clock_manager]

generate_target {instantiation_template} [get_files ./$ila_name.srcs/sources_1/ip/$ila_name/$ila_name.xci]
generate_target all [get_files  ./$ila_name.srcs/sources_1/ip/$ila_name/$ila_name.xci]
create_ip_run [get_files -of_objects [get_fileset sources_1] ./$ila_name.srcs/sources_1/ip/$ila_name/$ila_name.xci]
launch_run -jobs 8 ${ila_name}_synth_1
wait_on_run ${ila_name}_synth_1

