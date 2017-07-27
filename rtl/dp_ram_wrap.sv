/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */


`include "config.sv"

module dp_ram_wrap
  #(
    parameter ADDR_WIDTH = 8
  )(
    // Clock and Reset
    input  logic clk,

    input  logic                   en_a_i,
    input  logic [ADDR_WIDTH-1:0]  addr_a_i,
    input  logic [31:0]            wdata_a_i,
    output logic [31:0]            rdata_a_o,
    input  logic                   we_a_i,
    input  logic [3:0]             be_a_i,

    input  logic                   en_b_i,
    input  logic [ADDR_WIDTH-1:0]  addr_b_i,
    input  logic [31:0]            wdata_b_i,
    output logic [31:0]            rdata_b_o,
    input  logic                   we_b_i,
    input  logic [3:0]             be_b_i
  );

`ifdef PULP_FPGA_EMUL
  xilinx_mem_32768x32_dp
  dp_ram_i
  (
    .clka   ( clk                  ),
    .rsta   ( 1'b1                 ),

    .clkb   ( clk                  ),
    .rstb   ( 1'b1                 ),

    .ena    ( en_a_i               ),
    .addra  ( addr_a_i             ),
    .dina   ( wdata_a_i            ),
    .douta  ( rdata_a_o            ),
    .wea    ( be_a_i & {4{we_a_i}} ),

    .enb    ( en_b_i               ),
    .addrb  ( addr_b_i             ),
    .dinb   ( wdata_b_i            ),
    .doutb  ( rdata_b_o            ),
    .web    ( be_b_i & {4{we_b_i}} )
    );
`else
  dp_ram
  #(
    .ADDR_WIDTH ( ADDR_WIDTH )
    )
  dp_ram_i
  (
    .clk       ( clk       ),

    .en_a_i    ( en_a_i    ),
    .addr_a_i  ( addr_a_i  ),
    .wdata_a_i ( wdata_a_i ),
    .rdata_a_o ( rdata_a_o ),
    .we_a_i    ( we_a_i    ),
    .be_a_i    ( be_a_i    ),

    .en_b_i    ( en_b_i    ),
    .addr_b_i  ( addr_b_i  ),
    .wdata_b_i ( wdata_b_i ),
    .rdata_b_o ( rdata_b_o ),
    .we_b_i    ( we_b_i    ),
    .be_b_i    ( be_b_i    )
    );
`endif

endmodule

