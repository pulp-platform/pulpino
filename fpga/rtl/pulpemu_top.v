module pulpemu_top(DDR_addr, DDR_ba, DDR_cas_n, DDR_ck_n, DDR_ck_p, DDR_cke, DDR_cs_n, DDR_dm, DDR_dq, DDR_dqs_n, DDR_dqs_p, DDR_odt, DDR_ras_n, DDR_reset_n, DDR_we_n, FIXED_IO_ddr_vrn, FIXED_IO_ddr_vrp, FIXED_IO_mio, FIXED_IO_ps_clk, FIXED_IO_ps_porb, FIXED_IO_ps_srstb, );

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

  wire [14:0] BRAM_PORTA_addr;
  wire        BRAM_PORTA_clk;
  wire [63:0] BRAM_PORTA_din;
  wire [63:0] BRAM_PORTA_dout;
  wire        BRAM_PORTA_en;
  wire        BRAM_PORTA_rst;
  wire [7:0]  BRAM_PORTA_we;
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
  wire        ulpsoc_clk;
  wire        ps7_rst_n;
  wire        ps7_rst_pulp_n;
  wire        ps7_rst_clking_n;
`ifdef PULP3_PIRMIN
  wire        intr_mailbox;  
  wire        intr_rab_miss; 
  wire        intr_rab_multi;
  wire        intr_rab_prot; 
`endif

  wire        ref_clk_i;               // input
  wire        test_clk_i;              // input
  wire        mode_select_i;           // input
  wire        rst_ni;                  // input
  wire        clk_o;                   // output
  wire        test_mode_i;             // input
  wire        fetch_en_i;              // input
  wire        eoc_o;                   // output
  wire [1:0]  return_o;                // output
  wire        stdout_master_aw_valid;  // output
  wire [31:0] stdout_master_aw_addr;   // output
  wire [2:0]  stdout_master_aw_prot;   // output
  wire [3:0]  stdout_master_aw_region; // output
  wire [7:0]  stdout_master_aw_len;    // output
  wire [2:0]  stdout_master_aw_size;   // output
  wire [1:0]  stdout_master_aw_burst;  // output
  wire        stdout_master_aw_lock;   // output
  wire [3:0]  stdout_master_aw_cache;  // output
  wire [3:0]  stdout_master_aw_qos;    // output
  wire [9:0]  stdout_master_aw_id;     // output
  wire [5:0]  stdout_master_aw_user;   // output
  wire        stdout_master_aw_ready;  // input
  wire        stdout_master_ar_valid;  // output
  wire [31:0] stdout_master_ar_addr;   // output
  wire [2:0]  stdout_master_ar_prot;   // output
  wire [3:0]  stdout_master_ar_region; // output
  wire [7:0]  stdout_master_ar_len;    // output
  wire [2:0]  stdout_master_ar_size;   // output
  wire [1:0]  stdout_master_ar_burst;  // output
  wire        stdout_master_ar_lock;   // output
  wire [3:0]  stdout_master_ar_cache;  // output
  wire [3:0]  stdout_master_ar_qos;    // output
  wire [9:0]  stdout_master_ar_id;     // output
  wire [5:0]  stdout_master_ar_user;   // output
  wire        stdout_master_ar_ready;  // input
  wire        stdout_master_w_valid;   // output
  wire [63:0] stdout_master_w_data;    // output
  wire [7:0]  stdout_master_w_strb;    // output
  wire [5:0]  stdout_master_w_user;    // output
  wire        stdout_master_w_last;    // output
  wire        stdout_master_w_ready;   // input
  wire        stdout_master_r_valid;   // input
  wire [63:0] stdout_master_r_data;    // input
  wire [1:0]  stdout_master_r_resp;    // input
  wire        stdout_master_r_last;    // input
  wire [9:0]  stdout_master_r_id;      // input
  wire [5:0]  stdout_master_r_user;    // input
  wire        stdout_master_r_ready;   // output
  wire        stdout_master_b_valid;   // input
  wire [1:0]  stdout_master_b_resp;    // input
  wire [9:0]  stdout_master_b_id;      // input
  wire [5:0]  stdout_master_b_user;    // input
  wire        stdout_master_b_ready;   // output
  wire        stdout_slave_aw_valid;  // output
  wire [31:0] stdout_slave_aw_addr;   // output
  wire [2:0]  stdout_slave_aw_prot;   // output
  wire [3:0]  stdout_slave_aw_region; // output
  wire [7:0]  stdout_slave_aw_len;    // output
  wire [2:0]  stdout_slave_aw_size;   // output
  wire [1:0]  stdout_slave_aw_burst;  // output
  wire        stdout_slave_aw_lock;   // output
  wire [3:0]  stdout_slave_aw_cache;  // output
  wire [3:0]  stdout_slave_aw_qos;    // output
  wire [9:0]  stdout_slave_aw_id;     // output
  wire [5:0]  stdout_slave_aw_user;   // output
  wire        stdout_slave_aw_ready;  // input
  wire        stdout_slave_ar_valid;  // output
  wire [31:0] stdout_slave_ar_addr;   // output
  wire [2:0]  stdout_slave_ar_prot;   // output
  wire [3:0]  stdout_slave_ar_region; // output
  wire [7:0]  stdout_slave_ar_len;    // output
  wire [2:0]  stdout_slave_ar_size;   // output
  wire [1:0]  stdout_slave_ar_burst;  // output
  wire        stdout_slave_ar_lock;   // output
  wire [3:0]  stdout_slave_ar_cache;  // output
  wire [3:0]  stdout_slave_ar_qos;    // output
  wire [9:0]  stdout_slave_ar_id;     // output
  wire [5:0]  stdout_slave_ar_user;   // output
  wire        stdout_slave_ar_ready;  // input
  wire        stdout_slave_w_valid;   // output
  wire [31:0] stdout_slave_w_data;    // output
  wire [7:0]  stdout_slave_w_strb;    // output
  wire [5:0]  stdout_slave_w_user;    // output
  wire        stdout_slave_w_last;    // output
  wire        stdout_slave_w_ready;   // input
  wire        stdout_slave_r_valid;   // input
  wire [31:0] stdout_slave_r_data;    // input
  wire [1:0]  stdout_slave_r_resp;    // input
  wire        stdout_slave_r_last;    // input
  wire [9:0]  stdout_slave_r_id;      // input
  wire [5:0]  stdout_slave_r_user;    // input
  wire        stdout_slave_r_ready;   // output
  wire        stdout_slave_b_valid;   // input
  wire [1:0]  stdout_slave_b_resp;    // input
  wire [9:0]  stdout_slave_b_id;      // input
  wire [5:0]  stdout_slave_b_user;    // input
  wire        stdout_slave_b_ready;   // output
  wire        rab_slave_aw_valid;   // input
  wire [31:0] rab_slave_aw_addr;    // input
  wire [2:0]  rab_slave_aw_prot;    // input
  wire [3:0]  rab_slave_aw_region;  // input
  wire [7:0]  rab_slave_aw_len;     // input
  wire [2:0]  rab_slave_aw_size;    // input
  wire [1:0]  rab_slave_aw_burst;   // input
  wire        rab_slave_aw_lock;    // input
  wire [3:0]  rab_slave_aw_cache;   // input
  wire [3:0]  rab_slave_aw_qos;     // input
  wire [5:0]  rab_slave_aw_id;      // input
  wire [5:0]  rab_slave_aw_user;    // input
  wire        rab_slave_aw_ready;   // input
  wire        rab_slave_ar_valid;   // input
  wire [31:0] rab_slave_ar_addr;    // input
  wire [2:0]  rab_slave_ar_prot;    // input
  wire [3:0]  rab_slave_ar_region;  // input
  wire [7:0]  rab_slave_ar_len;     // input
  wire [2:0]  rab_slave_ar_size;    // input
  wire [1:0]  rab_slave_ar_burst;   // input
  wire        rab_slave_ar_lock;    // input
  wire [3:0]  rab_slave_ar_cache;   // input
  wire [3:0]  rab_slave_ar_qos;     // input
  wire [5:0]  rab_slave_ar_id;      // input
  wire [5:0]  rab_slave_ar_user;    // input
  wire        rab_slave_ar_ready;   // input
  wire        rab_slave_w_valid;    // input
  wire [63:0] rab_slave_w_data;     // input
  wire [7:0]  rab_slave_w_strb;     // input
  wire [5:0]  rab_slave_w_user;     // input
  wire        rab_slave_w_last;     // input
  wire        rab_slave_w_ready;    // input
  wire        rab_slave_r_valid;    // input
  wire [63:0] rab_slave_r_data;     // input
  wire [1:0]  rab_slave_r_resp;     // input
  wire        rab_slave_r_last;     // input
  wire [5:0]  rab_slave_r_id;       // input
  wire [5:0]  rab_slave_r_user;     // input
  wire        rab_slave_r_ready;    // input
  wire        rab_slave_b_valid;    // input
  wire [1:0]  rab_slave_b_resp;     // input
  wire [5:0]  rab_slave_b_id;       // input
  wire [5:0]  rab_slave_b_user;     // input
  wire        rab_slave_b_ready;    // input
  wire        rab_master_aw_valid;  // output
  wire [31:0] rab_master_aw_addr;   // output
  wire [2:0]  rab_master_aw_prot;   // output
  wire [3:0]  rab_master_aw_region; // output
  wire [7:0]  rab_master_aw_len;    // output
  wire [2:0]  rab_master_aw_size;   // output
  wire [1:0]  rab_master_aw_burst;  // output
  wire        rab_master_aw_lock;   // output
  wire [3:0]  rab_master_aw_cache;  // output
  wire [3:0]  rab_master_aw_qos;    // output
  wire [5:0]  rab_master_aw_id;     // output
  wire [5:0]  rab_master_aw_user;   // output
  wire        rab_master_aw_ready;  // input
  wire        rab_master_ar_valid;  // output
  wire [31:0] rab_master_ar_addr;   // output
  wire [2:0]  rab_master_ar_prot;   // output
  wire [3:0]  rab_master_ar_region; // output
  wire [7:0]  rab_master_ar_len;    // output
  wire [2:0]  rab_master_ar_size;   // output
  wire [1:0]  rab_master_ar_burst;  // output
  wire        rab_master_ar_lock;   // output
  wire [3:0]  rab_master_ar_cache;  // output
  wire [3:0]  rab_master_ar_qos;    // output
  wire [5:0]  rab_master_ar_id;     // output
  wire [5:0]  rab_master_ar_user;   // output
  wire        rab_master_ar_ready;  // input
  wire        rab_master_w_valid;   // output
  wire [63:0] rab_master_w_data;    // output
  wire [7:0]  rab_master_w_strb;    // output
  wire [5:0]  rab_master_w_user;    // output
  wire        rab_master_w_last;    // output
  wire        rab_master_w_ready;   // input
  wire        rab_master_r_valid;   // input
  wire [63:0] rab_master_r_data;    // input
  wire [1:0]  rab_master_r_resp;    // input
  wire        rab_master_r_last;    // input
  wire [5:0]  rab_master_r_id;      // input
  wire [5:0]  rab_master_r_user;    // input
  wire        rab_master_r_ready;   // output
  wire        rab_master_b_valid;   // input
  wire [1:0]  rab_master_b_resp;    // input
  wire [5:0]  rab_master_b_id;      // input
  wire [5:0]  rab_master_b_user;    // input
  wire        rab_master_b_ready;   // output
  wire        rab_lite_aclk;        // input
  wire        rab_lite_aresetn;     // input
  wire [10:0] rab_lite_aw_addr;     // input
  wire        rab_lite_aw_valid;    // input
  wire        rab_lite_aw_ready;    // output
  wire [31:0] rab_lite_w_data;      // input
  wire  [3:0] rab_lite_w_strb;      // input
  wire        rab_lite_w_valid;     // input
  wire        rab_lite_w_ready;     // output
  wire  [1:0] rab_lite_b_resp;      // output
  wire        rab_lite_b_valid;     // output
  wire        rab_lite_b_ready;     // input
  wire [10:0] rab_lite_ar_addr;     // input
  wire        rab_lite_ar_valid;    // input
  wire        rab_lite_ar_ready;    // output
  wire [31:0] rab_lite_r_data;      // output
  wire  [1:0] rab_lite_r_resp;      // output
  wire        rab_lite_r_valid;     // output
  wire        rab_lite_r_ready;     // input
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
  wire        i2c_scl_i;          // input    
  wire        i2c_scl_o;          // output 
  wire        i2c_scl_oe_o;       // output  
  wire        i2c_sda_i;          // input  
  wire        i2c_sda_o;          // output  
  wire        i2c_sda_oe_o;       // output   
  wire [7:0]  audio_i2s_sd_i;     // input 
  wire        audio_i2s_sck_o;    // output
  wire        audio_i2s_ws_o;     // output
  wire [1:0]  cam_i2s_sd_i;       // input 
  wire        cam_i2s_sck_o;      // output 
  wire        cam_i2s_ws_o;       // output
`ifdef PULP3_PIRMIN
  wire        intr_mailbox_o;          // output
  wire        intr_rab_miss_o;         // output
  wire        intr_rab_multi_o;        // output
  wire        intr_rab_prot_o;         // output
`endif

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

  // useless AXI id/user signals
  // assign rab_slave_aw_id   = 0;
  assign rab_slave_aw_user = 0;
  // assign rab_slave_ar_id   = 0;
  assign rab_slave_ar_user = 0;
  assign rab_slave_w_user  = 0;

  // other unused ulpsoc inputs
  assign uart_cts = 0;
  assign uart_dsr = 0;
  assign uart_rx  = 1'b0;
  assign i2c_scl_i = 1'b0;
  assign i2c_sda_i = 1'b0;
  assign audio_i2s_sd_i = 8'b0;
  assign cam_i2s_sd_i = 2'b0;
  assign si = 4'b0;
  assign se = 1'b0;
  assign rab_lite_aclk = 1'b0;
  assign rab_lite_aresetn = 1'b1;

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
    .end_of_operation  (end_of_operation ),
    .fetch_enable      (fetch_enable     ),
    .ps7_clk           (ps7_clk          ),
    .ps7_rst_n         (ps7_rst_n        ),
    // .ps7_rst_pulp_n    (ps7_rst_pulp_n   ),
    // .ps7_rst_clking_n  (ps7_rst_clking_n ),
    //.M_AXI_aclk   (clking_axi_aclk   ),
    //M_AXI_aresetn(clking_axi_aresetn),
    .rab_slave_awvalid     (rab_slave_aw_valid  ),
    .rab_slave_awaddr      (rab_slave_aw_addr   ),
    .rab_slave_awprot      (rab_slave_aw_prot   ),
    .rab_slave_awregion    (rab_slave_aw_region ),
    .rab_slave_awlen       (rab_slave_aw_len    ),
    .rab_slave_awsize      (rab_slave_aw_size   ),
    .rab_slave_awburst     (rab_slave_aw_burst  ),
    .rab_slave_awlock      (rab_slave_aw_lock   ),
    .rab_slave_awcache     (rab_slave_aw_cache  ),
    .rab_slave_awqos       (rab_slave_aw_qos    ),
    .rab_slave_awid        (rab_slave_aw_id     ),
    // .rab_slave_awuser      (rab_slave_aw_user   ),
    .rab_slave_awready     (rab_slave_aw_ready  ),
    .rab_slave_arvalid     (rab_slave_ar_valid  ),
    .rab_slave_araddr      (rab_slave_ar_addr   ),
    .rab_slave_arprot      (rab_slave_ar_prot   ),
    .rab_slave_arregion    (rab_slave_ar_region ),
    .rab_slave_arlen       (rab_slave_ar_len    ),
    .rab_slave_arsize      (rab_slave_ar_size   ),
    .rab_slave_arburst     (rab_slave_ar_burst  ),
    .rab_slave_arlock      (rab_slave_ar_lock   ),
    .rab_slave_arcache     (rab_slave_ar_cache  ),
    .rab_slave_arqos       (rab_slave_ar_qos    ),
    .rab_slave_arid        (rab_slave_ar_id     ),
    // .rab_slave_aruser      (rab_slave_ar_user   ),
    .rab_slave_arready     (rab_slave_ar_ready  ),
    .rab_slave_wvalid      (rab_slave_w_valid   ),
    .rab_slave_wdata       (rab_slave_w_data    ),
    .rab_slave_wstrb       (rab_slave_w_strb    ),
    // .rab_slave_wuser       (rab_slave_w_user    ),
    .rab_slave_wlast       (rab_slave_w_last    ),
    .rab_slave_wready      (rab_slave_w_ready   ),
    .rab_slave_rvalid      (rab_slave_r_valid   ),
    .rab_slave_rdata       (rab_slave_r_data    ),
    .rab_slave_rresp       (rab_slave_r_resp    ),
    .rab_slave_rlast       (rab_slave_r_last    ),
    .rab_slave_rid         (rab_slave_r_id      ),
    // .rab_slave_ruser       (rab_slave_r_user    ),
    .rab_slave_rready      (rab_slave_r_ready   ),
    .rab_slave_bvalid      (rab_slave_b_valid   ),
    .rab_slave_bresp       (rab_slave_b_resp    ),
    .rab_slave_bid         (rab_slave_b_id      ),
    // .rab_slave_buser       (rab_slave_b_user    ),
    .rab_slave_bready      (rab_slave_b_ready   ),
    .rab_master_awvalid    (rab_master_aw_valid ),
    .rab_master_awaddr     (rab_master_aw_addr  ),
    .rab_master_awprot     (rab_master_aw_prot  ),
    .rab_master_awregion   (rab_master_aw_region),
    .rab_master_awlen      (rab_master_aw_len   ),
    .rab_master_awsize     (rab_master_aw_size  ),
    .rab_master_awburst    (rab_master_aw_burst ),
    .rab_master_awlock     (rab_master_aw_lock  ),
    .rab_master_awcache    (rab_master_aw_cache ),
    .rab_master_awqos      (rab_master_aw_qos   ),
    // .rab_master_awid       (rab_master_aw_id    ),
    // .rab_master_awuser     (rab_master_aw_user  ),
    .rab_master_awready    (rab_master_aw_ready ),
    .rab_master_arvalid    (rab_master_ar_valid ),
    .rab_master_araddr     (rab_master_ar_addr  ),
    .rab_master_arprot     (rab_master_ar_prot  ),
    .rab_master_arregion   (rab_master_ar_region),
    .rab_master_arlen      (rab_master_ar_len   ),
    .rab_master_arsize     (rab_master_ar_size  ),
    .rab_master_arburst    (rab_master_ar_burst ),
    .rab_master_arlock     (rab_master_ar_lock  ),
    .rab_master_arcache    (rab_master_ar_cache ),
    .rab_master_arqos      (rab_master_ar_qos   ),
    .rab_master_arid       (rab_master_ar_id    ),
    .rab_master_aruser     (rab_master_ar_user  ),
    .rab_master_arready    (rab_master_ar_ready ),
    .rab_master_wvalid     (rab_master_w_valid  ),
    .rab_master_wdata      (rab_master_w_data   ),
    .rab_master_wstrb      (rab_master_w_strb   ),
    .rab_master_wuser      (rab_master_w_user   ),
    .rab_master_wlast      (rab_master_w_last   ),
    .rab_master_wready     (rab_master_w_ready  ),
    .rab_master_rvalid     (rab_master_r_valid  ),
    .rab_master_rdata      (rab_master_r_data   ),
    .rab_master_rresp      (rab_master_r_resp   ),
    .rab_master_rlast      (rab_master_r_last   ),
    .rab_master_rid        (rab_master_r_id     ),
    .rab_master_ruser      (rab_master_r_user   ),
    .rab_master_rready     (rab_master_r_ready  ),
    .rab_master_bvalid     (rab_master_b_valid  ),
    .rab_master_bresp      (rab_master_b_resp   ),
    .rab_master_bid        (rab_master_b_id     ),
    .rab_master_buser      (rab_master_b_user   ),
    .rab_master_bready     (rab_master_b_ready  ),
    .rab_lite_awaddr       (rab_lite_aw_addr    ),
    .rab_lite_awvalid      (rab_lite_aw_valid   ),
    .rab_lite_awready      (rab_lite_aw_ready   ),
    .rab_lite_wdata        (rab_lite_w_data     ),
    .rab_lite_wstrb        (rab_lite_w_strb     ),
    .rab_lite_wvalid       (rab_lite_w_valid    ),
    .rab_lite_wready       (rab_lite_w_ready    ),
    .rab_lite_bresp        (rab_lite_b_resp     ),
    .rab_lite_bvalid       (rab_lite_b_valid    ),
    .rab_lite_bready       (rab_lite_b_ready    ),
    .rab_lite_araddr       (rab_lite_ar_addr    ),
    .rab_lite_arvalid      (rab_lite_ar_valid   ),
    .rab_lite_arready      (rab_lite_ar_ready   ),
    .rab_lite_rdata        (rab_lite_r_data     ),
    .rab_lite_rresp        (rab_lite_r_resp     ),
    .rab_lite_rvalid       (rab_lite_r_valid    ),
    .rab_lite_rready       (rab_lite_r_ready    ),
    .stdout_slave_awvalid (stdout_slave_aw_valid ),
    .stdout_slave_awaddr  (stdout_slave_aw_addr  ),
    .stdout_slave_awprot  (stdout_slave_aw_prot  ),
    //.stdout_slave_awregion(stdout_slave_aw_region),
    .stdout_slave_awlen   (stdout_slave_aw_len   ),
    .stdout_slave_awsize  (stdout_slave_aw_size  ),
    .stdout_slave_awburst (stdout_slave_aw_burst ),
    .stdout_slave_awlock  (stdout_slave_aw_lock  ),
    .stdout_slave_awcache (stdout_slave_aw_cache ),
    //.stdout_slave_awqos   (stdout_slave_aw_qos   ),
    .stdout_slave_awready (stdout_slave_aw_ready ),
    .stdout_slave_arvalid (stdout_slave_ar_valid ),
    .stdout_slave_araddr  (stdout_slave_ar_addr  ),
    .stdout_slave_arprot  (stdout_slave_ar_prot  ),
    //.stdout_slave_arregion(stdout_slave_ar_region),
    .stdout_slave_arlen   (stdout_slave_ar_len   ),
    .stdout_slave_arsize  (stdout_slave_ar_size  ),
    .stdout_slave_arburst (stdout_slave_ar_burst ),
    .stdout_slave_arlock  (stdout_slave_ar_lock  ),
    .stdout_slave_arcache (stdout_slave_ar_cache ),
    //.stdout_slave_arqos   (stdout_slave_ar_qos   ),
    .stdout_slave_arready (stdout_slave_ar_ready ),
    .stdout_slave_wvalid  (stdout_slave_w_valid  ),
    .stdout_slave_wdata   (stdout_slave_w_data   ),
    .stdout_slave_wstrb   (stdout_slave_w_strb   ),
    .stdout_slave_wlast   (stdout_slave_w_last   ),
    .stdout_slave_wready  (stdout_slave_w_ready  ),
    .stdout_slave_rvalid  (stdout_slave_r_valid  ),
    .stdout_slave_rdata   (stdout_slave_r_data   ),
    .stdout_slave_rresp   (stdout_slave_r_resp   ),
    .stdout_slave_rlast   (stdout_slave_r_last   ),
    .stdout_slave_rready  (stdout_slave_r_ready  ),
    .stdout_slave_bvalid  (stdout_slave_b_valid  ),
    .stdout_slave_bresp   (stdout_slave_b_resp   ),
    .stdout_slave_bready  (stdout_slave_b_ready  ),
    .clking_axi_awaddr (clking_axi_awaddr ),
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
    .clking_axi_arvalid(clking_axi_arvalid),
    .clking_axi_arready(clking_axi_arready),
    .clking_axi_rdata  (clking_axi_rdata  ),
    .clking_axi_rresp  (clking_axi_rresp  ),
    .clking_axi_rvalid (clking_axi_rvalid ),
    .clking_axi_rready (clking_axi_rready ),
    .ulpsoc_clk        (ulpsoc_clk        )
`ifdef PULP3_PIRMIN
    ,
    .intr_mailbox      (intr_mailbox      ),
    .intr_rab_miss     (intr_rab_miss     ),
    .intr_rab_multi    (intr_rab_multi    ),
    .intr_rab_prot     (intr_rab_prot     )
`endif
  );

  assign ulpsoc_clk = s_clk_soc;

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
