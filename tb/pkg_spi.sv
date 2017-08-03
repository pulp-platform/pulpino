// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
typedef struct packed {
    logic [31:0] addr;
    logic [31:0] data;
} spi_stim;

class spi_master;
  /* SPI interface */
  local virtual spi_bus_master spi_i;

  local logic use_quad = 0;

  function new (virtual spi_bus_master s);
    this.spi_i = s;
  endfunction

  function void set_quad_spi(logic q);
    this.use_quad = q;
  endfunction

  function logic get_quad_spi();
    return this.use_quad;
  endfunction

  task send_cmd(input logic [7:0] cmd, input logic [31:0] addr);
    spi_i.set_csn(1'b0);

    spi_i.send(this.use_quad, {>>{cmd}});
    spi_i.send(this.use_quad, {>>{addr}});

    spi_i.set_csn(1'b1);
  endtask

  task send_data(input logic [31:0] data);
    spi_i.set_csn(1'b0);
    spi_i.send(this.use_quad, {>>{data}});
    spi_i.set_csn(1'b1);
  endtask

  task write_reg(input logic [7:0] cmd, input logic [7:0] reg_value);
    spi_i.set_csn(1'b0);

    spi_i.send(this.use_quad, {>>{cmd}});
    spi_i.send(this.use_quad, {>>{reg_value}});

    spi_i.set_csn(1'b1);
  endtask

  task read_word(input logic [7:0] cmd, input logic [31:0] addr, output logic [31:0] data);
    spi_i.set_csn(1'b0);

    this.send_cmd(cmd, addr);
    spi_i.receive(this.use_quad, {>>{data}});

    spi_i.set_csn(1'b1);
  endtask

  task send_file(input string filename);
    spi_stim stimuli [];
    $readmemh(filename, stimuli);

    spi_i.set_csn(1'b0);

    this.send_cmd(8'h2, stimuli[0].addr);

    for (int i = 0; i < stimuli.size(); i++)
    begin
      this.send_data(stimuli[i].data);
      if (stimuli[i].addr != stimuli[i+1].addr - 32'h4)
      begin
        spi_i.set_csn(1'b1);
        spi_i.set_csn(1'b0);
        this.send_cmd(8'h2, stimuli[i].addr);
      end
    end

    spi_i.set_csn(1'b1);
  endtask

  task spi_check(input string filename);

  endtask
endclass
