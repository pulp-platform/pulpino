// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

interface spi_slave
  #(
    parameter period = 50ns
  );

  timeunit      1ns;
  timeprecision 1ps;

  localparam SPI_STD     = 2'b00;
  localparam SPI_QUAD_TX = 2'b01;
  localparam SPI_QUAD_RX = 2'b10;

  logic       clk;
  logic [3:0] sdo;
  logic [3:0] sdi;
  logic       csn;
  logic [1:0] padmode;

  //---------------------------------------------------------------------------
  // Slave Tasks
  //---------------------------------------------------------------------------
  task wait_csn(logic csn_in);
    if (csn_in) begin
      if (~csn)
        wait(csn);
    end else begin
      if (csn)
        wait(~csn);
    end
  endtask

  task send(input logic use_quad, input logic data[]);
    for (int i = data.size()-1; i >= 0; i--)
    begin
      sdi[0] = data[i];
      clock(1);
    end
  endtask

  task clock(input int cycles);
    for(int i = 0; i < cycles; i++)
    begin
      if (clk) begin
        wait (~clk);
      end

      wait (clk);
    end
  endtask
endinterface
