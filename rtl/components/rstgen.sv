// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

module rstgen
(
    // PAD FRAME SIGNALS
    input  logic clk_i,
    input  logic rst_ni,

    // TEST MODE
    input  logic test_mode_i,

    // OUTPUT RESET
    output logic rst_no,
    output logic init_no
  );

  logic   s_rst_ff3,s_rst_ff2,s_rst_ff1,s_rst_ff0,s_rst_n;

  //
  // RESET GENERATION
  //

  always_ff @(posedge clk_i, negedge rst_ni)
  begin
    if (~rst_ni)
    begin
      s_rst_ff0  <= 1'b0;
      s_rst_ff1  <= 1'b0;
      s_rst_ff2  <= 1'b0;
      s_rst_ff3  <= 1'b0;
      s_rst_n    <= 1'b0;
    end
    else
    begin
      s_rst_ff3  <= 1'b1;
      s_rst_ff2  <= s_rst_ff3;
      s_rst_ff1  <= s_rst_ff2;
      s_rst_ff0  <= s_rst_ff1;
      s_rst_n    <= s_rst_ff0;
    end
  end

  always_comb
  begin
    if (test_mode_i == 1'b0)
      rst_no  = s_rst_n;
    else
      rst_no  = rst_ni;
  end

  always_comb
  begin
    if (test_mode_i == 1'b0)
      init_no = s_rst_n;
    else
      init_no = 1'b1;
  end

endmodule
