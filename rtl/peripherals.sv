
`include "axi_bus.sv"
`include "config.sv"

module peripherals
  #(
    parameter AXI_ADDR_WIDTH       = 32,
    parameter AXI_DATA_WIDTH       = 64,
    parameter AXI_USER_WIDTH       = 6,
    parameter AXI_SLAVE_ID_WIDTH   = 6,
    parameter AXI_MASTER_ID_WIDTH  = 6,
    parameter ROM_START_ADDR       = 32'h8000
  )
  (
    // Clock and Reset
    input logic clk_i,
    input logic rst_n,

    AXI_BUS.Master axi_spi_master,

    input  logic             spi_clk_i,
    input  logic             testmode_i,
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


    AXI_BUS.Slave  slave,

    output logic              uart_tx,
    input  logic              uart_rx,
    output logic              uart_rts,
    output logic              uart_dtr,
    input  logic              uart_cts,
    input  logic              uart_dsr,

    output logic              spi_master_clk,
    output logic              spi_master_csn0,
    output logic              spi_master_csn1,
    output logic              spi_master_csn2,
    output logic              spi_master_csn3,
    output logic       [1:0]  spi_master_mode,
    output logic              spi_master_sdo0,
    output logic              spi_master_sdo1,
    output logic              spi_master_sdo2,
    output logic              spi_master_sdo3,
    input  logic              spi_master_sdi0,
    input  logic              spi_master_sdi1,
    input  logic              spi_master_sdi2,
    input  logic              spi_master_sdi3,

    input  logic              scl_pad_i,
    output logic              scl_pad_o,
    output logic              scl_padoen_o,
    input  logic              sda_pad_i,
    output logic              sda_pad_o,
    output logic              sda_padoen_o,

    input  logic       [31:0] gpio_in,
    output logic       [31:0] gpio_out,
    output logic       [31:0] gpio_dir,
    output logic [31:0] [5:0] gpio_padcfg,

    input  logic              core_busy_i,
    output logic [31:0]       irq_o,
    input  logic              fetch_enable_i,
    output logic              fetch_enable_o,
    output logic              clk_gate_core_o,

    output logic              fll1_req_o,
    output logic              fll1_wrn_o,
    output logic [1:0]        fll1_add_o,
    output logic [31:0]       fll1_wdata_o,
    input  logic              fll1_ack_i,
    input  logic [31:0]       fll1_rdata_i,
    input  logic              fll1_lock_i,

    output logic [31:0] [5:0] pad_cfg_o,
    output logic       [31:0] pad_mux_o,
    output logic       [31:0] boot_addr_o
  );

  localparam APB_ADDR_WIDTH  = 12;
  localparam APB_NUM_SLAVES  = 8;

  logic                                s_penable;
  logic                                s_pwrite;
  logic        [APB_ADDR_WIDTH-1:0]    s_paddr;
  logic        [APB_NUM_SLAVES-1:0]    s_psel;
  logic                      [31:0]    s_pwdata;
  logic [APB_NUM_SLAVES-1:0] [31:0]    s_prdata;
  logic        [APB_NUM_SLAVES-1:0]    s_pready;
  logic        [APB_NUM_SLAVES-1:0]    s_pslverr;


  logic [1:0]   s_spim_event;
  logic [3:0]   timer_irq;
  logic s_uart_event;
  logic i2c_event;
  logic s_power_event;
  logic s_gpio_event;

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// SPI Slave, AXI Master                                      ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

  axi_spi_slave_wrap
  #(
    .AXI_ADDRESS_WIDTH  ( AXI_ADDR_WIDTH       ),
    .AXI_DATA_WIDTH     ( AXI_DATA_WIDTH       ),
    .AXI_USER_WIDTH     ( AXI_USER_WIDTH       ),
    .AXI_ID_WIDTH       ( AXI_MASTER_ID_WIDTH  )
  )
  axi_spi_slave_i
  (
    .clk_i      ( clk_i          ),
    .rst_ni     ( rst_n          ),

    .test_mode  ( testmode_i     ),

    .axi_master ( axi_spi_master ),

    .spi_clk    ( spi_clk_i      ),
    .spi_cs     ( spi_cs_i       ),
    .spi_mode   ( spi_mode_o     ),
    .spi_sdo0   ( spi_sdo0_o     ),
    .spi_sdo1   ( spi_sdo1_o     ),
    .spi_sdo2   ( spi_sdo2_o     ),
    .spi_sdo3   ( spi_sdo3_o     ),
    .spi_sdi0   ( spi_sdi0_i     ),
    .spi_sdi1   ( spi_sdi1_i     ),
    .spi_sdi2   ( spi_sdi2_i     ),
    .spi_sdi3   ( spi_sdi3_i     )
  );

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// AXI2APB Bridge                                             ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

  axi2apb_wrap
  #(
      .AXI_ADDR_WIDTH ( AXI_ADDR_WIDTH     ),
      .AXI_DATA_WIDTH ( AXI_DATA_WIDTH     ),
      .AXI_USER_WIDTH ( AXI_USER_WIDTH     ),
      .AXI_ID_WIDTH   ( AXI_SLAVE_ID_WIDTH ),
      .APB_ADDR_WIDTH ( APB_ADDR_WIDTH     ),
      .APB_NUM_SLAVES ( APB_NUM_SLAVES     )
  )
  axi2apb_i
  (
    .clk_i     ( clk_i     ),
    .rst_ni    ( rst_n     ),

    .axi_slave ( slave     ),

    .penable   ( s_penable ),
    .pwrite    ( s_pwrite  ),
    .paddr     ( s_paddr   ),
    .psel      ( s_psel    ),
    .pwdata    ( s_pwdata  ),
    .prdata    ( s_prdata  ),
    .pready    ( s_pready  ),
    .pslverr   ( s_pslverr )
  );

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// APB Slave 0: APB UART interface                            ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

  apb_uart i_apb_uart
  (
    .CLK      ( clk_i        ),
    .RSTN     ( rst_n        ),

    .PSEL     ( s_psel[0]    ),
    .PENABLE  ( s_penable    ),
    .PWRITE   ( s_pwrite     ),
    .PADDR    ( s_paddr[4:2] ),
    .PWDATA   ( s_pwdata     ),
    .PRDATA   ( s_prdata[0]  ),
    .PREADY   ( s_pready[0]  ),
    .PSLVERR  ( s_pslverr[0] ),

    .INT      ( s_uart_event ),   //Interrupt output

    .OUT1N    (),                    //Output 1
    .OUT2N    (),                    //Output 2
    .RTSN     ( uart_rts    ),       //RTS output
    .DTRN     ( uart_dtr    ),       //DTR output
    .CTSN     ( uart_cts    ),       //CTS input
    .DSRN     ( uart_dsr    ),       //DSR input
    .DCDN     ( 1'b1        ),       //DCD input
    .RIN      ( 1'b1        ),       //RI input
    .SIN      ( uart_rx     ),
    .SOUT     ( uart_tx     )
  );

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// APB Slave 1: APB GPIO interface                            ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

  apb_gpio apb_gpio_i
  (
    .HCLK       ( clk_i ),
    .HRESETn    ( rst_n ),

    .PADDR      ( s_paddr      ),
    .PWDATA     ( s_pwdata     ),
    .PWRITE     ( s_pwrite     ),
    .PSEL       ( s_psel[1]    ),
    .PENABLE    ( s_penable    ),
    .PRDATA     ( s_prdata[1]  ),
    .PREADY     ( s_pready[1]  ),
    .PSLVERR    ( s_pslverr[1] ),

    .gpio_in      ( gpio_in       ),
    .gpio_out     ( gpio_out      ),
    .gpio_dir     ( gpio_dir      ),
    .gpio_padcfg  ( gpio_padcfg   ),
    .power_event  ( s_power_event ),
    .interrupt    ( s_gpio_event  )
  );

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// APB Slave 2: APB SPI Master interface                      ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

  apb_spi_master
  #(
      .BUFFER_DEPTH(8)
  )
  apb_spi_master_i
  (
    .HCLK         ( clk_i ),
    .HRESETn      ( rst_n ),

    .PADDR        ( s_paddr      ),
    .PWDATA       ( s_pwdata     ),
    .PWRITE       ( s_pwrite     ),
    .PSEL         ( s_psel[2]    ),
    .PENABLE      ( s_penable    ),
    .PRDATA       ( s_prdata[2]  ),
    .PREADY       ( s_pready[2]  ),
    .PSLVERR      ( s_pslverr[2] ),

    .events_o     ( s_spim_event ),

    .spi_clk      ( spi_master_clk  ),
    .spi_csn0     ( spi_master_csn0 ),
    .spi_csn1     ( spi_master_csn1 ),
    .spi_csn2     ( spi_master_csn2 ),
    .spi_csn3     ( spi_master_csn3 ),
    .spi_mode     ( spi_master_mode ),
    .spi_sdo0     ( spi_master_sdo0 ),
    .spi_sdo1     ( spi_master_sdo1 ),
    .spi_sdo2     ( spi_master_sdo2 ),
    .spi_sdo3     ( spi_master_sdo3 ),
    .spi_sdi0     ( spi_master_sdi0 ),
    .spi_sdi1     ( spi_master_sdi1 ),
    .spi_sdi2     ( spi_master_sdi2 ),
    .spi_sdi3     ( spi_master_sdi3 )
  );

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// APB Slave 3: Timer Unit                                    ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

  apb_timer
  apb_timer_i
  (
    .HCLK       ( clk_i        ),
    .HRESETn    ( rst_n        ),

    .PADDR      ( s_paddr      ),
    .PWDATA     ( s_pwdata     ),
    .PWRITE     ( s_pwrite     ),
    .PSEL       ( s_psel[3]    ),
    .PENABLE    ( s_penable    ),
    .PRDATA     ( s_prdata[3]  ),
    .PREADY     ( s_pready[3]  ),
    .PSLVERR    ( s_pslverr[3] ),

    .irq_o      ( timer_irq    )
  );

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// APB Slave 4: Event Unit                                    ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

  apb_event_unit
  apb_event_unit_i
  (
    .HCLK             ( clk_i ),
    .HRESETn          ( rst_n ),

    .PADDR            ( s_paddr      ),
    .PWDATA           ( s_pwdata     ),
    .PWRITE           ( s_pwrite     ),
    .PSEL             ( s_psel[4]    ),
    .PENABLE          ( s_penable    ),
    .PRDATA           ( s_prdata[4]  ),
    .PREADY           ( s_pready[4]  ),
    .PSLVERR          ( s_pslverr[4] ),

    .irq_i            ( {timer_irq, s_spim_event, s_gpio_event, s_uart_event, i2c_event, 23'b0} ),
    .event_i          ( {timer_irq, s_spim_event, s_gpio_event, s_uart_event, i2c_event, 23'b0} ),
    .irq_o            ( irq_o              ),

    .fetch_enable_i   ( fetch_enable_i     ),
    .fetch_enable_o   ( fetch_enable_o     ),
    .clk_gate_core_o  ( clk_gate_core_o    ),
    .core_busy_i      ( core_busy_i        )
  );

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// APB Slave 5: I2C                                           ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

  apb_i2c
  apb_i2c_i
  (
    .HCLK         ( clk_i         ),
    .HRESETn      ( rst_n         ),

    .PADDR        ( s_paddr       ),
    .PWDATA       ( s_pwdata      ),
    .PWRITE       ( s_pwrite      ),
    .PSEL         ( s_psel[5]     ),
    .PENABLE      ( s_penable     ),
    .PRDATA       ( s_prdata[5]   ),
    .PREADY       ( s_pready[5]   ),
    .PSLVERR      ( s_pslverr[5]  ),
    .interrupt_o  ( i2c_event     ),
    .scl_pad_i    ( scl_pad_i     ),
    .scl_pad_o    ( scl_pad_o     ),
    .scl_padoen_o ( scl_padoen_o  ),
    .sda_pad_i    ( sda_pad_i     ),
    .sda_pad_o    ( sda_pad_o     ),
    .sda_padoen_o ( sda_padoen_o  )
  );


  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// APB Slave 6: FLL Ctrl                                      ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

    apb_fll_if apb_fll_if_i
    (
      .HCLK        ( clk_i        ),
      .HRESETn     ( rst_n        ),

      .PADDR       ( s_paddr      ),
      .PWDATA      ( s_pwdata     ),
      .PWRITE      ( s_pwrite     ),
      .PSEL        ( s_psel[6]    ),
      .PENABLE     ( s_penable    ),
      .PRDATA      ( s_prdata[6]  ),
      .PREADY      ( s_pready[6]  ),
      .PSLVERR     ( s_pslverr[6] ),

      .fll1_req    ( fll1_req_o   ),
      .fll1_wrn    ( fll1_wrn_o   ),
      .fll1_add    ( fll1_add_o   ),
      .fll1_data   ( fll1_wdata_o ),
      .fll1_ack    ( fll1_ack_i   ),
      .fll1_r_data ( fll1_rdata_i ),
      .fll1_lock   ( fll1_lock_i  ),

      .fll2_req    (              ),
      .fll2_wrn    (              ),
      .fll2_add    (              ),
      .fll2_data   (              ),
      .fll2_ack    (              ),
      .fll2_r_data ( 'b0          ),
      .fll2_lock   ( 1'b0         )
      );

  //////////////////////////////////////////////////////////////////
  ///                                                            ///
  /// APB Slave 7: PULPino control                               ///
  ///                                                            ///
  //////////////////////////////////////////////////////////////////

    apb_pulpino
    #(
      .BOOT_ADDR ( ROM_START_ADDR )
    )
    apb_pulpino_i
    (
      .HCLK        ( clk_i        ),
      .HRESETn     ( rst_n        ),

      .PADDR       ( s_paddr      ),
      .PWDATA      ( s_pwdata     ),
      .PWRITE      ( s_pwrite     ),
      .PSEL        ( s_psel[7]    ),
      .PENABLE     ( s_penable    ),
      .PRDATA      ( s_prdata[7]  ),
      .PREADY      ( s_pready[7]  ),
      .PSLVERR     ( s_pslverr[7] ),

      .pad_cfg_o   ( pad_cfg_o   ),
      .pad_mux_o   ( pad_mux_o   ),
      .boot_addr_o ( boot_addr_o  )
    );
endmodule
