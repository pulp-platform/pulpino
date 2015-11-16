
module ram_mux
  #(
    parameter ADDR_WIDTH = 32,
    parameter DATA_WIDTH = 32
  )(
    // Clock and Reset
    input logic clk,
    input logic rst_n,

    // port0 has priority
    input  logic                    port0_req_i,
    output logic                    port0_gnt_o,
    output logic                    port0_rvalid_o,
    input  logic [ADDR_WIDTH-1:0]   port0_addr_i,
    input  logic                    port0_we_i,
    input  logic [DATA_WIDTH/8-1:0] port0_be_i,
    output logic [DATA_WIDTH-1:0]   port0_rdata_o,
    input  logic [DATA_WIDTH-1:0]   port0_wdata_i,

    input  logic                    port1_req_i,
    output logic                    port1_gnt_o,
    output logic                    port1_rvalid_o,
    input  logic [ADDR_WIDTH-1:0]   port1_addr_i,
    input  logic                    port1_we_i,
    input  logic [DATA_WIDTH/8-1:0] port1_be_i,
    output logic [DATA_WIDTH-1:0]   port1_rdata_o,
    input  logic [DATA_WIDTH-1:0]   port1_wdata_i,

    // to RAM
    output logic                    ram_en_o,
    output logic [ADDR_WIDTH-1:0]   ram_addr_o,
    output logic                    ram_we_o,
    output logic [DATA_WIDTH/8-1:0] ram_be_o,
    input  logic [DATA_WIDTH-1:0]   ram_rdata_i,
    output logic [DATA_WIDTH-1:0]   ram_wdata_o
  );

  // RAM mux for data port of core and AXI interface
  always_comb
  begin
    port0_gnt_o = 1'b0;
    port1_gnt_o = 1'b0;

    // port0 has priority
    if(port0_req_i)
      port0_gnt_o   = 1'b1;
    else if(port1_req_i)
      port1_gnt_o   = 1'b1;
  end

  assign ram_en_o    = port0_req_i | port1_req_i;
  assign ram_addr_o  = port0_req_i ? port0_addr_i  : port1_addr_i;
  assign ram_wdata_o = port0_req_i ? port0_wdata_i : port1_wdata_i;
  assign ram_we_o    = port0_req_i ? port0_we_i    : port1_we_i;
  assign ram_be_o    = port0_req_i ? port0_be_i    : port1_be_i;

  assign port0_rdata_o = ram_rdata_i;
  assign port1_rdata_o = ram_rdata_i;

  // generate rvalid signals from gnt signals
  always_ff @(posedge clk, negedge rst_n)
  begin
    if (rst_n == 1'b0)
    begin
      port0_rvalid_o <= 1'b0;
      port1_rvalid_o <= 1'b0;
    end
    else
    begin
      port0_rvalid_o <= port0_gnt_o;
      port1_rvalid_o <= port1_gnt_o;
    end
  end

endmodule
