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
