set RTL ../../rtl
set IPS ../../ips
set FPGA_IPS ../ips
set FPGA_RTL ../rtl

# apb_spio_master
set SRC_APB_SPIM "
   $IPS/apb/apb_spi_master/apb_spi_master.sv \
   $IPS/apb/apb_spi_master/spi_master_apb_if.sv \
   $IPS/apb/apb_spi_master/spi_master_clkgen.sv \
   $IPS/apb/apb_spi_master/spi_master_controller.sv \
   $IPS/apb/apb_spi_master/spi_master_fifo.sv \
   $IPS/apb/apb_spi_master/spi_master_rx.sv \
   $IPS/apb/apb_spi_master/spi_master_tx.sv \
"

# apb_gpio
set SRC_APB_GPIO "
   $IPS/apb/apb_gpio/apb_gpio.sv
"

# apb_uart
set SRC_APB_UART "
   $IPS/apb/apb_uart/apb_uart.vhd \
   $IPS/apb/apb_uart/slib_clock_div.vhd \
   $IPS/apb/apb_uart/slib_counter.vhd \
   $IPS/apb/apb_uart/slib_edge_detect.vhd \
   $IPS/apb/apb_uart/slib_fifo.vhd \
   $IPS/apb/apb_uart/slib_input_filter.vhd \
   $IPS/apb/apb_uart/slib_input_sync.vhd \
   $IPS/apb/apb_uart/slib_mv_filter.vhd \
   $IPS/apb/apb_uart/uart_baudgen.vhd \
   $IPS/apb/apb_uart/uart_interrupt.vhd \
   $IPS/apb/apb_uart/uart_receiver.vhd \
   $IPS/apb/apb_uart/uart_transmitter.vhd \
"

# axi_slice
set SRC_AXI_SLICE "
   $IPS/axi/axi_slice/axi_ar_buffer.sv \
   $IPS/axi/axi_slice/axi_aw_buffer.sv \
   $IPS/axi/axi_slice/axi_b_buffer.sv \
   $IPS/axi/axi_slice/axi_buffer.sv \
   $IPS/axi/axi_slice/axi_r_buffer.sv \
   $IPS/axi/axi_slice/axi_slice.sv \
   $IPS/axi/axi_slice/axi_w_buffer.sv \
"

# axi2apb
set SRC_AXI2APB "
   $IPS/axi/axi2apb/axi2apb_cmd.sv \
   $IPS/axi/axi2apb/axi2apb_ctrl.sv \
   $IPS/axi/axi2apb/axi2apb_mux.sv \
   $IPS/axi/axi2apb/axi2apb_rd.sv \
   $IPS/axi/axi2apb/axi2apb.sv \
   $IPS/axi/axi2apb/AXI_2_APB.sv \
   $IPS/axi/axi2apb/axi2apb_wr.sv \
   $IPS/axi/axi2apb/prgen_fifo.sv \
"

set SRC_ADV_DEBUG_IF "
   $IPS/adv_dbg_if/rtl/adbg_axi_biu.sv \
   $IPS/adv_dbg_if/rtl/adbg_axi_module.sv \
   $IPS/adv_dbg_if/rtl/adbg_crc32.v \
   $IPS/adv_dbg_if/rtl/adbg_or1k_biu.sv \
   $IPS/adv_dbg_if/rtl/adbg_or1k_module.sv \
   $IPS/adv_dbg_if/rtl/adbg_or1k_status_reg.sv \
   $IPS/adv_dbg_if/rtl/adbg_tap_top.v \
   $IPS/adv_dbg_if/rtl/adbg_top.sv \
   $IPS/adv_dbg_if/rtl/adv_dbg_if.sv \
   $IPS/adv_dbg_if/rtl/bytefifo.v \
   $IPS/adv_dbg_if/rtl/syncflop.v \
   $IPS/adv_dbg_if/rtl/syncreg.v \
"

# axi_node
set SRC_AXI_NODE "
   $IPS/axi/axi_node/axi_address_decoder_AR.sv \
   $IPS/axi/axi_node/axi_address_decoder_AW.sv \
   $IPS/axi/axi_node/axi_address_decoder_BR.sv \
   $IPS/axi/axi_node/axi_address_decoder_BW.sv \
   $IPS/axi/axi_node/axi_address_decoder_DW.sv \
   $IPS/axi/axi_node/axi_AR_allocator.sv \
   $IPS/axi/axi_node/axi_ArbitrationTree.sv \
   $IPS/axi/axi_node/axi_ar_buffer.sv \
   $IPS/axi/axi_node/axi_AW_allocator.sv \
   $IPS/axi/axi_node/axi_aw_buffer.sv \
   $IPS/axi/axi_node/axi_b_buffer.sv \
   $IPS/axi/axi_node/axi_BR_allocator.sv \
   $IPS/axi/axi_node/axi_buffer.sv \
   $IPS/axi/axi_node/axi_BW_allocator.sv \
   $IPS/axi/axi_node/axi_DW_allocator.sv \
   $IPS/axi/axi_node/axi_FanInPrimitive_Req.sv \
   $IPS/axi/axi_node/axi_multiplexer.sv \
   $IPS/axi/axi_node/axi_node.sv \
   $IPS/axi/axi_node/axi_node_wrap_2x2.v \
   $IPS/axi/axi_node/axi_onehot_to_bin.sv \
   $IPS/axi/axi_node/axi_r_buffer.sv \
   $IPS/axi/axi_node/axi_regs_top.sv \
   $IPS/axi/axi_node/axi_request_block.sv \
   $IPS/axi/axi_node/axi_response_block.sv \
   $IPS/axi/axi_node/axi_RR_Flag_Req.sv \
   $IPS/axi/axi_node/axi_w_buffer.sv \
   $IPS/axi/axi_node/GENERIC_FIFO.sv \
"

# timer unit
set SRC_TIMER_UNIT " \
   $RTL/timer_unit.sv \
"

# generic register file
set SRC_REGFILE " \
   $FPGA_RTL/register_file_3r_2w.sv \
"

# or10n
set SRC_OR10N "
   $IPS/or10n/alu.sv \
   $IPS/or10n/controller.sv \
   $IPS/or10n/debug_unit.sv \
   $IPS/or10n/ex_stage.sv \
   $IPS/or10n/exc_controller.sv \
   $IPS/or10n/hwloop_controller.sv \
   $IPS/or10n/hwloop_regs.sv \
   $IPS/or10n/id_stage.sv \
   $IPS/or10n/if_stage.sv \
   $IPS/or10n/instr_core_interface.sv \
   $IPS/or10n/load_store_unit.sv \
   $IPS/or10n/mult.sv \
   $IPS/or10n/or10n_core.sv \
   $IPS/or10n/sp_registers.sv \
   $IPS/or10n/wb_stage.sv \
"

# memory cuts
set SRC_MEM_CUTS "
   $FPGA_IPS/xilinx_ic_ram_128x32/ip/xilinx_ic_ram_128x32_stub.v \
   $FPGA_IPS/xilinx_tcdm_bank_512x32/ip/xilinx_tcdm_bank_2048x32_stub.v \
   $FPGA_IPS/xilinx_tcdm_bank_512x32/ip/xilinx_tcdm_bank_1024x32_stub.v \
   $FPGA_IPS/xilinx_tcdm_bank_512x32/ip/xilinx_tcdm_bank_512x32_stub.v \
   $FPGA_IPS/xilinx_tcdm_bank_256x32/ip/xilinx_tcdm_bank_256x32_stub.v \
   $FPGA_IPS/xilinx_tcdm_bank_128x32/ip/xilinx_tcdm_bank_128x32_stub.v \
"

# common cells
set SRC_COMMONCELLS " \
   $IPS/common_cells/cluster_clock_buffer.sv \
   $IPS/common_cells/cluster_clock_gating.sv \
   $IPS/common_cells/cluster_clock_inverter.sv \
   $IPS/common_cells/cluster_clock_mux2.sv \
   $IPS/common_cells/cluster_clock_xor2.sv \
   $IPS/common_cells/cluster_level_shifter_in.sv \
   $IPS/common_cells/cluster_level_shifter_out.sv \
   $IPS/common_cells/rstgen.sv \
   $IPS/common_cells/edge_propagator_rx.sv \
   $IPS/common_cells/pulp_sync_wedge.sv \
"

# pulpino
set SRC_PULPINO " \
   $RTL/axi2apb_wrap.sv \
   $RTL/axi2mem.sv \
   $RTL/axi_node_intf_wrap.sv \
   $RTL/core2axi.sv \
   $RTL/core_region.sv \
   $RTL/dp_ram.sv \
   $RTL/dp_ram_wrap.sv \
   $RTL/peripherals.sv \
   $RTL/ram_mux.sv \
   $RTL/timer_unit.sv \
   $RTL/top.sv \
"

