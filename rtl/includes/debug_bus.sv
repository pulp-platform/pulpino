// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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
