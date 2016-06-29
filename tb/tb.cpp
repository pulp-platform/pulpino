// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#define TRACE
#define PRELOAD

#include "Vtop.h"
#include "verilated.h"
#include <iostream>
#include <fstream>
#include "pulpino.h"

#ifdef TRACE
  #include "verilated_vcd_c.h"
#endif


vluint64_t main_time = 0;       // Current simulation time
int return_code = 1;

double sc_time_stamp () {       // Called by $time in Verilog
    return main_time;           // converts to double, to match
                                // what SystemC does
}

int main(int argc, char **argv, char **env) {
  Verilated::commandArgs(argc, argv);

  if (argc < 2) {
    cout << "Please pass preload files" << endl;
    exit(1);
  }

  PULPino* pulpino = new PULPino();

  #ifdef TRACE
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    pulpino->top->trace(tfp, 99);
    tfp->open ("simx.vcd");
  #endif

  #ifdef PRELOAD
    cout << "Preloading memories" << endl;
    pulpino->preload_memories(argv[1], argv[2]);
  #endif

  cout << "Asserting hard reset" << endl;
  pulpino->top->rst_n = 0;
  pulpino->top->fetch_enable_i = 0;

  // main simulation
  while (!Verilated::gotFinish()) {

    pulpino->top->spi_cs_i = 1;

    if (main_time > 10) {
      if (main_time == 11) {
          cout << "Deasserting hard reset" << endl;
      }
      pulpino->top->rst_n = 1;   // Deassert reset
      // writing to boot register -> boot from internal memory
      pulpino->set_boot_reg(0x00);
    }

    if (main_time > 100) {
      pulpino->top->fetch_enable_i = 1;
    }

    if ((main_time % 10) == 1) {
      pulpino->top->clk = 1;       // Toggle clock
    }

    if ((main_time % 10) == 6) {
      pulpino->top->clk = 0;
    }

    if ((main_time % 10) == 0) {
      pulpino->top->spi_clk_i ^= 1; // toggle spi_clk but keep csn high
    }

    if ((pulpino->top->gpio_out & (1 << 8)) != 0) {
      cout << "Received EOC (End of Computation)" << endl;
      break;
    }

    #ifdef TRACE
      tfp->dump (main_time);
    #endif

    pulpino->top->eval();
    main_time++;
  }

  #ifdef TRACE
    tfp->close();
  #endif

  #ifdef PRELOAD
    return_code = pulpino->get_return_code();
  #endif

  delete pulpino;

  exit(return_code);
}