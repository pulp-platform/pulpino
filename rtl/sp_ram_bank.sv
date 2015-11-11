`define UMC65_SRAM_BANK_INSTANCE \
mem_bank_i \
( \
  .CLK       ( clk_i       ), \
  .CSN       ( csn[j]      ), \
  .BEN       ( ben[j]      ), \
  .WEN       ( wen[j]      ), \
  .A         ( add[j]      ), \
  .D         ( wdata[j]    ), \
  .Q         ( rdata[j]    )  \
)

module sp_ram_bank
  #(
    parameter ADDR_WIDTH = 17,
    parameter NUM_WORDS  = 32768,
    parameter NUM_BANKS  = 2,
    parameter BANK_SIZE  = 4096
  )(
    // Clock and Reset
    input  logic  							clk_i,
    input  logic 							rstn_i,
    input  logic                   			en_i,
    input  logic [ADDR_WIDTH-1:0]   	    addr_i,
    input  logic [31:0]            			wdata_i,
    output logic [31:0]            			rdata_o,
    input  logic                   			we_i,
    input  logic [3:0]             			be_i
  );

  	logic [$clog2(BANK_SIZE)-1:0][NUM_BANKS:0] add;
	logic [NUM_BANKS:0][31:0]                  wdata;
    logic                         			   csn;
    logic                         			   wen;
    logic [NUM_BANKS:0][3:0]                   ben;
    logic [NUM_BANKS:0][31:0]                  rdata;

    logic [$clog2(NUM_BANKS)-1:0] addr_q;
    
    logic bank_sel;

	// RAM select
	// Address
	// selector bit 

	genvar j;
	generate
	    for (j = 0; j < NUM_BANKS; j = j + 1) 
	    begin: sram_bank_gen

            assign bank_sel = (add[$clog2(NUM_BANKS)+$clog2(BANK_SIZE)-1:$clog2(BANK_SIZE)] == j);
	    	assign add[j]   = addr_i[$clog2(BANK_SIZE)-1:0];
	    	assign wdata[j] = wdata;
	    	assign csn[j]   = bank_sel ? (~en_i) : 1'b1;
	    	assign wen[j]   = (~we_i);
	    	assign ben[j]   = (~be_i);

 			case(BANK_SIZE)
	    		1024:
	    		begin
	    			shka65_1024x32x1
	    			`UMC65_SRAM_BANK_INSTANCE;
	    		end
	    		4096:
	    		begin
	    			shka65_4096x16x2
	    			`UMC65_SRAM_BANK_INSTANCE;
	    		end
	    		8192:
	    		begin
	    			shka65_8192x32x1
	    			`UMC65_SRAM_BANK_INSTANCE;
	    		end
	    		default:
	    		begin

	    		end
	    	endcase
	    end
	endgenerate

	always_ff @ (posedge clk_i, negedge rstn_i)
  	begin
	    if (~rstn_i)
	      addr_q <= 'b0;
	    else
	      addr_q <= addr_i[$clog2(NUM_BANKS)+$clog2(BANK_SIZE)-1:$clog2(BANK_SIZE)];
  	end

   // RAM out MUX
   assign rdata_o = rdata[addr_q];

endmodule