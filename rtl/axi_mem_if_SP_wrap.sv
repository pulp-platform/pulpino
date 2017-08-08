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

module axi_mem_if_SP_wrap
  #(
    parameter AXI_ADDR_WIDTH = 32,
    parameter AXI_DATA_WIDTH = 64,
    parameter AXI_ID_WIDTH   = 10,
    parameter AXI_USER_WIDTH = 10,
    parameter MEM_ADDR_WIDTH = 10
  )(
    // Clock and Reset
    input logic                         clk,
    input logic                         rst_n,
    input logic                         test_en_i,

    output logic                        mem_req_o,
    output logic [MEM_ADDR_WIDTH-1:0]   mem_addr_o,
    output logic                        mem_we_o,
    output logic [AXI_DATA_WIDTH/8-1:0] mem_be_o,
    input  logic [AXI_DATA_WIDTH-1:0]   mem_rdata_i,
    output logic [AXI_DATA_WIDTH-1:0]   mem_wdata_o,

    AXI_BUS.Slave  slave
  );

  logic       cen;
  logic       wen;

  axi_mem_if_SP
  #(
    .AXI4_ADDRESS_WIDTH  ( AXI_ADDR_WIDTH  ),
    .AXI4_RDATA_WIDTH    ( AXI_DATA_WIDTH  ),
    .AXI4_WDATA_WIDTH    ( AXI_DATA_WIDTH  ),
    .AXI4_ID_WIDTH       ( AXI_ID_WIDTH    ),
    .AXI4_USER_WIDTH     ( AXI_USER_WIDTH  ),
    .MEM_ADDR_WIDTH      ( MEM_ADDR_WIDTH  )
  )
  axi_mem_if_SP_i
  (
    .ACLK       ( clk             ),
    .ARESETn    ( rst_n           ),
    .test_en_i  ( test_en_i       ),

    .AWID_i     ( slave.aw_id     ),
    .AWADDR_i   ( slave.aw_addr   ),
    .AWLEN_i    ( slave.aw_len    ),
    .AWSIZE_i   ( slave.aw_size   ),
    .AWBURST_i  ( slave.aw_burst  ),
    .AWLOCK_i   ( slave.aw_lock   ),
    .AWCACHE_i  ( slave.aw_cache  ),
    .AWPROT_i   ( slave.aw_prot   ),
    .AWREGION_i ( slave.aw_region ),
    .AWUSER_i   ( slave.aw_user   ),
    .AWQOS_i    ( slave.aw_qos    ),
    .AWVALID_i  ( slave.aw_valid  ),
    .AWREADY_o  ( slave.aw_ready  ),

    .WDATA_i    ( slave.w_data    ),
    .WSTRB_i    ( slave.w_strb    ),
    .WLAST_i    ( slave.w_last    ),
    .WUSER_i    ( slave.w_user    ),
    .WVALID_i   ( slave.w_valid   ),
    .WREADY_o   ( slave.w_ready   ),

    .BID_o      ( slave.b_id      ),
    .BRESP_o    ( slave.b_resp    ),
    .BVALID_o   ( slave.b_valid   ),
    .BUSER_o    ( slave.b_user    ),
    .BREADY_i   ( slave.b_ready   ),

    .ARID_i     ( slave.ar_id     ),
    .ARADDR_i   ( slave.ar_addr   ),
    .ARLEN_i    ( slave.ar_len    ),
    .ARSIZE_i   ( slave.ar_size   ),
    .ARBURST_i  ( slave.ar_burst  ),
    .ARLOCK_i   ( slave.ar_lock   ),
    .ARCACHE_i  ( slave.ar_cache  ),
    .ARPROT_i   ( slave.ar_prot   ),
    .ARREGION_i ( slave.ar_region ),
    .ARUSER_i   ( slave.ar_user   ),
    .ARQOS_i    ( slave.ar_qos    ),
    .ARVALID_i  ( slave.ar_valid  ),
    .ARREADY_o  ( slave.ar_ready  ),

    .RID_o      ( slave.r_id      ),
    .RDATA_o    ( slave.r_data    ),
    .RRESP_o    ( slave.r_resp    ),
    .RLAST_o    ( slave.r_last    ),
    .RUSER_o    ( slave.r_user    ),
    .RVALID_o   ( slave.r_valid   ),
    .RREADY_i   ( slave.r_ready   ),

    .CEN_o      ( cen             ),
    .WEN_o      ( wen             ),
    .A_o        ( mem_addr_o      ),
    .D_o        ( mem_wdata_o     ),
    .BE_o       ( mem_be_o        ),
    .Q_i        ( mem_rdata_i     )
  );

  assign mem_req_o = ~cen;
  assign mem_we_o  = ~wen;

endmodule
