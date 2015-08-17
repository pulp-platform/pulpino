
# create project
create_project pulpino . -part $::env(XILINX_PART)
set_property board $::env(XILINX_BOARD) [current_project]
set_property include_dirs { \
  ../../ips/or10n/include \
  ../../rtl/include \
} [current_fileset]

# set up meaningful errors
source ../common/messages.tcl

source tcl/src_files.tcl

# add axi_slice
add_files -norecurse -scan_for_includes $SRC_AXI_SLICE

# add axi_slice_dc
add_files -norecurse -scan_for_includes $SRC_AXI_SLICE_DC

# add axi_node
add_files -norecurse -scan_for_includes $SRC_AXI_NODE

# add generic register file
add_files -norecurse -scan_for_includes $SRC_REGFILE

# add or10n
add_files -norecurse -scan_for_includes $SRC_OR10N

# add adv_dbg_if
add_files -norecurse -scan_for_includes $SRC_ADV_DEBUG_IF

# add memory cuts
add_files -norecurse $FPGA_IPS/xilinx_ic_ram_128x32/ip/xilinx_ic_ram_128x32.dcp
add_files -norecurse $FPGA_IPS/xilinx_tcdm_bank_2048x32/ip/xilinx_tcdm_bank_2048x32.dcp
add_files -norecurse $FPGA_IPS/xilinx_tcdm_bank_1024x32/ip/xilinx_tcdm_bank_1024x32.dcp
add_files -norecurse $FPGA_IPS/xilinx_tcdm_bank_512x32/ip/xilinx_tcdm_bank_512x32.dcp
add_files -norecurse $FPGA_IPS/xilinx_tcdm_bank_256x32/ip/xilinx_tcdm_bank_256x32.dcp
add_files -norecurse $FPGA_IPS/xilinx_tcdm_bank_128x32/ip/xilinx_tcdm_bank_128x32.dcp

# add ILA debug cores
# add_files -norecurse $FPGA_IPS/xilinx_core_ila/ip/xilinx_core_ila.dcp
# add_files -norecurse $FPGA_IPS/xilinx_icache_ila/ip/xilinx_icache_ila.dcp
# add_files -norecurse $FPGA_IPS/xilinx_tcdm_sram_ila/ip/xilinx_tcdm_sram_ila.dcp
# add_files -norecurse $FPGA_IPS/xilinx_tcdm_scm_ila/ip/xilinx_tcdm_scm_ila.dcp

# add common cells
add_files -norecurse $SRC_COMMONCELLS

# add pulpino
add_files -norecurse $SRC_PULPINO

# set ulpcluster as top
set_property top top [current_fileset]

# # create or10n mac unit (moved here from FPGA ips)
# create_ip -name xbip_multadd -vendor xilinx.com -library ip -version 3.0 -module_name xilinx_or10n_mac
# set_property -dict [list CONFIG.c_a_width {33} CONFIG.c_b_width {33} CONFIG.c_c_width {64} CONFIG.c_out_high {65} CONFIG.c_ab_latency {0} CONFIG.c_c_latency {0}] [get_ips xilinx_or10n_mac]
# generate_target {instantiation_template} [get_files /var/lib/jenkins/workspace/PULP3emu/PULP/pulp3/fpga/ulpcluster/ulpcluster.srcs/sources_1/ip/xilinx_or10n_mac/xilinx_or10n_mac.xci]
# update_compile_order -fileset sources_1
# generate_target all [get_files  /var/lib/jenkins/workspace/PULP3emu/PULP/pulp3/fpga/ulpcluster/ulpcluster.srcs/sources_1/ip/xilinx_or10n_mac/xilinx_or10n_mac.xci]
# create_ip_run [get_files -of_objects [get_fileset sources_1] /var/lib/jenkins/workspace/PULP3emu/PULP/pulp3/fpga/ulpcluster/ulpcluster.srcs/sources_1/ip/xilinx_or10n_mac/xilinx_or10n_mac.xci]
# launch_run -jobs 8 xilinx_or10n_mac_synth_1
# wait_on_run xilinx_or10n_mac_synth_1

# needed only if used in batch mode
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

# run synthesis
catch {synth_design -rtl -name rtl_1 -verilog_define PULP_FPGA_EMUL=1}
update_compile_order -fileset sources_1
synth_design -rtl -name rtl_1 -verilog_define PULP_FPGA_EMUL=1

create_clock -period 4.000 -name clk_i -waveform {0.000 2.000} [get_nets {clk_i}]
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
