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

module boot_rom_wrap
  #(
    parameter ADDR_WIDTH = `ROM_ADDR_WIDTH,
    parameter DATA_WIDTH = 32
  )(
    // Clock and Reset
    input  logic                  clk,
    input  logic                  rst_n,
    input  logic                  en_i,
    input  logic [ADDR_WIDTH-1:0] addr_i,
    output logic [DATA_WIDTH-1:0] rdata_o
  );

  boot_code
  boot_code_i
  (
    .CLK   ( clk                    ),
    .RSTN  ( rst_n                  ),
    .CSN   ( ~en_i                  ),
    .A     ( addr_i[ADDR_WIDTH-1:2] ),
    .Q     ( rdata_o                )
  );
endmodule
