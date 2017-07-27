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


// This module takes data over UART and prints them to the console
// A string is printed to the console as soon as a '\n' character is found
interface uart_bus
  #(
    parameter BAUD_RATE = 115200,
    parameter PARITY_EN = 0
    )
  (
    input  logic rx,
    output logic tx,

    input  logic rx_en
  );
  timeunit      1ps;
  timeprecision 1ps;

  localparam BIT_PERIOD = (1000000000/BAUD_RATE*1000);

  logic [7:0]       character;
  logic [256*8-1:0] stringa;
  logic             parity;
  integer           charnum;
  integer           file;
 
  initial
  begin
    tx   = 1'b1;
    file = $fopen("stdout/uart", "w");
  end

  always
  begin
    if (rx_en)
    begin
      @(negedge rx);
      #(BIT_PERIOD/2) ;
      for (int i=0;i<=7;i++)
      begin
        #BIT_PERIOD character[i] = rx;
      end

      if(PARITY_EN == 1)
      begin
        // check parity
        #BIT_PERIOD parity = rx;

        for (int i=7;i>=0;i--)
        begin
          parity = character[i] ^ parity;
        end

        if(parity == 1'b1)
        begin
          $display("Parity error detected");
        end
      end

      // STOP BIT
      #BIT_PERIOD;

      $fwrite(file, "%c", character);
      stringa[(255-charnum)*8 +: 8] = character;
      if (character == 8'h0A || charnum == 254) // line feed or max. chars reached
      begin
        if (character == 8'h0A)
          stringa[(255-charnum)*8 +: 8] = 8'h0; // null terminate string, replace line feed
        else
          stringa[(255-charnum-1)*8 +: 8] = 8'h0; // null terminate string

        $write("RX string: %s\n",stringa);
        charnum = 0;
        stringa = "";
      end
      else
      begin
        charnum = charnum + 1;
      end
    end
    else
    begin
      charnum = 0;
      stringa = "";
      #10;
    end
  end

  task send_char(input logic [7:0] c);
    int i;

    // start bit
    tx = 1'b0;

    for (i = 0; i < 8; i++) begin
      #(BIT_PERIOD);
      tx = c[i];
      $display("[UART] Sent %x at time %t",c[i],$time);
    end

    // stop bit
    #(BIT_PERIOD);
    tx = 1'b1;
    #(BIT_PERIOD);
  endtask
endinterface
