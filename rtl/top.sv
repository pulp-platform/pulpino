
`include "axi_bus.sv"

`define AXI_ADDR_WIDTH         32
`define AXI_DATA_WIDTH         32
`define AXI_ID_MASTER_WIDTH     2
`define AXI_ID_SLAVE_WIDTH      4
`define AXI_USER_WIDTH          1

module pulpino_top
  (
    // Clock and Reset
    input logic clk,
    input logic rst_n,

    input  logic             clk_sel_i,
    input  logic             scan_en_i,
    input  logic             testmode_i,
    input  logic             fetch_enable_i,

    //SPI Slave
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

    //SPI Master
    output logic             spi_master_clk_o,
    output logic             spi_master_csn0_o,
    output logic             spi_master_csn1_o,
    output logic             spi_master_csn2_o,
    output logic             spi_master_csn3_o,
    output logic [1:0]       spi_master_mode_o,
    output logic             spi_master_sdo0_o,
    output logic             spi_master_sdo1_o,
    output logic             spi_master_sdo2_o,
    output logic             spi_master_sdo3_o,
    input  logic             spi_master_sdi0_i,
    input  logic             spi_master_sdi1_i,
    input  logic             spi_master_sdi2_i,
    input  logic             spi_master_sdi3_i,

    input  logic              scl_pad_i,
    output logic              scl_pad_o,
    output logic              scl_padoen_o,
    input  logic              sda_pad_i,
    output logic              sda_pad_o,
    output logic              sda_padoen_o,

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
    output logic tdo_o,

    // PULPino specific pad config
    output logic [31:0] [5:0] pad_cfg_o,
    output logic       [31:0] pad_mux_o
  );

  logic clk_int;

  logic fetch_enable_int;
  logic fetch_enable_peripheral_int;
  logic core_busy_int;
  logic clk_gate_core_int;
  logic [31:0] irq_to_core_int;

  logic clk_fll_int;
  logic lock_fll_int;
  logic cfreq_fll_int;
  logic cfack_fll_int;
  logic cfgack_fll_int;
  logic [1:0]  cfgad_fll_int;
  logic [31:0] cfgd_fll_int;
  logic [31:0] cfgq_fll_int;
  logic cfgweb_n_fll_int;

  // clk select mux
  assign clk_int = (clk_sel_i) ? clk_fll_int : clk;

  assign fetch_enable_int = fetch_enable_i & fetch_enable_peripheral_int;

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
  
  //----------------------------------------------------------------------------//
  // FLL
  //----------------------------------------------------------------------------//

  umcL65_LL_FLL
  fll_i
  (
    .FLLCLK     ( clk_fll_int       ),
    .FLLOE      ( 1'b1              ),
    .REFCLK     ( clk               ),
    .LOCK       ( lock_fll_int      ),
    .CFGREQ     ( cfreq_fll_int     ),
    .CFGACK     ( cfgack_fll_int    ),
    .CFGAD      ( cfgad_fll_int     ),
    .CFGD       ( cfgd_fll_int      ),
    .CFGQ       ( cfgq_fll_int      ),
    .CFGWEB     ( cfgweb_n_fll_int  ),
    .RSTB       ( rst_n             ),
    .PWDB       ( 1'b1              ),
    .TM         ( testmode_i        ),
    .TE         ( scan_en_i         ),
    .TD         (                   ),
    .TQ         (                   )
  );

  //----------------------------------------------------------------------------//
  // Core region
  //----------------------------------------------------------------------------//
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
    .clk         ( clk_int  ),
    .rst_n       ( rst_n        ),

    .testmode_i     ( testmode_i        ),
    .fetch_enable_i ( fetch_enable_int  ),
    .irq_i          ( irq_to_core_int   ),
    .core_busy_o    ( core_busy_int     ),
    .clock_gating_i ( clk_gate_core_int ),

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

  //----------------------------------------------------------------------------//
  // Peripherals
  //----------------------------------------------------------------------------//
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
    .clk_i           ( clk_int        ),
    .rst_n           ( rst_n      ),

    .axi_spi_master  ( masters[2] ),

    .spi_clk_i  ( spi_clk_i      ),
    .testmode_i ( testmode_i ),
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

    .spi_master_clk  ( spi_master_clk_o),
    .spi_master_csn0 ( spi_master_csn0_o ),
    .spi_master_csn1 ( spi_master_csn1_o ),
    .spi_master_csn2 ( spi_master_csn2_o ),
    .spi_master_csn3 ( spi_master_csn3_o ),
    .spi_master_mode ( spi_master_mode_o ),
    .spi_master_sdo0 ( spi_master_sdo0_o ),
    .spi_master_sdo1 ( spi_master_sdo1_o ),
    .spi_master_sdo2 ( spi_master_sdo2_o ),
    .spi_master_sdo3 ( spi_master_sdo3_o ),
    .spi_master_sdi0 ( spi_master_sdi0_i ),
    .spi_master_sdi1 ( spi_master_sdi1_i ),
    .spi_master_sdi2 ( spi_master_sdi2_i ),
    .spi_master_sdi3 ( spi_master_sdi3_i ),

    .scl_pad_i    ( scl_pad_i     ),
    .scl_pad_o    ( scl_pad_o     ),
    .scl_padoen_o ( scl_padoen_o  ),
    .sda_pad_i    ( sda_pad_i     ),
    .sda_pad_o    ( sda_pad_o     ),
    .sda_padoen_o ( sda_padoen_o  ),

    .gpio_in         ( gpio_in         ),
    .gpio_out        ( gpio_out        ),
    .gpio_dir        ( gpio_dir        ),
    .gpio_padcfg     ( gpio_padcfg     ),

    .core_busy_i     ( core_busy_int                ),
    .irq_o           ( irq_to_core_int              ),
    .fetch_enable_o  ( fetch_enable_peripheral_int  ),
    .clk_gate_core_o ( clk_gate_core_int            ),

    .fll1_req_o      ( cfreq_fll_int        ),
    .fll1_wrn_o      ( cfgweb_n_fll_int     ),
    .fll1_add_o      ( cfgad_fll_int        ),
    .fll1_wdata_o    ( cfgd_fll_int         ),
    .fll1_ack_i      ( cfgack_fll_int       ),
    .fll1_rdata_i    ( cfgq_fll_int         ),
    .fll1_lock_i     ( lock_fll_int         )
  );


  //----------------------------------------------------------------------------//
  // Axi node
  //----------------------------------------------------------------------------//

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
    .clk     ( clk_int               ),
    .rst_n   ( rst_n                     ),

    .master  ( slaves  ),
    .slave   ( masters ),

    .start_addr_i ( { 32'h1A10_0000, 32'h0010_0000, 32'h0000_0000 } ),
    .end_addr_i   ( { 32'h1A11_FFFF, 32'h001F_FFFF, 32'h000F_FFFF } )
  );

endmodule

