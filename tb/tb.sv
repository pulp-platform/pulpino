
`define CLK_SEMIPERIOD   10.0ns  // 50 MHz

module tb;
  parameter  BAUDRATE = 3125000;

  logic s_clk;
  logic s_rst_n;

  logic uart_tx;
  logic uart_rx;

  logic gpio_out;

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


  top top_i
  (
    .clk   ( s_clk   ),
    .rst_n ( s_rst_n ),

    .uart_tx   ( uart_rx  ),
    .uart_rx   ( uart_tx  ),
    .uart_rts  (          ),
    .uart_dtr  (          ),
    .uart_cts  (          ),
    .uart_dsr  (          ),

    .gpio_in     ( ),
    .gpio_out    ( gpio_out ),
    .gpio_dir    ( ),
    .gpio_padcfg ( )
  );

  initial
  begin
    s_clk = 1'b1;
    forever s_clk = #(`CLK_SEMIPERIOD) ~s_clk;
  end

  initial
  begin
    s_rst_n <= 1'b0;

    // preload memories
    $readmemh("slm_files/l2_stim.slm",    top_i.core_region_i.instr_mem.mem);
    $readmemh("slm_files/tcdm_bank0.slm", top_i.core_region_i.data_mem.mem);

    #10000;

    s_rst_n <= 1'b1;

    wait(top_i.gpio_out[14]);
    $stop();
  end

endmodule
