/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */
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
