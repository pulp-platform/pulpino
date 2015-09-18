module clk_rst_gen
(
    input         ref_clk_i,
    input         rst_ni,

    input         clking_axi_aclk,
    input         clking_axi_aresetn,
    input  [10:0] clking_axi_awaddr,
    input         clking_axi_awvalid,
    output        clking_axi_awready,
    input  [31:0] clking_axi_wdata,
    input   [3:0] clking_axi_wstrb,
    input         clking_axi_wvalid,
    output        clking_axi_wready,
    output  [1:0] clking_axi_bresp,
    output        clking_axi_bvalid,
    input         clking_axi_bready,
    input  [10:0] clking_axi_araddr,
    input         clking_axi_arvalid,
    output        clking_axi_arready,
    output [31:0] clking_axi_rdata,
    output  [1:0] clking_axi_rresp,
    output        clking_axi_rvalid,
    input         clking_axi_rready,

    output        rstn_pulpino_o,
    output        clk_pulpino_o
);

    wire s_clk_int;

    xilinx_clock_manager clk_manager_i
    (
        .s_axi_aclk    ( clking_axi_aclk    ),
        .s_axi_aresetn ( clking_axi_aresetn ),
        .s_axi_awaddr  ( clking_axi_awaddr  ),
        .s_axi_awvalid ( clking_axi_awvalid ),
        .s_axi_awready ( clking_axi_awready ),
        .s_axi_wdata   ( clking_axi_wdata   ),
        .s_axi_wstrb   ( clking_axi_wstrb   ),
        .s_axi_wvalid  ( clking_axi_wvalid  ),
        .s_axi_wready  ( clking_axi_wready  ),
        .s_axi_bresp   ( clking_axi_bresp   ),
        .s_axi_bvalid  ( clking_axi_bvalid  ),
        .s_axi_bready  ( clking_axi_bready  ),
        .s_axi_araddr  ( clking_axi_araddr  ),
        .s_axi_arvalid ( clking_axi_arvalid ),
        .s_axi_arready ( clking_axi_arready ),
        .s_axi_rdata   ( clking_axi_rdata   ),
        .s_axi_rresp   ( clking_axi_rresp   ),
        .s_axi_rvalid  ( clking_axi_rvalid  ),
        .s_axi_rready  ( clking_axi_rready  ),
        .clk50_i       ( ref_clk_i          ),
        .clk_o         ( s_clk_int          ),
        .rst_no        (                    )
    );

    assign rstn_pulpino_o          = rst_ni;
    assign clk_pulpino_o           = s_clk_int;

endmodule
