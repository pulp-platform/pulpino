
`include "config.sv"

module instr_ram_wrap
  #(
    parameter ADDR_WIDTH = 17,
    parameter NUM_WORDS  = 32768
  )(
    // Clock and Reset
    input  logic clk,

    input  logic                   en_i,
    input  logic [ADDR_WIDTH-1:0]  addr_i,
    input  logic [31:0]            wdata_i,
    output logic [31:0]            rdata_o,
    input  logic                   we_i,
    input  logic [3:0]             be_i
  );

  //localparam RAM_ADDR_WIDTH = `LOG2(NUM_WORDS);

  logic is_boot;
  logic [31:0] rdata_boot;
  logic [31:0] rdata_ram;


  assign is_boot = (addr_i[ADDR_WIDTH-1] == 1'b1);


  sp_ram_wrap
  #(
    .ADDR_WIDTH ( ADDR_WIDTH ),
    .NUM_WORDS  ( NUM_WORDS      )
    )
  sp_ram_wrap_i
  (
    .clk     ( clk                        ),

    .en_i    ( en_i & (~is_boot)          ),
    .addr_i  ( addr_i[ADDR_WIDTH-1:0] ),
    .wdata_i ( wdata_i                    ),
    .rdata_o ( rdata_ram                  ),
    .we_i    ( we_i                       ),
    .be_i    ( be_i                       )
    );

  boot_rom_wrap
  boot_rom_wrap_i
  (
    .clk     ( clk                         ),

    .en_i    ( en_i & is_boot              ),
    .addr_i  ( addr_i[`ROM_ADDR_WIDTH-1:0] ),
    .rdata_o ( rdata_boot                  )
    );

  assign rdata_o = (is_boot == 1'b1) ? rdata_boot : rdata_ram;

endmodule
