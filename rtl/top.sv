
`include "axi_bus.sv"

`define AXI_ADDR_WIDTH         32
`define AXI_DATA_WIDTH         32
`define AXI_ID_MASTER_WIDTH     2
`define AXI_ID_SLAVE_WIDTH      4
`define AXI_USER_WIDTH          0

module pulpino_top
  (
    // Clock and Reset
    input logic clk,
    input logic rst_n,

    input  logic             spi_clk_i,
    input  logic             spi_cs_i,
    output logic [1:0]       spi_mode_o,
    output logic             spi_sdo0_o,
    output logic             spi_sdo1_o,
    output logic             spi_sdo2_o,
    output logic             spi_sdo3_o,
    input  logic             spi_sdi0_i,
    input  logic             spi_sdi1_i,
    input  logic             spi_sdi2_i,
    input  logic             spi_sdi3_i,

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
  slaves[2:0]();

  AXI_BUS
  #(
    .AXI_ADDR_WIDTH   ( `AXI_ADDR_WIDTH ),
    .AXI_DATA_WIDTH   ( `AXI_DATA_WIDTH ),
    .AXI_ID_WIDTH     ( `AXI_ID_MASTER_WIDTH   ),
    .AXI_USER_WIDTH   ( `AXI_USER_WIDTH )
  )
  masters[2:0]();




  core_region
  #(
    .AXI_ADDR_WIDTH       ( `AXI_ADDR_WIDTH      ),
    .AXI_DATA_WIDTH       ( `AXI_DATA_WIDTH      ),
    .AXI_ID_MASTER_WIDTH  ( `AXI_ID_MASTER_WIDTH ),
    .AXI_ID_SLAVE_WIDTH   ( `AXI_ID_SLAVE_WIDTH  ),
    .AXI_USER_WIDTH       ( `AXI_USER_WIDTH      )
  )
  core_region_i
  (
    .clk         ( clk        ),
    .rst_n       ( rst_n      ),

    .core_master ( masters[0] ),
    .dbg_master  ( masters[1] ),
    .data_slave  ( slaves[1]  ),
    .instr_slave ( slaves[0]  ),

    .tck_i       ( tck_i      ),
    .trstn_i     ( trstn_i    ),
    .tms_i       ( tms_i      ),
    .tdi_i       ( tdi_i      ),
    .tdo_o       ( tdo_o      )
  );


  peripherals
  #(
    .AXI_ADDR_WIDTH      ( `AXI_ADDR_WIDTH      ),
    .AXI_DATA_WIDTH      ( `AXI_DATA_WIDTH      ),
    .AXI_SLAVE_ID_WIDTH  ( `AXI_ID_SLAVE_WIDTH  ),
    .AXI_MASTER_ID_WIDTH ( `AXI_ID_MASTER_WIDTH ),
    .AXI_USER_WIDTH      ( `AXI_USER_WIDTH      )
  )
  peripherals_i
  (
    .clk             ( clk        ),
    .rst_n           ( rst_n      ),

    .axi_spi_master  ( masters[2] ),

    .spi_clk_i  ( spi_clk_i      ),
    .spi_cs_i   ( spi_cs_i       ),
    .spi_mode_o ( spi_mode_o     ),
    .spi_sdo0_o ( spi_sdo0_o     ),
    .spi_sdo1_o ( spi_sdo1_o     ),
    .spi_sdo2_o ( spi_sdo2_o     ),
    .spi_sdo3_o ( spi_sdo3_o     ),
    .spi_sdi0_i ( spi_sdi0_i     ),
    .spi_sdi1_i ( spi_sdi1_i     ),
    .spi_sdi2_i ( spi_sdi2_i     ),
    .spi_sdi3_i ( spi_sdi3_i     ),

    .slave           ( slaves[2] ),

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
    .NB_MASTER      ( 3                    ),
    .NB_SLAVE       ( 3                    ),
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

    .start_addr_i ( { 32'h1A10_0000, 32'h0010_0000, 32'h0000_0000 } ),
    .end_addr_i   ( { 32'h1A11_FFFF, 32'h001F_FFFF, 32'h000F_FFFF } )
  );

endmodule

