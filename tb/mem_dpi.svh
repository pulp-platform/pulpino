typedef struct {
  byte we;
  int  addr;
  int  size;
} mem_packet_t;

import "DPI-C"         function void mem_init(input int port);
import "DPI-C"         function int  mem_poll(output mem_packet_t packet, output byte buffer[1024]);
import "DPI-C"         function int  mem_push(input mem_packet_t packet, input byte buffer[1024]);

task mem_dpi;
  input int port;
  byte buffer[1024];
  mem_packet_t packet;
  int i;
  int j;
  int local_addr;
  int local_size;
  logic [31:0] rdata_temp;
  logic [31:0] rdata_temp_arr[];
  begin
    mem_init(port);

    while(1) begin
      // wait for 100 cycles between polls
      for (i = 0; i < 100; i++)
        @(posedge s_clk);

      if (mem_poll(packet, buffer) == 0) begin
        // we got a valid packet
        // let's perform the SPI transactions and send back the result
        local_addr = packet.addr;
        local_size = packet.size;
        i = 0;

        if (packet.we) begin
          // write

          // first align addresses to words
          if (local_addr[0]) begin
            spi_write_byte(use_qspi, local_addr, buffer[i]);
            i          += 1;
            local_addr += 1;
            local_size -= 1;
          end

          if (local_addr[1] && local_size >= 2) begin
            spi_write_halfword(use_qspi, local_addr, {buffer[i+1][7:0], buffer[i][7:0]});
            i          += 2;
            local_addr += 2;
            local_size -= 2;
          end

          // now main loop, always aligned
          // TODO: this can be replaced by one single burst
          while(local_size >= 4) begin
            spi_write_word(use_qspi, local_addr, {buffer[i+3][7:0], buffer[i+2][7:0], buffer[i+1][7:0], buffer[i][7:0]});
            i          += 4;
            local_addr += 4;
            local_size -= 4;
          end

          // now take care of the last max 3 bytes
          if (local_size >= 2) begin
            spi_write_halfword(use_qspi, local_addr, {buffer[i+1][7:0], buffer[i][7:0]});
            i          += 2;
            local_addr += 2;
            local_size -= 2;
          end

          if (local_size >= 1) begin
            spi_write_byte(use_qspi, local_addr, buffer[i]);
            i          += 1;
            local_addr += 1;
            local_size -= 1;
          end
        end else begin
          // read

          // first align addresses to words
          if (local_addr[0]) begin
            spi_read_byte(use_qspi, local_addr, rdata_temp[7:0]);
            buffer[i] = rdata_temp[7:0];
            i          += 1;
            local_addr += 1;
            local_size -= 1;
          end

          if (local_addr[1] && local_size >= 2) begin
            spi_read_halfword(use_qspi, local_addr, rdata_temp[15:0]);
            buffer[i]   = rdata_temp[ 7:0];
            buffer[i+1] = rdata_temp[15:8];
            i          += 2;
            local_addr += 2;
            local_size -= 2;
          end

          // now main loop, always aligned
          // Done in one single burst
          rdata_temp_arr = new[local_size/4];
          spi_read_nword(use_qspi, local_addr, local_size/4, rdata_temp_arr);
          for (j = 0; j < local_size/4; j++) begin
            buffer[i]   = rdata_temp_arr[j][ 7:0];
            buffer[i+1] = rdata_temp_arr[j][15:8];
            buffer[i+2] = rdata_temp_arr[j][23:16];
            buffer[i+3] = rdata_temp_arr[j][31:24];
            i += 4;
          end
          local_addr += (local_size/4) * 4;
          local_size -= (local_size/4) * 4;
          rdata_temp_arr.delete();

          // now take care of the last max 3 bytes
          if (local_size >= 2) begin
            spi_read_halfword(use_qspi, local_addr, rdata_temp[15:0]);
            buffer[i]   = rdata_temp[ 7:0];
            buffer[i+1] = rdata_temp[15:8];
            i          += 2;
            local_addr += 2;
            local_size -= 2;
          end

          if (local_size >= 1) begin
            spi_read_byte(use_qspi, local_addr, rdata_temp[7:0]);
            buffer[i] = rdata_temp[7:0];
            i          += 1;
            local_addr += 1;
            local_size -= 1;
          end
        end

        if (mem_push(packet, buffer) != 0)
          $display("mem_push has failed");
      end
    end
  end
endtask
