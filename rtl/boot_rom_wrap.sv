
`include "config.sv"

module boot_rom_wrap
  (
    // Clock and Reset
    input  logic clk,

    input  logic                         en_i,
    input  logic [`ROM_ADDR_WIDTH-1:0]  addr_i,
    output logic [31:0]                 rdata_o
  );

  localparam words = 2**`ROM_ADDR_WIDTH;

  logic [31:0] mem[words];

  assign rdata_o = mem[addr_i];

endmodule
