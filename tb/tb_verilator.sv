module tb_verilator
#(
  parameter CLK_DIV = 16'h7
)
(
  input  logic                  clk,
  input  logic                  rst_n,
  input  logic                  fetch_enable_i,
  input  logic                  spi_clk_i,
  input  logic                  spi_cs_i ,
  output logic                  spi_mode_o,
  output logic                  spi_sdo0_o,
  output logic                  spi_sdo1_o,
  output logic                  spi_sdo2_o,
  output logic                  spi_sdo3_o,
  input  logic                  spi_sdi0_i,
  input  logic                  spi_sdi1_i,
  input  logic                  spi_sdi2_i,
  input  logic                  spi_sdi3_i,
  output logic                  uart_rx,
  output logic [31:0]           gpio_out,

  output logic                  uart_busy
);

  logic uart_rx;
  logic rx_valid;
  logic parity_error;
  logic [7:0] rx_data;

  pulpino_top top_i
  (
    .clk               ( clk            ),
    .rst_n             ( rst_n          ),

    .clk_sel_i         ( 1'b0           ),
    .testmode_i        ( 1'b0           ),
    .fetch_enable_i    ( fetch_enable_i ),

    .spi_clk_i         ( spi_clk_i      ),
    .spi_cs_i          ( spi_cs_i       ),
    .spi_mode_o        ( spi_mode_o     ),
    .spi_sdo0_o        ( spi_sdo0_o     ),
    .spi_sdo1_o        ( spi_sdo1_o     ),
    .spi_sdo2_o        ( spi_sdo2_o     ),
    .spi_sdo3_o        ( spi_sdo3_o     ),
    .spi_sdi0_i        ( spi_sdi0_i     ),
    .spi_sdi1_i        ( spi_sdi1_i     ),
    .spi_sdi2_i        ( spi_sdi2_i     ),
    .spi_sdi3_i        ( spi_sdi3_i     ),

    .spi_master_clk_o  (              ),
    .spi_master_csn0_o (              ),
    .spi_master_csn1_o (              ),
    .spi_master_csn2_o (              ),
    .spi_master_csn3_o (              ),
    .spi_master_mode_o (              ),
    .spi_master_sdo0_o (              ),
    .spi_master_sdo1_o (              ),
    .spi_master_sdo2_o (              ),
    .spi_master_sdo3_o (              ),
    .spi_master_sdi0_i (              ),
    .spi_master_sdi1_i (              ),
    .spi_master_sdi2_i (              ),
    .spi_master_sdi3_i (              ),

    .scl_pad_i         (              ),
    .scl_pad_o         (              ),
    .scl_padoen_o      (              ),
    .sda_pad_i         (              ),
    .sda_pad_o         (              ),
    .sda_padoen_o      (              ),


    .uart_tx           ( uart_rx      ),
    .uart_rx           ( uart_rx      ),

    .gpio_in           (              ),
    .gpio_out          ( gpio_out     ),
    .gpio_dir          (              ),
    .gpio_padcfg       (              ),

    .tck_i             (              ),
    .trstn_i           (              ),
    .tms_i             (              ),
    .tdi_i             (              ),
    .tdo_o             (              )
  );

  // config as 8N1
  uart_rx uart_rx_i
  (
      .clk_i              ( clk              ),
      .rstn_i             ( rst_n            ),
      .rx_i               ( uart_rx          ),
      .cfg_en_i           ( 1'b1             ),
      .cfg_div_i          ( CLK_DIV          ),
      .cfg_parity_en_i    ( 1'b0             ),
      .cfg_bits_i         ( 2'b11            ),

      .busy_o             ( uart_busy        ),

      .err_o              ( parity_error     ),
      .err_clr_i          ( 1'b1             ),
      .rx_data_o          ( rx_data          ),
      .rx_valid_o         ( rx_valid         ),
      .rx_ready_i         ( 1'b1             ) // always ready heree
  );

  always_ff @(posedge rx_valid)
  begin
    if (parity_error == 1'b0)
      $write("%C", rx_data);
    else
      $display("Parity error detected");
  end
endmodule
