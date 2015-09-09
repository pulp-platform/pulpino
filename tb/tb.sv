
`define CLK_SEMIPERIOD   10.0ns  // 50 MHz

module tb;
  parameter  LOAD_L2       = "PRELOAD";   // valid values are "SPI", "STANDALONE" "PRELOAD", "" (no load of L2)
  parameter  ENABLE_VPI    = 0;
  parameter  BAUDRATE      = 3125000;

  logic s_clk   = 1'b0;
  logic s_rst_n = 1'b0;

  logic fetch_enable = 1'b0;

  logic [1:0]   padmode_spi_master;
  logic         spi_sck   = 1'b0;
  logic         spi_csn   = 1'b1;
  logic [1:0]   spi_mode;
  logic         spi_sdo0;
  logic         spi_sdo1;
  logic         spi_sdo2;
  logic         spi_sdo3;
  logic         spi_sdi0;
  logic         spi_sdi1;
  logic         spi_sdi2;
  logic         spi_sdi3;

  logic uart_tx;
  logic uart_rx;

  logic [31:0] gpio_out;

  logic tck;
  logic trstn;
  logic tms;
  logic tdi;
  logic tdo;


  generate if(ENABLE_VPI == 1)
  begin
    // JTAG VERIFICATION IP
    jtag_vpi
    #(
      .TIMEOUT_COUNT ( 6'd10 )
      )
    i_jtag
    (
      .clk_i    ( s_clk           ),
      .enable_i ( s_rst_n         ),

      .tms_o    ( tms    ),
      .tck_o    ( tck    ),
      .trst_o   ( trstn  ),
      .tdi_o    ( tdi    ),
      .tdo_i    ( tdo    )
     );
  end
  endgenerate


  uart_tb_rx
  #(
    .BAUD_RATE(BAUDRATE),
    .PARITY_EN(0)
  )
  uart_tb_rx_i
  (
    .rx(uart_rx),
    .rx_en(1'b1),
    .word_done()
  );


  pulpino_top top_i
  (
    .clk   ( s_clk   ),
    .rst_n ( s_rst_n ),

    .fetch_enable_i ( fetch_enable ),

    .spi_clk_i  ( spi_sck  ),
    .spi_cs_i   ( spi_csn  ),
    .spi_mode_o ( spi_mode ),
    .spi_sdo0_o ( spi_sdi0 ),
    .spi_sdo1_o ( spi_sdi1 ),
    .spi_sdo2_o ( spi_sdi2 ),
    .spi_sdo3_o ( spi_sdi3 ),
    .spi_sdi0_i ( spi_sdo0 ),
    .spi_sdi1_i ( spi_sdo1 ),
    .spi_sdi2_i ( spi_sdo2 ),
    .spi_sdi3_i ( spi_sdo3 ),

    .uart_tx   ( uart_rx  ),
    .uart_rx   ( uart_tx  ),
    .uart_rts  (          ),
    .uart_dtr  (          ),
    .uart_cts  (          ),
    .uart_dsr  (          ),

    .gpio_in     ( ),
    .gpio_out    ( gpio_out ),
    .gpio_dir    ( ),
    .gpio_padcfg ( ),

    .tck_i   ( tck   ),
    .trstn_i ( trstn ),
    .tms_i   ( tms   ),
    .tdi_i   ( tdi   ),
    .tdo_o   ( tdo   )
  );

  initial
  begin
    #(`CLK_SEMIPERIOD);
    s_clk = 1'b1;
    forever s_clk = #(`CLK_SEMIPERIOD) ~s_clk;
  end

  initial
  begin
    s_rst_n      = 1'b0;
    fetch_enable = 1'b0;

    #10000;

    s_rst_n = 1'b1;

    #10000;

    if (LOAD_L2 == "PRELOAD")
    begin
      // preload memories
      $readmemh("slm_files/l2_stim.slm",    top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_i.mem);
      $readmemh("slm_files/tcdm_bank0.slm", top_i.core_region_i.data_mem.sp_ram_i.mem);
    end
    else if (LOAD_L2 == "SPI")
    begin
      $readmemh("./slm_files/spi_stim.txt", stimuli);  // read in the stimuli vectors  == address_value

      spi_enable_qpi();

      spi_load();
    end

    fetch_enable = 1'b1;


    wait(top_i.gpio_out[14]);
    $stop();
  end

  // TODO: this is hack, do it properly!
  `include "tb_spi_pkg.sv"

endmodule
