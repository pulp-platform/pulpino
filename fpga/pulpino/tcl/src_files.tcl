set RTL ../../rtl
set IPS ../../ips
set FPGA_IPS ../ips
set FPGA_RTL ../rtl

# components
set SRC_COMPONENTS " \
   $RTL/components/pulp_clock_gating.sv \
   $RTL/components/cluster_clock_gating.sv \
   $RTL/components/cluster_clock_inverter.sv \
   $RTL/components/cluster_clock_mux2.sv \
   $RTL/components/rstgen.sv \
   $RTL/components/pulp_clock_inverter.sv \
   $RTL/components/pulp_clock_mux2.sv \
   $RTL/components/generic_fifo.sv \
   $RTL/components/sp_ram.sv \
"

# pulpino
set SRC_PULPINO " \
   $RTL/axi2apb_wrap.sv \
   $RTL/periph_bus_wrap.sv \
   $RTL/core2axi_wrap.sv \
   $RTL/axi_node_intf_wrap.sv \
   $RTL/axi_spi_slave_wrap.sv \
   $RTL/axi_slice_wrap.sv \
   $RTL/axi_mem_if_SP_wrap.sv \
   $RTL/core_region.sv \
   $RTL/instr_ram_wrap.sv \
   $RTL/sp_ram_wrap.sv \
   $RTL/boot_code.sv \
   $RTL/boot_rom_wrap.sv \
   $RTL/peripherals.sv \
   $RTL/ram_mux.sv \
   $RTL/pulpino_top.sv \
   $RTL/clk_rst_gen.sv \
   $FPGA_RTL/pulpino_wrap.v \
"

