
# create project
create_project pulpino . -part $::env(XILINX_PART)
set_property board $::env(XILINX_BOARD) [current_project]
set_property include_dirs { \
  ../../ips/riscv/include \
  ../../rtl/include \
} [current_fileset]

# set up meaningful errors
source ../common/messages.tcl

source tcl/src_files.tcl

# add memory cuts
add_files -norecurse $FPGA_IPS/xilinx_mem_32768x32_dp/ip/xilinx_mem_32768x32_dp.dcp
add_files -norecurse $FPGA_IPS/xilinx_mem_32768x32/ip/xilinx_mem_32768x32.dcp

# add axi_mem_if_DP
add_files -norecurse -scan_for_includes $SRC_AXI_MEM_IF_DP

# add axi_spi_slave
add_files -norecurse -scan_for_includes $SRC_AXI_SLAVE

# add apb_spim
add_files -norecurse -scan_for_includes $SRC_APB_SPIM

# add apb_gpio
add_files -norecurse -scan_for_includes $SRC_APB_GPIO

# add apb_uart
add_files -norecurse -scan_for_includes $SRC_APB_UART

# add axi2apb
add_files -norecurse -scan_for_includes $SRC_AXI2APB

# add axi_slice
add_files -norecurse -scan_for_includes $SRC_AXI_SLICE

# add axi_slice_dc
add_files -norecurse -scan_for_includes $SRC_AXI_SLICE_DC

# add axi_node
add_files -norecurse -scan_for_includes $SRC_AXI_NODE

## add or10n
#add_files -norecurse -scan_for_includes $SRC_OR10N

# add RI5CY
add_files -norecurse -scan_for_includes $SRC_RI5CY

# add adv_dbg_if
add_files -norecurse -scan_for_includes $SRC_ADV_DEBUG_IF

# add ILA debug cores
# add_files -norecurse $FPGA_IPS/xilinx_core_ila/ip/xilinx_core_ila.dcp
# add_files -norecurse $FPGA_IPS/xilinx_icache_ila/ip/xilinx_icache_ila.dcp
# add_files -norecurse $FPGA_IPS/xilinx_tcdm_sram_ila/ip/xilinx_tcdm_sram_ila.dcp
# add_files -norecurse $FPGA_IPS/xilinx_tcdm_scm_ila/ip/xilinx_tcdm_scm_ila.dcp

# add common cells
add_files -norecurse $SRC_COMMONCELLS

# add pulpino
add_files -norecurse $SRC_PULPINO

# set pulpino_top as top
set_property top pulpino [current_fileset]

# needed only if used in batch mode
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

# run synthesis
# first try will fail
catch {synth_design -rtl -name rtl_1 -verilog_define PULP_FPGA_EMUL=1}

update_compile_order -fileset sources_1
synth_design -rtl -name rtl_1 -verilog_define PULP_FPGA_EMUL=1

create_clock -period 4.000 -name clk -waveform {0.000 2.000} [get_nets {clk}]
set_property STEPS.SYNTH_DESIGN.ARGS.FLATTEN_HIERARCHY none [get_runs synth_1]
#set_property STEPS.SYNTH_DESIGN.ARGS.KEEP_EQUIVALENT_REGISTERS true [get_runs synth_1]
#set_property STEPS.SYNTH_DESIGN.ARGS.RESOURCE_SHARING off [get_runs synth_1]
#set_property STEPS.SYNTH_DESIGN.ARGS.NO_LC true [get_runs synth_1]
launch_runs synth_1
wait_on_run synth_1

# save EDIF netlist
open_run synth_1
write_edif -force pulpino.edf
write_verilog -force -mode synth_stub pulpino_stub.v
write_verilog -force -mode funcsim pulpino_funcsim.v
