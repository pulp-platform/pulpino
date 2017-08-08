// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

module random_stalls
(
    input  logic         clk,

    input  logic         core_req_i,
    output logic         core_gnt_o,
    input  logic [31:0]  core_addr_i,
    input  logic         core_we_i,
    input  logic [ 3:0]  core_be_i,
    input  logic [31:0]  core_wdata_i,
    output logic [31:0]  core_rdata_o,
    output logic         core_rvalid_o,

    output logic         data_req_o,
    input  logic         data_gnt_i,
    output logic [31:0]  data_addr_o,
    output logic         data_we_o,
    output logic [ 3:0]  data_be_o,
    output logic [31:0]  data_wdata_o,
    input  logic [31:0]  data_rdata_i,
    input  logic         data_rvalid_i
  );

  class rand_wait_cycles;
    rand int n;
    constraint default_c { n >= 0 ; n < 6;}
  endclass

  // random staller
  typedef struct {
    logic [31:0] addr;
    logic        we;
    logic [ 3:0] be;
    logic [31:0] wdata;
    logic [31:0] rdata;
  } stall_mem_t;

  mailbox core_reqs          = new (4);
  mailbox core_resps         = new (4);
  mailbox core_resps_granted = new (4);
  mailbox platform_transfers = new (4);

  // Core Request Side
  // Waits for requests and puts them in a queue, does not perform actual
  // requests to the platform
  initial
  begin
    stall_mem_t mem_acc;
    automatic rand_wait_cycles wait_cycles = new ();
    int temp;

    while(1) begin
      core_gnt_o = 1'b0;

      #1;
      if (!core_req_i)
        continue;


      // we got a request, now let's wait for a random number of cycles before
      // we give the grant
      temp = wait_cycles.randomize();

      while(wait_cycles.n != 0) begin
        @(posedge clk);
        wait_cycles.n--;
        #1;
      end

      // we waited for a random number of cycles, let's give the grant
      core_gnt_o = 1'b1;

      mem_acc.addr  = core_addr_i;
      mem_acc.be    = core_be_i;
      mem_acc.we    = core_we_i;
      mem_acc.wdata = core_wdata_i;

      core_reqs.put(mem_acc);

      @(posedge clk);

      core_resps_granted.put(1'b1);
    end
  end

  // Core Response Side
  // Waits for a response from the platform and then waits for a random number
  // of cycles before giving the rvalid
  initial
  begin
    stall_mem_t mem_acc;
    automatic rand_wait_cycles wait_cycles = new ();
    logic granted;
    int temp;

    while(1) begin
      @(posedge clk);
      core_rvalid_o = 1'b0;
      core_rdata_o = 'x;

      core_resps_granted.get(granted);
      core_resps.get(mem_acc);

      // we got a response, now let's wait for a random amount of cycles
      // we give the grant
      temp = wait_cycles.randomize();

      while(wait_cycles.n != 0) begin
        @(posedge clk);
        wait_cycles.n--;
      end

      // we waited for a random number of cycles, let's give the rvalid
      core_rdata_o  = mem_acc.rdata;
      core_rvalid_o = 1'b1;
    end
  end

  // platform request side
  // Waits for requests from the core and then performs the request on the
  // platform immediately
  // Simulates a "virtual" core
  initial
  begin
    stall_mem_t mem_acc;

    while(1) begin
      @(posedge clk);
      data_req_o   = 1'b0;
      data_addr_o  = '0;
      data_we_o    = 1'b0;
      data_be_o    = 4'b0;
      data_wdata_o = 'x;

      core_reqs.get(mem_acc);

      data_req_o   = 1'b1;
      data_addr_o  = mem_acc.addr;
      data_we_o    = mem_acc.we;
      data_be_o    = mem_acc.be;
      data_wdata_o = mem_acc.wdata;

      #1;
      while(!data_gnt_i) begin
        @(posedge clk);
        #1;
      end

      platform_transfers.put(mem_acc);
    end
  end

  // platform response side
  // Waits for rvalids and puts the responses into the core response mailbox
  initial
  begin
    stall_mem_t mem_acc;

    while(1) begin
      @(posedge clk);

      platform_transfers.get(mem_acc);

      while(!data_rvalid_i) begin
        @(posedge clk);
      end

      mem_acc.rdata = data_rdata_i;

      core_resps.put(mem_acc);
    end
  end
endmodule
