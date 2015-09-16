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
  FIXED_IO_ps_srstb
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

  wire [31:0] end_of_operation;
  wire [31:0] fetch_enable;
  wire        ps7_clk;
  wire        ps7_rst_n;
  wire        ps7_rst_pulp_n;
  wire        ps7_rst_clking_n;

  wire        ref_clk_i;               // input
  wire        test_clk_i;              // input
  wire        mode_select_i;           // input
  wire        rst_ni;                  // input
  wire        clk_o;                   // output
  wire        test_mode_i;             // input
  wire        fetch_en_i;              // input
  wire        eoc_o;                   // output
  wire [1:0]  return_o;                // output
  wire        tck_i;                   // input
  wire        trst_ni;                 // input
  wire        tms_i;                   // input
  wire        td_i;                    // input
  wire        td_o;                    // output
  wire        spi_master_clk;
  wire        spi_master_csn;
  wire [1:0]  spi_master_mode;
  wire        spi_master_sdo0;
  wire        spi_master_sdo1;
  wire        spi_master_sdo2;
  wire        spi_master_sdo3;
  wire        spi_master_sdi0;
  wire        spi_master_sdi1;
  wire        spi_master_sdi2;
  wire        spi_master_sdi3;

  wire [31:0] gpio_dir;                // output
  wire [31:0] gpio_in;                 // input
  wire [31:0] gpio_out;                // output

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
  wire        uart_rts;           // output
  wire        uart_dtr;           // output
  wire        uart_cts;           // input
  wire        uart_dsr;           // input

// clocke generator signals
  wire s_rstn_sync;
  wire s_initn_sync;
  wire s_rstn_cluster_sync;
  wire s_rstn_cluster_sync_soc;
  wire s_clk_soc;
  wire s_clk_cluster;

  assign ref_clk_i        = ps7_clk;
  assign clking_axi_aclk  = ps7_clk;
  assign clking_axi_aresetn = ps7_rst_clking_n;
  // assign test_clk_i       = ps7_clk;
  assign test_clk_i       = 0;
  assign mode_select_i    = 0;
  assign test_mode_i      = 1'b0;
  assign rst_ni           = fetch_enable[31];
  assign ps7_rst_pulp_n   = ps7_rst_n;
  assign ps7_rst_clking_n = ps7_rst_n;

  reg fetch_en_r;
  reg fetch_en_deb;

  assign fetch_en_i = fetch_en_r;

  reg [31:0] end_of_operation_r;
  always @(posedge ps7_clk or negedge ps7_rst_pulp_n)
  begin
    if(ps7_rst_pulp_n == 1'b0)
      end_of_operation_r = 32'b0;
    else begin
      end_of_operation_r[0]    = eoc_o;
      end_of_operation_r[2:1]  = return_o;
    end
  end
  assign end_of_operation = end_of_operation_r;

  always @(posedge ps7_clk or negedge ps7_rst_pulp_n)
  begin
    if(ps7_rst_pulp_n == 1'b0)
      fetch_en_r = 1'b0;
    else
      fetch_en_r = fetch_enable[0] & fetch_en_deb;
  end

  // jtag - constant for now
  assign tck_i   = 0;
  assign trst_ni = 0;
  assign tms_i   = 0;
  assign td_i    = 0;

  // spi master - constant for now
  assign spi_master_sdi0 = 0;
  assign spi_master_sdi1 = 0;
  assign spi_master_sdi2 = 0;
  assign spi_master_sdi3 = 0;


  // FETCH EN debouncing
  // FETCH EN is debounced in both transitions
  reg [5:0] fetch_en_counter;
  always @(posedge ps7_clk or negedge ps7_rst_pulp_n)
  begin
    if(ps7_rst_pulp_n==1'b0) begin
      fetch_en_deb <= 1'b1;
      fetch_en_counter <= 6'd0;
    end
    else begin
      fetch_en_deb <= 1'b1;
    end
  end

  // gpio in - constant for now
  assign gpio_in = 0;

  // other unused ulpsoc inputs
  assign uart_cts = 0;
  assign uart_dsr = 0;
  assign uart_rx  = 1'b0;

  // Zynq Processing System
  ps7_wrapper ps7_wrapper_i (
    .DDR_addr          (DDR_addr         ),
    .DDR_ba            (DDR_ba           ),
    .DDR_cas_n         (DDR_cas_n        ),
    .DDR_ck_n          (DDR_ck_n         ),
    .DDR_ck_p          (DDR_ck_p         ),
    .DDR_cke           (DDR_cke          ),
    .DDR_cs_n          (DDR_cs_n         ),
    .DDR_dm            (DDR_dm           ),
    .DDR_dq            (DDR_dq           ),
    .DDR_dqs_n         (DDR_dqs_n        ),
    .DDR_dqs_p         (DDR_dqs_p        ),
    .DDR_odt           (DDR_odt          ),
    .DDR_ras_n         (DDR_ras_n        ),
    .DDR_reset_n       (DDR_reset_n      ),
    .DDR_we_n          (DDR_we_n         ),

    .FIXED_IO_ddr_vrn  (FIXED_IO_ddr_vrn ),
    .FIXED_IO_ddr_vrp  (FIXED_IO_ddr_vrp ),
    .FIXED_IO_mio      (FIXED_IO_mio     ),
    .FIXED_IO_ps_clk   (FIXED_IO_ps_clk  ),
    .FIXED_IO_ps_porb  (FIXED_IO_ps_porb ),
    .FIXED_IO_ps_srstb (FIXED_IO_ps_srstb),

    .clking_axi_awaddr (clking_axi_awaddr ),
    .clking_axi_awprot (clking_axi_awprot ),
    .clking_axi_awvalid(clking_axi_awvalid),
    .clking_axi_awready(clking_axi_awready),
    .clking_axi_wdata  (clking_axi_wdata  ),
    .clking_axi_wstrb  (clking_axi_wstrb  ),
    .clking_axi_wvalid (clking_axi_wvalid ),
    .clking_axi_wready (clking_axi_wready ),
    .clking_axi_bresp  (clking_axi_bresp  ),
    .clking_axi_bvalid (clking_axi_bvalid ),
    .clking_axi_bready (clking_axi_bready ),
    .clking_axi_araddr (clking_axi_araddr ),
    .clking_axi_arprot (clking_axi_arprot ),
    .clking_axi_arvalid(clking_axi_arvalid),
    .clking_axi_arready(clking_axi_arready),
    .clking_axi_rdata  (clking_axi_rdata  ),
    .clking_axi_rresp  (clking_axi_rresp  ),
    .clking_axi_rvalid (clking_axi_rvalid ),
    .clking_axi_rready (clking_axi_rready ),

    .end_of_operation  (end_of_operation ),
    .fetch_enable      (fetch_enable     ),
    .ps7_clk           (ps7_clk          ),
    .ps7_rst_n         (ps7_rst_n        )

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

    .rstn_sync_o             ( s_rstn_sync             ),
    .initn_sync_o            ( s_initn_sync            ),
    .rstn_cluster_o          ( s_rstn_cluster_sync     ),
    .rstn_cluster_sync_soc_o ( s_rstn_cluster_sync_soc ),
    .clk_soc_o               ( s_clk_soc               ),
    .clk_cluster_o           ( s_clk_cluster           )
);

  // PULP SoC
  pulpino pulpino_wrap_i (
    .clk                     (s_clk_soc              ),
    .rst_n                   (s_rstn_cluster_sync    ),

    .tck_i                   (tck_i                  ),
    .trstn_i                 (trst_ni                ),
    .tms_i                   (tms_i                  ),
    .tdi_i                   (td_i                   ),
    .tdo_o                   (td_o                   ),

    .gpio_in                 (gpio_in                ),
    .gpio_out                (gpio_out               ),
    .gpio_dir                (gpio_dir               ),

    .uart_tx                 (uart_tx                ), // output
    .uart_rx                 (uart_rx                ), // input
    .uart_rts                (uart_rts               ), // output
    .uart_dtr                (uart_dtr               ), // output
    .uart_cts                (uart_cts               ), // input
    .uart_dsr                (uart_dsr               )  // input
  );

endmodule
