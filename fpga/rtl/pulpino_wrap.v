module pulpino(clk,
  rst_n,
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
  uart_tx,
  uart_rx,
  uart_rts,
  uart_dtr,
  uart_cts,
  uart_dsr,
  gpio_in,
  gpio_out,
  gpio_dir,
  tck_i,
  trstn_i,
  tms_i,
  tdi_i,
  tdo_o);

  // Clock and Reset
  input         clk;
  input         rst_n;

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

  output        uart_tx;
  input         uart_rx;
  output        uart_rts;
  output        uart_dtr;
  input         uart_cts;
  input         uart_dsr;

  input  [31:0] gpio_in;
  output [31:0] gpio_out;
  output [31:0] gpio_dir;

  // JTAG signals
  input  tck_i;
  input  trstn_i;
  input  tms_i;
  input  tdi_i;
  output tdo_o;

  // PULP SoC
  pulpino_top pulpino_i (
    .clk              ( clk              ),
    .rst_n            ( rst_n            ),

    .spi_clk_i        ( spi_clk_i        ),
    .spi_cs_i         ( spi_cs_i         ),
    .spi_mode_o       ( spi_mode_o       ),
    .spi_sdo0_o       ( spi_sdo0_o       ),
    .spi_sdo1_o       ( spi_sdo1_o       ),
    .spi_sdo2_o       ( spi_sdo2_o       ),
    .spi_sdo3_o       ( spi_sdo3_o       ),
    .spi_sdi0_i       ( spi_sdi0_i       ),
    .spi_sdi1_i       ( spi_sdi1_i       ),
    .spi_sdi2_i       ( spi_sdi2_i       ),
    .spi_sdi3_i       ( spi_sdi3_i       ),

    .uart_tx          ( uart_tx          ), // output
    .uart_rx          ( uart_rx          ), // input
    .uart_rts         ( uart_rts         ), // output
    .uart_dtr         ( uart_dtr         ), // output
    .uart_cts         ( uart_cts         ), // input
    .uart_dsr         ( uart_dsr         ), // input

    .gpio_in          ( gpio_in          ),
    .gpio_out         ( gpio_out         ),
    .gpio_dir         ( gpio_dir         ),
    .gpio_padcfg      (                  ),

    .tck_i            ( tck_i            ),
    .trstn_i          ( trstn_i          ),
    .tms_i            ( tms_i            ),
    .tdi_i            ( tdi_i            ),
    .tdo_o            ( tdo_o            )
  );

endmodule
