set RTL ../../rtl
set IPS ../../ips
set FPGA_IPS ../ips
set FPGA_RTL ../rtl

# axi_mem_if_DP
set SRC_AXI_MEM_IF_DP "
   $IPS/axi/axi_mem_if_DP/axi_mem_if_DP_hybr.sv \
   $IPS/axi/axi_mem_if_DP/axi_mem_if_DP.sv \
   $IPS/axi/axi_mem_if_DP/axi_mem_if_SP.sv \
   $IPS/axi/axi_mem_if_DP/axi_read_only_ctrl.sv \
   $IPS/axi/axi_mem_if_DP/axi_write_only_ctrl.sv \
"

# axi_spi_slave
set SRC_AXI_SLAVE "
   $IPS/axi/axi_spi_slave/axi_spi_slave.sv \
   $IPS/axi/axi_spi_slave/spi_slave_axi_plug.sv \
   $IPS/axi/axi_spi_slave/spi_slave_cmd_parser.sv \
   $IPS/axi/axi_spi_slave/spi_slave_controller.sv \
   $IPS/axi/axi_spi_slave/spi_slave_dc_fifo.sv \
   $IPS/axi/axi_spi_slave/spi_slave_regs.sv \
   $IPS/axi/axi_spi_slave/spi_slave_rx.sv \
   $IPS/axi/axi_spi_slave/spi_slave_syncro.sv \
   $IPS/axi/axi_spi_slave/spi_slave_tx.sv \
"

# apb_spi_master
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

# axi_slice_dc
set SRC_AXI_SLICE_DC " \
    $IPS/axi/axi_slice_dc/axi_slice_dc_master.sv \
    $IPS/axi/axi_slice_dc/axi_slice_dc_slave.sv \
    $IPS/axi/axi_slice_dc/dc_data_buffer.v \
    $IPS/axi/axi_slice_dc/dc_full_detector.v \
    $IPS/axi/axi_slice_dc/dc_synchronizer.v \
    $IPS/axi/axi_slice_dc/dc_token_ring_fifo_din.v \
    $IPS/axi/axi_slice_dc/dc_token_ring_fifo_dout.v \
    $IPS/axi/axi_slice_dc/dc_token_ring.v \
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

# RI5CY
set SRC_RI5CY "
   $IPS/riscv/alu.sv \
   $IPS/riscv/compressed_decoder.sv \
   $IPS/riscv/controller.sv \
   $IPS/riscv/decoder.sv \
   $IPS/riscv/cs_registers.sv \
   $IPS/riscv/debug_unit.sv \
   $IPS/riscv/exc_controller.sv \
   $IPS/riscv/ex_stage.sv \
   $IPS/riscv/hwloop_controller.sv \
   $IPS/riscv/hwloop_regs.sv \
   $IPS/riscv/id_stage.sv \
   $IPS/riscv/if_stage.sv \
   $IPS/riscv/prefetch_buffer.sv \
   $IPS/riscv/load_store_unit.sv \
   $IPS/riscv/mult.sv \
   $IPS/riscv/riscv_core.sv \
   $FPGA_RTL/riscv_register_file.sv \
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
   $IPS/common_cells/pulp_clock_inverter.sv \
   $IPS/common_cells/pulp_clock_mux2.sv \
   $IPS/common_cells/pulp_sync_wedge.sv \
"

# pulpino
set SRC_PULPINO " \
   $RTL/axi2apb_wrap.sv \
   $RTL/axi_node_intf_wrap.sv \
   $RTL/core2axi.sv \
   $RTL/core_region.sv \
   $RTL/instr_ram_wrap.sv \
   $RTL/boot_rom_wrap.sv \
   $RTL/sp_ram_wrap.sv \
   $RTL/peripherals.sv \
   $RTL/ram_mux.sv \
   $RTL/timer_unit.sv \
   $RTL/axi_spi_slave_wrap.sv \
   $RTL/axi_mem_if_SP_wrap.sv \
   $RTL/top.sv \
   $FPGA_RTL/pulpino_wrap.v \
"

