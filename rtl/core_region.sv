
`include "axi_bus.sv"

`define INSTR_ADDR_WIDTH  12
`define DATA_ADDR_WIDTH   11

module core_region
(
    // Clock and Reset
    input logic clk,
    input logic rst_n,

    AXI_BUS.Master core_master,
    AXI_BUS.Slave  data_slave,
    AXI_BUS.Slave  instr_slave
  );

  logic         core_instr_en;
  logic         core_instr_req;
  logic [31:0]  core_instr_addr;
  logic [31:0]  core_instr_rdata;

  logic         core_req;
  logic         core_gnt;
  logic         core_rvalid;
  logic [31:0]  core_addr;
  logic         core_we;
  logic [3:0]   core_be;
  logic [31:0]  core_rdata;
  logic [31:0]  core_wdata;


  logic         instr_mem_req;
  logic [31:0]  instr_mem_addr;
  logic         instr_mem_we;
  logic [3:0]   instr_mem_be;
  logic [31:0]  instr_mem_rdata;
  logic [31:0]  instr_mem_wdata;

  logic         data_mem_req;
  logic [31:0]  data_mem_addr;
  logic         data_mem_we;
  logic [3:0]   data_mem_be;
  logic [31:0]  data_mem_rdata;
  logic [31:0]  data_mem_wdata;



  enum logic [1:0] { AXI, RAM } CS, NS;

  logic         core_data_req;
  logic         core_data_gnt;
  logic         core_data_rvalid;
  logic [31:0]  core_data_rdata;

  logic         core_axi_req;
  logic         core_axi_gnt;
  logic         core_axi_rvalid;
  logic [31:0]  core_axi_rdata;



  or10n_core or10n_core
  (
    // Clock and Reset
    .clk             ( clk              ),
    .rst_n           ( rst_n            ),

    .boot_addr_i     ( 32'h1C00_0000    ),
    .core_id_i       ( 5'h0             ),
    .cluster_id_i    ( 5'h0             ),

    .instr_addr_o    ( core_instr_addr  ),
    .instr_req_o     ( core_instr_req   ),
    .instr_rdata_i   ( core_instr_rdata ),
    .instr_grant_i   ( 1'b1             ),
    .instr_rvalid_i  ( 1'b1             ),

    .data_addr_o     ( core_addr        ),
    .data_wdata_o    ( core_wdata       ),
    .data_we_o       ( core_we          ),
    .data_req_o      ( core_req         ),
    .data_be_o       ( core_be          ),
    .data_rdata_i    ( core_rdata       ),
    .data_gnt_i      ( core_gnt         ),
    .data_r_valid_i  ( core_rvalid      ),

    .irq_i           ( 1'b0             ),
    .irq_nm_i        ( 1'b0             ),

    .dbginf_stall_i  ( 1'b0             ),
    .dbginf_bp_o     (                  ),
    .dbginf_strobe_i ( 1'b0             ),
    .dbginf_ack_o    (                  ),
    .dbginf_we_i     ( 1'b0             ),
    .dbginf_addr_i   ( 16'h0            ),
    .dbginf_data_i   ( 32'h0            ),
    .dbginf_data_o   (                  ),

    .fetch_enable_i  ( 1'b1             ),
    .core_busy_o     (                  )
  );

  core2axi
  #(
    .AXI_ADDR_WIDTH ( 32 )
  )
  core2axi_i
  (
    .clk   ( clk   ),
    .rst_n ( rst_n ),

    .data_req_i    ( core_axi_req    ),
    .data_gnt_o    ( core_axi_gnt    ),
    .data_rvalid_o ( core_axi_rvalid ),
    .data_addr_i   ( core_addr       ),
    .data_we_i     ( core_we         ),
    .data_be_i     ( core_be         ),
    .data_rdata_o  ( core_axi_rdata  ),
    .data_wdata_i  ( core_wdata      ),

    .master        ( core_master     )
  );

  // DEMUX
  assign core_data_req = (core_addr[31:24] == 8'h10) & core_req;
  assign core_axi_req  = core_req & (~core_data_req);

  always_ff @(posedge clk, negedge rst_n)
  begin
    if (rst_n == 1'b0)
    begin
      CS <= RAM;
    end
    else
    begin
      CS <= NS;
    end
  end

  always_comb
  begin
    NS = CS;

    core_gnt    = 1'b0;
    core_rvalid = 1'b0;
    core_rdata  = 'h0;

    if (core_axi_req)
      NS = AXI;
    else if (core_data_req)
      NS = RAM;

    case (CS)
      AXI:
      begin
        core_gnt    = core_axi_gnt;
        core_rvalid = core_axi_rvalid;
        core_rdata  = core_axi_rdata;
      end

      RAM:
      begin
        core_gnt    = core_data_gnt;
        core_rvalid = core_data_rvalid;
        core_rdata  = core_data_rdata;
      end
    endcase
  end


  //----------------------------------------------------------------------------//
  // Instruction Memory
  //----------------------------------------------------------------------------//

  assign core_instr_en = (core_instr_addr[31:24] == 8'h1C) & core_instr_req;

  dp_ram
  #(
    .ADDR_WIDTH ( `INSTR_ADDR_WIDTH    )
  )
  instr_mem
  (
    .clk       ( clk                   ),

    .en_a_i    ( core_instr_en         ),
    .addr_a_i  ( core_instr_addr[`INSTR_ADDR_WIDTH+1:2] ),
    .wdata_a_i ( 32'h0                 ),
    .rdata_a_o ( core_instr_rdata      ),
    .we_a_i    ( 1'b0                  ),
    .be_a_i    ( 4'h0                  ),

    .en_b_i    ( instr_mem_req         ),
    .addr_b_i  ( instr_mem_addr[`INSTR_ADDR_WIDTH+1:2] ),
    .wdata_b_i ( instr_mem_wdata       ),
    .rdata_b_o ( instr_mem_rdata       ),
    .we_b_i    ( instr_mem_we          ),
    .be_b_i    ( instr_mem_be          )
  );


  axi2mem
  #(
    .AXI_ADDR_WIDTH ( 32   )
  )
  instr_mem_axi_if
  (
    .clk         ( clk                   ),
    .rst_n       ( rst_n                 ),

    .mem_req_o   ( instr_mem_req         ),
    .mem_addr_o  ( instr_mem_addr        ),
    .mem_we_o    ( instr_mem_we          ),
    .mem_be_o    ( instr_mem_be          ),
    .mem_rdata_i ( instr_mem_rdata       ),
    .mem_wdata_o ( instr_mem_wdata       ),

    .slave       ( instr_slave           )
  );

  //----------------------------------------------------------------------------//
  // Data Memory
  //----------------------------------------------------------------------------//

  assign core_data_gnt = core_data_req;

  always_ff @(posedge clk, negedge rst_n)
  begin
    if (rst_n == 1'b0)
    begin
      core_data_rvalid <= 1'b0;
    end
    else
    begin
      core_data_rvalid <= core_data_gnt;
    end
  end

  dp_ram
  #(
    .ADDR_WIDTH ( `DATA_ADDR_WIDTH    )
  )
  data_mem
  (
    .clk       ( clk                  ),

    .en_a_i    ( core_data_req        ),
    .addr_a_i  ( core_addr[`DATA_ADDR_WIDTH+1:2] ),
    .wdata_a_i ( core_wdata           ),
    .rdata_a_o ( core_data_rdata      ),
    .we_a_i    ( core_we              ),
    .be_a_i    ( core_be              ),

    .en_b_i    ( data_mem_req         ),
    .addr_b_i  ( data_mem_addr[`DATA_ADDR_WIDTH+1:2] ),
    .wdata_b_i ( data_mem_wdata       ),
    .rdata_b_o ( data_mem_rdata       ),
    .we_b_i    ( data_mem_we          ),
    .be_b_i    ( data_mem_be          )
  );

  axi2mem
  #(
    .AXI_ADDR_WIDTH ( 32   )
  )
  data_mem_axi_if
  (
    .clk         ( clk                  ),
    .rst_n       ( rst_n                ),

    .mem_req_o   ( data_mem_req         ),
    .mem_addr_o  ( data_mem_addr        ),
    .mem_we_o    ( data_mem_we          ),
    .mem_be_o    ( data_mem_be          ),
    .mem_rdata_i ( data_mem_rdata       ),
    .mem_wdata_o ( data_mem_wdata       ),

    .slave       ( data_slave           )
  );


endmodule
