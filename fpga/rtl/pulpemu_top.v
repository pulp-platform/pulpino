// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

module pulpemu_top(
  DDR_addr,
  DDR_ba,
  DDR_cas_n,
  DDR_ck_n,
  DDR_ck_p,
  DDR_cke,
  DDR_cs_n,
  DDR_dm,
  DDR_dq,
  DDR_dqs_n,
  DDR_dqs_p,
  DDR_odt,
  DDR_ras_n,
  DDR_reset_n,
  DDR_we_n,
  FIXED_IO_ddr_vrn,
  FIXED_IO_ddr_vrp,
  FIXED_IO_mio,
  FIXED_IO_ps_clk,
  FIXED_IO_ps_porb,
  FIXED_IO_ps_srstb,
  LD_o,
  sw_i,
  btn_i,
  oled_sclk_io,
  oled_sdin_io,
  oled_dc_o,
  oled_res_o,
  oled_vbat_o,
  oled_vdd_o,
  ext_tck_i,
  ext_trstn_i,
  ext_tdi_i,
  ext_tms_i,
  ext_tdo_o
  );

  inout  [14:0] DDR_addr;
  inout  [2:0]  DDR_ba;
  inout         DDR_cas_n;
  inout         DDR_ck_n;
  inout         DDR_ck_p;
  inout         DDR_cke;
  inout         DDR_cs_n;
  inout  [3:0]  DDR_dm;
  inout  [31:0] DDR_dq;
  inout  [3:0]  DDR_dqs_n;
  inout  [3:0]  DDR_dqs_p;
  inout         DDR_odt;
  inout         DDR_ras_n;
  inout         DDR_reset_n;
  inout         DDR_we_n;
  inout         FIXED_IO_ddr_vrn;
  inout         FIXED_IO_ddr_vrp;
  inout  [53:0] FIXED_IO_mio;
  inout         FIXED_IO_ps_clk;
  inout         FIXED_IO_ps_porb;
  inout         FIXED_IO_ps_srstb;

  output  [7:0] LD_o;
  input   [7:0] sw_i;
  input   [4:0] btn_i;
  inout         oled_sclk_io;
  inout         oled_sdin_io;
  output        oled_dc_o;
  output        oled_res_o;
  output        oled_vbat_o;
  output        oled_vdd_o;

  input         ext_tck_i;
  input         ext_trstn_i;
  input         ext_tdi_i;
  input         ext_tms_i;
  output        ext_tdo_o;


  wire [14:0] DDR_addr;
  wire [2:0]  DDR_ba;
  wire        DDR_cas_n;
  wire        DDR_ck_n;
  wire        DDR_ck_p;
  wire        DDR_cke;
  wire        DDR_cs_n;
  wire [3:0]  DDR_dm;
  wire [31:0] DDR_dq;
  wire [3:0]  DDR_dqs_n;
  wire [3:0]  DDR_dqs_p;
  wire        DDR_odt;
  wire        DDR_ras_n;
  wire        DDR_reset_n;
  wire        DDR_we_n;
  wire        FIXED_IO_ddr_vrn;
  wire        FIXED_IO_ddr_vrp;
  wire [53:0] FIXED_IO_mio;
  wire        FIXED_IO_ps_clk;
  wire        FIXED_IO_ps_porb;
  wire        FIXED_IO_ps_srstb;

  wire        ps7_clk;
  wire        ps7_rst_n;
  wire        ps7_rst_clking_n;

  wire        ref_clk_i;               // input
  wire        rst_ni;                  // input
  wire        fetch_en;                // input

  wire [31:0] fetch_enable;

  wire [31:0] jtag_emu_i; // input to PS
  wire [31:0] jtag_emu_o; // output from PS
  wire        tck_i;                   // input
  wire        trst_ni;                 // input
  wire        tms_i;                   // input
  wire        td_i;                    // input
  wire        td_o;                    // output

  wire        spi_mosi;
  wire        spi_miso;
  wire        spi_sck;
  wire        spi_cs;

  wire [31:0] gpio_dir;                // output
  wire [31:0] gpio_in;                 // input
  wire [31:0] gpio_in_ps7;             // output of ps7 => to pulpino
  wire [31:0] gpio_out;                // output

  wire        scl_oen;
  wire        scl_in;
  wire        scl_out;
  wire        sda_oen;
  wire        sda_in;
  wire        sda_out;

  reg   [7:0] LD_q;

  wire        clking_axi_aclk;    // input
  wire        clking_axi_aresetn; // input
  wire [10:0] clking_axi_awaddr;  // input
  wire  [2:0] clking_axi_awprot;  // input
  wire        clking_axi_awvalid; // input
  wire        clking_axi_awready; // output
  wire [31:0] clking_axi_wdata;   // input
  wire  [3:0] clking_axi_wstrb;   // input
  wire        clking_axi_wvalid;  // input
  wire        clking_axi_wready;  // output
  wire  [1:0] clking_axi_bresp;   // output
  wire        clking_axi_bvalid;  // output
  wire        clking_axi_bready;  // input
  wire [10:0] clking_axi_araddr;  // input
  wire  [2:0] clking_axi_arprot;  // input
  wire        clking_axi_arvalid; // input
  wire        clking_axi_arready; // output
  wire [31:0] clking_axi_rdata;   // output
  wire  [1:0] clking_axi_rresp;   // output
  wire        clking_axi_rvalid;  // output
  wire        clking_axi_rready;  // input

  wire        uart_tx;            // output
  wire        uart_rx;            // input

  // clock generator signals
  wire s_rstn_pulpino;
  wire s_clk_pulpino;

  assign ref_clk_i          = ps7_clk;
  assign clking_axi_aclk    = ps7_clk;
  assign clking_axi_aresetn = ps7_rst_clking_n;
  assign rst_ni             = fetch_enable[31];
  assign ps7_rst_pulp_n     = ps7_rst_n;
  assign ps7_rst_clking_n   = ps7_rst_n;

  reg fetch_en_r;

  assign fetch_en = fetch_en_r;

  always @(posedge ps7_clk or negedge ps7_rst_n)
  begin
    if(ps7_rst_n == 1'b0)
      fetch_en_r = 1'b0;
    else
      fetch_en_r = fetch_enable[0];
  end

  // JTAG signals
  // for JTAG EMU
  // assign tck_i            = jtag_emu_o[0];
  // assign trst_ni          = jtag_emu_o[1];
  // assign td_i             = jtag_emu_o[2];
  // assign tms_i            = jtag_emu_o[3];
  assign jtag_emu_i[3:0]  = 4'b0;
  assign jtag_emu_i[4]    = td_o;
  assign jtag_emu_i[31:5] = 27'b0;

  // for external JTAG
  assign tck_i   = ext_tck_i;
  assign trst_ni = ext_trstn_i;
  assign td_i    = ext_tdi_i;
  assign tms_i   = ext_tms_i;

  assign ext_tdo_o = td_o;


  // GPIO signals
  always @(posedge s_clk_pulpino or negedge s_rstn_pulpino)
  begin
    if (~s_rstn_pulpino)
      LD_q <= 8'b0;
    else
      LD_q <= gpio_out[15:8];
  end

  assign LD_o = LD_q;

  assign gpio_in[7:0]   = sw_i;
  assign gpio_in[15:8]  = 8'b0;
  assign gpio_in[20:16] = btn_i;
  assign gpio_in[31:21] = gpio_in_ps7[31:21];


  // I2C for LCD
  assign oled_sclk_io = (~scl_oen) ? scl_out : 1'bz;
  assign scl_in       = oled_sclk_io;
  assign oled_sdin_io = (~sda_oen) ? sda_out : 1'bz;
  assign sda_in       = oled_sdin_io;

  assign oled_vbat_o  = 1'b1;
  assign oled_vdd_o   = 1'b1;

  assign oled_dc_o    = gpio_out[16];
  assign oled_res_o   = gpio_out[17];

  // Zynq Processing System
  ps7_wrapper ps7_wrapper_i (
    .DDR_addr           ( DDR_addr           ),
    .DDR_ba             ( DDR_ba             ),
    .DDR_cas_n          ( DDR_cas_n          ),
    .DDR_ck_n           ( DDR_ck_n           ),
    .DDR_ck_p           ( DDR_ck_p           ),
    .DDR_cke            ( DDR_cke            ),
    .DDR_cs_n           ( DDR_cs_n           ),
    .DDR_dm             ( DDR_dm             ),
    .DDR_dq             ( DDR_dq             ),
    .DDR_dqs_n          ( DDR_dqs_n          ),
    .DDR_dqs_p          ( DDR_dqs_p          ),
    .DDR_odt            ( DDR_odt            ),
    .DDR_ras_n          ( DDR_ras_n          ),
    .DDR_reset_n        ( DDR_reset_n        ),
    .DDR_we_n           ( DDR_we_n           ),

    .FIXED_IO_ddr_vrn   ( FIXED_IO_ddr_vrn   ),
    .FIXED_IO_ddr_vrp   ( FIXED_IO_ddr_vrp   ),
    .FIXED_IO_mio       ( FIXED_IO_mio       ),
    .FIXED_IO_ps_clk    ( FIXED_IO_ps_clk    ),
    .FIXED_IO_ps_porb   ( FIXED_IO_ps_porb   ),
    .FIXED_IO_ps_srstb  ( FIXED_IO_ps_srstb  ),

    .clking_axi_awaddr  ( clking_axi_awaddr  ),
    .clking_axi_awprot  ( clking_axi_awprot  ),
    .clking_axi_awvalid ( clking_axi_awvalid ),
    .clking_axi_awready ( clking_axi_awready ),
    .clking_axi_wdata   ( clking_axi_wdata   ),
    .clking_axi_wstrb   ( clking_axi_wstrb   ),
    .clking_axi_wvalid  ( clking_axi_wvalid  ),
    .clking_axi_wready  ( clking_axi_wready  ),
    .clking_axi_bresp   ( clking_axi_bresp   ),
    .clking_axi_bvalid  ( clking_axi_bvalid  ),
    .clking_axi_bready  ( clking_axi_bready  ),
    .clking_axi_araddr  ( clking_axi_araddr  ),
    .clking_axi_arprot  ( clking_axi_arprot  ),
    .clking_axi_arvalid ( clking_axi_arvalid ),
    .clking_axi_arready ( clking_axi_arready ),
    .clking_axi_rdata   ( clking_axi_rdata   ),
    .clking_axi_rresp   ( clking_axi_rresp   ),
    .clking_axi_rvalid  ( clking_axi_rvalid  ),
    .clking_axi_rready  ( clking_axi_rready  ),

    .fetch_enable       ( fetch_enable       ),
    .ps7_clk            ( ps7_clk            ),
    .ps7_rst_n          ( ps7_rst_n          ),

    .UART_0_rxd         ( uart_tx            ),
    .UART_0_txd         ( uart_rx            ),

    .gpio_io_i          ( gpio_out           ),
    .gpio_io_o          ( gpio_in_ps7        ),
    .jtag_emu_i         ( jtag_emu_i         ),
    .jtag_emu_o         ( jtag_emu_o         ),

    .SPI0_MISO_I        ( spi_miso           ),
    .SPI0_MOSI_O        ( spi_mosi           ),
    .SPI0_MOSI_I        ( 1'b0               ),
    .SPI0_SCLK_O        ( spi_sck            ),
    .SPI0_SCLK_I        ( 1'b0               ),
    .SPI0_SS_O          ( spi_cs             ),
    .SPI0_SS_I          ( 1'b1               )
  );


  clk_rst_gen clk_rst_gen_i (
    .ref_clk_i               ( ref_clk_i               ),
    .rst_ni                  ( rst_ni                  ),

    .clking_axi_aclk         ( clking_axi_aclk         ),
    .clking_axi_aresetn      ( clking_axi_aresetn      ),
    .clking_axi_awaddr       ( clking_axi_awaddr       ),
    .clking_axi_awvalid      ( clking_axi_awvalid      ),
    .clking_axi_awready      ( clking_axi_awready      ),
    .clking_axi_wdata        ( clking_axi_wdata        ),
    .clking_axi_wstrb        ( clking_axi_wstrb        ),
    .clking_axi_wvalid       ( clking_axi_wvalid       ),
    .clking_axi_wready       ( clking_axi_wready       ),
    .clking_axi_bresp        ( clking_axi_bresp        ),
    .clking_axi_bvalid       ( clking_axi_bvalid       ),
    .clking_axi_bready       ( clking_axi_bready       ),
    .clking_axi_araddr       ( clking_axi_araddr       ),
    .clking_axi_arvalid      ( clking_axi_arvalid      ),
    .clking_axi_arready      ( clking_axi_arready      ),
    .clking_axi_rdata        ( clking_axi_rdata        ),
    .clking_axi_rresp        ( clking_axi_rresp        ),
    .clking_axi_rvalid       ( clking_axi_rvalid       ),
    .clking_axi_rready       ( clking_axi_rready       ),

    .rstn_pulpino_o          ( s_rstn_pulpino          ),
    .clk_pulpino_o           ( s_clk_pulpino           )
);

  // PULPino SoC
  pulpino  pulpino_wrap_i (
    .clk               ( s_clk_pulpino  ),
    .rst_n             ( s_rstn_pulpino ),

    .fetch_enable_i    ( fetch_en       ),

    .tck_i             ( tck_i          ),
    .trstn_i           ( trst_ni        ),
    .tms_i             ( tms_i          ),
    .tdi_i             ( td_i           ),
    .tdo_o             ( td_o           ),

    .spi_clk_i         ( spi_sck        ),
    .spi_cs_i          ( spi_cs         ),
    .spi_mode_o        (                ),
    .spi_sdi0_i        ( spi_mosi       ),
    .spi_sdi1_i        ( 1'b0           ),
    .spi_sdi2_i        ( 1'b0           ),
    .spi_sdi3_i        ( 1'b0           ),
    .spi_sdo0_o        ( spi_miso       ),
    .spi_sdo1_o        (                ),
    .spi_sdo2_o        (                ),
    .spi_sdo3_o        (                ),

    .spi_master_clk_o  (                ),
    .spi_master_csn0_o (                ),
    .spi_master_csn1_o (                ),
    .spi_master_csn2_o (                ),
    .spi_master_csn3_o (                ),
    .spi_master_mode_o (                ),
    .spi_master_sdi0_i ( 1'b0           ),
    .spi_master_sdi1_i ( 1'b0           ),
    .spi_master_sdi2_i ( 1'b0           ),
    .spi_master_sdi3_i ( 1'b0           ),
    .spi_master_sdo0_o (                ),
    .spi_master_sdo1_o (                ),
    .spi_master_sdo2_o (                ),
    .spi_master_sdo3_o (                ),

    .scl_i             ( scl_in         ),
    .scl_o             ( scl_out        ),
    .scl_oen_o         ( scl_oen        ),
    .sda_i             ( sda_in         ),
    .sda_o             ( sda_out        ),
    .sda_oen_o         ( sda_oen        ),

    .gpio_in           ( gpio_in        ),
    .gpio_out          ( gpio_out       ),
    .gpio_dir          ( gpio_dir       ),

    .uart_tx           ( uart_tx        ), // output
    .uart_rx           ( uart_rx        ), // input
    .uart_rts          (                ), // output
    .uart_dtr          (                ), // output
    .uart_cts          ( 1'b0           ), // input
    .uart_dsr          ( 1'b0           )  // input
  );

endmodule
