module sp_ram_bank
  #(
    parameter ADDR_WIDTH = 17,
    parameter NUM_WORDS  = 32768,
    parameter NUM_BANKS = 4,
  )(
    // Clock and Reset
    input  logic clk,

    input  logic                   en_i,
    input  logic [ADDR_WIDTH-1:0]  addr_i,
    input  logic [31:0]            wdata_i,
    output logic [31:0]            rdata_o,
    input  logic                   we_i,
    input  logic [3:0]             be_i,
  );

	logic [NUM_BANKS -1:0] ram_out_int;

	// RAM select
	genvar j;
	generate
    for (j = 0; j < NUM_BANKS; j = j + 1) 
    begin: ram_block
        SHKA65_4096X8X2CM16
	    ram_i
	    (
	    	.DO   ( ram_out_int[j]	 ),
	    	.A    ( addr_i ),
	      	.DI   ( wdata_i   	 ),
	      	.WEB  (   			 ),
	        .DVSE ( 1'b0  ), 
	        .DVS  ( 3'b0  ),
	      	.CK   ( clk   ), 
	      	.CSB  (       )
	    );
    end
endgenerate


   // RAM out MUX

endmodule