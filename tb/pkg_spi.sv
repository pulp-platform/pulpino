typedef struct packed {
    logic [31:0] addr;
    logic [31:0] data;
} spi_stim;

interface SPI
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

  modport Slave (
      input clk,
      output sdo,
      input sdi,
      input csn,
      output padmode
  );
  modport Master (
      output clk,
      output sdo,
      input sdi,
      output csn,
      output padmode
  );

  //---------------------------------------------------------------------------
  // Slave Tasks
  //---------------------------------------------------------------------------
  task s_wait_csn(logic csn_in);
    if (csn_in) begin
      if (~csn)
        wait(csn);
    end else begin
      if (csn)
        wait(~csn);
    end
  endtask

  task s_send(input logic use_quad, input logic data[]);
    for (int i = data.size()-1; i >= 0; i--)
    begin
      sdi[0] = data[i];
      s_clock(1);
    end
  endtask

  task s_clock(input int cycles);
    for(int i = 0; i < cycles; i++)
    begin
      if (clk) begin
        wait (~clk);
      end

      wait (clk);
    end
  endtask

  //---------------------------------------------------------------------------
  // Master Tasks
  //---------------------------------------------------------------------------
  task m_set_csn(logic csn_in);
    #(period/2);
    csn = csn_in;
    #(period/2);
  endtask

  task m_send(input logic use_quad, input logic data[]);
    if (use_quad)
    begin
      padmode = SPI_QUAD_TX;
      for (int i = data.size()/4; i > 0; i--)
      begin
        for (int j = 3; j >= 0; j-- )
          sdo[j] = data[4*i-j+1];

        m_clock(1);
      end
    end else begin
      padmode = SPI_STD;
      for (int i = data.size(); i > 0; i--)
      begin
        sdo[0] = data[i];
        m_clock(1);
      end
    end
  endtask

  task m_receive(input logic use_quad, output logic data[]);
    if (use_quad)
    begin
      padmode = SPI_QUAD_RX;
      for (int i = data.size()/4; i > 0; i--)
      begin
        for (int j = 3; j >= 0; j-- )
          data[4*i-j+1] = sdi[j];

        m_clock(1);
      end
    end else begin
      padmode = SPI_STD;
      for (int i = data.size(); i > 0; i--)
      begin
        data[i] = sdi[0];
        m_clock(1);
      end
    end
  endtask

  task m_clock(input int cycles);
    for(int i = 0; i < cycles; i++)
    begin
      #(period/2) clk = 1;
      #(period/2) clk = 0;
    end
  endtask
endinterface

class spi_master;
  /* SPI interface */
  local virtual SPI spi_i;

  local logic use_quad = 0;

  function new (virtual SPI s);
    this.spi_i = s;
  endfunction

  function void set_quad_spi(logic q);
    this.use_quad = q;
  endfunction

  function logic get_quad_spi();
    return this.use_quad;
  endfunction

  task send_cmd(input logic [7:0] cmd, input logic [31:0] addr);
    spi_i.m_set_csn(1'b0);

    spi_i.m_send(this.use_quad, {>>{cmd}});
    spi_i.m_send(this.use_quad, {>>{addr}});

    spi_i.m_set_csn(1'b1);
  endtask

  task send_data(input logic [31:0] data);
    spi_i.m_set_csn(1'b0);
    spi_i.m_send(this.use_quad, {>>{data}});
    spi_i.m_set_csn(1'b1);
  endtask

  task write_reg(input logic [7:0] cmd, input logic [7:0] reg_value);
    spi_i.m_set_csn(1'b0);

    spi_i.m_send(this.use_quad, {>>{cmd}});
    spi_i.m_send(this.use_quad, {>>{reg_value}});

    spi_i.m_set_csn(1'b1);
  endtask

  task read_word(input logic [7:0] cmd, input logic [31:0] addr, output logic [31:0] data);
    spi_i.m_set_csn(1'b0);

    this.send_cmd(cmd, addr);
    spi_i.m_receive(this.use_quad, {>>{data}});

    spi_i.m_set_csn(1'b1);
  endtask

  task send_file(input string filename);
    spi_stim stimuli [];
    $readmemh(filename, stimuli);

    spi_i.m_set_csn(1'b0);

    this.send_cmd(8'h2, stimuli[0].addr);

    for (int i = 0; i < stimuli.size(); i++)
    begin
      this.send_data(stimuli[i].data);
      if (stimuli[i].addr != stimuli[i+1].addr - 32'h4)
      begin
        spi_i.m_set_csn(1'b1);
        spi_i.m_set_csn(1'b0);
        this.send_cmd(8'h2, stimuli[i].addr);
      end
    end

    spi_i.m_set_csn(1'b1);
  endtask

  task spi_check(input string filename);

  endtask
endclass
