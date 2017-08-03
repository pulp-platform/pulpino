// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

module pulpino(
  clk,
  rst_n,

  fetch_enable_i,

  spi_clk_i,
  spi_cs_i,
  spi_mode_o,
  spi_sdo0_o,
  spi_sdo1_o,
  spi_sdo2_o,
  spi_sdo3_o,
  spi_sdi0_i,
  spi_sdi1_i,
  spi_sdi2_i,
  spi_sdi3_i,

  spi_master_clk_o,
  spi_master_csn0_o,
  spi_master_csn1_o,
  spi_master_csn2_o,
  spi_master_csn3_o,
  spi_master_mode_o,
  spi_master_sdo0_o,
  spi_master_sdo1_o,
  spi_master_sdo2_o,
  spi_master_sdo3_o,
  spi_master_sdi0_i,
  spi_master_sdi1_i,
  spi_master_sdi2_i,
  spi_master_sdi3_i,

  uart_tx,
  uart_rx,
  uart_rts,
  uart_dtr,
  uart_cts,
  uart_dsr,

  scl_i,
  scl_o,
  scl_oen_o,
  sda_i,
  sda_o,
  sda_oen_o,

  gpio_in,
  gpio_out,
  gpio_dir,

  tck_i,
  trstn_i,
  tms_i,
  tdi_i,
  tdo_o
  );

  // Clock and Reset
  input         clk;
  input         rst_n;

  input         fetch_enable_i;

  input         spi_clk_i;
  input         spi_cs_i;
  output  [1:0] spi_mode_o;
  output        spi_sdo0_o;
  output        spi_sdo1_o;
  output        spi_sdo2_o;
  output        spi_sdo3_o;
  input         spi_sdi0_i;
  input         spi_sdi1_i;
  input         spi_sdi2_i;
  input         spi_sdi3_i;

  output        spi_master_clk_o;
  output        spi_master_csn0_o;
  output        spi_master_csn1_o;
  output        spi_master_csn2_o;
  output        spi_master_csn3_o;
  output  [1:0] spi_master_mode_o;
  output        spi_master_sdo0_o;
  output        spi_master_sdo1_o;
  output        spi_master_sdo2_o;
  output        spi_master_sdo3_o;
  input         spi_master_sdi0_i;
  input         spi_master_sdi1_i;
  input         spi_master_sdi2_i;
  input         spi_master_sdi3_i;

  output        uart_tx;
  input         uart_rx;
  output        uart_rts;
  output        uart_dtr;
  input         uart_cts;
  input         uart_dsr;

  input         scl_i;
  output        scl_o;
  output        scl_oen_o;
  input         sda_i;
  output        sda_o;
  output        sda_oen_o;

  input  [31:0] gpio_in;
  output [31:0] gpio_out;
  output [31:0] gpio_dir;

  // JTAG signals
  input  tck_i;
  input  trstn_i;
  input  tms_i;
  input  tdi_i;
  output tdo_o;

  parameter USE_ZERO_RISCY = 0;
  parameter RISCY_RV32F = 0;
  parameter ZERO_RV32M = 0;
  parameter ZERO_RV32E = 0;
   
  // PULP SoC
  pulpino_top
  #(
    .USE_ZERO_RISCY    ( USE_ZERO_RISCY ),
    .RISCY_RV32F       ( RISCY_RV32F    ),
    .ZERO_RV32M        ( ZERO_RV32M     ),
    .ZERO_RV32E        ( ZERO_RV32E     )
  )
  pulpino_i
  (
    .clk               ( clk               ),
    .rst_n             ( rst_n             ),

    .clk_sel_i         ( 1'b0              ),
    .clk_standalone_i  ( 1'b0              ),

    .testmode_i        ( 1'b0              ),
    .fetch_enable_i    ( fetch_enable_i    ),
    .scan_enable_i     ( 1'b0              ),

    .spi_clk_i         ( spi_clk_i         ),
    .spi_cs_i          ( spi_cs_i          ),
    .spi_mode_o        ( spi_mode_o        ),
    .spi_sdo0_o        ( spi_sdo0_o        ),
    .spi_sdo1_o        ( spi_sdo1_o        ),
    .spi_sdo2_o        ( spi_sdo2_o        ),
    .spi_sdo3_o        ( spi_sdo3_o        ),
    .spi_sdi0_i        ( spi_sdi0_i        ),
    .spi_sdi1_i        ( spi_sdi1_i        ),
    .spi_sdi2_i        ( spi_sdi2_i        ),
    .spi_sdi3_i        ( spi_sdi3_i        ),

    .spi_master_clk_o  ( spi_master_clk_o  ),
    .spi_master_csn0_o ( spi_master_csn0_o ),
    .spi_master_csn1_o ( spi_master_csn1_o ),
    .spi_master_csn2_o ( spi_master_csn2_o ),
    .spi_master_csn3_o ( spi_master_csn3_o ),
    .spi_master_mode_o ( spi_master_mode_o ),
    .spi_master_sdo0_o ( spi_master_sdo0_o ),
    .spi_master_sdo1_o ( spi_master_sdo1_o ),
    .spi_master_sdo2_o ( spi_master_sdo2_o ),
    .spi_master_sdo3_o ( spi_master_sdo3_o ),
    .spi_master_sdi0_i ( spi_master_sdi0_i ),
    .spi_master_sdi1_i ( spi_master_sdi1_i ),
    .spi_master_sdi2_i ( spi_master_sdi2_i ),
    .spi_master_sdi3_i ( spi_master_sdi3_i ),

    .uart_tx           ( uart_tx           ), // output
    .uart_rx           ( uart_rx           ), // input
    .uart_rts          ( uart_rts          ), // output
    .uart_dtr          ( uart_dtr          ), // output
    .uart_cts          ( uart_cts          ), // input
    .uart_dsr          ( uart_dsr          ), // input

    .scl_pad_i         ( scl_i             ),
    .scl_pad_o         ( scl_o             ),
    .scl_padoen_o      ( scl_oen_o         ),
    .sda_pad_i         ( sda_i             ),
    .sda_pad_o         ( sda_o             ),
    .sda_padoen_o      ( sda_oen_o         ),

    .gpio_in           ( gpio_in           ),
    .gpio_out          ( gpio_out          ),
    .gpio_dir          ( gpio_dir          ),
    .gpio_padcfg       (                   ),

    .tck_i             ( tck_i             ),
    .trstn_i           ( trstn_i           ),
    .tms_i             ( tms_i             ),
    .tdi_i             ( tdi_i             ),
    .tdo_o             ( tdo_o             ),

    .pad_cfg_o         (                   ),
    .pad_mux_o         (                   )
  );

endmodule
