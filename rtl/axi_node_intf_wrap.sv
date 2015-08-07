
`include "axi_bus.sv"

module axi_node_intf_wrap
  #(
    parameter NB_MASTER      = 4,
    parameter NB_SLAVE       = 4,
    parameter AXI_ADDR_WIDTH = 32,
    parameter AXI_DATA_WIDTH = 32,
    parameter AXI_ID_WIDTH   = 10,
    parameter AXI_USER_WIDTH = 0
    )
  (
    // Clock and Reset
    input logic clk,
    input logic rst_n,

    AXI_BUS slave[NB_SLAVE-1:0],

    AXI_BUS master[NB_MASTER-1:0],

    // Memory map
    input  logic [NB_MASTER-1:0][AXI_DATA_WIDTH-1:0]  start_addr_i,
    input  logic [NB_MASTER-1:0][AXI_DATA_WIDTH-1:0]  end_addr_i
  );

  localparam AXI_STRB_WIDTH = AXI_DATA_WIDTH/8;
  localparam NB_REGION      = 1;

  // AXI ID WIDTHs for master and slave IPS
  localparam AXI_ID_WIDTH_TARG =   AXI_ID_WIDTH;
  localparam AXI_ID_WIDTH_INIT =   AXI_ID_WIDTH_TARG + `LOG2(NB_SLAVE-1);


  // Signals to slave periperhals
  logic [NB_MASTER-1:0][AXI_ID_WIDTH_INIT-1:0] s_master_aw_id;
  logic [NB_MASTER-1:0][AXI_ADDR_WIDTH-1:0]    s_master_aw_addr;
  logic [NB_MASTER-1:0][7:0]                   s_master_aw_len;
  logic [NB_MASTER-1:0][2:0]                   s_master_aw_size;
  logic [NB_MASTER-1:0][1:0]                   s_master_aw_burst;
  logic [NB_MASTER-1:0]                        s_master_aw_lock;
  logic [NB_MASTER-1:0][3:0]                   s_master_aw_cache;
  logic [NB_MASTER-1:0][2:0]                   s_master_aw_prot;
  logic [NB_MASTER-1:0][3:0]                   s_master_aw_region;
  logic [NB_MASTER-1:0][AXI_USER_WIDTH-1:0]    s_master_aw_user;
  logic [NB_MASTER-1:0][3:0]                   s_master_aw_qos;
  logic [NB_MASTER-1:0]                        s_master_aw_valid;
  logic [NB_MASTER-1:0]                        s_master_aw_ready;

  logic [NB_MASTER-1:0][AXI_ID_WIDTH_INIT-1:0] s_master_ar_id;
  logic [NB_MASTER-1:0][AXI_ADDR_WIDTH-1:0]    s_master_ar_addr;
  logic [NB_MASTER-1:0][7:0]                   s_master_ar_len;
  logic [NB_MASTER-1:0][2:0]                   s_master_ar_size;
  logic [NB_MASTER-1:0][1:0]                   s_master_ar_burst;
  logic [NB_MASTER-1:0]                        s_master_ar_lock;
  logic [NB_MASTER-1:0][3:0]                   s_master_ar_cache;
  logic [NB_MASTER-1:0][2:0]                   s_master_ar_prot;
  logic [NB_MASTER-1:0][3:0]                   s_master_ar_region;
  logic [NB_MASTER-1:0][AXI_USER_WIDTH-1:0]    s_master_ar_user;
  logic [NB_MASTER-1:0][3:0]                   s_master_ar_qos;
  logic [NB_MASTER-1:0]                        s_master_ar_valid;
  logic [NB_MASTER-1:0]                        s_master_ar_ready;

  logic [NB_MASTER-1:0][AXI_DATA_WIDTH-1:0]    s_master_w_data;
  logic [NB_MASTER-1:0][AXI_STRB_WIDTH-1:0]    s_master_w_strb;
  logic [NB_MASTER-1:0]                        s_master_w_last;
  logic [NB_MASTER-1:0][AXI_USER_WIDTH-1:0]    s_master_w_user;
  logic [NB_MASTER-1:0]                        s_master_w_valid;
  logic [NB_MASTER-1:0]                        s_master_w_ready;

  logic [NB_MASTER-1:0][AXI_ID_WIDTH_INIT-1:0] s_master_b_id;
  logic [NB_MASTER-1:0][1:0]                   s_master_b_resp;
  logic [NB_MASTER-1:0]                        s_master_b_valid;
  logic [NB_MASTER-1:0][AXI_USER_WIDTH-1:0]    s_master_b_user;
  logic [NB_MASTER-1:0]                        s_master_b_ready;

  logic [NB_MASTER-1:0][AXI_ID_WIDTH_INIT-1:0] s_master_r_id;
  logic [NB_MASTER-1:0][AXI_DATA_WIDTH-1:0]    s_master_r_data;
  logic [NB_MASTER-1:0][1:0]                   s_master_r_resp;
  logic [NB_MASTER-1:0]                        s_master_r_last;
  logic [NB_MASTER-1:0][AXI_USER_WIDTH-1:0]    s_master_r_user;
  logic [NB_MASTER-1:0]                        s_master_r_valid;
  logic [NB_MASTER-1:0]                        s_master_r_ready;

  // Signals from AXI masters
  logic [NB_SLAVE-1:0][AXI_ID_WIDTH_TARG-1:0] s_slave_aw_id;
  logic [NB_SLAVE-1:0][AXI_ADDR_WIDTH-1:0]    s_slave_aw_addr;
  logic [NB_SLAVE-1:0][7:0]                   s_slave_aw_len;
  logic [NB_SLAVE-1:0][2:0]                   s_slave_aw_size;
  logic [NB_SLAVE-1:0][1:0]                   s_slave_aw_burst;
  logic [NB_SLAVE-1:0]                        s_slave_aw_lock;
  logic [NB_SLAVE-1:0][3:0]                   s_slave_aw_cache;
  logic [NB_SLAVE-1:0][2:0]                   s_slave_aw_prot;
  logic [NB_SLAVE-1:0][3:0]                   s_slave_aw_region;
  logic [NB_SLAVE-1:0][AXI_USER_WIDTH-1:0]    s_slave_aw_user;
  logic [NB_SLAVE-1:0][3:0]                   s_slave_aw_qos;
  logic [NB_SLAVE-1:0]                        s_slave_aw_valid;
  logic [NB_SLAVE-1:0]                        s_slave_aw_ready;

  logic [NB_SLAVE-1:0][AXI_ID_WIDTH_TARG-1:0] s_slave_ar_id;
  logic [NB_SLAVE-1:0][AXI_ADDR_WIDTH-1:0]    s_slave_ar_addr;
  logic [NB_SLAVE-1:0][7:0]                   s_slave_ar_len;
  logic [NB_SLAVE-1:0][2:0]                   s_slave_ar_size;
  logic [NB_SLAVE-1:0][1:0]                   s_slave_ar_burst;
  logic [NB_SLAVE-1:0]                        s_slave_ar_lock;
  logic [NB_SLAVE-1:0][3:0]                   s_slave_ar_cache;
  logic [NB_SLAVE-1:0][2:0]                   s_slave_ar_prot;
  logic [NB_SLAVE-1:0][3:0]                   s_slave_ar_region;
  logic [NB_SLAVE-1:0][AXI_USER_WIDTH-1:0]    s_slave_ar_user;
  logic [NB_SLAVE-1:0][3:0]                   s_slave_ar_qos;
  logic [NB_SLAVE-1:0]                        s_slave_ar_valid;
  logic [NB_SLAVE-1:0]                        s_slave_ar_ready;

  logic [NB_SLAVE-1:0][AXI_DATA_WIDTH-1:0]    s_slave_w_data;
  logic [NB_SLAVE-1:0][AXI_STRB_WIDTH-1:0]    s_slave_w_strb;
  logic [NB_SLAVE-1:0]                        s_slave_w_last;
  logic [NB_SLAVE-1:0][AXI_USER_WIDTH-1:0]    s_slave_w_user;
  logic [NB_SLAVE-1:0]                        s_slave_w_valid;
  logic [NB_SLAVE-1:0]                        s_slave_w_ready;

  logic [NB_SLAVE-1:0][AXI_ID_WIDTH_TARG-1:0] s_slave_b_id;
  logic [NB_SLAVE-1:0][1:0]                   s_slave_b_resp;
  logic [NB_SLAVE-1:0]                        s_slave_b_valid;
  logic [NB_SLAVE-1:0][AXI_USER_WIDTH-1:0]    s_slave_b_user;
  logic [NB_SLAVE-1:0]                        s_slave_b_ready;

  logic [NB_SLAVE-1:0][AXI_ID_WIDTH_TARG-1:0] s_slave_r_id;
  logic [NB_SLAVE-1:0][AXI_DATA_WIDTH-1:0]    s_slave_r_data;
  logic [NB_SLAVE-1:0][1:0]                   s_slave_r_resp;
  logic [NB_SLAVE-1:0]                        s_slave_r_last;
  logic [NB_SLAVE-1:0][AXI_USER_WIDTH-1:0]    s_slave_r_user;
  logic [NB_SLAVE-1:0]                        s_slave_r_valid;
  logic [NB_SLAVE-1:0]                        s_slave_r_ready;

  // Signals Used to configure the AXI node
  logic [NB_REGION-1:0][NB_MASTER-1:0][AXI_ADDR_WIDTH-1:0] s_start_addr;
  logic [NB_REGION-1:0][NB_MASTER-1:0][AXI_ADDR_WIDTH-1:0] s_end_addr;
  logic [NB_REGION-1:0][NB_MASTER-1:0][AXI_ADDR_WIDTH-1:0] s_valid_rule;
  logic [NB_SLAVE-1:0][NB_MASTER-1:0]                      s_connectivity_map;


  generate
    genvar i;
    for(i = 0; i < NB_MASTER; i++)
    begin
      assign                        master[i].aw_id[AXI_ID_WIDTH_INIT-1:0] = s_master_aw_id[i];
      assign                        master[i].aw_addr                      = s_master_aw_addr[i];
      assign                        master[i].aw_len                       = s_master_aw_len[i];
      assign                        master[i].aw_size                      = s_master_aw_size[i];
      assign                        master[i].aw_burst                     = s_master_aw_burst[i];
      assign                        master[i].aw_lock                      = s_master_aw_lock[i];
      assign                        master[i].aw_cache                     = s_master_aw_cache[i];
      assign                        master[i].aw_prot                      = s_master_aw_prot[i];
      assign                        master[i].aw_region                    = s_master_aw_region[i];
      assign                        master[i].aw_user                      = s_master_aw_user[i];
      assign                        master[i].aw_qos                       = s_master_aw_qos[i];
      assign                        master[i].aw_valid                     = s_master_aw_valid[i];
      assign s_master_aw_ready[i] = master[i].aw_ready;

      assign                        master[i].ar_id[AXI_ID_WIDTH_INIT-1:0] = s_master_ar_id[i];
      assign                        master[i].ar_addr                      = s_master_ar_addr[i];
      assign                        master[i].ar_len                       = s_master_ar_len[i];
      assign                        master[i].ar_size                      = s_master_ar_size[i];
      assign                        master[i].ar_burst                     = s_master_ar_burst[i];
      assign                        master[i].ar_lock                      = s_master_ar_lock[i];
      assign                        master[i].ar_cache                     = s_master_ar_cache[i];
      assign                        master[i].ar_prot                      = s_master_ar_prot[i];
      assign                        master[i].ar_region                    = s_master_ar_region[i];
      assign                        master[i].ar_user                      = s_master_ar_user[i];
      assign                        master[i].ar_qos                       = s_master_ar_qos[i];
      assign                        master[i].ar_valid                     = s_master_ar_valid[i];
      assign s_master_ar_ready[i] = master[i].ar_ready;

      assign                        master[i].w_data  = s_master_w_data[i];
      assign                        master[i].w_strb  = s_master_w_strb[i];
      assign                        master[i].w_last  = s_master_w_last[i];
      assign                        master[i].w_user  = s_master_w_user[i];
      assign                        master[i].w_valid = s_master_w_valid[i];
      assign s_master_w_ready[i]  = master[i].w_ready;

      assign s_master_b_id[i]     = master[i].b_id[AXI_ID_WIDTH_INIT-1:0];
      assign s_master_b_resp[i]   = master[i].b_resp;
      assign s_master_b_valid[i]  = master[i].b_valid;
      assign s_master_b_user[i]   = master[i].b_user;
      assign                        master[i].b_ready = s_master_b_ready[i];

      assign s_master_r_id[i]     = master[i].r_id[AXI_ID_WIDTH_INIT-1:0];
      assign s_master_r_data[i]   = master[i].r_data;
      assign s_master_r_resp[i]   = master[i].r_resp;
      assign s_master_r_last[i]   = master[i].r_last;
      assign s_master_r_user[i]   = master[i].r_user;
      assign s_master_r_valid[i]  = master[i].r_valid;
      assign                        master[i].r_ready = s_master_r_ready[i];

      assign s_start_addr[0][i] = start_addr_i[i];
      assign s_end_addr[0][i]   = end_addr_i[i];
    end
  endgenerate

  generate
    genvar j;
    for(j = 0; j < NB_SLAVE; j++)
    begin
      assign s_slave_aw_id[j]     = slave[j].aw_id[AXI_ID_WIDTH_TARG-1:0];
      assign s_slave_aw_addr[j]   = slave[j].aw_addr;
      assign s_slave_aw_len[j]    = slave[j].aw_len;
      assign s_slave_aw_size[j]   = slave[j].aw_size;
      assign s_slave_aw_burst[j]  = slave[j].aw_burst;
      assign s_slave_aw_lock[j]   = slave[j].aw_lock;
      assign s_slave_aw_cache[j]  = slave[j].aw_cache;
      assign s_slave_aw_prot[j]   = slave[j].aw_prot;
      assign s_slave_aw_region[j] = slave[j].aw_region;
      assign s_slave_aw_user[j]   = slave[j].aw_user;
      assign s_slave_aw_qos[j]    = slave[j].aw_qos;
      assign s_slave_aw_valid[j]  = slave[j].aw_valid;
      assign                        slave[j].aw_ready = s_slave_aw_ready[j];

      assign s_slave_ar_id[j]     = slave[j].ar_id[AXI_ID_WIDTH_TARG-1:0];
      assign s_slave_ar_addr[j]   = slave[j].ar_addr;
      assign s_slave_ar_len[j]    = slave[j].ar_len;
      assign s_slave_ar_size[j]   = slave[j].ar_size;
      assign s_slave_ar_burst[j]  = slave[j].ar_burst;
      assign s_slave_ar_lock[j]   = slave[j].ar_lock;
      assign s_slave_ar_cache[j]  = slave[j].ar_cache;
      assign s_slave_ar_prot[j]   = slave[j].ar_prot;
      assign s_slave_ar_region[j] = slave[j].ar_region;
      assign s_slave_ar_user[j]   = slave[j].ar_user;
      assign s_slave_ar_qos[j]    = slave[j].ar_qos;
      assign s_slave_ar_valid[j]  = slave[j].ar_valid;
      assign                        slave[j].ar_ready = s_slave_ar_ready[j];

      assign s_slave_w_data[j]    = slave[j].w_data;
      assign s_slave_w_strb[j]    = slave[j].w_strb;
      assign s_slave_w_last[j]    = slave[j].w_last;
      assign s_slave_w_user[j]    = slave[j].w_user;
      assign s_slave_w_valid[j]   = slave[j].w_valid;
      assign                        slave[j].w_ready = s_slave_w_ready[j];

      assign                        slave[j].b_id[AXI_ID_WIDTH_TARG-1:0] = s_slave_b_id[j];
      assign                        slave[j].b_resp                      = s_slave_b_resp[j];
      assign                        slave[j].b_valid                     = s_slave_b_valid[j];
      assign                        slave[j].b_user                      = s_slave_b_user[j];
      assign s_slave_b_ready[j]   = slave[j].b_ready;

      assign                        slave[j].r_id[AXI_ID_WIDTH_TARG-1:0] = s_slave_r_id[j];
      assign                        slave[j].r_data                      = s_slave_r_data[j];
      assign                        slave[j].r_resp                      = s_slave_r_resp[j];
      assign                        slave[j].r_last                      = s_slave_r_last[j];
      assign                        slave[j].r_user                      = s_slave_r_user[j];
      assign                        slave[j].r_valid                     = s_slave_r_valid[j];
      assign s_slave_r_ready[j]   = slave[j].r_ready;
    end
  endgenerate

  axi_node
    #(
      .AXI_ADDRESS_W      ( AXI_ADDR_WIDTH    ),
      .AXI_DATA_W         ( AXI_DATA_WIDTH    ),
      .AXI_LITE_ADDRESS_W ( AXI_ADDR_WIDTH    ),
      .AXI_LITE_DATA_W    ( AXI_DATA_WIDTH    ),
      .N_INIT_PORT        ( NB_MASTER         ),
      .N_TARG_PORT        ( NB_SLAVE          ),
      .AXI_ID_IN          ( AXI_ID_WIDTH_TARG ),
      .AXI_USER_W         ( AXI_USER_WIDTH    ),
      .N_REGION           ( NB_REGION         )
      )
  axi_node_i
    (
     .clk(clk),
     .rst_n(rst_n),

     .targ_awid_i(s_slave_aw_id),
     .targ_awaddr_i(s_slave_aw_addr),
     .targ_awlen_i(s_slave_aw_len),
     .targ_awsize_i(s_slave_aw_size),
     .targ_awburst_i(s_slave_aw_burst),
     .targ_awlock_i(s_slave_aw_lock),
     .targ_awcache_i(s_slave_aw_cache),
     .targ_awprot_i(s_slave_aw_prot),
     .targ_awregion_i(s_slave_aw_region),
     .targ_awqos_i(s_slave_aw_qos),
     .targ_awuser_i(s_slave_aw_user),
     .targ_awvalid_i(s_slave_aw_valid),
     .targ_awready_o(s_slave_aw_ready),

     .targ_wdata_i(s_slave_w_data),
     .targ_wstrb_i(s_slave_w_strb),
     .targ_wlast_i(s_slave_w_last),
     .targ_wuser_i(s_slave_w_user),
     .targ_wvalid_i(s_slave_w_valid),
     .targ_wready_o(s_slave_w_ready),

     .targ_bid_o(s_slave_b_id),
     .targ_bresp_o(s_slave_b_resp),
     .targ_buser_o(s_slave_b_user),
     .targ_bvalid_o(s_slave_b_valid),
     .targ_bready_i(s_slave_b_ready),

     .targ_arid_i(s_slave_ar_id),
     .targ_araddr_i(s_slave_ar_addr),
     .targ_arlen_i(s_slave_ar_len),
     .targ_arsize_i(s_slave_ar_size),
     .targ_arburst_i(s_slave_ar_burst),
     .targ_arlock_i(s_slave_ar_lock),
     .targ_arcache_i(s_slave_ar_cache),
     .targ_arprot_i(s_slave_ar_prot),
     .targ_arregion_i(s_slave_ar_region),
     .targ_aruser_i(s_slave_ar_user),
     .targ_arqos_i(s_slave_ar_qos),
     .targ_arvalid_i(s_slave_ar_valid),
     .targ_arready_o(s_slave_ar_ready),

     .targ_rid_o(s_slave_r_id),
     .targ_rdata_o(s_slave_r_data),
     .targ_rresp_o(s_slave_r_resp),
     .targ_rlast_o(s_slave_r_last),
     .targ_ruser_o(s_slave_r_user),
     .targ_rvalid_o(s_slave_r_valid),
     .targ_rready_i(s_slave_r_ready),

     .init_awid_o(s_master_aw_id),
     .init_awaddr_o(s_master_aw_addr),
     .init_awlen_o(s_master_aw_len),
     .init_awsize_o(s_master_aw_size),
     .init_awburst_o(s_master_aw_burst),
     .init_awlock_o(s_master_aw_lock),
     .init_awcache_o(s_master_aw_cache),
     .init_awprot_o(s_master_aw_prot),
     .init_awregion_o(s_master_aw_region),
     .init_awqos_o(s_master_aw_qos),
     .init_awuser_o(s_master_aw_user),
     .init_awvalid_o(s_master_aw_valid),
     .init_awready_i(s_master_aw_ready),

     .init_wdata_o(s_master_w_data),
     .init_wstrb_o(s_master_w_strb),
     .init_wlast_o(s_master_w_last),
     .init_wuser_o(s_master_w_user),
     .init_wvalid_o(s_master_w_valid),
     .init_wready_i(s_master_w_ready),

     .init_bid_i(s_master_b_id),
     .init_bresp_i(s_master_b_resp),
     .init_buser_i(s_master_b_user),
     .init_bvalid_i(s_master_b_valid),
     .init_bready_o(s_master_b_ready),

     .init_arid_o(s_master_ar_id),
     .init_araddr_o(s_master_ar_addr),
     .init_arlen_o(s_master_ar_len),
     .init_arsize_o(s_master_ar_size),
     .init_arburst_o(s_master_ar_burst),
     .init_arlock_o(s_master_ar_lock),
     .init_arcache_o(s_master_ar_cache),
     .init_arprot_o(s_master_ar_prot),
     .init_arregion_o(s_master_ar_region),
     .init_aruser_o(s_master_ar_user),
     .init_arqos_o(s_master_ar_qos),
     .init_arvalid_o(s_master_ar_valid),
     .init_arready_i(s_master_ar_ready),

     .init_rid_i(s_master_r_id),
     .init_rdata_i(s_master_r_data),
     .init_rresp_i(s_master_r_resp),
     .init_rlast_i(s_master_r_last),
     .init_ruser_i(s_master_r_user),
     .init_rvalid_i(s_master_r_valid),
     .init_rready_o(s_master_r_ready),

     .s_axi_awaddr('0),
     .s_axi_awvalid('0),
     .s_axi_awready(),
     .s_axi_wdata('0),
     .s_axi_wstrb('0),
     .s_axi_wvalid('0),
     .s_axi_wready(),
     .s_axi_bresp(),
     .s_axi_bvalid(),
     .s_axi_bready('1),
     .s_axi_araddr('0),
     .s_axi_arvalid('0),
     .s_axi_arready(),
     .s_axi_rdata(),
     .s_axi_rresp(),
     .s_axi_rvalid(),
     .s_axi_rready('1),

     .init_START_ADDR_i(s_start_addr),
     .init_END_ADDR_i(s_end_addr),
     .init_valid_rule_i(s_valid_rule),
     .init_connectivity_map_i(s_connectivity_map)
     );


  assign s_valid_rule       = '1;

  assign s_connectivity_map = '1;

endmodule

