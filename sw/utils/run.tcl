vsim -quiet tb \
  -L tech_lib \
  -L cluster_interconnect_lib \
  -L jtag_lib \
  -L axi_id_remap_lib \
  -L axi2per_lib \
  -L per2axi_lib \
  -L wb2axi_lib \
  -L axi_node_lib \
  -L axi_mem_if_lib \
  -L axi_slice_lib \
  -L axi_slice_dc_lib \
  -L axi_spi_master_lib \
  -L axi_spi_slave_lib \
  -L axi_gpreg_lib \
  -L axi2mem_lib \
  -L common_cells_lib \
  -L fll_lib \
  -L cvp_lib \
  -L axi_cvp_if_lib \
  -L mchan_lib \
  -L clock_divider_lib \
  -L scm_lib \
  -L pulp_common_cells_lib \
  -L or10n_lib \
  -L icache_lib \
  -L cluster_peripherals_lib \
  -L axi2apb_lib \
  -L apb_gpio_lib \
  -L apb_spi_master_lib \
  -L apb_cvp_if_lib \
  -L apb_i2c_lib \
  -L apb_audio_lib \
  -L apb_uart_lib \
  -L apb_clkdiv_lib \
  -L apb_cerebro_ctrl_lib \
  -L apb_soc_ctrl_lib \
  -L adv_dbg_if_lib \
  -L vip_axi_monitor_lib \
  +nowarn \
  +nowarnTRAN \
  +nowarnTSCALE \
  +nowarnTFMPC \
  -t ps \
  -voptargs="+acc -suppress 2103"

run 50 ns

mem load -i slm_files/l2_ram_cut0_lo.slm -format hex /tb/ulpsoc_wrap_i/ulpsoc_i/l2_ram_i/l2_mem_i/rvt/cut\[0\]/cut_lo/Mem
mem load -i slm_files/l2_ram_cut0_hi.slm -format hex /tb/ulpsoc_wrap_i/ulpsoc_i/l2_ram_i/l2_mem_i/rvt/cut\[0\]/cut_hi/Mem
mem load -i slm_files/l2_ram_cut1_lo.slm -format hex /tb/ulpsoc_wrap_i/ulpsoc_i/l2_ram_i/l2_mem_i/rvt/cut\[1\]/cut_lo/Mem
mem load -i slm_files/l2_ram_cut1_hi.slm -format hex /tb/ulpsoc_wrap_i/ulpsoc_i/l2_ram_i/l2_mem_i/rvt/cut\[1\]/cut_hi/Mem
mem load -i slm_files/l2_ram_cut2_lo.slm -format hex /tb/ulpsoc_wrap_i/ulpsoc_i/l2_ram_i/l2_mem_i/rvt/cut\[2\]/cut_lo/Mem
mem load -i slm_files/l2_ram_cut2_hi.slm -format hex /tb/ulpsoc_wrap_i/ulpsoc_i/l2_ram_i/l2_mem_i/rvt/cut\[2\]/cut_hi/Mem
mem load -i slm_files/l2_ram_cut3_lo.slm -format hex /tb/ulpsoc_wrap_i/ulpsoc_i/l2_ram_i/l2_mem_i/rvt/cut\[3\]/cut_lo/Mem
mem load -i slm_files/l2_ram_cut3_hi.slm -format hex /tb/ulpsoc_wrap_i/ulpsoc_i/l2_ram_i/l2_mem_i/rvt/cut\[3\]/cut_hi/Mem

run -a
