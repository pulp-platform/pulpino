//////////////////////////////////////////////////////////////////////
////                                                              ////
////  dbg_comm_vpi.v                                              ////
////                                                              ////
////                                                              ////
////  This file is part of the SoC/OpenRISC Development Interface ////
////  http://www.opencores.org/cores/DebugInterface/              ////
////                                                              ////
////                                                              ////
////  Author(s):                                                  ////
////       Igor Mohor (igorm@opencores.org)                       ////
////       Gyorgy Jeney (nog@sdf.lonestar.net)                    ////
////       Nathan Yawn (nathan.yawn@opencores.org)                ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000-2008 Authors                              ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: dbg_comm_vpi.v,v $
// Revision 1.2  2009-05-17 20:55:57  Nathan
// Changed email address to opencores.org
//
// Revision 1.1  2008/07/26 17:33:20  Nathan
// Added debug comm module for use with VPI / network communication.
//
// Revision 1.1  2002/03/28 19:59:54  lampret
// Added bench directory
//
// Revision 1.1.1.1  2001/11/04 18:51:07  lampret
// First import.
//
// Revision 1.3  2001/09/24 14:06:13  mohor
// Changes connected to the OpenRISC access (SPR read, SPR write).
//
// Revision 1.2  2001/09/20 10:10:30  mohor
// Working version. Few bugs fixed, comments added.
//
// Revision 1.1.1.1  2001/09/13 13:49:19  mohor
// Initial official release.
//
//
//
//
//


`define TIMEOUT_COUNT 6'h08  // 1/2 of a TCK clock will be this many clk_i ticks.  Must be less than 6 bits.

module dbg_comm_vpi
#(
  parameter TCP_PORT      = "4567",
  parameter TIMEOUT_COUNT = 6'h08 // 1/2 of a TCK clock will be this many clk_i ticks.  Must be less than 6 bits.
)
(
  input     clk_i,
  input     enable_i,

  output    tms_o,
  output    tck_o,
  output    trst_o,
  output    tdi_o,
  input     tdo_i
  );

  //parameter Tp = 20;

  logic [4:0] memory;  // [0:0];

  logic [3:0] in_word_r;
  logic [5:0] clk_count;


  initial
  begin
    clk_count <= TIMEOUT_COUNT + 1;  // Start with the timeout clock stopped
  end

  // Handle commands from the upper level
  initial
  begin
    in_word_r = 'h0;
    memory    = 'h0;
    $jp_init(TCP_PORT);

    // wait for enable before doing anything
    @(posedge enable_i);

    while(1)
    begin
      #1000; // 1ns
      $jp_in(memory);  // This will not change memory[][] if no command has been sent from jp
      if(memory[4])  // was memory[0][4]
      begin
        in_word_r = memory[3:0];
        memory[4] = 1'b0;
        clk_count = 6'b000000;  // Reset the timeout clock in case jp wants to wait for a timeout / half TCK period
      end
    end
  end


  // Send the output bit to the upper layer
  always @(tdo_i)
  begin
    $jp_out(tdo_i);
  end


  assign tck_o  = in_word_r[0];
  assign trst_o = in_word_r[1];
  assign tdi_o  = in_word_r[2];
  assign tms_o  = in_word_r[3];


  // Send timeouts / wait periods to the upper layer
  always @(posedge clk_i)
  begin
    if(clk_count < `TIMEOUT_COUNT) clk_count[5:0] = clk_count[5:0] + 1;
    else if(clk_count == `TIMEOUT_COUNT) begin
      $jp_wait_time();
      clk_count[5:0] = clk_count[5:0] + 1;
    end
    // else it's already timed out, don't do anything
  end

endmodule
