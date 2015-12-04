
`include "config.sv"

module boot_rom_wrap
  (
    // Clock and Reset
    input  logic clk,
    input  logic rstn_i,
    input  logic                         en_i,
    input  logic [`ROM_ADDR_WIDTH-1:0]  addr_i,
    output logic [31:0]                 rdata_o
  );

  boot_code
  boot_code_i
  (
    .CLK   ( clk     ),
    .RSTN  ( rstn_i  ),

    .CSN   ( ~en_i   ),
    .A     ( addr_i  ),
    .Q     ( rdata_o )
  );
endmodule
