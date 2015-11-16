`include "config.sv"

module sp_ram_wrap
  #(
    parameter RAM_SIZE   = 32768, // in words
    parameter ADDR_WIDTH = $clog2(RAM_SIZE)
  )(
    // Clock and Reset
    input  logic clk,
    input  logic rstn_i,
    input  logic                   en_i,
    input  logic [ADDR_WIDTH-1:0]  addr_i,
    input  logic [31:0]            wdata_i,
    output logic [31:0]            rdata_o,
    input  logic                   we_i,
    input  logic [3:0]             be_i,
    input  logic                   bypass_en_i
  );

`ifdef PULP_FPGA_EMUL
  xilinx_mem_32768x32
  sp_ram_i
  (
    .clka   ( clk                ),
    .rsta   ( 1'b0               ), // reset is active high

    .ena    ( en_i               ),
    .addra  ( addr_i             ),
    .dina   ( wdata_i            ),
    .douta  ( rdata_o            ),
    .wea    ( be_i & {4{we_i}}   )
  );

  // TODO: we should kill synthesis when the ram size is larger than 32768
  // words

`elsif ASIC

   // RAM bypass logic
   logic [31:0] ram_out_int;
   // assign rdata_o = (bypass_en_i) ? wdata_i : ram_out_int;
   assign rdata_o = ram_out_int;

   sp_ram_bank
   #(
    .NUM_BANKS  ( RAM_SIZE/1024 ),
    .BANK_SIZE  ( 1024          )
   )
   sp_ram_bank_i
   (
    .clk_i   ( clk                     ),
    .rstn_i  ( rstn_i                  ),
    .en_i    ( en_i                    ),
    .addr_i  ( addr_i                  ),
    .wdata_i ( wdata_i                 ),
    .rdata_o ( ram_out_int             ),
    .we_i    ( (we_i & ~bypass_en_i)   ),
    .be_i    ( be_i                    )
   );

`else
  sp_ram
  #(
    .ADDR_WIDTH ( ADDR_WIDTH ),
    .NUM_WORDS  ( RAM_SIZE   )
  )
  sp_ram_i
  (
    .clk     ( clk       ),

    .en_i    ( en_i      ),
    .addr_i  ( addr_i    ),
    .wdata_i ( wdata_i   ),
    .rdata_o ( rdata_o   ),
    .we_i    ( we_i      ),
    .be_i    ( be_i      )
  );
`endif

endmodule

