`include "config.sv"

module sp_ram_wrap
  #(
    parameter ADDR_WIDTH = 8
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

`ifdef PULP_FPGA_EMUL
  xilinx_mem_32768x32
  sp_ram_i
  (
    .clk   ( clk                ),
    .rst   ( 1'b1               ),

    .en    ( en_i               ),
    .addr  ( addr_i             ),
    .din   ( wdata_i            ),
    .dout  ( rdata_o            ),
    .we    ( be_i & {4{we_i}}   )
    );
`else
  sp_ram
  #(
    .ADDR_WIDTH ( ADDR_WIDTH )
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

