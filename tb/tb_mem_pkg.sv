`ifdef ASIC
  logic [31:0]     data_mem[];  // this variable holds the whole memory content
  logic [31:0]     instr_mem[]; // this variable holds the whole memory content
  event            event_mem_load;

  task mem_preload;
    begin
      $display("Preloading memory");

      instr_mem = new [tb.top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_bank_i.NUM_BANKS * tb.top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_bank_i.BANK_SIZE];
      data_mem  = new [tb.top_i.core_region_i.data_mem.sp_ram_bank_i.NUM_BANKS                * tb.top_i.core_region_i.data_mem.sp_ram_bank_i.BANK_SIZE];

      $readmemh("slm_files/l2_stim.slm",    instr_mem);
      $readmemh("slm_files/tcdm_bank0.slm", data_mem);

      -> event_mem_load;
    end
  endtask

  generate
    genvar data_idx;
    for(data_idx = 0; data_idx < tb.top_i.core_region_i.data_mem.sp_ram_bank_i.NUM_BANKS; data_idx++)
    begin : data_preload_gen
      initial
      begin
        integer      addr;
        integer      bank_size;
        logic [31:0] data;

        bank_size = tb.top_i.core_region_i.data_mem.sp_ram_bank_i.BANK_SIZE;

        @ (event_mem_load);

        for(addr = 0; addr < bank_size; addr++) begin
          data = data_mem[(data_idx * bank_size) + addr];

          tb.top_i.core_region_i.data_mem.sp_ram_bank_i.sram_bank_gen[data_idx].bank.mem_bank_i.cut.Memory_byte0[addr][ 7: 0] = data[ 7: 0];
          tb.top_i.core_region_i.data_mem.sp_ram_bank_i.sram_bank_gen[data_idx].bank.mem_bank_i.cut.Memory_byte1[addr][ 7: 0] = data[15: 8];
          tb.top_i.core_region_i.data_mem.sp_ram_bank_i.sram_bank_gen[data_idx].bank.mem_bank_i.cut.Memory_byte2[addr][ 7: 0] = data[23:16];
          tb.top_i.core_region_i.data_mem.sp_ram_bank_i.sram_bank_gen[data_idx].bank.mem_bank_i.cut.Memory_byte3[addr][ 7: 0] = data[31:24];
        end

        $display("[TB] Data Memory Preload Done for bank %d", data_idx);
      end
    end
  endgenerate

  generate
    genvar instr_idx;
    for(instr_idx = 0; instr_idx < tb.top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_bank_i.NUM_BANKS; instr_idx++)
    begin : instr_preload_gen
      initial
      begin
        integer      addr;
        integer      bank_size;
        logic [31:0] data;

        bank_size = tb.top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_bank_i.BANK_SIZE;

        @ (event_mem_load);

        for(addr = 0; addr < bank_size; addr++) begin
          data = instr_mem[(instr_idx * bank_size) + addr];

          tb.top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_bank_i.sram_bank_gen[instr_idx].bank.mem_bank_i.cut.Memory_byte0[addr][ 7: 0] = data[ 7: 0];
          tb.top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_bank_i.sram_bank_gen[instr_idx].bank.mem_bank_i.cut.Memory_byte1[addr][ 7: 0] = data[15: 8];
          tb.top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_bank_i.sram_bank_gen[instr_idx].bank.mem_bank_i.cut.Memory_byte2[addr][ 7: 0] = data[23:16];
          tb.top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_bank_i.sram_bank_gen[instr_idx].bank.mem_bank_i.cut.Memory_byte3[addr][ 7: 0] = data[31:24];
        end

        $display("[TB] Instr Memory Preload Done for bank %d", instr_idx);
      end
    end
  endgenerate
`else
  task mem_preload;
    reg [1023:0] l2_imem_file;
    reg [1023:0] l2_dmem_file;
    begin
      $display("Preloading memory");
      #10;

      if(!$value$plusargs("l2_imem=%s", l2_imem_file))
         l2_imem_file = "slm_files/l2_stim.slm";
       $display("Preloading instruction memory from %0s", l2_imem_file);
      $readmemh(l2_imem_file, top_i.core_region_i.instr_mem.sp_ram_wrap_i.sp_ram_i.mem);

      if(!$value$plusargs("l2_dmem=%s", l2_dmem_file))
         l2_dmem_file = "slm_files/tcdm_bank0.slm";
       $display("Preloading data memory from %0s", l2_dmem_file);
      $readmemh(l2_dmem_file, top_i.core_region_i.data_mem.sp_ram_i.mem);

    end
  endtask
`endif
