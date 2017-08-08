// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


package pkg_i2c_model;
  localparam SEND    = 2'b01;
  localparam RECV    = 2'b10;
  localparam PASSIVE = 2'b00;
  localparam WAIT    = 2'b11;
endpackage

module i2c_buf
  (
    inout  logic        scl_io,
    inout  logic        sda_io,

    output logic        scl_pad_i,
    input logic         scl_pad_o,
    input logic         scl_padoen_o,
    output logic        sda_pad_i,
    input logic         sda_pad_o,
    input logic         sda_padoen_o

  );

  assign sda_io = ~sda_padoen_o ? sda_pad_o : 1'bZ;
  assign sda_pad_i = sda_io;

  assign scl_io = ~scl_padoen_o ? scl_pad_o : 1'bZ;
  assign scl_pad_i = scl_io;
endmodule

module i2c_model_phy
  (
    inout  logic       scl_io,
    inout  logic       sda_io,

    input  logic       rst_ni,

    input  logic [1:0] mode_i,
    output logic       done_o,
    input  logic       send_ack_i,
    output logic       start_o,
    output logic       stop_o,

    input  logic [7:0] data_i,
    output logic [7:0] data_o
  );

  enum logic [2:0] { IDLE, RECV_START, RECV_DATA, RECV_ACK, SEND_DATA, SEND_ACK, WAIT_STOP } CS, NS;
  logic [3:0] counter_Q, counter_N;
  logic [7:0] shift_Q;
  logic       load;

  logic sda_in;
  logic sda_oe, sda_oe_n;

  logic scl_in;
  logic scl_oe;

  //---------------------------------------------------------------------------
  // PADs for SCL and SDA
  //---------------------------------------------------------------------------
  assign sda_io = sda_oe ? 1'b0 : 1'bZ;
  assign sda_in = sda_io;

  assign scl_io = scl_oe ? 1'b0 : 1'bZ;
  assign scl_in = scl_io;

  //---------------------------------------------------------------------------
  // state machine for receiving/sending of bytes
  //---------------------------------------------------------------------------
  always_comb
  begin
    sda_oe_n  = 1'b0;
    scl_oe    = 1'b0;

    start_o   = 1'b0;
    done_o    = 1'b0;
    NS        = CS;
    counter_N = counter_Q;

    load      = 1'b0;

    case (CS)
      IDLE: begin
        if (~sda_in) begin
          start_o = 1'b1;
          NS       = RECV_START;
        end
      end

      RECV_START: begin
        start_o   = 1'b1;
        counter_N = 4'd1;
        NS        = RECV_DATA;
      end

      RECV_DATA: begin
        counter_N = counter_Q + 4'd1;

        if (counter_Q == 4'd7) begin
          NS   = RECV_ACK;
        end
      end

      RECV_ACK: begin
        done_o    = 1'b1;
        counter_N = 4'd0;

        if (send_ack_i) begin
          sda_oe_n = 1'b1;

          case (mode_i)
            pkg_i2c_model::SEND:    begin NS = SEND_DATA; load = 1'b1; end
            pkg_i2c_model::RECV:    begin NS = RECV_DATA; end
            pkg_i2c_model::PASSIVE: begin NS = WAIT_STOP; end
          endcase
        end else begin
          NS = WAIT_STOP;
        end
      end

      SEND_DATA: begin
        counter_N = counter_Q + 4'd1;
        sda_oe_n  = ~shift_Q[7];

        if (counter_Q == 4'd7) begin
          NS   = SEND_ACK;
        end
      end

      SEND_ACK: begin
        counter_N = 4'd0;
        load      = 1'b1;
        done_o    = 1'b1;

        case (mode_i)
          pkg_i2c_model::SEND:    NS = SEND_DATA;
          pkg_i2c_model::RECV:    NS = RECV_DATA;
          pkg_i2c_model::PASSIVE: NS = WAIT_STOP;
        endcase
      end

      // TODO
      WAIT_STOP: begin
        sda_oe_n = 1'b0;
      end
    endcase
  end

  // stop condition detection
  // simulation hack, not synthesizable
  // detects low->high when scl is high
  initial
  begin
    while(1) begin
      stop_o = 1'b0;
      @(posedge scl_in);
      #5ns;

      if (sda_in)
        continue; // alreay high

      // try to detect stop as long as scl is high
      #5ns;
      while(scl_in) begin
        if (sda_in) begin
          stop_o = 1'b1;
          #5ns;
          break;
        end
        #5ns;
      end
    end
  end

  //---------------------------------------------------------------------------
  // DDR registers for FSM
  //---------------------------------------------------------------------------
  always_ff @(posedge scl_in, negedge scl_in, negedge rst_ni, posedge stop_o)
  begin
    if (~rst_ni || stop_o) begin
      CS = IDLE;
    end else begin
      if (~scl_in) begin
        // falling edge
        if (start_o)
          CS = NS;
      end else if (scl_in) begin
        // rising edge
        CS = NS;
      end
    end
  end


  //---------------------------------------------------------------------------
  // single-edge triggered registers for data
  //---------------------------------------------------------------------------
  always_ff @(posedge scl_in, negedge rst_ni)
  begin
    if (~rst_ni)
      shift_Q <= '0;
    else
      if (load)
        shift_Q <= data_i;
      else
        shift_Q <= {shift_Q[6:0], sda_in};
  end

  assign data_o = shift_Q;

  //---------------------------------------------------------------------------
  // single-edge triggered register for output
  always_ff @(negedge scl_in, negedge rst_ni)
  begin
    if (~rst_ni)
      sda_oe = 1'b0;
    else
      sda_oe = sda_oe_n;
  end

  //---------------------------------------------------------------------------
  // single-edge triggered registers for counter
  always_ff @(posedge scl_in, negedge rst_ni)
  begin
    if (~rst_ni)
      counter_Q <= '0;
    else
      counter_Q <= counter_N;
  end

endmodule

module i2c_eeprom_model
  #(
    parameter ADDRESS = 7'b010_1010
  )
  (
    inout  logic scl_io,
    inout  logic sda_io,
    input  logic rst_ni  // ideally this would be an internal POR
  );

  enum logic [2:0] { I2C_ADDR, WRITE_ADDR_HI, WRITE_ADDR_LO, WRITE_DATA, READ_DATA, NOT_SELECTED } CS, NS;

  logic [1:0] phy_mode;
  logic       phy_done;
  logic       phy_start;
  logic       phy_stop;
  logic       phy_send_ack;

  logic [7:0] phy_data_in;
  logic [7:0] phy_data_out;

  logic [65535:0] [7:0] mem_Q, mem_N;
  logic                 mem_we;
  logic [15:0]          addr_Q, addr_N;

  //---------------------------------------------------------------------------
  // PHY, handles sending and receiving of individual bits
  //---------------------------------------------------------------------------
  i2c_model_phy
  i_i2c_model_phy
  (
    .scl_io     ( scl_io       ),
    .sda_io     ( sda_io       ),

    .rst_ni     ( rst_ni       ),

    .mode_i     ( phy_mode     ),
    .done_o     ( phy_done     ),
    .send_ack_i ( phy_send_ack ),
    .start_o    ( phy_start    ),
    .stop_o     ( phy_stop     ),

    .data_i     ( phy_data_in  ),
    .data_o     ( phy_data_out )
  );

  //---------------------------------------------------------------------------
  // State machine, takes care of bytes and thus the I2C "higher layer"
  // protocol
  //---------------------------------------------------------------------------
  always_comb
  begin
    NS           = CS;
    addr_N       = addr_Q;
    phy_send_ack = 1'b0;
    phy_mode     = pkg_i2c_model::PASSIVE;

    mem_we       = 1'b0;

    case (CS)
      I2C_ADDR: begin
        phy_mode = pkg_i2c_model::RECV;
        if (phy_done) begin
          if (phy_data_out[7:1] == ADDRESS) begin
            phy_send_ack = 1'b1;

            if (phy_data_out[0]) begin
              phy_mode = pkg_i2c_model::SEND;
              NS = READ_DATA;
            end else begin
              phy_mode = pkg_i2c_model::RECV;
              NS = WRITE_ADDR_HI;
            end
          end else begin
              phy_mode = pkg_i2c_model::PASSIVE;
            NS = NOT_SELECTED;
          end
        end
      end

      WRITE_ADDR_HI: begin
        phy_mode     = pkg_i2c_model::RECV;
        phy_send_ack = 1'b1;
        addr_N[15:8] = phy_data_in;

        if (phy_done)
          NS = WRITE_ADDR_LO;
      end

      WRITE_ADDR_LO: begin
        phy_mode     = pkg_i2c_model::RECV;
        phy_send_ack = 1'b1;
        addr_N[ 7:0] = phy_data_in;

        if (phy_done)
          NS = WRITE_DATA;
      end

      WRITE_DATA: begin
        phy_mode = pkg_i2c_model::RECV;
        phy_send_ack = 1'b1;

        if (phy_done) begin
          addr_N = addr_Q + 16'd1;
          mem_we = 1'b1;
        end
      end

      READ_DATA: begin
        phy_mode = pkg_i2c_model::SEND;

        if (phy_done)
          addr_N = addr_Q + 16'd1;
      end

      NOT_SELECTED:; // stay here
      default:;
    endcase

    if (phy_start)
      NS = I2C_ADDR;
    else if (phy_stop)
      NS = NOT_SELECTED;
  end

  //---------------------------------------------------------------------------
  // registers for state machine
  //---------------------------------------------------------------------------
  always_ff @(posedge scl_io, negedge rst_ni)
  begin
    if (~rst_ni) begin
      CS     <= I2C_ADDR;
      addr_Q <= '0;
    end else begin
      CS     <= NS;
      addr_Q <= addr_N;
    end
  end


  //---------------------------------------------------------------------------
  // EEPROM memory
  //---------------------------------------------------------------------------
  always_ff @(posedge scl_io, negedge rst_ni)
  begin
    if (~rst_ni)
      mem_Q <= '{default: '0};
    else
      mem_Q <= mem_N;
  end

  always_comb
  begin
    mem_N = mem_Q;

    if (mem_we)
      mem_N[addr_Q] = phy_data_out;
  end

  assign phy_data_in = mem_Q[addr_N];

endmodule
