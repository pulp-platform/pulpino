`include "axi_bus.sv"

module axi2mem
  #(
    parameter AXI_ADDR_WIDTH = 32,
    parameter AXI_ID_WIDTH   = 10
  )(
    // Clock and Reset
    input logic clk,
    input logic rst_n,

    output logic                        mem_req_o,
    output logic [AXI_ADDR_WIDTH-1:0]   mem_addr_o,
    output logic                        mem_we_o,
    output logic [3:0]                  mem_be_o,
    input  logic [31:0]                 mem_rdata_i,
    output logic [31:0]                 mem_wdata_o,

    AXI_BUS.Slave  slave
  );

  enum logic [1:0] { IDLE, READ, WRITE_DATA, WRITE_DONE } CS, NS;
  logic   [AXI_ADDR_WIDTH-1:0]  addr, addr_q;
  logic   [AXI_ID_WIDTH-1:0]    arid, arid_q;
  logic   [AXI_ID_WIDTH-1:0]    awid, awid_q;

  always_ff @(posedge clk, negedge rst_n)
  begin
    if (rst_n == 1'b0)
    begin
      CS     <= IDLE;
      addr_q <= 'h0;
    end
    else
    begin
      CS     <= NS;
      addr_q <= addr;
    end
  end

  always_comb
  begin
    NS             = CS;
    addr           = addr_q;
    awid           = awid_q;
    arid           = arid_q;

    slave.ar_ready = 1'b0;
    slave.aw_ready = 1'b0;
    slave.r_valid  = 1'b0;
    slave.r_id     = arid_q;
    slave.r_resp   = `OKAY;
    slave.w_ready  = 1'b0;
    slave.b_id     = awid_q;
    slave.b_resp   = `OKAY;
    slave.b_valid  = 1'b0;

    mem_req_o      = 1'b0;
    mem_addr_o     = addr_q;
    mem_we_o       = 1'b0;

    case (CS)
      IDLE:
      begin
        if (slave.ar_valid)
        begin
          NS   = READ;
          addr = slave.ar_addr;
          arid = slave.ar_id;

          slave.ar_ready = 1'b1;

          mem_req_o  = 1'b1;
          mem_addr_o = slave.ar_addr;
        end
        else if(slave.aw_valid)
        begin
          NS  = WRITE_DATA;
          addr = slave.aw_addr;
          awid = slave.aw_id;

          slave.aw_ready = 1'b1;
        end
      end

      READ:
      begin
        slave.r_valid = 1'b1;
        slave.r_id    = arid_q;
        slave.r_resp  = `OKAY;

        if (slave.r_ready)
          NS = IDLE;
      end

      WRITE_DATA:
      begin
        mem_we_o = 1'b1;

        slave.w_ready = 1'b1;

        if (slave.w_valid)
        begin
          NS = WRITE_DONE;

          mem_req_o = 1'b1;
        end
      end

      WRITE_DONE:
      begin
        slave.b_id    = awid_q;
        slave.b_resp  = `OKAY;
        slave.b_valid = 1'b1;

        if(slave.b_ready)
          NS = IDLE;
      end

    endcase
  end

  assign slave.r_data = mem_rdata_i;
  assign mem_wdata_o  = slave.w_data;
  assign mem_be_o     = slave.w_strb;

endmodule
