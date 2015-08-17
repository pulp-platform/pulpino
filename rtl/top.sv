
`include "axi_bus.sv"

`define AXI_ADDR_WIDTH    32
`define AXI_DATA_WIDTH    64
`define AXI_ID_MASTER_WIDTH     1
`define AXI_ID_SLAVE_WIDTH      2
`define AXI_USER_WIDTH    0

module top
  (
    // Clock and Reset
    input logic clk,
    input logic rst_n,

    output logic              uart_tx,
    input  logic              uart_rx,
    output logic              uart_rts,
    output logic              uart_dtr,
    input  logic              uart_cts,
    input  logic              uart_dsr,

    input  logic       [31:0] gpio_in,
    output logic       [31:0] gpio_out,
    output logic       [31:0] gpio_dir,
    output logic [31:0] [5:0] gpio_padcfg,

    // JTAG signals
    input  logic tck_i,
    input  logic trstn_i,
    input  logic tms_i,
    input  logic tdi_i,
    output logic tdo_o
  );

  AXI_BUS
  #(
    .AXI_ADDR_WIDTH   ( `AXI_ADDR_WIDTH ),
    .AXI_DATA_WIDTH   ( `AXI_DATA_WIDTH ),
    .AXI_ID_WIDTH     ( `AXI_ID_SLAVE_WIDTH   ),
    .AXI_USER_WIDTH   ( `AXI_USER_WIDTH )
  )
  slaves[1:0]();

  AXI_BUS
  #(
    .AXI_ADDR_WIDTH   ( `AXI_ADDR_WIDTH ),
    .AXI_DATA_WIDTH   ( `AXI_DATA_WIDTH ),
    .AXI_ID_WIDTH     ( `AXI_ID_MASTER_WIDTH   ),
    .AXI_USER_WIDTH   ( `AXI_USER_WIDTH )
  )
  masters[1:0]();




  core_region core_region_i
  (
    .clk         ( clk        ),
    .rst_n       ( rst_n      ),

    .core_master ( masters[0] ),
    .dbg_master  ( masters[1] ),
    .data_slave  ( slaves[0]  ),

    .tck_i       ( tck_i      ),
    .trstn_i     ( trstn_i    ),
    .tms_i       ( tms_i      ),
    .tdi_i       ( tdi_i      ),
    .tdo_o       ( tdo_o      )
  );


  peripherals
  #(
    .AXI_ADDR_WIDTH ( `AXI_ADDR_WIDTH ),
    .AXI_DATA_WIDTH ( `AXI_DATA_WIDTH ),
    .AXI_ID_WIDTH   ( `AXI_ID_SLAVE_WIDTH   ),
    .AXI_USER_WIDTH ( `AXI_USER_WIDTH )
  )
  peripherals_i
  (
    .clk         ( clk       ),
    .rst_n       ( rst_n     ),

    .slave       ( slaves[1] ),

    .uart_tx         ( uart_tx  ),
    .uart_rx         ( uart_rx  ),
    .uart_rts        ( uart_rts ),
    .uart_dtr        ( uart_dtr ),
    .uart_cts        ( uart_cts ),
    .uart_dsr        ( uart_dsr ),

    .spi_master_clk  ( ),
    .spi_master_csn0 ( ),
    .spi_master_csn1 ( ),
    .spi_master_csn2 ( ),
    .spi_master_csn3 ( ),
    .spi_master_mode ( ),
    .spi_master_sdo0 ( ),
    .spi_master_sdo1 ( ),
    .spi_master_sdo2 ( ),
    .spi_master_sdo3 ( ),
    .spi_master_sdi0 ( ),
    .spi_master_sdi1 ( ),
    .spi_master_sdi2 ( ),
    .spi_master_sdi3 ( ),

    .gpio_in         ( gpio_in     ),
    .gpio_out        ( gpio_out    ),
    .gpio_dir        ( gpio_dir    ),
    .gpio_padcfg     ( gpio_padcfg )
  );



  axi_node_intf_wrap
  #(
    .NB_MASTER      ( 2                    ),
    .NB_SLAVE       ( 2                    ),
    .AXI_ADDR_WIDTH ( `AXI_ADDR_WIDTH      ),
    .AXI_DATA_WIDTH ( `AXI_DATA_WIDTH      ),
    .AXI_ID_WIDTH   ( `AXI_ID_MASTER_WIDTH ),
    .AXI_USER_WIDTH ( `AXI_USER_WIDTH      )
  )
  axi_interconnect_i
  (
    .clk     ( clk                       ),
    .rst_n   ( rst_n                     ),

    .master  ( slaves  ),
    .slave   ( masters ),

    .start_addr_i ( { 32'h1A10_0000, 32'h0000_0000 } ),
    .end_addr_i   ( { 32'h1A11_FFFF, 32'h0000_7FFF } )
  );

endmodule

