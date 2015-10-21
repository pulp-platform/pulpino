// ============================================================================= //
//                           COPYRIGHT NOTICE                                    //
// Copyright 2014 Multitherman Laboratory - University of Bologna                //
// ALL RIGHTS RESERVED                                                           //
// This confidential and proprietary software may be used only as authorised by  //
// a licensing agreement from Multitherman Laboratory - University of Bologna.   //
// The entire notice above must be reproduced on all authorized copies and       //
// copies may only be made to the extent permitted by a licensing agreement from //
// Multitherman Laboratory - University of Bologna.                              //
// ============================================================================= //

// ============================================================================= //
// Company:        Multitherman Laboratory @ DEIS - University of Bologna        //
//                    Viale Risorgimento 2 40136                                 //
//                    Bologna - fax 0512093785 -                                 //
//                                                                               //
// Engineer:       Igor Loi - igor.loi@unibo.it                                  //
//                                                                               //
//                                                                               //
// Additional contributions by:                                                  //
//                                                                               //
//                                                                               //
//                                                                               //
// Create Date:    01/02/2014                                                    //
// Design Name:    MISC                                                          //
// Module Name:    generic_fifo                                                  //
// Project Name:   PULP                                                          //
// Language:       SystemVerilog                                                 //
//                                                                               //
// Description:   A simple FIFO used in the D_address_decoder, and D_allocator   //
//                to store the destinations ports                                //
//                                                                               //
// Revision:                                                                     //
// Revision v0.1 - 01/02/2014 : File Created                                     //
// Revision v0.2 - 02/09/2015 : Updated with a global CG cell                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// ============================================================================= //

module generic_fifo 
#( 
   parameter                       DATA_WIDTH = 32,
   parameter                       DATA_DEPTH = 8
)
(
   input  logic                                    clk,
   input  logic                                    rst_n,
   //PUSH SIDE
   input  logic [DATA_WIDTH-1:0]                   data_i,
   input  logic                                    valid_i,
   output logic                                    grant_o,
   //POP SIDE
   output logic [DATA_WIDTH-1:0]                   data_o,
   output logic                                    valid_o,
   input  logic                                    grant_i,

   input  logic                                    test_mode_i
);


   // Local Parameter
   localparam  ADDR_DEPTH = $clog2(DATA_DEPTH);
   enum logic [1:0] { EMPTY, FULL, MIDDLE } CS, NS;
   // Internal Signals

   logic       gate_clock;
   logic       clk_gated;

   logic [ADDR_DEPTH-1:0]          Pop_Pointer_CS,  Pop_Pointer_NS;
   logic [ADDR_DEPTH-1:0]          Push_Pointer_CS, Push_Pointer_NS;
   logic [DATA_WIDTH-1:0]          FIFO_REGISTERS[DATA_DEPTH-1:0];
   integer                         i;



   // Parameter Check
   // synopsys translate_off
   initial
   begin : parameter_check
      integer param_err_flg;
      param_err_flg = 0;

      if (DATA_WIDTH < 1)
      begin
         param_err_flg = 1;
         $display("ERROR: %m :\n  Invalid value (%d) for parameter DATA_WIDTH (legal range: greater than 1)", DATA_WIDTH );
      end

      if (DATA_DEPTH < 1)
      begin
         param_err_flg = 1;
         $display("ERROR: %m :\n  Invalid value (%d) for parameter DATA_DEPTH (legal range: greater than 1)", DATA_DEPTH );
      end                   
   end
   // synopsys translate_on

`ifndef PULP_FPGA_EMUL
   cluster_clock_gating cg_cell
   (
     .clk_i     ( clk         ),
     .en_i      (~gate_clock  ),
     .test_en_i ( test_mode_i ),
     .clk_o     ( clk_gated   )
   );
`else
   assign clk_gated = clk;
`endif

   // UPDATE THE STATE
   always_ff @(posedge clk, negedge rst_n)
   begin
       if(rst_n == 1'b0)
       begin
               CS              <= EMPTY;
               Pop_Pointer_CS  <= {ADDR_DEPTH {1'b0}};
               Push_Pointer_CS <= {ADDR_DEPTH {1'b0}};
       end
       else
       begin
               CS              <= NS;
               Pop_Pointer_CS  <= Pop_Pointer_NS;
               Push_Pointer_CS <= Push_Pointer_NS;
       end
   end


   // Compute Next State
   always_comb
   begin
      gate_clock      = 1'b0;

      case(CS)

      EMPTY:
      begin
          grant_o = 1'b1;
          valid_o = 1'b0;

          case(valid_i)
          1'b0 : 
          begin 
                  NS                      = EMPTY;
                  Push_Pointer_NS = Push_Pointer_CS;
                  Pop_Pointer_NS  = Pop_Pointer_CS;
                  gate_clock      = 1'b1;
          end

          1'b1: 
          begin 
                  NS                      = MIDDLE;
                  Push_Pointer_NS = Push_Pointer_CS + 1'b1;
                  Pop_Pointer_NS  = Pop_Pointer_CS;
          end

          endcase
      end//~EMPTY

      MIDDLE:
      begin
          grant_o = 1'b1;
          valid_o = 1'b1;

          case({valid_i,grant_i})

          2'b01:
          begin
                  gate_clock      = 1'b1;

                  if((Pop_Pointer_CS == Push_Pointer_CS -1 ) || ((Pop_Pointer_CS == DATA_DEPTH-1) && (Push_Pointer_CS == 0) ))
                          NS              = EMPTY;
                  else
                          NS              = MIDDLE;

                  Push_Pointer_NS = Push_Pointer_CS;

                  if(Pop_Pointer_CS == DATA_DEPTH-1)
                          Pop_Pointer_NS  = 0;
                  else
                          Pop_Pointer_NS  = Pop_Pointer_CS + 1'b1;
          end

          2'b00 : 
          begin
                  gate_clock      = 1'b1; 
                  NS                      = MIDDLE;
                  Push_Pointer_NS = Push_Pointer_CS;
                  Pop_Pointer_NS  = Pop_Pointer_CS;
          end

          2'b11: 
          begin
                  NS              = MIDDLE;

                  if(Push_Pointer_CS == DATA_DEPTH-1)
                          Push_Pointer_NS = 0;
                  else
                          Push_Pointer_NS = Push_Pointer_CS + 1'b1;

                  if(Pop_Pointer_CS == DATA_DEPTH-1)
                          Pop_Pointer_NS  = 0;
                  else
                          Pop_Pointer_NS  = Pop_Pointer_CS  + 1'b1;
          end

          2'b10:
          begin 
                  if(( Push_Pointer_CS == Pop_Pointer_CS - 1) || ( (Push_Pointer_CS == DATA_DEPTH-1) && (Pop_Pointer_CS == 0) ))
                          NS              = FULL;
                  else
                          NS        = MIDDLE;

                  if(Push_Pointer_CS == DATA_DEPTH - 1)
                          Push_Pointer_NS = 0;
                  else
                          Push_Pointer_NS = Push_Pointer_CS + 1'b1;

                  Pop_Pointer_NS  = Pop_Pointer_CS;
          end

          endcase                     
      end

      FULL:
      begin
          grant_o = 1'b0;
          valid_o = 1'b1;
          gate_clock      = 1'b1;

          case(grant_i)
          1'b1: 
          begin 
                  NS              = MIDDLE;

                  Push_Pointer_NS = Push_Pointer_CS;

                  if(Pop_Pointer_CS == DATA_DEPTH-1)
                          Pop_Pointer_NS  = 0;
                  else
                          Pop_Pointer_NS  = Pop_Pointer_CS  + 1'b1;
          end

          1'b0:
          begin 
                  NS              = FULL;
                  Push_Pointer_NS = Push_Pointer_CS;
                  Pop_Pointer_NS  = Pop_Pointer_CS;
          end
          endcase                 

      end // end of FULL

      default :
      begin
          gate_clock      = 1'b1;
          grant_o       = 1'b0;
          valid_o       = 1'b0;
          NS              = EMPTY;
          Pop_Pointer_NS  = 0;
          Push_Pointer_NS = 0;
      end

      endcase
   end

   always_ff @(posedge clk_gated, negedge rst_n)
   begin
      if(rst_n == 1'b0)
      begin
      for (i=0; i< DATA_DEPTH; i++)
         FIFO_REGISTERS[i] <= {DATA_WIDTH {1'b0}};
      end
      else
      begin
         if((grant_o == 1'b1) && (valid_i == 1'b1))
            FIFO_REGISTERS[Push_Pointer_CS] <= data_i;
      end
   end

   assign data_o = FIFO_REGISTERS[Pop_Pointer_CS];

endmodule // generic_fifo