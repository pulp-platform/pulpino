// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


module axi_slice_wrap
#(
    parameter AXI_ADDR_WIDTH = 32,
    parameter AXI_DATA_WIDTH = 64,
    parameter AXI_USER_WIDTH = 6,
    parameter AXI_ID_WIDTH   = 6,
    parameter SLICE_DEPTH    = 2
)
(
    input logic    clk_i,
    input logic    rst_ni,
    input logic    test_en_i,

    AXI_BUS.Slave  axi_slave,
    AXI_BUS.Master axi_master
);

  generate
    if( SLICE_DEPTH > 1)
    begin : WITH_SLICE
      axi_slice
      #(
        .AXI_ADDR_WIDTH ( AXI_ADDR_WIDTH ),
        .AXI_DATA_WIDTH ( AXI_DATA_WIDTH ),
        .AXI_USER_WIDTH ( AXI_USER_WIDTH ),
        .AXI_ID_WIDTH   ( AXI_ID_WIDTH   ),
        .SLICE_DEPTH    ( SLICE_DEPTH    )
      )
      axi_slice_i
      (
        .clk_i                 ( clk_i                            ),
        .rst_ni                ( rst_ni                           ),
        .test_en_i             ( test_en_i                        ),

        .axi_slave_aw_valid_i  ( axi_slave.aw_valid               ),
        .axi_slave_aw_addr_i   ( axi_slave.aw_addr                ),
        .axi_slave_aw_prot_i   ( axi_slave.aw_prot                ),
        .axi_slave_aw_region_i ( axi_slave.aw_region              ),
        .axi_slave_aw_len_i    ( axi_slave.aw_len                 ),
        .axi_slave_aw_size_i   ( axi_slave.aw_size                ),
        .axi_slave_aw_burst_i  ( axi_slave.aw_burst               ),
        .axi_slave_aw_lock_i   ( axi_slave.aw_lock                ),
        .axi_slave_aw_cache_i  ( axi_slave.aw_cache               ),
        .axi_slave_aw_qos_i    ( axi_slave.aw_qos                 ),
        .axi_slave_aw_id_i     ( axi_slave.aw_id[AXI_ID_WIDTH-1:0]),
        .axi_slave_aw_user_i   ( axi_slave.aw_user                ),
        .axi_slave_aw_ready_o  ( axi_slave.aw_ready               ),

        .axi_slave_ar_valid_i  ( axi_slave.ar_valid               ),
        .axi_slave_ar_addr_i   ( axi_slave.ar_addr                ),
        .axi_slave_ar_prot_i   ( axi_slave.ar_prot                ),
        .axi_slave_ar_region_i ( axi_slave.ar_region              ),
        .axi_slave_ar_len_i    ( axi_slave.ar_len                 ),
        .axi_slave_ar_size_i   ( axi_slave.ar_size                ),
        .axi_slave_ar_burst_i  ( axi_slave.ar_burst               ),
        .axi_slave_ar_lock_i   ( axi_slave.ar_lock                ),
        .axi_slave_ar_cache_i  ( axi_slave.ar_cache               ),
        .axi_slave_ar_qos_i    ( axi_slave.ar_qos                 ),
        .axi_slave_ar_id_i     ( axi_slave.ar_id[AXI_ID_WIDTH-1:0]),
        .axi_slave_ar_user_i   ( axi_slave.ar_user                ),
        .axi_slave_ar_ready_o  ( axi_slave.ar_ready               ),

        .axi_slave_w_valid_i   ( axi_slave.w_valid                ),
        .axi_slave_w_data_i    ( axi_slave.w_data                 ),
        .axi_slave_w_strb_i    ( axi_slave.w_strb                 ),
        .axi_slave_w_user_i    ( axi_slave.w_user                 ),
        .axi_slave_w_last_i    ( axi_slave.w_last                 ),
        .axi_slave_w_ready_o   ( axi_slave.w_ready                ),

        .axi_slave_r_valid_o   ( axi_slave.r_valid                ),
        .axi_slave_r_data_o    ( axi_slave.r_data                 ),
        .axi_slave_r_resp_o    ( axi_slave.r_resp                 ),
        .axi_slave_r_last_o    ( axi_slave.r_last                 ),
        .axi_slave_r_id_o      ( axi_slave.r_id[AXI_ID_WIDTH-1:0] ),
        .axi_slave_r_user_o    ( axi_slave.r_user                 ),
        .axi_slave_r_ready_i   ( axi_slave.r_ready                ),

        .axi_slave_b_valid_o   ( axi_slave.b_valid                ),
        .axi_slave_b_resp_o    ( axi_slave.b_resp                 ),
        .axi_slave_b_id_o      ( axi_slave.b_id[AXI_ID_WIDTH-1:0] ),
        .axi_slave_b_user_o    ( axi_slave.b_user                 ),
        .axi_slave_b_ready_i   ( axi_slave.b_ready                ),

        .axi_master_aw_valid_o ( axi_master.aw_valid               ),
        .axi_master_aw_addr_o  ( axi_master.aw_addr                ),
        .axi_master_aw_prot_o  ( axi_master.aw_prot                ),
        .axi_master_aw_region_o( axi_master.aw_region              ),
        .axi_master_aw_len_o   ( axi_master.aw_len                 ),
        .axi_master_aw_size_o  ( axi_master.aw_size                ),
        .axi_master_aw_burst_o ( axi_master.aw_burst               ),
        .axi_master_aw_lock_o  ( axi_master.aw_lock                ),
        .axi_master_aw_cache_o ( axi_master.aw_cache               ),
        .axi_master_aw_qos_o   ( axi_master.aw_qos                 ),
        .axi_master_aw_id_o    ( axi_master.aw_id[AXI_ID_WIDTH-1:0]),
        .axi_master_aw_user_o  ( axi_master.aw_user                ),
        .axi_master_aw_ready_i ( axi_master.aw_ready               ),

        .axi_master_ar_valid_o ( axi_master.ar_valid               ),
        .axi_master_ar_addr_o  ( axi_master.ar_addr                ),
        .axi_master_ar_prot_o  ( axi_master.ar_prot                ),
        .axi_master_ar_region_o( axi_master.ar_region              ),
        .axi_master_ar_len_o   ( axi_master.ar_len                 ),
        .axi_master_ar_size_o  ( axi_master.ar_size                ),
        .axi_master_ar_burst_o ( axi_master.ar_burst               ),
        .axi_master_ar_lock_o  ( axi_master.ar_lock                ),
        .axi_master_ar_cache_o ( axi_master.ar_cache               ),
        .axi_master_ar_qos_o   ( axi_master.ar_qos                 ),
        .axi_master_ar_id_o    ( axi_master.ar_id[AXI_ID_WIDTH-1:0]),
        .axi_master_ar_user_o  ( axi_master.ar_user                ),
        .axi_master_ar_ready_i ( axi_master.ar_ready               ),

        .axi_master_w_valid_o  ( axi_master.w_valid                ),
        .axi_master_w_data_o   ( axi_master.w_data                 ),
        .axi_master_w_strb_o   ( axi_master.w_strb                 ),
        .axi_master_w_user_o   ( axi_master.w_user                 ),
        .axi_master_w_last_o   ( axi_master.w_last                 ),
        .axi_master_w_ready_i  ( axi_master.w_ready                ),

        .axi_master_r_valid_i  ( axi_master.r_valid                ),
        .axi_master_r_data_i   ( axi_master.r_data                 ),
        .axi_master_r_resp_i   ( axi_master.r_resp                 ),
        .axi_master_r_last_i   ( axi_master.r_last                 ),
        .axi_master_r_id_i     ( axi_master.r_id[AXI_ID_WIDTH-1:0] ),
        .axi_master_r_user_i   ( axi_master.r_user                 ),
        .axi_master_r_ready_o  ( axi_master.r_ready                ),

        .axi_master_b_valid_i  ( axi_master.b_valid                ),
        .axi_master_b_resp_i   ( axi_master.b_resp                 ),
        .axi_master_b_id_i     ( axi_master.b_id[AXI_ID_WIDTH-1:0] ),
        .axi_master_b_user_i   ( axi_master.b_user                 ),
        .axi_master_b_ready_o  ( axi_master.b_ready                )
      );
    end
    else
    begin : NO_SLICE
      assign axi_master.aw_valid  =  axi_slave.aw_valid;
      assign axi_master.aw_addr   =  axi_slave.aw_addr;
      assign axi_master.aw_prot   =  axi_slave.aw_prot;
      assign axi_master.aw_region =  axi_slave.aw_region;
      assign axi_master.aw_len    =  axi_slave.aw_len;
      assign axi_master.aw_size   =  axi_slave.aw_size;
      assign axi_master.aw_burst  =  axi_slave.aw_burst;
      assign axi_master.aw_lock   =  axi_slave.aw_lock;
      assign axi_master.aw_cache  =  axi_slave.aw_cache;
      assign axi_master.aw_qos    =  axi_slave.aw_qos;
      assign axi_master.aw_id     =  axi_slave.aw_id;
      assign axi_master.aw_user   =  axi_slave.aw_user;
      assign axi_slave.aw_ready   =  axi_master.aw_ready;

      assign axi_master.ar_valid  =  axi_slave.ar_valid;
      assign axi_master.ar_addr   =  axi_slave.ar_addr;
      assign axi_master.ar_prot   =  axi_slave.ar_prot;
      assign axi_master.ar_region =  axi_slave.ar_region;
      assign axi_master.ar_len    =  axi_slave.ar_len;
      assign axi_master.ar_size   =  axi_slave.ar_size;
      assign axi_master.ar_burst  =  axi_slave.ar_burst;
      assign axi_master.ar_lock   =  axi_slave.ar_lock;
      assign axi_master.ar_cache  =  axi_slave.ar_cache;
      assign axi_master.ar_qos    =  axi_slave.ar_qos;
      assign axi_master.ar_id     =  axi_slave.ar_id;
      assign axi_master.ar_user   =  axi_slave.ar_user;
      assign axi_slave.ar_ready   =  axi_master.ar_ready;

      assign axi_master.w_valid   =  axi_slave.w_valid;
      assign axi_master.w_data    =  axi_slave.w_data;
      assign axi_master.w_strb    =  axi_slave.w_strb;
      assign axi_master.w_user    =  axi_slave.w_user;
      assign axi_master.w_last    =  axi_slave.w_last;
      assign axi_slave.w_ready    =  axi_master.w_ready;

      assign axi_slave.r_valid    =  axi_master.r_valid;
      assign axi_slave.r_data     =  axi_master.r_data;
      assign axi_slave.r_resp     =  axi_master.r_resp;
      assign axi_slave.r_last     =  axi_master.r_last;
      assign axi_slave.r_id       =  axi_master.r_id;
      assign axi_slave.r_user     =  axi_master.r_user;
      assign axi_master.r_ready   =  axi_slave.r_ready;

      assign axi_slave.b_valid    = axi_master.b_valid;
      assign axi_slave.b_resp     = axi_master.b_resp;
      assign axi_slave.b_id       = axi_master.b_id;
      assign axi_slave.b_user     = axi_master.b_user;
      assign axi_master.b_ready   = axi_slave.b_ready;
    end
  endgenerate
endmodule
