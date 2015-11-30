# yes I know this looks ugly, but for some reason vsim deso not recognize the VSIM_FLAGS as correct arguments :-/
# e.g. it did not recognize the -pli ./something.so argument
#
set CORE_LIB "riscv_lib"

if {[info exists env(PULP_CORE)]} {
  if {$env(PULP_CORE) == "OR10N"} {
    set CORE_LIB "or10n_lib"
  }
}

set cmd "vsim -quiet $TB \
  -L $CORE_LIB \
  -L imperio_lib \
  -L axi_spi_slave_lib \
  -L axi_slice_lib \
  -L axi_slice_dc_lib \
  -L fll_lib \
  -L adv_dbg_if_lib \
  -L axi_mem_if_DP_lib \
  -L axi_node_lib \
  -L axi2apb_lib \
  -L apb_fll_if_lib \
  -L apb_uart_lib \
  -L apb_pulpino_lib \
  -L apb_gpio_lib \
  -L apb_event_unit_lib \
  -L apb_timer_lib \
  -L apb_i2c_lib \
  -L apb_spi_master_lib \
  -L standalone_lib \
  +nowarnTRAN \
  +nowarnTSCALE \
  +nowarnTFMPC \
  -t ps \
  +MEMLOAD=$MEMLOAD \
  -voptargs=\"+acc -suppress 2103\" \
  $VSIM_FLAGS"

eval $cmd
