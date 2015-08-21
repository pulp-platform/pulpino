  `define SPI_STD     2'b00
  `define SPI_QUAD_TX 2'b01
  `define SPI_QUAD_RX 2'b10

  `define SPI_SEMIPERIOD      50000    //10Mhz SPI CLK

  `define DELAY_BETWEEN_SPI   100000

  int                   num_stim,num_exp,num_cycles,num_err = 0;   // counters for statistics

  logic                 more_stim = 1;

  logic [31:0]          spi_data;
  logic [31:0]          spi_addr;
  logic [31:0]          spi_addr_old;

  logic [63:0]          stimuli  [10000:0];                // array for the stimulus vectors

  task spi_send_cmd_addr;
    input          use_qspi;
    input    [7:0] command;
    input   [31:0] addr;
    begin
      if (use_qspi)
      begin
        for (int i = 2; i > 0; i--)
        begin
          spi_sdo3 = command[4*i-1];
          spi_sdo2 = command[4*i-2];
          spi_sdo1 = command[4*i-3];
          spi_sdo0 = command[4*i-4];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end
      else
      begin
        for (int i = 7; i >= 0; i--)
        begin
          spi_sdo0 = command[i];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end

      if (use_qspi)
      begin
        for (int i = 8; i > 0; i--)
        begin
          spi_sdo3 = addr[4*i-1];
          spi_sdo2 = addr[4*i-2];
          spi_sdo1 = addr[4*i-3];
          spi_sdo0 = addr[4*i-4];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end
      else
      begin
        for (int i = 31; i >= 0; i--)
        begin
          spi_sdo0 = addr[i];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end
    end
  endtask

  task spi_send_data;
    input          use_qspi;
    input   [31:0] data;
    begin
      if (use_qspi)
      begin
        for (int i = 8; i > 0; i--)
        begin
          spi_sdo3 = data[4*i-1];
          spi_sdo2 = data[4*i-2];
          spi_sdo1 = data[4*i-3];
          spi_sdo0 = data[4*i-4];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end
      else
      begin
        for (int i = 31; i >= 0; i--)
        begin
          spi_sdo0 = data[i];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end
    end
  endtask


  task spi_load;
    spi_addr        = stimuli[num_stim][63:32]; // assign address
    spi_data        = stimuli[num_stim][31:0];  // assign data

    $display("[SPI] Loading L2 in QPI mode");
    spi_csn  = 1'b0;
    #100  spi_send_cmd_addr(1,8'h2,spi_addr);

    spi_addr_old = spi_addr - 32'h4;

    while (more_stim)                        // loop until we have no more stimuli)
    begin
      spi_addr        = stimuli[num_stim][63:32]; // assign address
      spi_data        = stimuli[num_stim][31:0];  // assign data

      if (spi_addr != (spi_addr_old + 32'h4))
      begin
        $display("[SPI] Prev address %h current addr %h",spi_addr_old,spi_addr);
        #100 spi_csn  = 1'b1;
        #`DELAY_BETWEEN_SPI;
        spi_csn  = 1'b0;
        #100  spi_send_cmd_addr(1,8'h2,spi_addr);
      end
      spi_send_data(1,spi_data[31:0]);

      num_stim     = num_stim + 1;             // increment stimuli
      spi_addr_old = spi_addr;
      if (num_stim > 9999 | stimuli[num_stim]===64'bx ) // make sure we have more stimuli
        more_stim = 0;                    // if not set variable to 0, will prevent additional stimuli to be applied
    end                                   // end while loop
    #100 spi_csn  = 1'b1;
    #`DELAY_BETWEEN_SPI;
  endtask

  task spi_write_reg;
    input          use_qspi;
    input    [7:0] command;
    input    [7:0] reg_val;
    begin
      spi_csn  = 1'b0;
      #100; 
      if (use_qspi)
      begin
        for (int i = 2; i > 0; i--)
        begin
          spi_sdo3 = command[4*i-1];
          spi_sdo2 = command[4*i-2];
          spi_sdo1 = command[4*i-3];
          spi_sdo0 = command[4*i-4];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end
      else
      begin
        for (int i = 7; i >= 0; i--)
        begin
          spi_sdo0 = command[i];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end

      if (use_qspi)
      begin
        for (int i = 2; i > 0; i--)
        begin
          spi_sdo3 = reg_val[4*i-1];
          spi_sdo2 = reg_val[4*i-2];
          spi_sdo1 = reg_val[4*i-3];
          spi_sdo0 = reg_val[4*i-4];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end
      else
      begin
        for (int i = 7; i >= 0; i--)
        begin
          spi_sdo0 = reg_val[i];
          #`SPI_SEMIPERIOD spi_sck = 1;
          #`SPI_SEMIPERIOD spi_sck = 0;
        end
      end
      #100 spi_csn  = 1'b1;
      #`DELAY_BETWEEN_SPI;
    end
  endtask

  task spi_enable_qpi;
    $display("[SPI] Enabling QPI mode");
    //Sets QPI mode
    spi_write_reg(0,8'h1,8'h1);

    padmode_spi_master = `SPI_QUAD_TX;
  endtask
