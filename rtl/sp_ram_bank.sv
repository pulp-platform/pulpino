`define UMC65_SRAM_BANK_INSTANCE \
mem_bank_i \
( \
  .CLK       ( clk_i       ), \
  .CSN       ( csn[i]      ), \
  .BEN       ( ben[i]      ), \
  .WEN       ( wen[i]      ), \
  .A         ( add[i]      ), \
  .D         ( wdata[i]    ), \
  .Q         ( rdata[i]    )  \
)

module sp_ram_bank
  #(
    parameter NUM_BANKS  = 2,
    parameter BANK_SIZE  = 4096, // in words
    parameter ADDR_WIDTH = $clog2(NUM_BANKS) + $clog2(BANK_SIZE)
  )(
    // Clock and Reset
    input  logic                   clk_i,
    input  logic                   rstn_i,
    input  logic                   en_i,
    input  logic [ADDR_WIDTH-1:0]  addr_i,
    input  logic [31:0]            wdata_i,
    output logic [31:0]            rdata_o,
    input  logic                   we_i,
    input  logic [3:0]             be_i
  );

  localparam N_WIRES  = NUM_BANKS - 1;
  localparam N_LEVELS = $clog2(NUM_BANKS);

  logic [NUM_BANKS-1:0][$clog2(BANK_SIZE)-1:0] add;
  logic [NUM_BANKS-1:0][31:0]                  wdata;
  logic [NUM_BANKS-1:0]                        csn;
  logic [NUM_BANKS-1:0]                        wen;
  logic [NUM_BANKS-1:0][ 3:0]                  ben;
  logic [NUM_BANKS-1:0][31:0]                  rdata;

  logic [$clog2(NUM_BANKS)-1:0]                addr_q;



  // RAM select
  // Address
  // selector bit

  genvar i;
  generate
    for (i = 0; i < NUM_BANKS; i++)
    begin: sram_bank_gen

      logic bank_sel;

      assign bank_sel = (addr_i[$clog2(NUM_BANKS)+$clog2(BANK_SIZE)-1:$clog2(BANK_SIZE)] == i);
      assign add[i]   = addr_i[$clog2(BANK_SIZE)-1:0];
      assign wdata[i] = wdata_i;
      assign csn[i]   = bank_sel ? (~en_i) : 1'b1;
      assign wen[i]   = (~we_i);
      assign ben[i]   = (~be_i);

      case(BANK_SIZE)
        1024: begin: bank
          umc65_1024x32x1
          `UMC65_SRAM_BANK_INSTANCE;
        end
        4096: begin: bank
          umc65_4096x16x2
          `UMC65_SRAM_BANK_INSTANCE;
        end
        8192: begin: bank
          umc65_8192x32x1
          `UMC65_SRAM_BANK_INSTANCE;
        end

        default:
        begin
          `ifndef SYNTHESIS
          $error("INVALID BANK SIZE FOUND: %d", BANK_SIZE);
          `endif
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
  // logarithmic tree

  genvar j, k;
  generate
    logic [N_WIRES-1:0] [31:0] rdata_levels;

    for (j = 0; j < N_LEVELS; j++)
    begin: rdata_mux

      for (k = 0; k < 2**j; k++) begin
        if (j == 0) begin
          // last node, drives the output
          mux_sel2 fan_in_i
          (
            .sel_i ( addr_q[N_LEVELS - j - 1]  ),
            .in_i  ( rdata_levels[2*k+1:2*k]   ),
            .out_o ( rdata_o                   )
          );
        end else if (j < N_LEVELS - 1) begin
          // middle nodes, just does muxing
          mux_sel2 fan_in_i
          (
            .sel_i ( addr_q[N_LEVELS - j - 1]                                ),
            .in_i  ( rdata_levels[((2**j)*2-2) + 2*k + 1:((2**j)*2-2) + 2*k] ),
            .out_o ( rdata_levels[((2**(j-1))*2-2) + k]                      )
          );
        end else begin
          // first node, takes the output of the rams, input into the tree
          mux_sel2 fan_in_i
          (
            .sel_i ( addr_q[N_LEVELS - j - 1]                                ),
            .in_i  ( rdata[2*k+1:2*k]                                        ),
            .out_o ( rdata_levels[((2**(j-1))*2-2) + k]                      )
          );
        end
      end
    end
  endgenerate

endmodule

module mux_sel2
(
    input  logic              sel_i,
    input  logic [1:0] [31:0] in_i,
    output logic       [31:0] out_o
  );

  assign out_o = sel_i ? in_i[1] : in_i[0];

endmodule
