// Copyright 2015 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

`include "axi_bus.sv"

module core2axi
  #(
    parameter AXI_ADDR_WIDTH = 32
  )(
    // Clock and Reset
    input logic clk,
    input logic rst_n,

    input  logic                        data_req_i,
    output logic                        data_gnt_o,
    output logic                        data_rvalid_o,
    input  logic [AXI_ADDR_WIDTH-1:0]   data_addr_i,
    input  logic                        data_we_i,
    input  logic [3:0]                  data_be_i,
    output logic [31:0]                 data_rdata_o,
    input  logic [31:0]                 data_wdata_i,

    AXI_BUS.Master  master
  );


  enum logic [2:0] { IDLE, READ_WAIT, READ_ACK, WRITE_DATA, WRITE_WAIT } CS, NS;

  logic    gnt_q;

  logic    id, id_q;
  logic    done;


  always_comb
  begin
    NS   = CS;
    done = 1'b0;

    data_gnt_o = 1'b0;

    master.aw_valid = 1'b0;
    master.ar_valid = 1'b0;
    master.r_ready  = 1'b0;
    master.w_valid  = 1'b0;
    master.b_ready  = 1'b0;

    case (CS)
      IDLE:
      begin
        if (data_req_i)
        begin

          // send address over aw channel for writes,
          // over ar channels for reads
          if (data_we_i)
          begin
            master.aw_valid = 1'b1;

            if (master.aw_ready)
              NS = WRITE_DATA;
          end
          else
          begin
            master.ar_valid = 1'b1;

            if (master.ar_ready)
              NS = READ_WAIT;
          end
        end
      end

      WRITE_DATA:
      begin
        master.w_valid = 1'b1;

        if (master.w_ready)
        begin
          NS = WRITE_WAIT;
          data_gnt_o = 1'b1;
        end
      end

      WRITE_WAIT:
      begin
        master.b_ready = 1'b1;

        if (master.b_valid)
        begin
          done = 1'b1;
          NS = IDLE;
        end
      end

      READ_WAIT:
      begin
        if (master.r_valid)
        begin
          data_gnt_o = 1'b1;
          NS = READ_ACK;
        end
      end

      READ_ACK:
      begin
        done = 1'b1;
        master.r_ready = 1'b1;
        NS = IDLE;
      end

      default:
      begin
        NS = IDLE;
      end
    endcase
  end

  // registers
  always_ff @(posedge clk, negedge rst_n)
  begin
    if (rst_n == 1'b0)
    begin
      CS        <= IDLE;
      gnt_q     <= 1'b0;
      id_q      <= 'h0;
    end
    else
    begin
      CS        <= NS;
      gnt_q     <= data_gnt_o;
      id_q      <= id;
    end
  end


  assign master.aw_id     = id;
  assign master.aw_addr   = data_addr_i;
  assign master.aw_size   = 'h0;
  assign master.aw_len    = 'h0;
  assign master.aw_burst  = 'h0;
  assign master.aw_prot   = 'h0;
  assign master.aw_region = 'h0;
  assign master.aw_lock   = 'h0;
  assign master.aw_cache  = 'h0;
  assign master.aw_qos    = 'h0;
  assign master.aw_user   = 'h0;

  assign master.ar_id     = id;
  assign master.ar_addr   = data_addr_i;
  assign master.ar_size   = 3'b010;
  assign master.ar_len    = 4'b0000;
  assign master.ar_burst  = 2'b01;
  assign master.ar_prot   = 'h0;
  assign master.ar_region = 'h0;
  assign master.ar_lock   = 'h0;
  assign master.ar_cache  = 'h0;
  assign master.ar_qos    = 'h0;
  assign master.ar_user   = 'h0;

  assign master.w_last  = 'h1;
  assign master.w_user  = 'h0;
  assign master.w_strb  = data_be_i;
  assign master.w_data  = data_wdata_i;

  assign data_rdata_o   = master.r_data[31:0];

  assign data_rvalid_o  = gnt_q;


  assign id = (done) ? (~id_q) : id_q;

endmodule
