
`include "axi_bus.sv"
`include "config.sv"


module core_region
#(
    parameter AXI_ADDR_WIDTH       = 32,
    parameter AXI_DATA_WIDTH       = 64,
    parameter AXI_ID_MASTER_WIDTH  = 10,
    parameter AXI_ID_SLAVE_WIDTH   = 10,
    parameter AXI_USER_WIDTH       = 0
  )
(
    // Clock and Reset
    input logic         clk,
    input logic         rst_n,

    input  logic        testmode_i,
    input  logic        fetch_enable_i,
    input  logic [31:0] irq_i,
    output logic        core_busy_o,
    input  logic        clock_gating_i,
    input  logic [31:0] boot_addr_i,

    AXI_BUS.Master      core_master,
    AXI_BUS.Master      dbg_master,
    AXI_BUS.Slave       data_slave,
    AXI_BUS.Slave       instr_slave,

    // JTAG signals
    input  logic        tck_i,
    input  logic        trstn_i,
    input  logic        tms_i,
    input  logic        tdi_i,
    output logic        tdo_o
  );

  // signals from/to core
  logic         core_instr_req;
  logic         core_instr_gnt;
  logic         core_instr_rvalid;
  logic [31:0]  core_instr_addr;
  logic [31:0]  core_instr_rdata;

  logic         core_lsu_req;
  logic         core_lsu_gnt;
  logic         core_lsu_rvalid;
  logic [31:0]  core_lsu_addr;
  logic         core_lsu_we;
  logic [3:0]   core_lsu_be;
  logic [31:0]  core_lsu_rdata;
  logic [31:0]  core_lsu_wdata;

  logic         core_data_req;
  logic         core_data_gnt;
  logic         core_data_rvalid;
  logic [31:0]  core_data_addr;
  logic         core_data_we;
  logic [3:0]   core_data_be;
  logic [31:0]  core_data_rdata;
  logic [31:0]  core_data_wdata;


  // signals to/from AXI mem
  logic         is_axi_addr;
  logic         axi_mem_req;
  logic [`RAM_ADDR_WIDTH-1:0]  axi_mem_addr;
  logic         axi_mem_we;
  logic [3:0]   axi_mem_be;
  logic [31:0]  axi_mem_rdata;
  logic [31:0]  axi_mem_wdata;

  // signals to/from AXI instr
  logic         axi_instr_req;
  logic [`INSTR_RAM_ADDR_WIDTH-1:0]  axi_instr_addr;
  logic         axi_instr_we;
  logic [3:0]   axi_instr_be;
  logic [31:0]  axi_instr_rdata;
  logic [31:0]  axi_instr_wdata;


  // signals to/from instr mem
  logic                        instr_mem_en;
  logic [`INSTR_RAM_ADDR_WIDTH+1:0]  instr_mem_addr;
  logic                        instr_mem_we;
  logic [3:0]                  instr_mem_be;
  logic [31:0]                 instr_mem_rdata;
  logic [31:0]                 instr_mem_wdata;

  // signals to/from data mem
  logic                        data_mem_en;
  logic [`RAM_ADDR_WIDTH+1:0]  data_mem_addr;
  logic                        data_mem_we;
  logic [3:0]                  data_mem_be;
  logic [31:0]                 data_mem_rdata;
  logic [31:0]                 data_mem_wdata;



  enum logic [0:0] { AXI, RAM } lsu_resp_CS, lsu_resp_NS;

  // signals to/from core2axi
  logic         core_axi_req;
  logic         core_axi_gnt;
  logic         core_axi_rvalid;
  logic [31:0]  core_axi_addr;
  logic         core_axi_we;
  logic [3:0]   core_axi_be;
  logic [31:0]  core_axi_rdata;
  logic [31:0]  core_axi_wdata;

  // adv. dbg unit to core signals
  logic [0:0]             dbginf_stall;
  logic [0:0]             dbginf_bp;
  logic [0:0]             dbginf_strobe;
  logic [0:0]             dbginf_ack;
  logic [0:0]             dbginf_rst;
  logic [0:0]             dbginf_we;
  logic [0:0] [15:0]      dbginf_addr;
  logic [0:0] [31:0]      dbginf_datai;
  logic [0:0] [31:0]      dbginf_datao;

  logic         clk_core_int;

  AXI_BUS
  #(
    .AXI_ADDR_WIDTH   ( AXI_ADDR_WIDTH ),
    .AXI_DATA_WIDTH   ( AXI_DATA_WIDTH ),
    .AXI_ID_WIDTH     ( AXI_ID_MASTER_WIDTH   ),
    .AXI_USER_WIDTH   ( AXI_USER_WIDTH )
  )
  core_master_int();

  //----------------------------------------------------------------------------//
  // Core clock gating
  //----------------------------------------------------------------------------//
  cluster_clock_gating core_clock_gate
  (
    .clk_o     ( clk_core_int   ),
    .en_i      ( clock_gating_i ),
    .test_en_i ( testmode_i     ),
    .clk_i     ( clk            )
  );


  //----------------------------------------------------------------------------//
  // Core Instantiation
  //----------------------------------------------------------------------------//

  `ifdef RISCV
    riscv_core
    #(
      .N_EXT_PERF_COUNTERS ( 0 )
    )
    RISCV_CORE
    (
      .clk             ( clk_core_int      ),
      .rst_n           ( rst_n             ),

      .test_en_i       ( testmode_i        ),

      .boot_addr_i     ( boot_addr_i       ),
      .core_id_i       ( 5'h0              ),
      .cluster_id_i    ( 5'h0              ),

      .instr_addr_o    ( core_instr_addr   ),
      .instr_req_o     ( core_instr_req    ),
      .instr_rdata_i   ( core_instr_rdata  ),
      .instr_gnt_i     ( core_instr_gnt    ),
      .instr_rvalid_i  ( core_instr_rvalid ),

      .data_addr_o     ( core_lsu_addr     ),
      .data_wdata_o    ( core_lsu_wdata    ),
      .data_we_o       ( core_lsu_we       ),
      .data_req_o      ( core_lsu_req      ),
      .data_be_o       ( core_lsu_be       ),
      .data_rdata_i    ( core_lsu_rdata    ),
      .data_gnt_i      ( core_lsu_gnt      ),
      .data_rvalid_i   ( core_lsu_rvalid   ),
      .data_err_i      ( 1'b0              ),

      .irq_i           ( irq_i             ),

      .dbginf_stall_i  ( dbginf_stall[0]   ),
      .dbginf_bp_o     ( dbginf_bp[0]      ),
      .dbginf_strobe_i ( dbginf_strobe[0]  ),
      .dbginf_ack_o    ( dbginf_ack[0]     ),
      .dbginf_we_i     ( dbginf_we[0]      ),
      .dbginf_addr_i   ( dbginf_addr[0]    ),
      .dbginf_data_i   ( dbginf_datao[0]   ),
      .dbginf_data_o   ( dbginf_datai[0]   ),

      .fetch_enable_i  ( fetch_enable_i    ),
      .core_busy_o     ( core_busy_o       ),

      .ext_perf_counters_i (               )
    );
  `else
    or10n_core
    #(
      .N_EXT_PERF_COUNTERS ( 0 )
    )
    OR10N_CORE
    (
      // Clock and Reset
      .clk             ( clk_core_int      ),
      .rst_n           ( rst_n             ),

      .boot_addr_i     ( 32'h0000_0000     ),
      .core_id_i       ( 5'h0              ),
      .cluster_id_i    ( 5'h0              ),

      .instr_addr_o    ( core_instr_addr   ),
      .instr_req_o     ( core_instr_req    ),
      .instr_rdata_i   ( core_instr_rdata  ),
      .instr_gnt_i     ( core_instr_gnt    ),
      .instr_rvalid_i  ( core_instr_rvalid ),

      .data_addr_o     ( core_lsu_addr     ),
      .data_wdata_o    ( core_lsu_wdata    ),
      .data_we_o       ( core_lsu_we       ),
      .data_req_o      ( core_lsu_req      ),
      .data_be_o       ( core_lsu_be       ),
      .data_rdata_i    ( core_lsu_rdata    ),
      .data_gnt_i      ( core_lsu_gnt      ),
      .data_rvalid_i   ( core_lsu_rvalid   ),

      .irq_i           ( (|irq_i)          ),
      .irq_nm_i        ( 1'b0              ),

      .dbginf_stall_i  ( dbginf_stall[0]   ),
      .dbginf_bp_o     ( dbginf_bp[0]      ),
      .dbginf_strobe_i ( dbginf_strobe[0]  ),
      .dbginf_ack_o    ( dbginf_ack[0]     ),
      .dbginf_we_i     ( dbginf_we[0]      ),
      .dbginf_addr_i   ( dbginf_addr[0]    ),
      .dbginf_data_i   ( dbginf_datao[0]   ),
      .dbginf_data_o   ( dbginf_datai[0]   ),

      .fetch_enable_i  ( fetch_enable_i    ),
      .core_busy_o     ( core_busy_o       ),

      .ext_perf_counters_i (               )
    );
  `endif


  core2axi
  #(
    .AXI_ADDR_WIDTH ( AXI_ADDR_WIDTH )
  )
  core2axi_i
  (
    .clk   ( clk   ),
    .rst_n ( rst_n ),

    .data_req_i    ( core_axi_req    ),
    .data_gnt_o    ( core_axi_gnt    ),
    .data_rvalid_o ( core_axi_rvalid ),
    .data_addr_i   ( core_axi_addr   ),
    .data_we_i     ( core_axi_we     ),
    .data_be_i     ( core_axi_be     ),
    .data_rdata_o  ( core_axi_rdata  ),
    .data_wdata_i  ( core_axi_wdata  ),

    .master        ( core_master_int )
  );

  //----------------------------------------------------------------------------//
  // AXI Slices
  //----------------------------------------------------------------------------//

   axi_slice_wrap
   #(
     .AXI_ADDR_WIDTH ( AXI_ADDR_WIDTH       ),
     .AXI_DATA_WIDTH ( AXI_DATA_WIDTH       ),
     .AXI_USER_WIDTH ( AXI_USER_WIDTH       ),
     .AXI_ID_WIDTH   ( AXI_ID_MASTER_WIDTH  ),
     .SLICE_DEPTH    ( 2                    )
   )
   axi_slice_core2axi
   (
     .clk_i      ( clk                 ),
     .rst_ni     ( rst_n               ),

     .test_en_i  ( testmode_i          ),

     .axi_slave  ( core_master_int ),
     .axi_master ( core_master     )
   );


  //----------------------------------------------------------------------------//
  // DEMUX
  //----------------------------------------------------------------------------//
  assign is_axi_addr     = (core_lsu_addr[31:20] != 12'h001);
  assign core_data_req   = (~is_axi_addr) & core_lsu_req;
  assign core_axi_req    =   is_axi_addr  & core_lsu_req;

  assign core_data_addr  = core_lsu_addr;
  assign core_data_we    = core_lsu_we;
  assign core_data_be    = core_lsu_be;
  assign core_data_wdata = core_lsu_wdata;

  assign core_axi_addr   = core_lsu_addr;
  assign core_axi_we     = core_lsu_we;
  assign core_axi_be     = core_lsu_be;
  assign core_axi_wdata  = core_lsu_wdata;

  always_ff @(posedge clk, negedge rst_n)
  begin
    if (rst_n == 1'b0)
      lsu_resp_CS <= RAM;
    else
      lsu_resp_CS <= lsu_resp_NS;
  end

  // figure out where the next response will be coming from
  always_comb
  begin
    lsu_resp_NS = lsu_resp_CS;
    core_lsu_gnt = 1'b0;

    if (core_axi_req)
    begin
      core_lsu_gnt = core_axi_gnt;
      lsu_resp_NS = AXI;
    end
    else if (core_data_req)
    begin
      core_lsu_gnt = core_data_gnt;
      lsu_resp_NS = RAM;
    end
  end

  // route response back to LSU
  assign core_lsu_rdata  = (lsu_resp_CS == AXI) ? core_axi_rdata : core_data_rdata;
  assign core_lsu_rvalid = core_axi_rvalid | core_data_rvalid;



  //----------------------------------------------------------------------------//
  // Instruction RAM
  //----------------------------------------------------------------------------//

  instr_ram_wrap
  #(
    .RAM_SIZE  ( `NUM_WORD          )
  )
  instr_mem
  (
    .clk     ( clk                                  ),
    .rst_n   ( rst_n                                ),
    .en_i    ( instr_mem_en                         ),
    .addr_i  ( instr_mem_addr[`INSTR_RAM_ADDR_WIDTH+1:2]  ),
    .wdata_i ( instr_mem_wdata                      ),
    .rdata_o ( instr_mem_rdata                      ),
    .we_i    ( instr_mem_we                         ),
    .be_i    ( instr_mem_be                         ),
    .bypass_en_i ( testmode_i )
  );

  axi_mem_if_SP_wrap
  #(
    .AXI_ADDR_WIDTH  ( AXI_ADDR_WIDTH     ),
    .AXI_DATA_WIDTH  ( AXI_DATA_WIDTH     ),
    .AXI_ID_WIDTH    ( AXI_ID_SLAVE_WIDTH ),
    .AXI_USER_WIDTH  ( AXI_USER_WIDTH     ),
    .MEM_ADDR_WIDTH  ( `INSTR_RAM_ADDR_WIDTH    )
  )
  instr_mem_axi_if
  (
    .clk         ( clk               ),
    .rst_n       ( rst_n             ),

    .mem_req_o   ( axi_instr_req     ),
    .mem_addr_o  ( axi_instr_addr    ),
    .mem_we_o    ( axi_instr_we      ),
    .mem_be_o    ( axi_instr_be      ),
    .mem_rdata_i ( axi_instr_rdata   ),
    .mem_wdata_o ( axi_instr_wdata   ),

    .slave       ( instr_slave       )
  );


  ram_mux
  #(
    .ADDR_WIDTH ( `INSTR_RAM_ADDR_WIDTH+2 ),
    .DATA_WIDTH ( 32                )
  )
  instr_ram_mux_i
  (
    .clk            ( clk               ),
    .rst_n          ( rst_n             ),

    .port0_req_i    ( axi_instr_req     ),
    .port0_gnt_o    (                   ),
    .port0_rvalid_o (                   ),
    .port0_addr_i   ( {axi_instr_addr[`INSTR_RAM_ADDR_WIDTH-1:0],2'b00} ),
    .port0_we_i     ( axi_instr_we      ),
    .port0_be_i     ( axi_instr_be      ),
    .port0_rdata_o  ( axi_instr_rdata   ),
    .port0_wdata_i  ( axi_instr_wdata   ),

    .port1_req_i    ( core_instr_req    ),
    .port1_gnt_o    ( core_instr_gnt    ),
    .port1_rvalid_o ( core_instr_rvalid ),
    .port1_addr_i   ( core_instr_addr[`INSTR_RAM_ADDR_WIDTH+1:0] ),
    .port1_we_i     ( 1'b0              ),
    .port1_be_i     ( 4'b1111           ),
    .port1_rdata_o  ( core_instr_rdata  ),
    .port1_wdata_i  ( 32'h0             ),

    .ram_en_o       ( instr_mem_en      ),
    .ram_addr_o     ( instr_mem_addr[`INSTR_RAM_ADDR_WIDTH+1:0] ),
    .ram_we_o       ( instr_mem_we      ),
    .ram_be_o       ( instr_mem_be      ),
    .ram_rdata_i    ( instr_mem_rdata   ),
    .ram_wdata_o    ( instr_mem_wdata   )
  );


  //----------------------------------------------------------------------------//
  // Data RAM
  //----------------------------------------------------------------------------//
  sp_ram_wrap
  #(
    .RAM_SIZE ( `NUM_WORD )
  )
  data_mem
  (
    .clk     ( clk                                  ),
    .rstn_i  ( rst_n                                ),
    .en_i    ( data_mem_en                          ),
    .addr_i  ( data_mem_addr[`RAM_ADDR_WIDTH+1:2]   ),
    .wdata_i ( data_mem_wdata                       ),
    .rdata_o ( data_mem_rdata                       ),
    .we_i    ( data_mem_we                          ),
    .be_i    ( data_mem_be                          ),
    .bypass_en_i  ( testmode_i )
  );

  axi_mem_if_SP_wrap
  #(
    .AXI_ADDR_WIDTH  ( AXI_ADDR_WIDTH       ),
    .AXI_DATA_WIDTH  ( AXI_DATA_WIDTH       ),
    .AXI_ID_WIDTH    ( AXI_ID_SLAVE_WIDTH   ),
    .AXI_USER_WIDTH  ( AXI_USER_WIDTH       ),
    .MEM_ADDR_WIDTH  ( `RAM_ADDR_WIDTH      )
  )
  data_mem_axi_if
  (
    .clk         ( clk               ),
    .rst_n       ( rst_n             ),

    .mem_req_o   ( axi_mem_req       ),
    .mem_addr_o  ( axi_mem_addr      ),
    .mem_we_o    ( axi_mem_we        ),
    .mem_be_o    ( axi_mem_be        ),
    .mem_rdata_i ( axi_mem_rdata     ),
    .mem_wdata_o ( axi_mem_wdata     ),

    .slave       ( data_slave        )
  );


  ram_mux
  #(
    .ADDR_WIDTH ( `RAM_ADDR_WIDTH+2 ),
    .DATA_WIDTH ( 32                )
  )
  data_ram_mux_i
  (
    .clk            ( clk              ),
    .rst_n          ( rst_n            ),

    .port0_req_i    ( axi_mem_req      ),
    .port0_gnt_o    (                  ),
    .port0_rvalid_o (                  ),
    .port0_addr_i   ( {axi_mem_addr[`RAM_ADDR_WIDTH-1:0],2'b00} ),
    .port0_we_i     ( axi_mem_we       ),
    .port0_be_i     ( axi_mem_be       ),
    .port0_rdata_o  ( axi_mem_rdata    ),
    .port0_wdata_i  ( axi_mem_wdata    ),

    .port1_req_i    ( core_data_req    ),
    .port1_gnt_o    ( core_data_gnt    ),
    .port1_rvalid_o ( core_data_rvalid ),
    .port1_addr_i   ( core_data_addr[`RAM_ADDR_WIDTH+1:0] ),
    .port1_we_i     ( core_data_we     ),
    .port1_be_i     ( core_data_be     ),
    .port1_rdata_o  ( core_data_rdata  ),
    .port1_wdata_i  ( core_data_wdata  ),

    .ram_en_o       ( data_mem_en      ),
    .ram_addr_o     ( data_mem_addr[`RAM_ADDR_WIDTH+1:0] ),
    .ram_we_o       ( data_mem_we      ),
    .ram_be_o       ( data_mem_be      ),
    .ram_rdata_i    ( data_mem_rdata   ),
    .ram_wdata_o    ( data_mem_wdata   )
  );


  //----------------------------------------------------------------------------//
  // Advanced Debug Unit
  //----------------------------------------------------------------------------//

  adv_dbg_if
  #(
    .NB_CORES           ( 1                   ),
    .AXI_ADDR_WIDTH     ( AXI_ADDR_WIDTH      ),
    .AXI_DATA_WIDTH     ( AXI_DATA_WIDTH      ),
    .AXI_USER_WIDTH     ( AXI_USER_WIDTH      ),
    .AXI_ID_WIDTH       ( AXI_ID_MASTER_WIDTH )
    )
  adv_dbg_if_i
  (
    .tms_pad_i   ( tms_i        ),
    .tck_pad_i   ( tck_i        ),
    .trstn_pad_i ( trstn_i      ),
    .tdi_pad_i   ( tdi_i        ),
    .tdo_pad_o   ( tdo_o        ),

    .test_mode_i ( testmode_i   ),

    .cpu_addr_o  ( dbginf_addr     ),
    .cpu_data_i  ( dbginf_datai    ),
    .cpu_data_o  ( dbginf_datao    ),
    .cpu_bp_i    ( dbginf_bp       ),
    .cpu_stall_o ( dbginf_stall    ),
    .cpu_stb_o   ( dbginf_strobe   ),
    .cpu_we_o    ( dbginf_we       ),
    .cpu_ack_i   ( dbginf_ack      ),
    .cpu_rst_o   ( dbginf_rst      ),

    .axi_aclk             ( clk                  ),
    .axi_aresetn          ( rst_n                ),

    .axi_master_aw_valid  ( dbg_master.aw_valid  ),
    .axi_master_aw_addr   ( dbg_master.aw_addr   ),
    .axi_master_aw_prot   ( dbg_master.aw_prot   ),
    .axi_master_aw_region ( dbg_master.aw_region ),
    .axi_master_aw_len    ( dbg_master.aw_len    ),
    .axi_master_aw_size   ( dbg_master.aw_size   ),
    .axi_master_aw_burst  ( dbg_master.aw_burst  ),
    .axi_master_aw_lock   ( dbg_master.aw_lock   ),
    .axi_master_aw_cache  ( dbg_master.aw_cache  ),
    .axi_master_aw_qos    ( dbg_master.aw_qos    ),
    .axi_master_aw_id     ( dbg_master.aw_id     ),
    .axi_master_aw_user   ( dbg_master.aw_user   ),
    .axi_master_aw_ready  ( dbg_master.aw_ready  ),

    .axi_master_ar_valid  ( dbg_master.ar_valid  ),
    .axi_master_ar_addr   ( dbg_master.ar_addr   ),
    .axi_master_ar_prot   ( dbg_master.ar_prot   ),
    .axi_master_ar_region ( dbg_master.ar_region ),
    .axi_master_ar_len    ( dbg_master.ar_len    ),
    .axi_master_ar_size   ( dbg_master.ar_size   ),
    .axi_master_ar_burst  ( dbg_master.ar_burst  ),
    .axi_master_ar_lock   ( dbg_master.ar_lock   ),
    .axi_master_ar_cache  ( dbg_master.ar_cache  ),
    .axi_master_ar_qos    ( dbg_master.ar_qos    ),
    .axi_master_ar_id     ( dbg_master.ar_id     ),
    .axi_master_ar_user   ( dbg_master.ar_user   ),
    .axi_master_ar_ready  ( dbg_master.ar_ready  ),

    .axi_master_w_valid   ( dbg_master.w_valid   ),
    .axi_master_w_data    ( dbg_master.w_data    ),
    .axi_master_w_strb    ( dbg_master.w_strb    ),
    .axi_master_w_user    ( dbg_master.w_user    ),
    .axi_master_w_last    ( dbg_master.w_last    ),
    .axi_master_w_ready   ( dbg_master.w_ready   ),

    .axi_master_r_valid   ( dbg_master.r_valid   ),
    .axi_master_r_data    ( dbg_master.r_data    ),
    .axi_master_r_resp    ( dbg_master.r_resp    ),
    .axi_master_r_last    ( dbg_master.r_last    ),
    .axi_master_r_id      ( dbg_master.r_id      ),
    .axi_master_r_user    ( dbg_master.r_user    ),
    .axi_master_r_ready   ( dbg_master.r_ready   ),

    .axi_master_b_valid   ( dbg_master.b_valid   ),
    .axi_master_b_resp    ( dbg_master.b_resp    ),
    .axi_master_b_id      ( dbg_master.b_id      ),
    .axi_master_b_user    ( dbg_master.b_user    ),
    .axi_master_b_ready   ( dbg_master.b_ready   )
    );

endmodule
