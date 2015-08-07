////////////////////////////////////////////////////////////////////////////////
// Company:        Multitherman Laboratory @ DEIS - University of Bologna     //
//                    Viale Risorgimento 2 40136                              //
//                    Bologna - fax 0512093785 -                              //
//                                                                            //
// Engineer:       Davide Rossi - davide.rossi@unibo.it                       //
//                                                                            //
// Additional contributions by:                                               //
//                                                                            //
//                                                                            //
//                                                                            //
// Create Date:    13/02/2013                                                 //
// Design Name:    ULPSoC                                                     //
// Module Name:    ulpcluster_top                                             //
// Project Name:   ULPSoC                                                     //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    ULPSoC cluster                                             //
//                                                                            //
//                                                                            //
// Revision:                                                                  //
// Revision v0.1 - File Created                                               //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

`include "axi_bus.sv"

module axi2apb_wrap
#(
    parameter AXI_ADDR_WIDTH = 32,
    parameter AXI_DATA_WIDTH = 64,
    parameter AXI_USER_WIDTH = 6,
    parameter AXI_ID_WIDTH   = 6,
    parameter APB_ADDR_WIDTH = 12,
    parameter APB_NUM_SLAVES = 8
)
(
    input logic	         clk_i,
    input logic	         rst_ni,

    AXI_BUS.Slave        axi_slave,

    output logic                                penable,
    output logic                                pwrite,
    output logic        [APB_ADDR_WIDTH-1:0]    paddr,
    output logic        [APB_NUM_SLAVES-1:0]    psel,
    output logic                      [31:0]    pwdata,
    input  logic [APB_NUM_SLAVES-1:0] [31:0]    prdata,
    input  logic        [APB_NUM_SLAVES-1:0]    pready,
    input  logic        [APB_NUM_SLAVES-1:0]    pslverr
);


   //********************************************************
   //************** AXI2MEM WRAPER **************************
   //********************************************************
    axi2apb
      #(
        .AXI_ADDR_WIDTH(AXI_ADDR_WIDTH),
        .AXI_DATA_WIDTH(AXI_DATA_WIDTH),
        .AXI_ID_WIDTH(AXI_ID_WIDTH),
        .APB_NUM_SLAVES(APB_NUM_SLAVES),
        .APB_ADDR_WIDTH(APB_ADDR_WIDTH)
        )
    axi2apb_i
      (
       .clk(clk_i),
       .rstn(rst_ni),

       .AWID(axi_slave.aw_id),
       .AWADDR(axi_slave.aw_addr),
       .AWLEN(axi_slave.aw_len),
       .AWSIZE(axi_slave.aw_size),
       .AWVALID(axi_slave.aw_valid),
       .AWREADY(axi_slave.aw_ready),
       .WDATA(axi_slave.w_data),
       .WSTRB(axi_slave.w_strb),
       .WLAST(axi_slave.w_last),
       .WVALID(axi_slave.w_valid),
       .WREADY(axi_slave.w_ready),
       .BID(axi_slave.b_id),
       .BRESP(axi_slave.b_resp),
       .BVALID(axi_slave.b_valid),
       .BREADY(axi_slave.b_ready),
       .ARID(axi_slave.ar_id),
       .ARADDR(axi_slave.ar_addr),
       .ARLEN(axi_slave.ar_len),
       .ARSIZE(axi_slave.ar_size),
       .ARVALID(axi_slave.ar_valid),
       .ARREADY(axi_slave.ar_ready),
       .RID(axi_slave.r_id),
       .RDATA(axi_slave.r_data),
       .RRESP(axi_slave.r_resp),
       .RLAST(axi_slave.r_last),
       .RVALID(axi_slave.r_valid),
       .RREADY(axi_slave.r_ready),
       .penable(penable),
       .pwrite(pwrite),
       .paddr(paddr),
       .psel(psel),
       .pwdata(pwdata),
       .prdata(prdata),
       .pready(pready),
       .pslverr(pslverr)
      );

endmodule
