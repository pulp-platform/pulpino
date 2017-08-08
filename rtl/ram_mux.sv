// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

module ram_mux
  #(
    parameter ADDR_WIDTH = 32,
    parameter OUT_WIDTH = 32,
    parameter IN0_WIDTH = 32, // in widths cannot be bigger than out width
    parameter IN1_WIDTH = 32
  )(
    // Clock and Reset
    input logic                     clk,
    input logic                     rst_n,

    // port0 has priority
    input  logic                    port0_req_i,
    output logic                    port0_gnt_o,
    output logic                    port0_rvalid_o,
    input  logic [ADDR_WIDTH-1:0]   port0_addr_i,
    input  logic                    port0_we_i,
    input  logic [IN0_WIDTH/8-1:0]  port0_be_i,
    output logic [IN0_WIDTH-1:0]    port0_rdata_o,
    input  logic [IN0_WIDTH-1:0]    port0_wdata_i,

    input  logic                    port1_req_i,
    output logic                    port1_gnt_o,
    output logic                    port1_rvalid_o,
    input  logic [ADDR_WIDTH-1:0]   port1_addr_i,
    input  logic                    port1_we_i,
    input  logic [IN1_WIDTH/8-1:0]  port1_be_i,
    output logic [IN1_WIDTH-1:0]    port1_rdata_o,
    input  logic [IN1_WIDTH-1:0]    port1_wdata_i,

    // to RAM
    output logic                    ram_en_o,
    output logic [ADDR_WIDTH-1:0]   ram_addr_o,
    output logic                    ram_we_o,
    output logic [OUT_WIDTH/8-1:0]  ram_be_o,
    input  logic [OUT_WIDTH-1:0]    ram_rdata_i,
    output logic [OUT_WIDTH-1:0]    ram_wdata_o
  );

  localparam IN0_ADDR_HIGH = $clog2(OUT_WIDTH/8) - 1;
  localparam IN0_ADDR_LOW  = $clog2(IN0_WIDTH/8);
  localparam IN0_RATIO     = OUT_WIDTH/IN0_WIDTH;

  logic [OUT_WIDTH/8-1:0] port0_be;

  // size adaptation
  genvar i0;
  generate
    if (IN0_ADDR_HIGH >= IN0_ADDR_LOW)
    begin

      logic                                port0_addr_q;
      logic [IN0_RATIO-1:0][IN0_WIDTH-1:0] port0_rdata;

      always_ff @(posedge clk, negedge rst_n)
      begin
        if (~rst_n)
          port0_addr_q <= '0;
        else
          if (port0_gnt_o)
            port0_addr_q <= port0_addr_i[IN0_ADDR_HIGH:IN0_ADDR_LOW];
      end

      for(i0 = 0; i0 < IN0_RATIO; i0++)
      begin
        assign port0_be[(i0+1) * IN0_WIDTH/8 -1:i0*IN0_WIDTH/8] =
                (i0 == port0_addr_i[IN0_ADDR_HIGH:IN0_ADDR_LOW]) ? port0_be_i : '0;

        assign port0_rdata[i0] = ram_rdata_i[(i0+1)*IN0_WIDTH-1:i0*IN0_WIDTH];
      end

      assign port0_rdata_o = port0_rdata[port0_addr_q];
    end else begin
      // nothing to be done, just pass through
      assign port0_be = port0_be_i;
      assign port0_rdata_o = ram_rdata_i;
    end
  endgenerate


  localparam IN1_ADDR_HIGH = $clog2(OUT_WIDTH/8) - 1;
  localparam IN1_ADDR_LOW  = $clog2(IN1_WIDTH/8);
  localparam IN1_RATIO     = OUT_WIDTH/IN1_WIDTH;

  logic [OUT_WIDTH/8-1:0] port1_be;

  // size adaptation
  genvar i1;
  generate
    if (IN1_ADDR_HIGH >= IN1_ADDR_LOW)
    begin
      logic                                port1_addr_q;
      logic [IN1_RATIO-1:0][IN1_WIDTH-1:0] port1_rdata;

      always_ff @(posedge clk, negedge rst_n)
      begin
        if (~rst_n)
          port1_addr_q <= '0;
        else
          if (port1_gnt_o)
            port1_addr_q <= port1_addr_i[IN1_ADDR_HIGH:IN1_ADDR_LOW];
      end

      for(i1 = 0; i1 < OUT_WIDTH/IN1_WIDTH; i1++)
      begin
        assign port1_be[(i1+1) * IN1_WIDTH/8 -1:i1*IN1_WIDTH/8] =
                (i1 == port1_addr_i[IN1_ADDR_HIGH:IN1_ADDR_LOW]) ? port1_be_i : '0;

        assign port1_rdata[i1] = ram_rdata_i[(i1+1)*IN1_WIDTH-1:i1*IN1_WIDTH];
      end

      assign port1_rdata_o = port1_rdata[port1_addr_q];
    end else begin
      // nothing to be done, just pass through
      assign port1_be = port1_be_i;
      assign port1_rdata_o = ram_rdata_i;
    end
  endgenerate


  // RAM mux for data port of core and AXI interface
  always_comb
  begin
    port0_gnt_o = 1'b0;
    port1_gnt_o = 1'b0;

    // port0 has priority
    if(port0_req_i)
      port0_gnt_o   = 1'b1;
    else if(port1_req_i)
      port1_gnt_o   = 1'b1;
  end

  assign ram_en_o    = port0_req_i | port1_req_i;
  assign ram_addr_o  = port0_req_i ? port0_addr_i  : port1_addr_i;
  assign ram_wdata_o = port0_req_i ? {OUT_WIDTH/IN0_WIDTH{port0_wdata_i}} : {OUT_WIDTH/IN1_WIDTH{port1_wdata_i}};
  assign ram_we_o    = port0_req_i ? port0_we_i    : port1_we_i;
  assign ram_be_o    = port0_req_i ? port0_be      : port1_be;

  // generate rvalid signals from gnt signals
  always_ff @(posedge clk, negedge rst_n)
  begin
    if (rst_n == 1'b0)
    begin
      port0_rvalid_o <= 1'b0;
      port1_rvalid_o <= 1'b0;
    end
    else
    begin
      port0_rvalid_o <= port0_gnt_o;
      port1_rvalid_o <= port1_gnt_o;
    end
  end

endmodule
