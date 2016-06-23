// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "Vtop.h"
#include "verilated.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

vluint64_t main_time = 0;       // Current simulation time

double sc_time_stamp () {       // Called by $time in Verilog
    return main_time;           // converts to double, to match
                                // what SystemC does
}

void preload_memories() {

}

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}

int main(int argc, char **argv, char **env) {
  Verilated::commandArgs(argc, argv);

  if (argc < 2) {
    cout << "Please pass preload files" << endl;
    exit(1);
  }

  std::ifstream instructions(argv[1]);
  std::ifstream data_stream(argv[2]);

  Vtop* top = new Vtop;
  cout << "Asserting hard reset" << endl;
  top->rst_n = 0;
  top->fetch_enable_i = 0;

  int i = 0;
  for (std::string instr; std::getline(instructions, instr);) {
   top->pulpino_top__DOT__core_region_i__DOT__instr_mem__DOT__sp_ram_wrap_i__DOT__sp_ram_i__DOT__mem[i] = std::stoul(split(instr, ' ').at(1), 0, 16);
   i++;
  }

  i = 0;
  for (std::string data; std::getline(data_stream, data);) {
   top->pulpino_top__DOT__core_region_i__DOT__data_mem__DOT__sp_ram_i__DOT__mem[i] = std::stoul(split(data, ' ').at(1), 0, 16);
   i++;
  }

  while (!Verilated::gotFinish()) {
    if (main_time > 10) {
        if (main_time == 11) {
            cout << "Deasserting hard reset" << endl;
            cout << "Preloading memories" << endl;
        }
        top->rst_n = 1;   // Deassert reset
        // writing to boot register -> boot from internal memory
        top->pulpino_top__DOT__peripherals_i__DOT__apb_pulpino_i__DOT__boot_adr_q = 0x00;
    }

    if (main_time > 100) {
        top->fetch_enable_i = 1;
        // for (int i = 0; i < 100; i++)
        // cout << top->pulpino_top__DOT__core_region_i__DOT__instr_mem__DOT__sp_ram_wrap_i__DOT__sp_ram_i__DOT__mem[i] << endl;
    }

    if ((main_time % 10) == 1) {
        top->clk = 1;       // Toggle clock
    }

    if ((main_time % 10) == 6) {
        top->clk = 0;
    }
    if (main_time > 3000)
        break;

    // cout << top->gpio_out << endl;
    top->eval();
    main_time++;
  }

  delete top;
  exit(0);
}