// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "pulpino.h"
#include <iostream>

PULPino::PULPino() {
    top = new Vtb_verilator;
}

PULPino::~PULPino(void) {
    delete top;
}

void PULPino::set_boot_reg(unsigned int boot_address) {
    top->tb_verilator__DOT__top_i__DOT__peripherals_i__DOT__apb_pulpino_i__DOT__boot_adr_q = boot_address;
}

void PULPino::preload_memories(std::string instr_file, std::string data_file) {
  std::ifstream instruction_stream(instr_file);
  std::ifstream data_stream(data_file);
  int i = 0;
  // preload instruction memory
  for (std::string instr; std::getline(instruction_stream, instr);) {
   top->tb_verilator__DOT__top_i__DOT__core_region_i__DOT__instr_mem__DOT__sp_ram_wrap_i__DOT__sp_ram_i__DOT__mem[i] = std::stoul(split(instr, ' ').at(1), 0, 16);
   i++;
  }
  // preload data memory
  i = 0;
  for (std::string data; std::getline(data_stream, data);) {
   top->tb_verilator__DOT__top_i__DOT__core_region_i__DOT__data_mem__DOT__sp_ram_i__DOT__mem[i] = std::stoul(split(data, ' ').at(1), 0, 16);
   i++;
  }
}

int PULPino::get_return_code(void) {
  return top->tb_verilator__DOT__top_i__DOT__peripherals_i__DOT__apb_pulpino_i__DOT__status_q;
}

vector<string> PULPino::split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}