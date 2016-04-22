typedef struct {
  byte we;
  byte be;
  int  addr;
  int  wdata;
  int  rdata;
} mem_packet_t;

import "DPI-C"         function void mem_init(input int port);
import "DPI-C"         function int  mem_poll(output mem_packet_t packet);
import "DPI-C"         function void mem_push(input mem_packet_t packet);

task mem_dpi;
  input int port;
  mem_packet_t packet;
  int i;
  begin
    mem_init(port);

    while(1) begin
      // wait for 100 cycles between polls
      for (i = 0; i < 100; i++)
        @(posedge s_clk);

      if (mem_poll(packet)) begin
        // we got a valid packet
        // let's perform an SPI transaction and send back the result
        if (packet.we)
          spi_write_word(1, packet.addr, packet.wdata);
        else
          spi_read_word(1, 8'hB, packet.addr, packet.rdata);

        mem_push(packet);
      end
    end
  end
endtask
