# yes I know this looks ugly, but for some reason vsim deso not recognize the VSIM_FLAGS as correct arguments :-/
# e.g. it did not recognize the -pli ./something.so argument
#  -L or10n_lib
set cmd "vsim -quiet $TB \
  -L riscv_lib \
  -L axi_spi_slave_lib \
  -L axi_slice_dc_lib \
  -L scm_lib \
  -L adv_dbg_if_lib \
  -L axi_mem_if_DP_lib \
  -L axi_node_lib \
  -L axi2apb_lib \
  -L apb_uart_lib \
  -L apb_gpio_lib \
  -L apb_spi_master_lib \
  -L pulp_common_cells_lib \
  -L standalone_lib \
  +nowarn \
  +nowarnTRAN \
  +nowarnTSCALE \
  +nowarnTFMPC \
  -t ps \
  -voptargs=\"+acc -suppress 2103\" \
  $VSIM_FLAGS"

eval $cmd
