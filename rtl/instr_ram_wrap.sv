
`include "config.sv"

module instr_ram_wrap
  #(
    parameter RAM_SIZE   = 32768,               // in words
    parameter ADDR_WIDTH = $clog2(RAM_SIZE) + 1 // one bit more than necessary, for the boot rom
  )(
    // Clock and Reset
    input  logic clk,
    input  logic rst_n,

    input  logic                   en_i,
    input  logic [ADDR_WIDTH-1:0]  addr_i,
    input  logic [31:0]            wdata_i,
    output logic [31:0]            rdata_o,
    input  logic                   we_i,
    input  logic [3:0]             be_i,
    input  logic                   bypass_en_i
  );

  logic is_boot, is_boot_q;
  logic [31:0] rdata_boot;
  logic [31:0] rdata_ram;


  assign is_boot = (addr_i[ADDR_WIDTH-1] == 1'b1);


  sp_ram_wrap
  #(
    .RAM_SIZE ( RAM_SIZE  )
  )
  sp_ram_wrap_i
  (
    .clk         ( clk                        ),
    .rstn_i      ( rst_n                      ),

    .en_i        ( en_i & (~is_boot)          ),
    .addr_i      ( addr_i[ADDR_WIDTH-2:0]     ),
    .wdata_i     ( wdata_i                    ),
    .rdata_o     ( rdata_ram                  ),
    .we_i        ( we_i                       ),
    .be_i        ( be_i                       ),
    .bypass_en_i ( bypass_en_i                )
  );

  boot_rom_wrap
  boot_rom_wrap_i
  (
    .clk     ( clk                         ),

    .en_i    ( en_i & is_boot              ),
    .addr_i  ( addr_i[`ROM_ADDR_WIDTH-1:0] ),
    .rdata_o ( rdata_boot                  )
  );


  assign rdata_o = (is_boot_q == 1'b1) ? rdata_boot : rdata_ram;


  // Delay the boot signal for one clock cycle to correctly select the rdata
  // from boot rom vs normal ram
  always_ff @(posedge clk, negedge rst_n)
  begin
    if (rst_n == 1'b0)
      is_boot_q <= 1'b0;
    else
      is_boot_q <= is_boot;
  end

endmodule
