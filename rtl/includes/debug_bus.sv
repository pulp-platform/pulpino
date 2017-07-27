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

`ifndef DEBUG_BUS_SV
`define DEBUG_BUS_SV

`include "config.sv"

interface DEBUG_BUS
#(
    parameter ADDR_WIDTH = 15
);

  logic                  req;
  logic                  gnt;
  logic                  rvalid;
  logic [ADDR_WIDTH-1:0] addr;
  logic                  we;
  logic [31: 0]          wdata;
  logic [31: 0]          rdata;


  // Master Side
  //***************************************
  modport Master
  (
    output      req,  addr,   we, wdata,
    input       gnt,  rvalid,     rdata
  );

  // Slave Side
  //***************************************
  modport Slave
  (
    input       req,  addr,   we, wdata,
    output      gnt,  rvalid,     rdata
  );

endinterface

`endif
