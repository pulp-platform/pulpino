////////////////////////////////////////////////////////////////////////////////
// Company:        Multitherman Laboratory @ DEIS - University of Bologna     //
//                    Viale Risorgimento 2 40136                              //
//                    Bologna - fax 0512093785 -                              //
//                                                                            //
// Engineer:       Davide Rossi - davide.rossi@unibo.it                       //
//                                                                            //
// Additional contributions by:                                               //
//                                                                            //
//                                                                            //
//                                                                            //
// Create Date:    26/02/2013                                                 // 
// Design Name:    ULPSoC                                                     // 
// Module Name:    rstgen                                                     //
// Project Name:   ULPSoC                                                     //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    reset generator: asserted asynchronously,                  //
//                 released synchronously                                     //
//                                                                            //
//                                                                            //
// Revision:                                                                  //
// Revision v0.1 - File Created                                               //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

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
   
   always @(posedge clk_i or negedge rst_ni)
     begin
	if (~(rst_ni))
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
