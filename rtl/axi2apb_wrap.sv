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

module axi2apb_wrap
#(
    parameter AXI_ADDR_WIDTH   = 32,
    parameter AXI_DATA_WIDTH   = 32,
    parameter AXI_USER_WIDTH   = 6,
    parameter AXI_ID_WIDTH     = 6,
    parameter APB_ADDR_WIDTH   = 32
)
(
    input logic                                 clk_i,
    input logic                                 rst_ni,
    input logic                                 test_en_i,

    AXI_BUS.Slave                               axi_slave,

    APB_BUS.Master                              apb_master

);


  //********************************************************
  //************** AXI2APB WRAPER **************************
  //********************************************************

  generate if (AXI_DATA_WIDTH == 32)
    begin
      axi2apb32
      #(
        .AXI4_ADDRESS_WIDTH ( AXI_ADDR_WIDTH ),
        .AXI4_RDATA_WIDTH   ( AXI_DATA_WIDTH ),
        .AXI4_WDATA_WIDTH   ( AXI_DATA_WIDTH ),
        .AXI4_ID_WIDTH      ( AXI_ID_WIDTH   ),
        .AXI4_USER_WIDTH    ( 1              ),

        .BUFF_DEPTH_SLAVE   ( 2              ),
        .APB_ADDR_WIDTH     ( APB_ADDR_WIDTH )
      )
      axi2apb_i
      (
        .ACLK       ( clk_i               ),
        .ARESETn    ( rst_ni              ),
        .test_en_i  ( test_en_i           ),

        .AWID_i     ( axi_slave.aw_id     ),
        .AWADDR_i   ( axi_slave.aw_addr   ),
        .AWLEN_i    ( axi_slave.aw_len    ),
        .AWSIZE_i   ( axi_slave.aw_size   ),
        .AWBURST_i  ( axi_slave.aw_burst  ),
        .AWLOCK_i   ( axi_slave.aw_lock   ),
        .AWCACHE_i  ( axi_slave.aw_cache  ),
        .AWPROT_i   ( axi_slave.aw_prot   ),
        .AWREGION_i ( axi_slave.aw_region ),
        .AWUSER_i   ( axi_slave.aw_user   ),
        .AWQOS_i    ( axi_slave.aw_qos    ),
        .AWVALID_i  ( axi_slave.aw_valid  ),
        .AWREADY_o  ( axi_slave.aw_ready  ),

        .WDATA_i    ( axi_slave.w_data    ),
        .WSTRB_i    ( axi_slave.w_strb    ),
        .WLAST_i    ( axi_slave.w_last    ),
        .WUSER_i    ( axi_slave.w_user    ),
        .WVALID_i   ( axi_slave.w_valid   ),
        .WREADY_o   ( axi_slave.w_ready   ),

        .BID_o      ( axi_slave.b_id      ),
        .BRESP_o    ( axi_slave.b_resp    ),
        .BVALID_o   ( axi_slave.b_valid   ),
        .BUSER_o    ( axi_slave.b_user    ),
        .BREADY_i   ( axi_slave.b_ready   ),

        .ARID_i     ( axi_slave.ar_id     ),
        .ARADDR_i   ( axi_slave.ar_addr   ),
        .ARLEN_i    ( axi_slave.ar_len    ),
        .ARSIZE_i   ( axi_slave.ar_size   ),
        .ARBURST_i  ( axi_slave.ar_burst  ),
        .ARLOCK_i   ( axi_slave.ar_lock   ),
        .ARCACHE_i  ( axi_slave.ar_cache  ),
        .ARPROT_i   ( axi_slave.ar_prot   ),
        .ARREGION_i ( axi_slave.ar_region ),
        .ARUSER_i   ( axi_slave.ar_user   ),
        .ARQOS_i    ( axi_slave.ar_qos    ),
        .ARVALID_i  ( axi_slave.ar_valid  ),
        .ARREADY_o  ( axi_slave.ar_ready  ),

        .RID_o      ( axi_slave.r_id      ),
        .RDATA_o    ( axi_slave.r_data    ),
        .RRESP_o    ( axi_slave.r_resp    ),
        .RLAST_o    ( axi_slave.r_last    ),
        .RUSER_o    ( axi_slave.r_user    ),
        .RVALID_o   ( axi_slave.r_valid   ),
        .RREADY_i   ( axi_slave.r_ready   ),

        .PENABLE    ( apb_master.penable  ),
        .PWRITE     ( apb_master.pwrite   ),
        .PADDR      ( apb_master.paddr    ),
        .PSEL       ( apb_master.psel     ),
        .PWDATA     ( apb_master.pwdata   ),
        .PRDATA     ( apb_master.prdata   ),
        .PREADY     ( apb_master.pready   ),
        .PSLVERR    ( apb_master.pslverr  )
      );
    end
    else if (AXI_DATA_WIDTH == 64)
    begin
      axi2apb
      #(
        .AXI4_ADDRESS_WIDTH ( AXI_ADDR_WIDTH ),
        .AXI4_RDATA_WIDTH   ( AXI_DATA_WIDTH ),
        .AXI4_WDATA_WIDTH   ( AXI_DATA_WIDTH ),
        .AXI4_ID_WIDTH      ( AXI_ID_WIDTH   ),
        .AXI4_USER_WIDTH    ( 1              ),

        .BUFF_DEPTH_SLAVE   ( 2              ),
        .APB_ADDR_WIDTH     ( APB_ADDR_WIDTH )
      )
      axi2apb_i
      (
        .ACLK       ( clk_i               ),
        .ARESETn    ( rst_ni              ),
        .test_en_i  ( test_en_i           ),

        .AWID_i     ( axi_slave.aw_id     ),
        .AWADDR_i   ( axi_slave.aw_addr   ),
        .AWLEN_i    ( axi_slave.aw_len    ),
        .AWSIZE_i   ( axi_slave.aw_size   ),
        .AWBURST_i  ( axi_slave.aw_burst  ),
        .AWLOCK_i   ( axi_slave.aw_lock   ),
        .AWCACHE_i  ( axi_slave.aw_cache  ),
        .AWPROT_i   ( axi_slave.aw_prot   ),
        .AWREGION_i ( axi_slave.aw_region ),
        .AWUSER_i   ( axi_slave.aw_user   ),
        .AWQOS_i    ( axi_slave.aw_qos    ),
        .AWVALID_i  ( axi_slave.aw_valid  ),
        .AWREADY_o  ( axi_slave.aw_ready  ),

        .WDATA_i    ( axi_slave.w_data    ),
        .WSTRB_i    ( axi_slave.w_strb    ),
        .WLAST_i    ( axi_slave.w_last    ),
        .WUSER_i    ( axi_slave.w_user    ),
        .WVALID_i   ( axi_slave.w_valid   ),
        .WREADY_o   ( axi_slave.w_ready   ),

        .BID_o      ( axi_slave.b_id      ),
        .BRESP_o    ( axi_slave.b_resp    ),
        .BVALID_o   ( axi_slave.b_valid   ),
        .BUSER_o    ( axi_slave.b_user    ),
        .BREADY_i   ( axi_slave.b_ready   ),

        .ARID_i     ( axi_slave.ar_id     ),
        .ARADDR_i   ( axi_slave.ar_addr   ),
        .ARLEN_i    ( axi_slave.ar_len    ),
        .ARSIZE_i   ( axi_slave.ar_size   ),
        .ARBURST_i  ( axi_slave.ar_burst  ),
        .ARLOCK_i   ( axi_slave.ar_lock   ),
        .ARCACHE_i  ( axi_slave.ar_cache  ),
        .ARPROT_i   ( axi_slave.ar_prot   ),
        .ARREGION_i ( axi_slave.ar_region ),
        .ARUSER_i   ( axi_slave.ar_user   ),
        .ARQOS_i    ( axi_slave.ar_qos    ),
        .ARVALID_i  ( axi_slave.ar_valid  ),
        .ARREADY_o  ( axi_slave.ar_ready  ),

        .RID_o      ( axi_slave.r_id      ),
        .RDATA_o    ( axi_slave.r_data    ),
        .RRESP_o    ( axi_slave.r_resp    ),
        .RLAST_o    ( axi_slave.r_last    ),
        .RUSER_o    ( axi_slave.r_user    ),
        .RVALID_o   ( axi_slave.r_valid   ),
        .RREADY_i   ( axi_slave.r_ready   ),

        .PENABLE    ( apb_master.penable  ),
        .PWRITE     ( apb_master.pwrite   ),
        .PADDR      ( apb_master.paddr    ),
        .PSEL       ( apb_master.psel     ),
        .PWDATA     ( apb_master.pwdata   ),
        .PRDATA     ( apb_master.prdata   ),
        .PREADY     ( apb_master.pready   ),
        .PSLVERR    ( apb_master.pslverr  )
      );
    end
  endgenerate

endmodule
