// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


`include "axi_bus.sv"

module core2axi_wrap
#(
    parameter AXI_ADDR_WIDTH   = 32,
    parameter AXI_DATA_WIDTH   = 32,
    parameter AXI_USER_WIDTH   = 6,
    parameter AXI_ID_WIDTH     = 6,
    parameter REGISTERED_GRANT = "FALSE"
)
(
    input logic                       clk_i,
    input logic                       rst_ni,

    input  logic                      data_req_i,
    output logic                      data_gnt_o,
    output logic                      data_rvalid_o,
    input  logic [AXI_ADDR_WIDTH-1:0] data_addr_i,
    input  logic                      data_we_i,
    input  logic [3:0]                data_be_i,
    output logic [31:0]               data_rdata_o,
    input  logic [31:0]               data_wdata_i,

    AXI_BUS.Master                    master
);


  //********************************************************
  //************** AXI2APB WRAPER **************************
  //********************************************************
  core2axi
  #(
    .AXI4_ADDRESS_WIDTH ( AXI_ADDR_WIDTH   ),
    .AXI4_RDATA_WIDTH   ( AXI_DATA_WIDTH   ),
    .AXI4_WDATA_WIDTH   ( AXI_DATA_WIDTH   ),
    .AXI4_ID_WIDTH      ( AXI_ID_WIDTH     ),
    .AXI4_USER_WIDTH    ( AXI_USER_WIDTH   ),
    .REGISTERED_GRANT   ( REGISTERED_GRANT )
  )
  core2axi_i
  (
    .clk_i         ( clk_i               ),
    .rst_ni        ( rst_ni              ),

    .data_req_i    ( data_req_i          ),
    .data_gnt_o    ( data_gnt_o          ),
    .data_rvalid_o ( data_rvalid_o       ),
    .data_addr_i   ( data_addr_i         ),
    .data_we_i     ( data_we_i           ),
    .data_be_i     ( data_be_i           ),
    .data_rdata_o  ( data_rdata_o        ),
    .data_wdata_i  ( data_wdata_i        ),

    .aw_id_o       ( master.aw_id        ),
    .aw_addr_o     ( master.aw_addr      ),
    .aw_len_o      ( master.aw_len       ),
    .aw_size_o     ( master.aw_size      ),
    .aw_burst_o    ( master.aw_burst     ),
    .aw_lock_o     ( master.aw_lock      ),
    .aw_cache_o    ( master.aw_cache     ),
    .aw_prot_o     ( master.aw_prot      ),
    .aw_region_o   ( master.aw_region    ),
    .aw_user_o     ( master.aw_user      ),
    .aw_qos_o      ( master.aw_qos       ),
    .aw_valid_o    ( master.aw_valid     ),
    .aw_ready_i    ( master.aw_ready     ),

    .w_data_o      ( master.w_data       ),
    .w_strb_o      ( master.w_strb       ),
    .w_last_o      ( master.w_last       ),
    .w_user_o      ( master.w_user       ),
    .w_valid_o     ( master.w_valid      ),
    .w_ready_i     ( master.w_ready      ),

    .b_id_i        ( master.b_id         ),
    .b_resp_i      ( master.b_resp       ),
    .b_valid_i     ( master.b_valid      ),
    .b_user_i      ( master.b_user       ),
    .b_ready_o     ( master.b_ready      ),

    .ar_id_o       ( master.ar_id        ),
    .ar_addr_o     ( master.ar_addr      ),
    .ar_len_o      ( master.ar_len       ),
    .ar_size_o     ( master.ar_size      ),
    .ar_burst_o    ( master.ar_burst     ),
    .ar_lock_o     ( master.ar_lock      ),
    .ar_cache_o    ( master.ar_cache     ),
    .ar_prot_o     ( master.ar_prot      ),
    .ar_region_o   ( master.ar_region    ),
    .ar_user_o     ( master.ar_user      ),
    .ar_qos_o      ( master.ar_qos       ),
    .ar_valid_o    ( master.ar_valid     ),
    .ar_ready_i    ( master.ar_ready     ),

    .r_id_i        ( master.r_id         ),
    .r_data_i      ( master.r_data       ),
    .r_resp_i      ( master.r_resp       ),
    .r_last_i      ( master.r_last       ),
    .r_user_i      ( master.r_user       ),
    .r_valid_i     ( master.r_valid      ),
    .r_ready_o     ( master.r_ready      )
  );

endmodule
