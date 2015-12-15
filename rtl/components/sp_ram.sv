// Copyright 2015 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

module sp_ram
  #(
    parameter ADDR_WIDTH = 8,
    parameter NUM_WORDS  = 256
  )(
    // Clock and Reset
    input  logic clk,

    input  logic                   en_i,
    input  logic [ADDR_WIDTH-1:0]  addr_i,
    input  logic [31:0]            wdata_i,
    output logic [31:0]            rdata_o,
    input  logic                   we_i,
    input  logic [3:0]             be_i
  );

  localparam words = NUM_WORDS;

  logic [3:0][7:0] mem[words];

  always @(posedge clk)
  begin
    if (en_i && we_i)
    begin
      if (be_i[0])
        mem[addr_i][0] <= wdata_i[7:0];
      if (be_i[1])
        mem[addr_i][1] <= wdata_i[15:8];
      if (be_i[2])
        mem[addr_i][2] <= wdata_i[23:16];
      if (be_i[3])
        mem[addr_i][3] <= wdata_i[31:24];
    end

    rdata_o <= mem[addr_i];
  end

endmodule
