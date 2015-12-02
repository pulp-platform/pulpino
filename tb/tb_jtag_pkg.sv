`define JTAG_SOC_INSTR_WIDTH         4
`define JTAG_CLUSTER_INSTR_WIDTH     4

`define JTAG_SOC_AXIREG              4'b0100
`define JTAG_SOC_BYPASS      4'b1111

`define JTAG_INSTR_WIDTH  (`JTAG_SOC_INSTR_WIDTH + `JTAG_CLUSTER_INSTR_WIDTH)

task jtag_wait_halfperiod(input int cycles);
   #(50000*cycles);
endtask

task jtag_clock(input int cycles);
   for(int i=0; i<cycles; i=i+1) begin
      tb.tck <= 1'b0;
      jtag_wait_halfperiod(1);
      tb.tck <= 1'b1;
      jtag_wait_halfperiod(1);
      tb.tck <= 1'b0;
   end
endtask

task jtag_reset();
   tb.tms   <= 1'b0;
   tb.tck   <= 1'b0;
   tb.trstn <= 1'b0;
   tb.tdi    <= 1'b0;
   jtag_wait_halfperiod(2);
   tb.trstn <= 1'b1;
endtask

task jtag_softreset();
   tb.tms   <= 1'b1;
   tb.trstn <= 1'b1;
   tb.tdi    <= 1'b0;
   jtag_clock(5); //enter RST
   tb.tms   <= 1'b0;
   jtag_clock(1); // back to IDLE

endtask

class JTAG_reg #(int unsigned size = 32, logic [`JTAG_INSTR_WIDTH-1:0] instr = 'h0);

   task idle();
      tb.trstn <= 1'b1;
      // from SHIFT_DR to RUN_TEST : tms sequence 10
      tb.tms   <= 1'b1;
      tb.tdi    <= 1'b0;
      jtag_clock(1);
      tb.tms   <= 1'b0;
      jtag_clock(1);
   endtask

   task update_and_goto_shift();
      tb.trstn <= 1'b1;
      // from SHIFT_DR to RUN_TEST : tms sequence 110
      tb.tms   <= 1'b1;
      tb.tdi    <= 1'b0;
      jtag_clock(1);
      tb.tms   <= 1'b1;
      jtag_clock(1);
      tb.tms   <= 1'b0;
      jtag_clock(1);
      jtag_clock(1);
   endtask

   task jtag_goto_SHIFT_IR();
      tb.trstn <= 1'b1;
      tb.tdi    <= 1'b0;
      // from IDLE to SHIFT_IR : tms sequence 1100
      tb.tms   <= 1'b1;
      jtag_clock(2);
      tb.tms   <= 1'b0;
      jtag_clock(2);
   endtask

   task jtag_goto_SHIFT_DR();
      tb.trstn <= 1'b1;
      tb.tdi    <= 1'b0;
      // from IDLE to SHIFT_IR : tms sequence 100
      tb.tms   <= 1'b1;
      jtag_clock(1);
      tb.tms   <= 1'b0;
      jtag_clock(2);
   endtask

   task jtag_shift_SHIFT_IR();
      tb.trstn <= 1'b1;
      tb.tms   <= 1'b0;
      for(int i=0; i < `JTAG_INSTR_WIDTH; i=i+1) begin
         if (i == `JTAG_INSTR_WIDTH-1)
              tb.tms   <= 1'b1;
         tb.tdi <= instr[i];
         jtag_clock(1);
      end
   endtask

      task jtag_shift_NBITS_SHIFT_DR(input int unsigned numbits, input logic[size-1:0] datain,output logic[size-1:0] dataout);
      tb.trstn <= 1'b1;
      tb.tms   <= 1'b0;
      for(int i=0; i<numbits; i=i+1) begin
         if (i == (numbits-1))
            tb.tms   <= 1'b1;
         tb.tdi = datain[i];
         jtag_clock(1);
         dataout[i] = tb.tdo;
      end
   endtask

   task shift_nbits_noex(input int unsigned numbits, input logic[size-1:0] datain,output logic[size-1:0] dataout);
      tb.trstn <= 1'b1;
      tb.tms   <= 1'b0;
      for(int i=0; i<numbits; i=i+1) begin
         tb.tdi = datain[i];
         jtag_clock(1);
         dataout[i] = tb.tdo;
      end
   endtask

   task start_shift();
      this.jtag_goto_SHIFT_DR();
   endtask

   task shift_nbits(input int unsigned numbits, input logic[size-1:0] datain,output logic[size-1:0] dataout);
        this.jtag_shift_NBITS_SHIFT_DR(numbits,datain,dataout);
   endtask

   task setIR();
      this.jtag_goto_SHIFT_IR();
      this.jtag_shift_SHIFT_IR();
      this.idle();
   endtask

   task shift(input logic[size-1:0] datain,output logic[size-1:0] dataout);
      this.jtag_goto_SHIFT_DR();
      this.jtag_shift_NBITS_SHIFT_DR(size,datain,dataout);
      this.idle();
   endtask

endclass

`define ADV_DBG_AXI4_MODULE 6'b100000
`define ADV_DBG_CPU_MODULE 6'b100001

`define ADV_DBG_AXI4_NOP     5'h0
`define ADV_DBG_AXI4_WRITE8  5'h1
`define ADV_DBG_AXI4_WRITE16 5'h2
`define ADV_DBG_AXI4_WRITE32 5'h3
`define ADV_DBG_AXI4_WRITE64 5'h4
`define ADV_DBG_AXI4_READ8   5'h5
`define ADV_DBG_AXI4_READ16  5'h6
`define ADV_DBG_AXI4_READ32  5'h7
`define ADV_DBG_AXI4_READ64  5'h8
`define ADV_DBG_AXI4_WREG    5'h9
`define ADV_DBG_AXI4_SELREG  5'hD

`define ADV_DBG_CPU_NOP     5'h0
`define ADV_DBG_CPU_WRITE   5'h3
`define ADV_DBG_CPU_READ    5'h7
`define ADV_DBG_CPU_WREG    5'h9
`define ADV_DBG_CPU_SELREG  5'hD

`define ADV_DBG_CPU_REG_STATUS 3'b000

class adv_dbg_if_t;

   JTAG_reg #(.size(256), .instr({`JTAG_SOC_AXIREG, `JTAG_SOC_BYPASS})) jtag_cluster_dbg;

   task init();
      jtag_cluster_dbg = new;
      jtag_cluster_dbg.idle();
      jtag_cluster_dbg.setIR();
   endtask

   task axi4_nop();
      // TO BE CHECKED
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
//      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_NOP, 32'b0, 0}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 NOP command.");
   endtask

   task axi4_write8(input logic[31:0] addr, input int nwords, input logic [255:0][7:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_WRITE8, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits_noex(9, {data[0], 1'b1}, dataout);
      for(int i=1; i<nwords; i++)
         jtag_cluster_dbg.shift_nbits_noex(8, data[i], dataout);
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'h11111111}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 WRITE8 burst @%h for %d bytes.", addr, nwords);
   endtask

   task axi4_write16(input logic[31:0] addr, input int nwords, input logic [255:0][15:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_WRITE16, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits_noex(17, {data[0], 1'b1}, dataout);
      for(int i=1; i<nwords; i++)
         jtag_cluster_dbg.shift_nbits_noex(16, data[i], dataout);
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'h11111111}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 WRITE16 burst @%h for %d bytes.", addr, nwords*2);
   endtask

   task axi4_write32(input logic[31:0] addr, input int nwords, input logic [255:0][31:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_WRITE32, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits_noex(33, {data[0], 1'b1}, dataout);
      for(int i=1; i<nwords; i++)
         jtag_cluster_dbg.shift_nbits_noex(32, data[i], dataout);
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'h11111111}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 WRITE32 burst @%h for %d bytes.", addr, nwords*4);
   endtask

   task axi4_write64(input logic[31:0] addr, input int nwords, input logic [255:0][63:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_WRITE64, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits_noex(65, {data[0], 1'b1}, dataout);
      for(int i=1; i<nwords; i++)
         jtag_cluster_dbg.shift_nbits_noex(64, data[i], dataout);
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'h11111111}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 WRITE64 burst @%h for %d bytes.", addr, nwords*8);
   endtask

   task axi4_read8(input logic[31:0] addr, input int nwords, output logic [255:0][7:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_READ8, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      while(1) // wait for a '1' from the AXI module
      begin
        jtag_cluster_dbg.shift_nbits_noex(1, {1'b0}, dataout);
        if(dataout[0] == 1'b1) break;
      end
      jtag_cluster_dbg.shift_nbits_noex(8, 8'b0, dataout);
      data[0] = dataout[7:0];
      for(int i=1; i<nwords; i++) begin
         jtag_cluster_dbg.shift_nbits_noex(8, 8'b0, dataout);
         data[i] = dataout[7:0];
      end
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'b0}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 READ8 burst @%h for %d bytes.", addr, nwords);
   endtask

   task axi4_read16(input logic[31:0] addr, input int nwords, output logic [255:0][15:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_READ16, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      while(1) // wait for a '1' from the AXI module
      begin
        jtag_cluster_dbg.shift_nbits_noex(1, {1'b0}, dataout);
        if(dataout[0] == 1'b1) break;
      end
      jtag_cluster_dbg.shift_nbits_noex(16, 16'b0, dataout);
      data[0] = dataout[15:0];
      for(int i=1; i<nwords; i++) begin
         jtag_cluster_dbg.shift_nbits_noex(16, 16'b0, dataout);
         data[i] = dataout[15:0];
      end
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'b0}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 READ16 burst @%h for %d bytes.", addr, nwords*2);
   endtask

   task axi4_read32(input logic[31:0] addr, input int nwords, output logic [255:0][31:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_READ32, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      while(1) // wait for a '1' from the AXI module
      begin
        jtag_cluster_dbg.shift_nbits_noex(1, {1'b0}, dataout);
        if(dataout[0] == 1'b1) break;
      end
      jtag_cluster_dbg.shift_nbits_noex(32, 32'b0, dataout);
      data[0] = dataout[31:0];
      for(int i=1; i<nwords; i++) begin
         jtag_cluster_dbg.shift_nbits_noex(32, 32'b0, dataout);
         data[i] = dataout[31:0];
      end
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'b0}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 READ32 burst @%h for %d bytes.", addr, nwords*4);
   endtask

   task axi4_read64(input logic[31:0] addr, input int nwords, output logic [255:0][63:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_AXI4_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(54,{1'b0, `ADV_DBG_AXI4_READ64, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      while(1) // wait for a '1' from the AXI module
      begin
        jtag_cluster_dbg.shift_nbits_noex(1, {1'b0}, dataout);
        if(dataout[0] == 1'b1) break;
      end
      jtag_cluster_dbg.shift_nbits_noex(64, 64'b0, dataout);
      data[0] = dataout[63:0];
      for(int i=1; i<nwords; i++) begin
         jtag_cluster_dbg.shift_nbits_noex(64, 64'b0, dataout);
         data[i] = dataout[63:0];
      end
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'b0}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] AXI4 READ64 burst @%h for %d bytes.", addr, nwords*8);
   endtask

   task cpu_write(input logic [3:0] cpu_id, input logic[31:0] addr, input int nwords, input logic [255:0][31:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_CPU_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(58,{1'b0, `ADV_DBG_CPU_WRITE, cpu_id, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits_noex(33, {data[0], 1'b1}, dataout);
      for(int i=1; i<nwords; i++)
         jtag_cluster_dbg.shift_nbits_noex(32, data[i], dataout);
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'h11111111}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] CPU WRITE burst @%h for %d bytes.", addr, nwords*4);
   endtask

   task cpu_read(input logic [3:0] cpu_id, input logic[31:0] addr, input int nwords, output logic [255:0][31:0] data);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_CPU_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(58,{1'b0, `ADV_DBG_CPU_READ, cpu_id, addr, nwords[15:0]}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      while(1) // wait for a '1' from the OR1K module
      begin
        jtag_cluster_dbg.shift_nbits_noex(1, {1'b0}, dataout);
        if(dataout[0] == 1'b1) break;
      end
      jtag_cluster_dbg.shift_nbits_noex(32, 32'b0, dataout);
      data[0] = dataout[31:0];
      for(int i=1; i<nwords; i++) begin
         jtag_cluster_dbg.shift_nbits_noex(32, 32'b0, dataout);
         data[i] = dataout[31:0];
      end
      jtag_cluster_dbg.shift_nbits(34, {2'b0, 32'b0}, dataout); // for now we completely ignore CRC
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] CPU READ burst @%h for %d bytes.", addr, nwords*4);
   endtask

   task cpu_wait_for_stall();
      logic [255:0] dataout;
      while(1)
      begin
        jtag_cluster_dbg.start_shift();
        jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_CPU_MODULE}, dataout);
        jtag_cluster_dbg.update_and_goto_shift();
        jtag_cluster_dbg.shift_nbits(9,{1'b0, `ADV_DBG_CPU_NOP, `ADV_DBG_CPU_REG_STATUS, 2'b0}, dataout);
        jtag_cluster_dbg.idle();

        if(dataout[0] == 1'b1) break;
      end
   endtask

   task cpu_stall(input logic [3:0] cpu_mask);
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_CPU_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(13,{1'b0, 1'b0, `ADV_DBG_CPU_WREG, `ADV_DBG_CPU_REG_STATUS, cpu_mask}, dataout);
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] CPU STALL command.");
   endtask

   task cpu_reset();
      logic [255:0] dataout;
      jtag_cluster_dbg.start_shift();
      jtag_cluster_dbg.shift_nbits(7, {1'b0, `ADV_DBG_CPU_MODULE}, dataout);
      jtag_cluster_dbg.update_and_goto_shift();
      jtag_cluster_dbg.shift_nbits(13,{1'b0, 1'b0, `ADV_DBG_CPU_WREG, `ADV_DBG_CPU_REG_STATUS, 4'b0000}, dataout);
      jtag_cluster_dbg.idle();
      $display("[adv_dbg_if] CPU RESET command.");
   endtask

   task cpu_read_gpr(input logic [3:0] cpu_id, input logic [4:0] addr, output logic [31:0] data);
     logic [255:0][31:0] tmp;
     adv_dbg_if.cpu_read(cpu_id, {16'b0, 6'b1, 5'b0, addr}, 1, tmp);
     data = tmp[0];
   endtask

   task cpu_write_gpr(input logic [3:0] cpu_id, input logic [4:0] addr, input logic [31:0] data);
     logic [255:0][31:0] tmp;
     tmp[0] = data;
     adv_dbg_if.cpu_write(cpu_id, {16'b0, 6'b1, 5'b0, addr}, 1, tmp);
   endtask

endclass

adv_dbg_if_t adv_dbg_if = new;