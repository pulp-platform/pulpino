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


module clk_rst_gen
(
    input  logic                            clk_i,
    input  logic                            rstn_i,

    input  logic                            clk_sel_i,
    input  logic                            clk_standalone_i,
    input  logic                            testmode_i,
    input  logic                            scan_en_i,
    input  logic                            scan_i,
    output logic                            scan_o,

    input  logic                            fll_req_i,
    input  logic                            fll_wrn_i,
    input  logic                [1:0]       fll_add_i,
    input  logic               [31:0]       fll_data_i,
    output logic                            fll_ack_o,
    output logic               [31:0]       fll_r_data_o,
    output logic                            fll_lock_o,

    output logic                            clk_o,
    output logic                            rstn_o
);

  logic clk_fll_int;
  logic clk_int;

  cluster_clock_mux2
  clk_mux_i
  (
      .clk_sel_i ( clk_sel_i     ),
      .clk0_i    ( clk_i         ),
      .clk1_i    ( clk_fll_int   ),
      .clk_o     ( clk_int       )
  );

  //----------------------------------------------------------------------------//
  // FLL
  //----------------------------------------------------------------------------//

`ifdef ASIC
  umcL65_LL_FLL
  fll_i
  (
    .FLLCLK     ( clk_fll_int       ),
    .FLLOE      ( 1'b1              ),
    .REFCLK     ( clk_i             ),
    .LOCK       ( fll_lock_o        ),
    .CFGREQ     ( fll_req_i         ),
    .CFGACK     ( fll_ack_o         ),
    .CFGAD      ( fll_add_i         ),
    .CFGD       ( fll_data_i        ),
    .CFGQ       ( fll_r_data_o      ),
    .CFGWEB     ( fll_wrn_i         ),
    .RSTB       ( rstn_i            ),
    .PWDB       ( clk_sel_i         ),
    .STAB       ( clk_standalone_i  ),
    .TM         ( testmode_i        ),
    .TE         ( scan_en_i         ),
    .TD         ( scan_i            ),
    .TQ         ( scan_o            )
  );
`else
  assign fll_ack_o    = fll_req_i;
  assign fll_r_data_o = 1'b0;
  assign fll_lock_o   = 1'b0;
  assign scan_o       = 1'b0;
`endif

  //----------------------------------------------------------------------------//
  // Reset synchronizer
  //----------------------------------------------------------------------------//
  rstgen i_rst_gen_soc
  (
      // PAD FRAME SIGNALS
      .clk_i               ( clk_int         ),
      .rst_ni              ( rstn_i          ),

      // TEST MODE
      .test_mode_i         ( testmode_i      ),

      // OUTPUT RESET
      .rst_no              ( rstn_o          ),
      .init_no             (                 )
  );


  assign clk_o = clk_int;

endmodule
