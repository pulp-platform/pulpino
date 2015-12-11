// Copyright 2015 ETH Zurich and University of Bologna.
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
    parameter AXI_ADDR_WIDTH = 32,
    parameter AXI_DATA_WIDTH = 64,
    parameter AXI_USER_WIDTH = 6,
    parameter AXI_ID_WIDTH   = 6,
    parameter APB_ADDR_WIDTH = 12,
    parameter APB_NUM_SLAVES = 8
)
(
    input logic                                 clk_i,
    input logic                                 rst_ni,

    AXI_BUS.Slave                               axi_slave,

    output logic                                penable,
    output logic                                pwrite,
    output logic        [APB_ADDR_WIDTH-1:0]    paddr,
    output logic        [APB_NUM_SLAVES-1:0]    psel,
    output logic                      [31:0]    pwdata,
    input  logic [APB_NUM_SLAVES-1:0] [31:0]    prdata,
    input  logic        [APB_NUM_SLAVES-1:0]    pready,
    input  logic        [APB_NUM_SLAVES-1:0]    pslverr
);


  //********************************************************
  //************** AXI2APB WRAPER **************************
  //********************************************************
  axi2apb
  #(
    .AXI_ADDR_WIDTH ( AXI_ADDR_WIDTH ),
    .AXI_DATA_WIDTH ( AXI_DATA_WIDTH ),
    .AXI_ID_WIDTH   ( AXI_ID_WIDTH   ),
    .APB_NUM_SLAVES ( APB_NUM_SLAVES ),
    .APB_ADDR_WIDTH ( APB_ADDR_WIDTH )
    )
  axi2apb_i
  (
    .clk     ( clk_i              ),
    .rstn    ( rst_ni             ),

    .AWID    ( axi_slave.aw_id    ),
    .AWADDR  ( axi_slave.aw_addr  ),
    .AWLEN   ( axi_slave.aw_len   ),
    .AWSIZE  ( axi_slave.aw_size  ),
    .AWVALID ( axi_slave.aw_valid ),
    .AWREADY ( axi_slave.aw_ready ),
    .WDATA   ( axi_slave.w_data   ),
    .WSTRB   ( axi_slave.w_strb   ),
    .WLAST   ( axi_slave.w_last   ),
    .WVALID  ( axi_slave.w_valid  ),
    .WREADY  ( axi_slave.w_ready  ),
    .BID     ( axi_slave.b_id     ),
    .BRESP   ( axi_slave.b_resp   ),
    .BVALID  ( axi_slave.b_valid  ),
    .BREADY  ( axi_slave.b_ready  ),
    .ARID    ( axi_slave.ar_id    ),
    .ARADDR  ( axi_slave.ar_addr  ),
    .ARLEN   ( axi_slave.ar_len   ),
    .ARSIZE  ( axi_slave.ar_size  ),
    .ARVALID ( axi_slave.ar_valid ),
    .ARREADY ( axi_slave.ar_ready ),
    .RID     ( axi_slave.r_id     ),
    .RDATA   ( axi_slave.r_data   ),
    .RRESP   ( axi_slave.r_resp   ),
    .RLAST   ( axi_slave.r_last   ),
    .RVALID  ( axi_slave.r_valid  ),
    .RREADY  ( axi_slave.r_ready  ),
    .penable ( penable            ),
    .pwrite  ( pwrite             ),
    .paddr   ( paddr              ),
    .psel    ( psel               ),
    .pwdata  ( pwdata             ),
    .prdata  ( prdata             ),
    .pready  ( pready             ),
    .pslverr ( pslverr            )
  );

endmodule
